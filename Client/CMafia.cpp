/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMafia.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;
bool						bOldChatWindowState;

CMafia::CMafia( void )
{
	// Reset
	m_bMapOpen = false;
	m_bSummer = true;
}

CMafia::~CMafia( void )
{
	// Delete the file system
	SAFE_DELETE( m_pFileSystem );
	
	// Delete the navigation
	SAFE_DELETE( m_pNavigation );
}

void CMafia::LoadPointers( void )
{
	// Load the physfs pointer
	m_pFileSystem = new CM2PhysFS( *(M2PhysFS **)COffsets::VAR_CPhysFS );

	// Load the navigation pointer
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
	pCore->GetHud()->InfoAreaShow( bToggle );
}

void CMafia::MoneyShow( bool bToggle )
{
	CLua::Executef( "game.hud:MoneyShow( %s )", bToggle ? "true" : "false" );
}

void CMafia::ScoreShow( bool bToggle )
{
	pCore->GetHud()->ScoreShow( bToggle );
}

void CMafia::SpeedoMeterShow( bool bToggle )
{
	CLua::Executef( "game.hud:SpeedoMeterShow( %s )", bToggle ? "true" : "false" );
}

void CMafia::ActionButtonsShow( bool bToggle )
{
	pCore->GetHud()->ActionButtonsShow( bToggle );
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

	// Store the weather
	m_strWeather = strWeather;
}

void CMafia::ChangeSeason ( bool bSummer )
{
	// Get the current player position
	CVector3 vecPosition;
	pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition ( &vecPosition );

	// Are we changing to summer?
	if ( bSummer && !m_bSummer )
	{
		// Set summer
		m_bSummer = true;

		// Load summer
		CLua::Execute ( "game.game:GetActivePlayer():SetPos(0)" );
		CLua::Execute ( "game.sds:ActivateStreamMapLine( \"free_summer_load\" )" );
		CLua::Execute ( "game.gfx:SetWeatherTemplate( \"DT_RTRclear_day_afternoon\" )" );
		CLua::Executef ( "game.game:GetActivePlayer():SetPos(Math:newVector(%f, %f, %f))", vecPosition.fX, vecPosition.fY, vecPosition.fZ );
	}
	else if ( !bSummer && m_bSummer )
	{
		// Set winter
		m_bSummer = false;

		// Load winter
		CLua::Execute ( "game.game:GetActivePlayer():SetPos(0)" );
		CLua::Execute ( "game.sds:ActivateStreamMapLine( \"free_winter_load\" )" );
		CLua::Execute ( "game.gfx:SetWeatherTemplate( \"DTFreeRideDaySnow\" )" );
		CLua::Executef ( "game.game:GetActivePlayer():SetPos(Math:newVector(%f, %f, %f))", vecPosition.fX, vecPosition.fY, vecPosition.fZ );
	}
}

void CMafia::Spawn( bool bFade )
{
	// Hide the main menu
	if( pCore->GetGUI()->GetMainMenu() )
		pCore->GetGUI()->GetMainMenu()->SetVisible( false );

	// Change the weather
	//ChangeWeather ( (pCore->IsSummer () ? "DT_RTRclear_day_afternoon" : "DTFreeRideDaySnow"), 500 );
	//ChangeWeather ( "DT_RTRclear_day_afternoon", 500 );
	
	// Set the default sensitity multiplier
	SetMouseSensitivityMultiplier( 1.0f );

	// Should we fade in ?
	if( bFade )
	{
		// Re-fade the screen
		pCore->GetHud()->FadeIn( 1000 );

		// Fade in the sound
		FadeSound( false, 1 );
	}

	// Handle the spawn with the localplayer
	pCore->GetPlayerManager()->GetLocalPlayer()->HandleSpawn( false );

	// Disable npc generators and far ambients
	SwitchGenerators( false, false );

	// Remove vehicles from garage
	CLua::Execute( "game.garage:CheatAddCars( false )" );
	CLua::Execute( "game.garage:CheatAbandonCars()" );
	CLua::Execute( "game.garage:SetMaxGaragePlaces( 0 )" );

	// Set all shops found
	CLua::Execute( "game.shop:SetAllShopExplored()" );

	// Enable the hud
	pCore->GetHud()->Show( true );

	// Disable low-health FX
	//pCore->GetHud()->ShowLowHealthFX( false );

	// Disable action buttons
	//ActionButtonsShow( false );

	// Load season
	pCore->GetGame()->ChangeSeason ( pCore->IsSummer () );

	// Set the radio to the summer setting
	SetSummerRadio ( pCore->IsSummer () );

	// Unlock player controls
	pCore->GetPlayerManager()->GetLocalPlayer()->LockControls( false );

	// Unlock camera control
	pCore->GetCamera()->LockControl( false );

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
	// Toggle the timer
	pCore->GetHud()->GetHudTimer()->Toggle( true );

	// Set the timer time
	pCore->GetHud()->GetHudTimer()->SetTime( fTime );
}

void CMafia::StartTimer( void )
{
	// Start the timer
	pCore->GetHud()->GetHudTimer()->Start();
}

void CMafia::StopTimer( void )
{
	// Start the timer
	pCore->GetHud()->GetHudTimer()->Stop();
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
		SetRadioContent( "Classic", "Empire_07010" );
		SetRadioContent( "Delta", "Empire_07010" );
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
	// Should we open the map?
	if ( bOpen )
	{
		// Trigger the script event
		if ( pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientOpenMap" ).GetInteger () == 1 )
		{
			// Open the map
			CLua::Execute ( "game.gui:OpenMap(1)" );

			// Get the chat window visibility
			bOldChatWindowState = pCore->GetChat()->IsVisible ();

			// Hide the chat window
			pCore->GetChat()->SetVisible ( false );

			// Mark as opened
			m_bMapOpen = true;

			return true;
		}
	}
	else
	{
		// Restore the chat window visibility
		pCore->GetChat()->SetVisible ( bOldChatWindowState );

		// Call the script event
		if( pCore->GetClientScriptingManager() )
			pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientCloseMap" );

		// Mark as not opened
		m_bMapOpen = false;

		return true;
	}

	return false;
}

void CMafia::OnGameStart( void )
{
	// Lock player controls
	pCore->GetPlayerManager()->GetLocalPlayer()->LockControls( true );

	// Lock camera control
	pCore->GetCamera()->LockControl( true );

	// Mute the sound
	FadeSound( true, 0 );
}