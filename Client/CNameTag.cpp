/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CNameTag.cpp
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#include	"StdInc.h"

#define RENDER_DISTANCE_PLAYER	35.0
#define RENDER_DISTANCE_PED		10.0
#define BOX_WIDTH				68.0
#define BOX_HEIGHT				10.0

extern	CCore			* pCore;

CNameTag::CNameTag(void)
{
	// Reset values
	for (EntityId id; id < MAX_PLAYERS; id++)
	{
		m_playerVectors[id] = { 0.0, 0.0, 0.0 };
	}
	// Reset values
	for (EntityId id; id < MAX_PEDS; id++)
	{
		m_pedVectors[id] = { 0.0, 0.0, 0.0 };
	}
}

CNameTag::~CNameTag(void)
{
}

void CNameTag::PreAll(void)
{
	PrePlayer();
	PrePed();
}

void CNameTag::All(void)
{
	Player();
	Ped();
}

void CNameTag::PrePed(void)
{
	for (EntityId id = 0; id < MAX_PEDS; id++)
	{
		// Is ped connected ?
		if (pCore->GetPedManager()->IsActive(id))
		{
			// Current player position
			CVector3 pos;
			pCore->GetPedManager()->Get(id)->GetPed()->GetPosition(&pos);
			pos.fZ += 2.0;

			// Convert coordinates
			CVector3 vecScreen;
			pCore->GetGraphics()->WorldToScreen(pos, &vecScreen);

			// Store values
			m_pedVectors[id] = vecScreen;
		}
	}
}

void CNameTag::Ped(void)
{
	// Get the player position
	CVector3 localPos;
	pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition(&localPos);

	// Loop through all peds
	for (EntityId i = 0; i < MAX_PEDS; i++)
	{
		// Is the current ped valid?
		if (pCore->GetPedManager()->IsActive(i) && pCore->GetPedManager()->Get(i)->GetPed()->IsOnScreen())
		{
			// Get a pointer to the ped
			CPed * pPed = pCore->GetPedManager()->Get(i);

			// Is ped valid and we want to show nick ?
			if (pPed && pPed->GetShowNick() == true)
			{
				CVector3 vecPos, vecScreen;

				// Get the ped's position
				pPed->GetPed()->GetPosition(&vecPos);

				// Get the distance
				float fDistance = Math::GetDistanceBetweenPoints(localPos, vecPos);

				// Make sure distance is ok
				if (fDistance <= RENDER_DISTANCE_PED){
					// The text to display
					String text = String("%s", pPed->GetNick().Get());

					// Little edit to fix text position...
					vecPos.fZ += 1.95f;
					vecPos.fX += 0.17f;

					// Convert position to screen position
					pCore->GetGraphics()->WorldToScreen(vecPos, &vecScreen);

					// Draw the tag
					pCore->GetGraphics()->DrawText(vecScreen.fX, vecScreen.fY, 0xFFFFFFFF, 1.0f, "tahoma-bold", true, text.Get());
				}
			}
		}
	}
}

void CNameTag::PrePlayer(void)
{
	for (EntityId id = 0; id < MAX_PLAYERS; id++)
	{
		// We don't want localPlayer
		if (id != pCore->GetPlayerManager()->GetLocalPlayer()->GetId())
		{
			// Is player connected ?
			if (pCore->GetPlayerManager()->IsActive(id))
			{
				// Current player position
				CVector3 pos;
				pCore->GetPlayerManager()->Get(id)->GetPosition(&pos);
				pos.fZ += 2.0;
				
				// Convert coordinates
				CVector3 vecScreen;
				pCore->GetGraphics()->WorldToScreen(pos, &vecScreen);

				// Store values
				m_playerVectors[id] = vecScreen;
			}
		}
	}
}

void CNameTag::Player(void)
{
	CVector3 localPos;
	CVector3 playerPos;
	float fDistance;
	float fScale = 1.0;
	float dimensionHeight;
	float dimensionWidth;
	String text;
	int	healthWidth;

	for (EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if (i != pCore->GetPlayerManager()->GetLocalPlayer()->GetId() && pCore->GetPlayerManager()->IsActive(i) && pCore->GetPlayerManager()->Get(i)->GetPlayerPed() && pCore->GetPlayerManager()->Get(i)->GetPlayerPed()->IsOnScreen())
		{
			pCore->GetPlayerManager()->Get(i)->GetPosition(&playerPos);
			pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition(&localPos);
			fDistance = Math::GetDistanceBetweenPoints(localPos, playerPos);

			if (fDistance <= RENDER_DISTANCE_PLAYER)
			{
				// Thing to draw
				text = String("%s (%d)", pCore->GetPlayerManager()->Get(i)->GetNick().Get());

				// Dimensions
				dimensionWidth = pCore->GetGraphics()->GetTextWidth(text, fScale, "tahoma-bold");
				dimensionHeight = pCore->GetGraphics()->GetFontHeight(fScale, "tahoma-bold");

				// Health bar
				healthWidth = (((Math::Clamp< float >(0.0, pCore->GetPlayerManager()->Get(i)->GetHealth(), 720.0) * 100.0) / 720.0) / 100 * (BOX_WIDTH - 4.0));

				// Colors
				CColor color1(0, 0, 0, 160);
				CColor color2(0, 110, 0, 160);
				CColor color3(0, 255, 0, 160);

				// We draw texts
				pCore->GetGraphics()->DrawTextA((m_playerVectors[i].fX - dimensionWidth / 2) + 1, m_playerVectors[i].fY + 1, (DWORD)0xFF000000, fScale, "tahoma-bold", false, text.Get());
				pCore->GetGraphics()->DrawTextA((m_playerVectors[i].fX - dimensionWidth / 2), m_playerVectors[i].fY, pCore->GetPlayerManager()->Get(i)->GetColour(), fScale, "tahoma-bold", false, text.Get());

				// We draw boxes
				pCore->GetGraphics()->DrawBox((m_playerVectors[i].fX - (BOX_WIDTH / 2)), (m_playerVectors[i].fY + 16.0), BOX_WIDTH, BOX_HEIGHT, color1.dwHexColor);
				pCore->GetGraphics()->DrawBox((m_playerVectors[i].fX - (BOX_WIDTH / 2) + 2.0), (m_playerVectors[i].fY + 18.0), (BOX_WIDTH - 4.0), (BOX_HEIGHT - 4.0), color2.dwHexColor);
				pCore->GetGraphics()->DrawBox((m_playerVectors[i].fX - (BOX_WIDTH / 2) + 2.0), (m_playerVectors[i].fY + 18.0), healthWidth, (BOX_HEIGHT - 4.0), color3.dwHexColor);
			}
		}
	}
}