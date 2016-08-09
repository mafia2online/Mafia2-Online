/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CCore.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"BaseInc.h"
#include	<ShellAPI.h>

#include	"Scripting\CSquirrelArguments.h"

#include	"CString.h"

#include	"SharedUtility.h"

#include	"CWPMHook.h"

#include	"CRC.h"

#include	"CGameFiles.h"
#include	"CSettings.h"

#include	"CMafia.h"
#include	"CCommands.h"
#include	"CEvents.h"
#include	"CGraphics.h"
#include	"CStreamer.h"
#include	"CFPSCounter.h"
#include	"CLua.h"
#include	"CModelManager.h"
#include	"CM2Camera.h"

#include	"CNetworkModule.h"
#include	"CIE.h"
#include	"CM2Hud.h"

#include	"CLoadingScreen.h"
#include	"CChat.h"

#include	"CGUI.h"
#include	"CEGUI.h"
#include	"CDirect3D9Hook.h"
#include	"CDirectInput8Hook.h"
#include	"gui_impl\CGUI_Impl.h"

#include	"CMainMenu.h"
#include	"CServerBrowser.h"
#include	"CMenuSettings.h"

#include	"CAudioManager.h"

#include	"CM2VideoSettings.h"

#include	"CClientScriptingManager.h"

#include	"CVehicleManager.h"

#include	"CPedManager.h"

#include	"COffsets.h"
#include	"CPatcher.h"
#include	"CPatches.h"

#include	"CScreenShot.h"

#include	"CNameTag.h"
#include	"../Shared/Network/CNetworkStats.h"

#include	"CPlayerManager.h"
#include	"CVehicleManager.h"
#include	"CLocalPlayer.h"

#include	"engine\CM2Entity.h"
#include	"engine\CM2Ped.h"

#include	"Timers\CTimerManager.h"
#include	"CFileTransferManager.h"

#include	"C3DTextLabelManager.h"
#include	"CKeyBinds.h"
#include	"CModelManager.h"
#include	"CBlipManager.h"

#include	"CNetworkModelManager.h"

#include	"CCore.h"

IDirect3DStateBlock9	* pStateBlock = NULL;
CSquirrelArguments		pArguments;
bool					bDeviceLost = false;
bool					bInputHookCreated = false;

extern					DWORD sub_CD00A0__ECX;

DWORD					GetModelHasFromModelName_ECX;

CCore::CCore( void )
{
	// Get the game directory from the registry
	char szGameDir[MAX_PATH];
	SharedUtility::ReadRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia2-Online", "GameDir", NULL, szGameDir, sizeof(szGameDir) );
	SetGameDirectory( szGameDir );

	// Get the mod directory from the registry
	char szModDir[MAX_PATH];
	SharedUtility::ReadRegistryString( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia2-Online", "InstallLocation", NULL, szModDir, sizeof(szModDir) );
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
	SetServerName( "M2-Online Server" );
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
	m_pNameTag = NULL;
	m_pAudioManager = NULL;
	m_p3DTextLabelManager = NULL;
}

CCore::~CCore( void )
{
#ifdef _DEBUG
	CLogFile::Printf( "CCore::~CCore" );
#endif

	// Delete the instances
	SAFE_DELETE( m_pNetworkModule );
	SAFE_DELETE(m_pGraphics);
	SAFE_DELETE(m_pFPSCounter);
	SAFE_DELETE(m_pStreamer);
	SAFE_DELETE(m_pModelManager);
	SAFE_DELETE(m_pAudioManager);
	SAFE_DELETE(m_p3DTextLabelManager);

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
		MessageBox( NULL, "Failed to start anti-cheat module.\n\nCan't start Mafia2-Online", "Mafia2-Online - Error", MB_OK|MB_ICONERROR );
		return false;
	}

	// Calculate the exe version
	CFileChecksum pChecksum;
	pChecksum.Calculate( String( "%s\\pc\\Mafia2.exe", m_strGameDirectory.Get() ) );

	CLogFile::Printf( "EXE signature: 0x%p", pChecksum.GetChecksum() );

	// Is this not the v1 exe?
	if (SharedUtility::IsEXESignatureAllowed(pChecksum.GetChecksum()) == false)
	{
		if ( MessageBox ( NULL, "Invalid game version detected.\n\nCan't start Mafia 2 Online\n\nPlease contact Tyldar with your EXE Signature (In core file in logs directory)", "Mafia2-Online - Error", MB_OK|MB_ICONERROR ) == MB_OK ) {
			ShellExecute ( NULL, "open", "http://forum.mafia2-online.com", NULL, NULL, SW_SHOWNORMAL );
		}
		return false;
	}

	// Check the game files
	if( !CGameFiles::CheckFiles() )
	{
		MessageBox( NULL, String( "%s\n\nPlease reinstall Mafia 2 or Mafia2-Online", CGameFiles::GetLastError().Get() ).Get(), "Mafia2-ONline - Error", MB_OK|MB_ICONERROR );
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

	// Create the streamer instance
	m_pStreamer = new CStreamer;

	// Create the model mgr instance
	m_pModelManager = new CModelManager;

	// We detect the graphic card
	if (m_pGraphics->IsUsingChipset()){
		MessageBox(NULL, "Your game is currently starting on graphic chipset\n\nYou may encounter graphical problems thats not belong to M2-Online", "Mafia2-Online - Warning", MB_OK | MB_ICONWARNING);
		CLogFile::Print("[GRAPHIC] : Using chipset");
	} else if (m_pGraphics->IsUsingAMD()){
		CLogFile::Print("[GRAPHIC] : Using AMD");
	} else if (m_pGraphics->IsUsingNVIDIA()){
		CLogFile::Print("[GRAPHIC] : Using NVIDIA");
	} else {
		CLogFile::Print("[RAPHIC] : Unknow graphic card");
	}

	return true;
}

void CCore::Shutdown( void )
{
	// Are we connected to the network?
	if( CCore::Instance()->GetNetworkModule()->IsConnected() )
	{
		// Disconnect and shutdown RakNet
		CCore::Instance()->GetNetworkModule()->Disconnect( false );
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
	CCore::Instance()->SetCamera( new CM2Camera( *(M2Camera **)COffsets::VAR_CCamera ) );

	// Create and set the hud instance
	CCore::Instance()->SetHud( new CM2Hud( *(M2Hud **)COffsets::VAR_CHud ) );

	// Setup the game pointers
	CCore::Instance()->GetGame()->LoadPointers ();

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

	// Setup mod gui instances
	m_pGUI->SetupGUI();

	// Initialise the loading screen
	CLoadingScreen::Start();

	// Create the chat instance
	m_pChat = new CChat( m_pGUI->GetCEGUI(), 30.0f, 25.0f );
	m_pChat->AddInfoMessage( "%s %s started.", MOD_NAME, MOD_VERS_STR );

	// Refresh server browser
	m_pGUI->GetServerBrowser()->Refresh ();

	// Create the audio manager
	m_pAudioManager = new CAudioManager;

	if (m_pAudioManager)
		m_pAudioManager->Initialize();

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
		CCore::Instance()->GetGraphics()->DrawText ( (CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fX - CCore::Instance()->GetGraphics()->GetTextWidth("Connection Problem", 1.0f, "tahoma-bold") - 5), 5, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, "tahoma-bold", true, "Connection Problem" );

	// Call the vehicle manager processor
	if ( m_pVehicleManager )
		m_pVehicleManager->Process ();

	// Pulse the ped manager
	if( m_pPedManager )
		m_pPedManager->Pulse();

#ifdef DEBUG
	if (m_pPlayerManager && m_pPlayerManager->GetLocalPlayer() && m_pPlayerManager->GetLocalPlayer()->IsSpawned()) {
		CM2Ped * pPlayerPed = m_pPlayerManager->GetLocalPlayer()->GetPlayerPed();
		M2PlayerControls playerControls = pPlayerPed->GetPed()->m_playerControls;

		CCore::Instance()->GetGraphics()->DrawText(300, 300, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, "tahoma-bold", true, "Is shooting: %s\nMovement State: %d\nModifiers: %d\nMouse Flags: %d\nKeyboard Flags: %d\nIs Aiming: %s\n", playerControls.m_bIsShooting ? "Yes" : "No", playerControls.m_playerMovementState, playerControls.m_byteModifiers, playerControls.m_byteMouseFlags, playerControls.m_byteKeyboardFlags, playerControls.m_bIsAiming ? "Yes" : "No");
	}
#endif

	

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

	// Render the nameTags
	if (m_pNameTag)
		m_pNameTag->Draw();

	// Render the 3DTextLabels
	if (m_p3DTextLabelManager && CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsSpawned() == true){
		m_p3DTextLabelManager->Render();
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

	// Process the audio manager
	if (m_pAudioManager)
		m_pAudioManager->Process();

	// Pulse the video settings
	CM2VideoSettings::Pulse ();

	// DEBUG
#ifdef DEBUG
	static C_SyncObject *_test = NULL;
	if( GetAsyncKeyState( VK_F7 ) & 0x1 )
	{
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls(true);
		_test = CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed()->PlayAnimEffect("00-WALK-A", true);
	}

	if (GetAsyncKeyState(VK_F8) & 0x1)
	{
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed()->AnimEffectStop(_test);
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls(false);
	}

	if (GetAsyncKeyState(VK_F6) & 0x1)
	{
		CCore::Instance()->GetChat()->AddDebugMessage("IsStealthMoving : %s", (CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed()->IsStealthMoving()) ? "Oui" : "Non");
	}
#endif

	// Call the script event
	if( m_pClientScriptingManager )
		m_pClientScriptingManager->GetEvents()->Call( "onClientProcess" );
}

void CCore::HandleAntiCheatEvent( DWORD dwMessage, unsigned int uiBaseAddress, size_t nSize )
{
	CLogFile::Printf( "HandleAntiCheatEvent - %d, 0x%p, %d", dwMessage, uiBaseAddress, nSize );

	// Shutdown the core
	Shutdown();
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
	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls( true );

	// Create the key binds instance
	m_pKeyBinds = new CKeyBinds;

	// Create the blip manager
	m_pBlipManager = new CBlipManager;

	// Create the vehicle manager
	m_pVehicleManager = new CVehicleManager;

	// Create the ped manager
	m_pPedManager = new CPedManager;

	// Create the timer manager
	m_pTimerManager = new CTimerManager;

	// Create the 3DTextLabelManager
	m_p3DTextLabelManager = new C3DTextLabelManager;

	// Create the client scripting manager
	m_pClientScriptingManager = new CClientScriptingManager;

	// Create the file transfer instance
	m_pFileTransferManager = new CFileTransferManager;

	// Create the rendering interface
	m_pNameTag = new CNameTag;

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
	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls( true );

	// Delete the keybinds
	SAFE_DELETE( m_pKeyBinds );

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

	// Delete the 3DTextLabel manager
	SAFE_DELETE(m_p3DTextLabelManager);

	// Clear the model manager
	CNetworkModelManager::Cleanup ();

	// Mark as multiplayer stopped
	SetMultiplayerStarted( false );

	CLogFile::Printf( "CCore::StopMultiplayer - Multiplayer stopped!" );
}

void CCore::TakeScreenshot( void )
{
	// Is the camera instance not valid yet?
	if( !CCore::Instance()->GetCamera() )
		return;

	// Mark as taking screenshot
	m_bCaptureScreenshot = true;
}