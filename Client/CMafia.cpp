/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMafia.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CMainMenu.h"
#include "CGUI.h"

#include "CM2PhysFS.h"
#include "engine/CM2Navigation.h"

#include "CMafia.h"

#include "CM2Hud.h"

#include "CString.h"
#include "CLua.h"

#include "CCore.h"

#include "COffsets.h"

#include "Math/CVector3.h"

#include "CNetworkPlayer.h"
#include "CLocalPlayer.h"

#include "CPlayerManager.h"
#include "CLocalPlayer.h"

#include "CM2Camera.h"

#include "GUI/ChatBox.h"

#include "CEvents.h"

bool						bOldChatWindowState;

CMafia::CMafia( void )
	: m_strWeather()
	, m_bFocus(false)
	, m_bMapOpen(false)
	, m_bSummer(true)

	, m_pFileSystem(nullptr)
	, m_pNavigation(nullptr)
{
}

CMafia::~CMafia( void )
{
	SAFE_DELETE( m_pFileSystem );
	SAFE_DELETE( m_pNavigation );
}

void CMafia::LoadPointers( void )
{
	m_pFileSystem = new CM2PhysFS( *(M2PhysFS **)COffsets::VAR_CPhysFS );
	m_pNavigation = new CM2Navigation( *(M2Navigation **)COffsets::VAR_CNavigation );
}

void CMafia::FadeSound( bool bFadeOut, float fTimeInSeconds )
{
	DWORD C_SoundManager = *(DWORD *)0x1AAF2D8;
	DWORD C_SoundManager__VFTable__FadeSound = 0x13E78E0;

	_asm push fTimeInSeconds;
	_asm push bFadeOut;
	_asm mov ecx, C_SoundManager;
	_asm call C_SoundManager__VFTable__FadeSound;
}

void CMafia::InfoAreaShow( bool bToggle )
{
	CCore::Instance()->GetHud()->InfoAreaShow( bToggle );
}

void CMafia::MoneyShow( bool bToggle )
{
	CLua::Executef( "game.hud:MoneyShow( %s )", bToggle ? "true" : "false" );
}

void CMafia::ScoreShow( bool bToggle )
{
	CCore::Instance()->GetHud()->ScoreShow( bToggle );
}

void CMafia::SpeedoMeterShow( bool bToggle )
{
	CLua::Executef( "game.hud:SpeedoMeterShow( %s )", bToggle ? "true" : "false" );
}

void CMafia::ActionButtonsShow( bool bToggle )
{
	CCore::Instance()->GetHud()->ActionButtonsShow( bToggle );
}

void CMafia::PlayMusic( const char * szMusic )
{
	CLua::Executef( "game.game:PlayMusic( \"%s\", true, 1, 1, 1 )", szMusic );
}

void CMafia::StopMusic( void )
{
	CLua::Execute( "game.game:StopMusic(1)" );
}

void CMafia::ChangeWeather( String strWeather, int iChangeTime )
{
	DWORD C_Game__SetWeatherTemplate = 0xB29F50;
	const char * szWeather = strWeather.Get();

	_asm push szWeather;
	_asm call C_Game__SetWeatherTemplate;

	m_strWeather = strWeather;
}

void CMafia::ChangeSeason ( bool bSummer )
{
	CVector3 vecPosition;
	CLocalPlayer::Instance()->GetPosition ( &vecPosition );

	if ( bSummer && !m_bSummer )
	{
		m_bSummer = true;
		CLua::Execute ( "game.game:GetActivePlayer():SetPos(0)" );
		CLua::Execute ( "game.sds:ActivateStreamMapLine( \"free_summer_load\" )" );
		CLua::Execute ( "game.gfx:SetWeatherTemplate( \"DT_RTRclear_day_afternoon\" )" );
		CLua::Executef ( "game.game:GetActivePlayer():SetPos(Math:newVector(%f, %f, %f))", vecPosition.fX, vecPosition.fY, vecPosition.fZ );
	}
	else if ( !bSummer && m_bSummer )
	{
		m_bSummer = false;
		CLua::Execute ( "game.game:GetActivePlayer():SetPos(0)" );
		CLua::Execute ( "game.sds:ActivateStreamMapLine( \"free_winter_load\" )" );
		CLua::Execute ( "game.gfx:SetWeatherTemplate( \"DTFreeRideDaySnow\" )" );
		CLua::Executef ( "game.game:GetActivePlayer():SetPos(Math:newVector(%f, %f, %f))", vecPosition.fX, vecPosition.fY, vecPosition.fZ );
	}
}

void CMafia::Spawn( bool bFade )
{
	if (CGUI::Instance()->GetMainMenu()) {
		CGUI::Instance()->GetMainMenu()->SetVisible(false);
		CGUI::Instance()->SetCursorVisible(false);
		m_bSummer = !m_bSummer;
		ChangeSeason(!m_bSummer);
	}

	CM2Hud *pHud = CCore::Instance()->GetHud();

	SetMouseSensitivityMultiplier( 1.0f );

	if( bFade )
	{
		pHud->FadeIn( 1000 );
		FadeSound( false, 1 );
	}

	CLocalPlayer::Instance()->HandleSpawn( false );

	// Disable npc generators and far ambients
	SwitchGenerators( false, false );

	// Remove vehicles from garage
	CLua::Execute( "game.garage:CheatAddCars( false )" );
	CLua::Execute( "game.garage:CheatAbandonCars()" );
	CLua::Execute( "game.garage:SetMaxGaragePlaces( 0 )" );

	//CLua::Execute( "game.shop:SetAllShopExplored()" );

	pHud->Show( true );
	CCore::Instance()->GetHud()->ShowLowHealthFX( false );
	ActionButtonsShow( false );

	bool bIsSummer = CCore::Instance()->IsSummer();

	CCore::Instance()->GetGame()->ChangeSeason ( bIsSummer );
	SetSummerRadio ( bIsSummer );
	CLocalPlayer::Instance()->LockControls( false );
	CCore::Instance()->GetCamera()->LockControl( false );
	CCore::Instance()->GetHud()->StopGPS();

	CM2Ped *ped = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed();
	if (ped) {
		ped->RemoveAllWeapons();
		ped->SetSelectedWeapon(1);
	}

	// Don't reload game after death
	*(BYTE *)0x1BB057D = 1;
}

void CMafia::SwitchGenerators( bool bPeds, bool bFarAmbients )
{
	// Toggle random pedestrians
	DWORD dwFunc = 0xAB5610;
	_asm push bPeds;
	_asm call dwFunc;

	// Toggle far ambients
	dwFunc = 0xAC4440;
	_asm push bFarAmbients;
	_asm call dwFunc;
}

void CMafia::CreateTimer( float fTime )
{
	CM2HudTimer *pTimer = CCore::Instance()->GetHud()->GetHudTimer();
	assert ( pTimer );

	pTimer->Toggle ( true );
	pTimer->SetTime ( fTime );
}

void CMafia::StartTimer( void )
{
	CM2HudTimer *pTimer = CCore::Instance()->GetHud()->GetHudTimer();
	assert ( pTimer );

	pTimer->Start();
}

void CMafia::StopTimer( void )
{
	CM2HudTimer *pTimer = CCore::Instance()->GetHud()->GetHudTimer();
	assert ( pTimer );

	pTimer->Stop();
}

void CMafia::SetMouseSensitivityMultiplier( float fSensitivity )
{
	*(float *)0x1BAB62C = fSensitivity;
}

float CMafia::GetMouseSensitivityMultiplier( void )
{
	return *(float *)0x1BAB62C;
}

void CMafia::SetRadioContent( const char * szChannel, const char * szContent )
{
	DWORD C_Radio__SetContent = 0xABFAA0;
	const char* szUnknown = "all";

	_asm push szContent;
	_asm push szUnknown;
	_asm push szChannel;
	_asm call C_Radio__SetContent;
}

void CMafia::SetSummerRadio( bool bSummerRadio )
{
	if( bSummerRadio )
	{
		SetRadioContent( "Empire", "Empire_07010" );
		SetRadioContent( "Classic", "Classic_07010" );
		SetRadioContent( "Delta", "Delta_07010" );
	}
	else
	{
		SetRadioContent( "Empire", "Empire_02004" );
		SetRadioContent( "Classic", "Classic_02004" );
		SetRadioContent( "Delta", "Delta_02004" );
	}
}

void * CMafia::allocate( int iSize )
{
	void* pMemory = NULL;

	_asm push iSize;
	_asm call COffsets::FUNC_CGame__Allocate;
	_asm add esp, 4;
	_asm mov pMemory, eax;

	return pMemory;
}

void CMafia::free( void* pMemory )
{
	_asm push pMemory;
	_asm call COffsets::FUNC_CGame__Free;
	_asm add esp, 4;
}

void * CMafia::allocateBuffer( int iLen )
{
	void* pBuffer = NULL;

	_asm push iLen;
	_asm call COffsets::FUNC_CGame__AllocateBuffer;
	_asm add esp, 4;
	_asm mov pBuffer, eax;

	return pBuffer;
}

bool CMafia::OpenMap ( bool bOpen )
{
	CCore *pCore = CCore::Instance();

	ChatBox *const chatBox = ChatBox::Instance();
	if ( bOpen )
	{
		// Trigger the script event
		if ( CEvents::Instance()->Call( "onClientOpenMap" ).GetInteger () == 1 )
		{
			// Open the map
			CLua::Execute ( "game.gui:OpenMap(1)" );

			// Get the chat window visibility
			bOldChatWindowState = chatBox->IsVisible ();

			// Hide the chat window
			chatBox->SetVisible ( false );

			// Mark as opened
			m_bMapOpen = true;

			return true;
		}
	}
	else
	{
		// Restore the chat window visibility
		chatBox->SetVisible ( bOldChatWindowState );

		// Call the script event
		CEvents::Instance()->Call( "onClientCloseMap" );

		// Mark as not opened
		m_bMapOpen = false;

		return true;
	}

	return false;
}

void CMafia::OnGameStart( void )
{
	CLocalPlayer::Instance()->LockControls( true );

	CCore::Instance()->GetCamera()->LockControl( true );

	FadeSound( true, 0 );
}

void CMafia::DisableTranslocator(bool disable)
{
	CLua::Executef("game.game:DisableTranslocator(%b)", disable);
}