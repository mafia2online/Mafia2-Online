/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CNameTag.cpp
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

CNameTag::CNameTag(void)
{
}

CNameTag::~CNameTag(void)
{
}

void CNameTag::All(void)
{
	Player();
	Ped();
}

void CNameTag::Ped(void)
{
	// Loop through all peds
	for (EntityId i = 0; i < MAX_PEDS; i++)
	{
		// Is the current ped valid?
		if (pCore->GetPedManager()->IsActive(i) && pCore->GetPedManager()->Get(i)->GetPed()->IsOnScreen())
		{
			// Get a pointer to the ped
			CPed * pPed = pCore->GetPedManager()->Get(i);

			// Is ped valid?
			if (pPed)
			{
				CVector3 vecPos, vecScreen;
				pPed->GetPed()->GetPosition(&vecPos);

				pCore->GetGraphics()->WorldToScreen(vecPos, &vecScreen);
				pCore->GetGraphics()->DrawText(vecScreen.fX, vecScreen.fY, 0xFFFFFFFF, 1.0f, "tahoma-bold", true, "Ped (%d)", pPed->GetPed()->GetGUID());
			}
		}
	}
}

void CNameTag::Player(void)
{
	for (EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if (i != pCore->GetPlayerManager()->GetLocalPlayer()->GetId() && pCore->GetPlayerManager()->IsActive(i) && pCore->GetPlayerManager()->Get(i)->GetPlayerPed() && pCore->GetPlayerManager()->Get(i)->GetPlayerPed()->IsOnScreen())
		{
			// Get the player position
			CVector3 localPos;
			pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition(&localPos);

			CVector3 playerPos;
			pCore->GetPlayerManager()->Get(i)->GetPosition(&playerPos);

			// Get the distance
			float fDistance = Math::GetDistanceBetweenPoints(localPos, playerPos);

			// Get the screen position from the world
			CVector3 vecScreen;
			pCore->GetGraphics()->WorldToScreen(playerPos, &vecScreen);

			if (fDistance <= 35.0){

				// We get the remote player name and ID
				String strNick = pCore->GetPlayerManager()->Get(i)->GetNick();
				String text = String("%s (%d)", strNick.Get(), i);

				// We define the required vars
				float boxWidth = 68.0;
				float boxHeight = 10.0;

				float fScale = 1.0;

				// Font dimensions
				float dimensionWidth = pCore->GetGraphics()->GetTextWidth(text, fScale, "tahoma-bold");
				float dimensionHeight = pCore->GetGraphics()->GetFontHeight(fScale, "tahoma-bold");

				// Progress by current health
				int healthWidth = (((Math::Clamp< float >(0.0, pCore->GetPlayerManager()->Get(i)->GetHealth(), 720.0) * 100.0) / 720.0) / 100 * (boxWidth - 4.0));

				// Colors
				CColor color1(0, 0, 0, 160);
				CColor color2(0, 110, 0, 160);
				CColor color3(0, 255, 0, 160);

				// We draw texts
				pCore->GetGraphics()->DrawTextA((vecScreen.fX - dimensionWidth / 2) + 1, vecScreen.fY + 1, (DWORD)0xFF000000, fScale, "tahoma-bold", false, text.Get());
				pCore->GetGraphics()->DrawTextA((vecScreen.fX - dimensionWidth / 2), vecScreen.fY, pCore->GetPlayerManager()->Get(i)->GetColour(), fScale, "tahoma-bold", false, text.Get());

				// We draw boxes
				pCore->GetGraphics()->DrawBox((vecScreen.fX - (boxWidth / 2)), (vecScreen.fY + 16.0), boxWidth, boxHeight, color1.dwHexColor);
				pCore->GetGraphics()->DrawBox((vecScreen.fX - (boxWidth / 2) + 2.0), (vecScreen.fY + 18.0), (boxWidth - 4.0), (boxHeight - 4.0), color2.dwHexColor);
				pCore->GetGraphics()->DrawBox((vecScreen.fX - (boxWidth / 2) + 2.0), (vecScreen.fY + 18.0), healthWidth, (boxHeight - 4.0), color3.dwHexColor);
			}
		}
	}
}