/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CCore.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"
#include	<ShellAPI.h>

extern	CCore			* pCore;
IDirect3DStateBlock9	* pStateBlock = NULL;
CSquirrelArguments		pArguments;
bool					bDeviceLost = false;
bool					bInputHookCreated = false;

extern					DWORD sub_CD00A0__ECX;

#define					EXE_VERSION_SKIDROW		0xAE196A1E
#define					EXE_VERSION_STEAM		0xB1ADE8F2

DWORD					GetModelHasFromModelName_ECX;

CCore::CCore( void )
{
	// Get the game directory from the registry
	char szGameDir[MAX_PATH];
	SharedUtility::ReadRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia 2 Multiplayer", "GameDir", NULL, szGameDir, sizeof(szGameDir) );
	SetGameDirectory( szGameDir );

	// Get the mod directory from the registry
	char szModDir[MAX_PATH];
	SharedUtility::ReadRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia 2 Multiplayer", "InstallLocation", NULL, szModDir, sizeof(szModDir) );
	SetModDirectory( szModDir );

	// Reset
	m_bInitialised = false;
	m_bCaptureScreenshot = false;
	m_bRenderNetworkStats = false;
	m_bConnectionProblem = false;

	// Mark the game as not loaded
	SetGameLoaded( false );

	// Mark as multiplayer not started
	SetMultiplayerStarted( false );

	// Set default server info
	SetServerName( "M2-MP Server" );
	SetServerMaxPlayers( 0 );
	SetGlobalSpeedLimited ( false );
	SetSummer ( true );

	// Reset instances
	m_pNetworkModule = NULL;
	m_pPlayerManager = NULL;
	m_pCamera = NULL;
	m_pHud = NULL;
	m_pClientScriptingManager = NULL;
	m_pFPSCounter = NULL;
	m_pTimerManager = NULL;
	m_pFileTransferManager = NULL;
	m_pBlipManager = NULL;
	m_pVehicleManager = NULL;
	m_pPedManager = NULL;
	m_pGame = NULL;
	m_pGUI = NULL;
	m_pKeyBinds = NULL;
	m_pStreamer = NULL;
	m_pModelManager = NULL;
	m_pUpdater = NULL;
}

CCore::~CCore( void )
{
#ifdef _DEBUG
	CLogFile::Printf( "CCore::~CCore" );
#endif

	// Delete the network manager
	SAFE_DELETE( m_pNetworkModule );

	// Uninstall anti-cheat
	CWPMHook::Uninstall();

	// End the process
	TerminateProcess( GetCurrentProcess(), 0 );
}

bool CCore::Initialise( void )
{
	// Are we already initialsed?
	if( m_bInitialised )
		return false;

	// Open the log file
	CLogFile::Open( SharedUtility::GetAbsolutePath( "logs\\core.log" ).Get() );

	// Did the anti-cheat fail to start?
	if( !CWPMHook::Install() )
	{
		MessageBox( NULL, "Failed to start anti-cheat module.\n\nCan't start Mafia 2 Multiplayer", "Mafia 2 Multiplayer - Error", MB_OK|MB_ICONERROR );
		return false;
	}

	// Calculate the exe version
	CFileChecksum pChecksum;
	pChecksum.Calculate( String( "%s\\pc\\Mafia2.exe", m_strGameDirectory.Get() ) );

	CLogFile::Printf( "EXE signature: 0x%p", pChecksum.GetChecksum() );

	// Is this not the v1 exe?
	if( pChecksum.GetChecksum() != EXE_VERSION_SKIDROW )
	{
		if ( MessageBox ( NULL, "Invalid game version detected.\n\nCan't start Mafia 2 Multiplayer\n\nDo you want to goto the downloads page now?", "Mafia 2 Multiplayer - Error", MB_OK|MB_ICONERROR ) == MB_OK ) {
			ShellExecute ( NULL, "open", "http://m2-multiplayer.com/page/download", NULL, NULL, SW_SHOWNORMAL );
		}
		return false;
	}

	// Check the game files
	if( !CGameFiles::CheckFiles() )
	{
		MessageBox( NULL, String( "%s\n\nPlease reinstall Mafia 2 or Mafia 2 Multiplayer", CGameFiles::GetLastError().Get() ).Get(), "Mafia 2 Multiplayer - Error", MB_OK|MB_ICONERROR );
		return false;
	}

	// Load the settings file
	CSettings::Load( "settings.xml" );
	CSettings::ParseCommandLine ( GetCommandLine() );

	CLogFile::Printf ( "IP: %s, Port: %s", CVAR_GET_STRING ( "ip" ), CVAR_GET_STRING ( "port" ) );

	// Get the applicatin base address
	m_uiBaseAddress = (unsigned int)GetModuleHandle( NULL );

	// Subtract the image size from the base address
	m_uiBaseAddress -= 0x400000;

	// Create the game instance
	m_pGame = new CMafia;

	// Initialise the offsets
	COffsets::Initialise( 0, m_uiBaseAddress );

	// Apply the patches
	CPatches::Initialise ();

	// Initialise lua
	CLua::Initialise ();

	// Create the graphics instance
	m_pGraphics = new CGraphics;

	// Create the fps counter instance
	m_pFPSCounter = new CFPSCounter;

	// Create the key binds instance
	m_pKeyBinds = new CKeyBinds;

	// Create the streamer instance
	m_pStreamer = new CStreamer;

	// Create the model mgr instance
	m_pModelManager = new CModelManager;

	return true;
}

void CCore::Shutdown( void )
{
	// Are we connected to the network?
	if( pCore->GetNetworkModule()->IsConnected() )
	{
		// Disconnect and shutdown RakNet
		pCore->GetNetworkModule()->Disconnect( false );
	}

	// Delete ourselfs
	delete this;
}

void CCore::OnGameLoad( void )
{
#ifdef _DEBUG
	CLogFile::Printf ( "CCore::OnGameLoad" );
#endif

	// Is the game already loaded?
	if( IsGameLoaded() )
		return;

	// Load engine pointers
	IE::LoadPointers ();

	// Create and set the camera instance
	pCore->SetCamera( new CM2Camera( *(M2Camera **)COffsets::VAR_CCamera ) );

	// Create and set the hud instance
	pCore->SetHud( new CM2Hud( *(M2Hud **)COffsets::VAR_CHud ) );

	// Setup the game pointers
	pCore->GetGame()->LoadPointers ();

#ifdef DEBUG
	CLogFile::Printf ( "LocalPed: 0x%p", IE::GetGame()->m_pLocalPed);
#endif

	// Disable the game over writting the localplayer model manager pointer
	// Don't move this into CPatches or else the localplayer model manager won't be set!
	CPatcher::InstallNopPatch ( 0x5A650C, 6 );

	// Start multiplayer
	StartMultiplayer ();

	// Startup the network module
	m_pNetworkModule->Startup ();

	// Call the game start event
	m_pGame->OnGameStart ();

	// Mark the game as loaded
	SetGameLoaded( true );
}

void CCore::OnGamePreLoad( void )
{
	// Is the game loaded?
	if( IsGameLoaded() )
		return;

	// Deinitialsie the loading screen
	CLoadingScreen::Finish();

	// Cleanup any decompressed files
	CGameFiles::CleanFiles();

	// Create the network module instance
	m_pNetworkModule = new CNetworkModule;

	// Hide the chat window
	m_pChat->SetVisible( false );

	// Show the main menu
	m_pGUI->GetMainMenu()->SetVisible( true );

	// Load the game settings
	m_pGUI->GetSettings()->LoadSettings ();
}

void CCore::OnDeviceCreate( IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters )
{
	// Initialise the graphics module
	m_pGraphics->Initialise( pDevice );

	// Create the gui instance
	m_pGUI = new CGUI( pDevice );

	// Setup m2mp gui instances
	m_pGUI->SetupGUI();

	// Initialise the loading screen
	CLoadingScreen::Start();

	// Create the chat instance
	m_pChat = new CChat( m_pGUI->GetCEGUI(), 30.0f, 25.0f );
	m_pChat->AddInfoMessage( "%s %s started.", MOD_NAME, MOD_VERS_STR );

	// Refresh server browser
	m_pGUI->GetServerBrowser()->Refresh ();

#ifdef _DEBUG
	CLogFile::Printf ( "CCore::OnDeviceCreate - Done" );
#endif
}

void CCore::OnDeviceLost( IDirect3DDevice9 * pDevice )
{
	// Have we already lost the device?
	if ( bDeviceLost )
	{
		CLogFile::Printf ( "ERROR - Direct3D device lost, but some objects were not released. Game is in an unrecoverable state now. Exiting." );
		MessageBox ( NULL, "Direct3D device lost, but some objects were not released. Game is in an unrecoverable state now. Exiting.", "Direct3D Device Error", MB_ICONERROR|MB_OK );
		TerminateProcess ( GetCurrentProcess(), 0 );
	}

	// Release the saved stateblock
	if( pStateBlock )
		SAFE_RELEASE( pStateBlock );

	// Mark as lost device
	bDeviceLost = true;

	// Let the graphics module know the device is lost
	if( m_pGraphics )
		m_pGraphics->OnLostDevice( pDevice );

	// Is the gui instance valid?
	if( m_pGUI )
	{
		// Notify the main menu
		m_pGUI->GetMainMenu()->OnDeviceLost ();

		CLogFile::Printf ( "Core - DeviceLost - 2.1" );

		// Notify the gui
		m_pGUI->OnDeviceLost ();

		CLogFile::Printf ( "Core - OnDeviceLost - 2.2" );
	}

	// Let the video settings know we have the device back
	CM2VideoSettings::SetWaitingForDeviceReset ( true );

#ifdef _DEBUG
	CLogFile::Printf ( "CCore::OnDeviceLost - Done" );
#endif
}

void CCore::OnDeviceReset( IDirect3DDevice9 * pDevice )
{
	// Let the graphics module know the device is reset
	if( m_pGraphics )
		m_pGraphics->OnRestoreDevice( pDevice );

	// Let the gui know
	if( m_pGUI )
		m_pGUI->OnDeviceReset();

	// Let the video settings know we have the device back
	CM2VideoSettings::SetWaitingForDeviceReset ( false );

	// Call the script event
	if( m_pClientScriptingManager )
		m_pClientScriptingManager->GetEvents()->Call( "onClientDeviceReset" );

	// Mark as not lost device
	bDeviceLost = false;
}

void CCore::OnDevicePreRender( void )
{
	// Call the vehicle manager pre processor
	if ( m_pVehicleManager )
		m_pVehicleManager->PreProcess ();

	// Call the script event
	if( m_pClientScriptingManager && !m_pGUI->GetMainMenu()->IsVisible () )
		m_pClientScriptingManager->GetEvents()->Call( "onClientFramePreRender" );
}

void CCore::OnDeviceRender( void )
{
	// Has the device been lost?
	if( bDeviceLost )
		return;

	// Call the device begin scene
	m_pGraphics->GetDevice()->BeginScene ();

	// Try create the state block
	if( FAILED( m_pGraphics->GetDevice()->CreateStateBlock( D3DSBT_ALL, &pStateBlock ) ) )
		return;

	// Capture the device state
	pStateBlock->Capture ();

	// Call our begin scene
	if ( m_pGraphics )
		m_pGraphics->OnSceneBegin ();

	// Is the scripting manager active?
	if( m_pClientScriptingManager && !m_pGUI->GetMainMenu()->IsVisible () )
	{
		// Call the script event
		pArguments.push( false );
		m_pClientScriptingManager->GetEvents()->Call( "onClientFrameRender", &pArguments );
		pArguments.clear();
	}

	// Render the loading screen
	CLoadingScreen::Render ();

	// Render the chat window
	if ( m_pChat->IsVisible () )
		m_pChat->Render ();

	// Render the mainmenu
	if ( m_pGUI->GetMainMenu () && m_pGUI->GetMainMenu()->IsVisible () )
		m_pGUI->GetMainMenu()->Render ();

	// Clean out any unused textures
	if ( m_pGraphics )
		m_pGraphics->ExpireCachedTextures ();

	// Render the gui
	if ( m_pGUI )
		m_pGUI->Render ();

	// Is there a connection problem?
	if ( IsConnectionProblem () )
		pCore->GetGraphics()->DrawText ( (pCore->GetGUI()->GetCEGUI()->GetResolution().fX - pCore->GetGraphics()->GetTextWidth("Connection Problem", 1.0f, "tahoma-bold") - 5), 5, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, "tahoma-bold", true, "Connection Problem" );

	// Call the vehicle manager processor
	if ( m_pVehicleManager )
		m_pVehicleManager->Process ();

	// Pulse the ped manager
	if( m_pPedManager )
		m_pPedManager->Pulse();

	if ( m_pPlayerManager && m_pPlayerManager->GetLocalPlayer() && m_pPlayerManager->GetLocalPlayer()->IsSpawned() ) {
		CM2Ped * pPlayerPed = m_pPlayerManager->GetLocalPlayer()->GetPlayerPed();
		C_PlayerControls playerControls = pPlayerPed->GetPed()->m_playerControls;

		pCore->GetGraphics()->DrawText ( 300, 300, D3DCOLOR_ARGB(255,255,0,0), 1.0f, "tahoma-bold", true, "Is Moving: %s\nMovement State: %d\nModifiers: %d\nMouse Flags: %d\nKeyboard Flags: %d\nIs Aiming: %s\nIs Crouching: %s", playerControls.m_bIsMoving ? "Yes" : "No", playerControls.m_ePlayerMovementState, playerControls.m_byteModifiers, playerControls.m_byteMouseFlags, playerControls.m_byteKeyboardFlags, playerControls.m_bIsAiming ? "Yes" : "No", playerControls.m_bIsCrouching ? "Yes" : "No" );
	}

	if ( m_pPlayerManager && m_pPlayerManager->GetLocalPlayer() && m_pPlayerManager->GetLocalPlayer()->IsInVehicle() ) {
		CNetworkVehicle * pNetworkVehicle = m_pPlayerManager->GetLocalPlayer()->GetVehicle ();
		CVector3 vecSpeed = pNetworkVehicle->GetVehicle()->GetVehicle()->m_vecMoveSpeed;

		pCore->GetGraphics()->DrawText ( 300, 300, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, "tahoma-bold", true, "Fuel: %f, Speed: %f (%f, %f, %f), Wheels: %f (%f), Lights: %s", pNetworkVehicle->GetFuel (), pNetworkVehicle->GetSpeed(), vecSpeed.fX, vecSpeed.fY, vecSpeed.fZ, pNetworkVehicle->GetVehicle()->GetSteer(), pNetworkVehicle->GetSteer(), pNetworkVehicle->GetVehicle()->GetLightState() ? "Enabled" : "Disabled" );
	}

	// Was the hide stuff key pressed?
	if ( GetAsyncKeyState ( VK_F10 ) & 0x1 )
	{
		// Toggle the hud
		m_pHud->Show ( !m_pHud->IsShowing () );

		// Toggle the chat
		m_pChat->SetVisible ( !m_pChat->IsVisible () );

		// Toggle the local player model
		m_pPlayerManager->GetLocalPlayer()->GetPlayerPed()->ShowModel ( !m_pPlayerManager->GetLocalPlayer()->GetPlayerPed()->IsModelShowing () );
	}

	// Was the net stats key pressed?
	if( GetAsyncKeyState( VK_F11 ) & 0x1 )
	{
		// Toggle the render
		m_bRenderNetworkStats = !m_bRenderNetworkStats;
	}

	// Was the screenshot key pressed?
	if( GetAsyncKeyState( VK_F12 ) & 0x1 )
	{
		// Flag as takescreenshot for next frame
		TakeScreenshot ();
	}

	// Are we capturing a screenshot?
	if( m_bCaptureScreenshot && !CScreenShot::IsSaving() )
	{
		// Get the screen dimensions
		unsigned long ulScreenWidth = m_pCamera->GetGameCamera()->m_iWindowWidth;
		unsigned long ulScreenHeight = m_pCamera->GetGameCamera()->m_iWindowHeight;

		// Create the data buffer
		unsigned char * ucData = new unsigned char [ ulScreenHeight * (ulScreenWidth * 4) ];

		// Try get the frontbuffer data
		if( m_pGraphics->GetFrontBufferPixels( &ucData ) )
		{
			// Write the screenshot (The worker thread will cleanup ucData once done!)
			CScreenShot::BeginWrite( ucData );
		}

		// Mark as not capturing screenshot
		m_bCaptureScreenshot = false;
	}

	// Should we render the network stats?
	if( m_bRenderNetworkStats )
	{
		// Draw the network stats
		m_pGraphics->DrawText( (m_pGUI->GetCEGUI()->GetResolution().fX - 275), 30, 0xFFFFFFFF, 1.0f, "tahoma-bold", false, DT_NOCLIP, CNetworkStats::GetStats().Get() );
	}

	// Is the scripting manager active?
	if( m_pClientScriptingManager && !m_pGUI->GetMainMenu()->IsVisible () )
	{
		// Call the script event
		pArguments.push( true );
		m_pClientScriptingManager->GetEvents()->Call( "onClientFrameRender", &pArguments );
		pArguments.clear();
	}

	// Restore the old device state
	if ( pStateBlock )
		pStateBlock->Apply();

	// Release the state block
	SAFE_RELEASE( pStateBlock );

	// Call the device end scene
	if ( m_pGraphics && m_pGraphics->GetDevice () )
		m_pGraphics->GetDevice()->EndScene();

	// Pulse the FPS counter
	if ( m_pFPSCounter )
		m_pFPSCounter->Pulse ();
}

int iIndex = 0;
CNetworkPlayer * pTestPlayer = NULL;
CNetworkVehicle * pTestVehicle = NULL;
CM2Ped * pPlayerPed = NULL;
void CCore::OnGameProcess( void )
{
	// Pulse the network
	if( m_pNetworkModule )
		m_pNetworkModule->Pulse ();

	// Pulse the timer manager
	if( m_pTimerManager )
		m_pTimerManager->Pulse ();

	// Pulse the file transfer manager
	if( m_pFileTransferManager )
		m_pFileTransferManager->Pulse ();

	// Process the streamer
	if( m_pStreamer )
		m_pStreamer->Process ();

	// Pulse the video settings
	CM2VideoSettings::Pulse ();

#ifdef DEBUG
	if( GetAsyncKeyState( VK_F3 ) & 0x1 )
	{
		CVector3 vecPos;
		m_pPlayerManager->GetLocalPlayer()->GetPosition(&vecPos);

		m_pPlayerManager->Add(10, "Player", 0xFFFFFFFF);
		m_pPlayerManager->Get(10)->Create();
		m_pPlayerManager->Get(10)->Teleport(vecPos);

		pCore->GetChat()->AddDebugMessage ( "Ped: 0x%p", m_pPlayerManager->Get(10)->GetPlayerPed()->GetEntity() );
	}

	if( GetAsyncKeyState( VK_F7 ) & 0x1 )
	{
		m_pPlayerManager->GetLocalPlayer()->GetVehicle()->GetVehicle()->SetLightState ( !m_pPlayerManager->GetLocalPlayer()->GetVehicle()->GetVehicle()->GetLightState() );
		//pCore->GetChat()->AddDebugMessage ( "Player: 0x%p", m_pPlayerManager->GetLocalPlayer()->GetPlayerPed()->GetPed() );
	}

	if( GetAsyncKeyState( VK_F8 ) & 0x1 )
	{
		CVector3 vecPos;
		m_pPlayerManager->GetLocalPlayer()->GetPosition ( &vecPos );

		pPlayerPed = new CM2Ped ( IE::CreatePlayerPed () );
		pPlayerPed->SetPosition ( vecPos );
		pPlayerPed->Activate ();

		pCore->GetChat()->AddDebugMessage ( "Ped: 0x%p", pPlayerPed->GetPed() );
	}

	if ( pPlayerPed ) {
		//memcpy ( &pPlayerPed->GetPed()->m_playerControls, &m_pPlayerManager->GetLocalPlayer()->GetPlayerPed()->GetPed()->m_playerControls, sizeof ( C_PlayerControls ) );

		C_PlayerControls local = m_pPlayerManager->GetLocalPlayer()->GetPlayerPed()->GetPed()->m_playerControls;
		C_PlayerControls remote = pPlayerPed->GetPed()->m_playerControls;

		// TODO: LOOK INTO THIS
		remote.m_ePlayerMovementState = local.m_ePlayerMovementState;
		remote.m_byteKeyboardFlags = local.m_byteKeyboardFlags;
		remote.m_bIsMoving = local.m_bIsMoving;
		remote.m_bIsCrouching = local.m_bIsCrouching;
		remote.m_byteUnknown1 = local.m_byteUnknown1;
	}

	/*if( GetAsyncKeyState( VK_F9 ) & 0x1 )
	{
		//float steer = m_pPlayerManager->GetLocalPlayer()->GetPlayerPed()->GetCurrentVehicle()->m_fSteer;
		CNetworkVehicle * net_vehicle = m_pVehicleManager->Get(0);
		M2Vehicle * vehicle = net_vehicle->GetVehicle()->GetVehicle();

		//vehicle->m_fMaxSteerAngle

		// 0.87 - 0.66

		DWORD dwFunc = 0x11FB4E0;
		DWORD dwVehicleData = (DWORD)(vehicle) + 0xA8;
		float steer = 1.0;
		_asm push steer;
		_asm mov ecx, dwVehicleData;
		_asm call dwFunc;

		//pCore->GetChat()->AddDebugMessage ( "Steer: %f, Added: %f", vehicle->m_fSteer, vehicle->m_fAddedSteer );
	}*/
#endif

	// Call the script event
	if( m_pClientScriptingManager )
		m_pClientScriptingManager->GetEvents()->Call( "onClientProcess" );
}

void CCore::HandleAntiCheatEvent( DWORD dwMessage, unsigned int uiBaseAddress, size_t nSize )
{
	//CLogFile::Printf( "HandleAntiCheatEvent - %d, 0x%p, %d", dwMessage, uiBaseAddress, nSize );

	// Shutdown the core
	//Shutdown();
}

void CCore::StartMultiplayer( void )
{
	// Are we already started?
	if( IsMultiplayerStarted() )
		return;

	CLogFile::Printf( "CCore::StartMultiplayer" );

	// Hide the chat
	m_pChat->SetVisible( false );

	// Fade the screen out
	m_pHud->FadeOut( 0 );

	// Fade the sound out
	m_pGame->FadeSound( true, 0 );

	// Create the player manager
	m_pPlayerManager = new CPlayerManager;

	// Set the localplayer nick
	m_pPlayerManager->GetLocalPlayer()->SetNick( GetNick () );

	// Lock player controls
	pCore->GetPlayerManager()->GetLocalPlayer()->LockControls( true );

	// Create the blip manager
	m_pBlipManager = new CBlipManager;

	// Create the vehicle manager
	m_pVehicleManager = new CVehicleManager;

	// Create the ped manager
	m_pPedManager = new CPedManager;

	// Create the timer manager
	m_pTimerManager = new CTimerManager;

	// Create the client scripting manager
	m_pClientScriptingManager = new CClientScriptingManager;

	// Create the file transfer instance
	m_pFileTransferManager = new CFileTransferManager;

	// Mark as multiplayer started
	SetMultiplayerStarted( true );

	CLogFile::Printf( "CCore::StartMultiplayer - Multiplayer started!" );
}

void CCore::StopMultiplayer( void )
{
	// Are we not already started?
	if( !IsMultiplayerStarted() )
		return;

	CLogFile::Printf( "CCore::StopMultiplayer" );

	// Mark as no connection problem
	SetConnectionProblem ( false );

	// Hide the chat window
	m_pChat->SetVisible( false );

	// Fade the screen out
	m_pHud->FadeOut( 0 );

	// Fade the sound out
	m_pGame->FadeSound( true, 0 );

	// Clear the chat
	m_pChat->Clear();

	// Clear all chat history
	m_pChat->ClearHistory();

	// Lock player controls
	pCore->GetPlayerManager()->GetLocalPlayer()->LockControls( true );

	// Delete the clientscript manager
	SAFE_DELETE( m_pClientScriptingManager );

	// Delete the blip manager
	SAFE_DELETE( m_pBlipManager );

	// Delete the vehicle manager
	SAFE_DELETE( m_pVehicleManager );

	// Delete the player manager
	SAFE_DELETE( m_pPlayerManager );

	// Delete the ped manager
	SAFE_DELETE( m_pPedManager );

	// Delete the timer manager
	SAFE_DELETE( m_pTimerManager );

	// Delete the file transfer
	SAFE_DELETE( m_pFileTransferManager );

	// Clear the model manager
	CNetworkModelManager::Cleanup ();

	// Mark as multiplayer stopped
	SetMultiplayerStarted( false );

	CLogFile::Printf( "CCore::StopMultiplayer - Multiplayer stopped!" );
}

void CCore::TakeScreenshot( void )
{
	// Is the camera instance not valid yet?
	if( !pCore->GetCamera() )
		return;

	// Mark as taking screenshot
	m_bCaptureScreenshot = true;
}