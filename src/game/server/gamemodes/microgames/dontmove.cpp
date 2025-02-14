// WarioWare mod by Headshotnoby

#include <engine/shared/config.h>
#include "dontmove.h"

const char *modes[2] = {"Don't move!", "Don't stop moving!"};


MGDontMove::MGDontMove(CGameContext* pGameServer, CGameControllerWarioWare* pController) : Microgame(pGameServer, pController)
{
	m_microgameName = "dontmove";
	m_boss = false;
}

void MGDontMove::Start()
{
	m_Mode = rand() % 2;			
	GameServer()->SendBroadcast(modes[m_Mode], -1);
	Controller()->setPlayerTimers(g_Config.m_WwSndMgDontMove_Offset, g_Config.m_WwSndMgDontMove_Length);
	for (int i=0; i<MAX_CLIENTS; i++) Controller()->g_Complete[i] = true;
}

void MGDontMove::End()
{
	// nothing to clean
}

void MGDontMove::Tick()
{
	float timeLeft = Controller()->getTimeLength() - Controller()->getTimer();

	for (int i=0; i<MAX_CLIENTS; i++)
	{
		CCharacter *Char = GameServer()->GetPlayerChar(i);
		if (not Char) continue;

		if (timeLeft < 2750 and timeLeft > 200)
		{
			if ((not m_Mode and (Char->IsMoving() or Char->GetInput()->m_Hook&1 or Char->GetInput()->m_Fire&1)) or // don't move
				(m_Mode and (not Char->IsMoving() and (Char->Core()->m_Vel.x < 1 and Char->Core()->m_Vel.x > -1) and (Char->Core()->m_Vel.y < 1 and Char->Core()->m_Vel.y > -1)))) // don't stop moving
			{
				Controller()->g_Complete[i] = false;
				Char->Die(i, WEAPON_WORLD, timeLeft/1000.f);
			}
		}
	}
}