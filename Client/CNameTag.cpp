/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CNameTag.cpp
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#include "CNameTag.h"

#include "CCore.h"

#include "CGraphics.h"

#include "CGUI.h"
#include "CServerBrowser.h"
#include "CMainMenu.h"

#include "Math/CVector3.h"
#include "Math/CMaths.h"

#include "CString.h"

#include "CCommon.h"

#include "CPed.h"
#include "CPedManager.h"
#include "engine/CM2Entity.h"
#include "engine/CM2Ped.h"

#include "CColor.h"

#include "CPlayerManager.h"

#include "CNetworkPlayer.h"
#include "CLocalPlayer.h"
#include "CRemotePlayer.h"


extern	CCore			* pCore;

const float PLAYER_NAMETAG_Z_OFFSET = 2.0f;
const float PED_NAMETAG_Z_OFFSET	= 2.0f;

const float NAMETAG_SCALE			= 1.0f;

const float RENDER_DISTANCE_PLAYER	= 35.0f;
const float RENDER_DISTANCE_PED		= 10.0f;
const float BOX_WIDTH				= 68.0f;
const float BOX_HEIGHT				= 10.0f;

/// Constructor.
CNameTag::CNameTag(void)
{
}

/// Destructor.
CNameTag::~CNameTag(void)
{
}

/// Draw nametags for players and peds.
void CNameTag::Draw(void)
{
	CGUI *pGUI = pCore->GetGUI();
	if ( pGUI->GetMainMenu()->IsVisible() || pGUI->GetServerBrowser()->IsVisible() )
		return;

	DrawPlayer();
	DrawPed();
}

/// Draw ped nametags.
void CNameTag::DrawPed(void)
{
	CVector3 localPos;

	CLocalPlayer *pLocalPlayer = pCore->GetPlayerManager()->GetLocalPlayer();
	if (! pLocalPlayer)
		return;
	pLocalPlayer->GetPosition(&localPos);

	for ( EntityId i = 0; i < MAX_PEDS; ++i )
	{
		CPed *pPed = CCore::Instance()->GetPedManager()->Get(i);

		if ( !pPed )
			continue;

		CM2Ped *pGamePed = pPed->GetPed();

		if ( !pGamePed->IsOnScreen() )
			continue;

		CVector3 pedPos;
		pGamePed->GetPosition ( &pedPos );

		if (!Math::IsDistanceBetweenPointsLessThen(localPos, pedPos, RENDER_DISTANCE_PED)) {
			continue;
		}

		const String &nick = pPed->GetNick();
		float dimensionWidth = pCore->GetGraphics()->GetTextWidth ( nick, NAMETAG_SCALE, "tahoma-bold" );

		pedPos.fZ += PED_NAMETAG_Z_OFFSET;

		CVector3 vecScreen;
		pCore->GetGraphics()->WorldToScreen(pedPos, &vecScreen);

		pCore->GetGraphics()->DrawText ( (vecScreen.fX - dimensionWidth / 2) + 1, vecScreen.fY + 1, (DWORD)0xFFFFFFFF, NAMETAG_SCALE, "tahoma-bold", false, nick );
	}
}

/// Draw players nametags.
void CNameTag::DrawPlayer(void)
{
	static const CColor colBackground		( 0, 0, 0, 160 );
	static const CColor colInnerBackground	( 0, 110, 0, 160 );
	static const CColor colContent			( 0, 255, 0, 160 );

	CPlayerManager *pPlayerManager = pCore->GetPlayerManager();
	assert(pPlayerManager);

	CVector3 localPos;
	CLocalPlayer *pLocalPlayer = pPlayerManager->GetLocalPlayer();
	if (! pLocalPlayer)
		return;

	if (!pLocalPlayer->IsRenderingNametags() && !pLocalPlayer->IsRenderingHealthbar()) {
		return;
	}

	pLocalPlayer->GetPosition ( &localPos );

	CGraphics *pGraphics = pCore->GetGraphics();

	EntityId localPlayerId = pLocalPlayer->GetId();
	for ( EntityId i = 0; i < MAX_PLAYERS; ++i )
	{
		if ( i == localPlayerId )
			continue;

		CRemotePlayer *pRemotePlayer = pPlayerManager->Get ( i );
		if ( !pRemotePlayer )
			continue;

		CM2Ped *pPed = pRemotePlayer->GetPlayerPed();
		if ( !pPed || !pPed->IsOnScreen() )
			continue;

		CVector3 playerPos;
		pRemotePlayer->GetPosition ( &playerPos );

		if (!Math::IsDistanceBetweenPointsLessThen(localPos, playerPos, RENDER_DISTANCE_PLAYER)) {
			continue;
		}

		String text;
		text.Format ( "%s (%d)", pRemotePlayer->GetNick().Get(), i );

		float dimensionWidth = pGraphics->GetTextWidth ( text, NAMETAG_SCALE, "tahoma-bold" );

		int healthWidth = (int)( ( ( Math::Clamp< float > ( 0.0, pRemotePlayer->GetHealth(), 720 ) * 100 ) / 720 ) / 100 * ( BOX_WIDTH - 4 ) );


		playerPos.fZ += PLAYER_NAMETAG_Z_OFFSET;

		CVector3 vecScreen;
		pCore->GetGraphics()->WorldToScreen ( playerPos, &vecScreen );

		if (pLocalPlayer->IsRenderingNametags())
			pGraphics->DrawText((vecScreen.fX - dimensionWidth / 2) + 1, vecScreen.fY + 1, pRemotePlayer->GetColour(), NAMETAG_SCALE, "tahoma-bold", false, text.Get());

		if (pLocalPlayer->IsRenderingHealthbar()) {
			pGraphics->DrawBox ( (vecScreen.fX - (BOX_WIDTH / 2)), (vecScreen.fY + 16.0), BOX_WIDTH, BOX_HEIGHT, colBackground.ToUint() );
			pGraphics->DrawBox ( (vecScreen.fX - (BOX_WIDTH / 2) + 2.0), (vecScreen.fY + 18.0), (BOX_WIDTH - 4.0), (BOX_HEIGHT - 4.0), colInnerBackground.ToUint() );
			pGraphics->DrawBox ( (vecScreen.fX - (BOX_WIDTH / 2) + 2.0), (vecScreen.fY + 18.0), healthWidth, (BOX_HEIGHT - 4.0), colContent.ToUint() );
		}
	}
}
