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

IDirect3DStateBlock9	* pStateBlock = nullptr;
CSquirrelArguments		pArguments;
bool					bDeviceLost = false;
bool					bInputHookCreated = false;

extern					DWORD sub_CD00A0__ECX;

DWORD					GetModelHasFromModelName_ECX;

int						iIndex	 = 0;
CNetworkPlayer			* pTestPlayer = nullptr;
CNetworkVehicle			* pTestVehicle = nullptr;
CM2Ped					* pPlayerPed = nullptr;

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

	m_bInitialised = false;
	m_bCaptureScreenshot = false;
	m_bRenderNetworkStats = false;
	m_bConnectionProblem = false;

	SetGameLoaded( false );
	SetMultiplayerStarted( false );

	SetServerName( "M2-Online Server" );
	SetServerMaxPlayers( 0 );
	SetGlobalSpeedLimited ( false );
	SetSummer ( true );

	m_pNetworkModule = nullptr;
	m_pPlayerManager = nullptr;
	m_pCamera = nullptr;
	m_pHud = nullptr;
	m_pClientScriptingManager = nullptr;
	m_pFPSCounter = nullptr;
	m_pTimerManager = nullptr;
	m_pFileTransferManager = nullptr;
	m_pBlipManager = nullptr;
	m_pVehicleManager = nullptr;
	m_pPedManager = nullptr;
	m_pGame = nullptr;
	m_pGUI = nullptr;
	m_pKeyBinds = nullptr;
	m_pStreamer = nullptr;
	m_pModelManager = nullptr;
	m_pUpdater = nullptr;
	m_pNameTag = nullptr;
	m_pAudioManager = nullptr;
	m_p3DTextLabelManager = nullptr;
	m_pScreenshotManager = nullptr;
}

CCore::~CCore( void )
{
#ifdef _DEBUG
	CLogFile::Printf( "CCore::~CCore" );
#endif

	SAFE_DELETE( m_pNetworkModule );
	SAFE_DELETE(m_pGraphics);
	SAFE_DELETE(m_pFPSCounter);
	SAFE_DELETE(m_pStreamer);
	SAFE_DELETE(m_pModelManager);
	SAFE_DELETE(m_pAudioManager);
	SAFE_DELETE(m_p3DTextLabelManager);
	SAFE_DELETE(m_pScreenshotManager);

	CWPMHook::Uninstall();

	TerminateProcess( GetCurrentProcess(), 0 );
}

bool CCore::Initialise( void )
{
	if( m_bInitialised )
		return false;

	CLogFile::Open( SharedUtility::GetAbsolutePath( "logs\\core.log" ).Get() );
	if( !CWPMHook::Install() )
	{
		MessageBox( NULL, "Failed to start anti-cheat module.\n\nCan't start Mafia2-Online", "Mafia2-Online - Error", MB_OK|MB_ICONERROR );
		return false;
	}

	CFileChecksum pChecksum;
	pChecksum.Calculate( String( "%s\\pc\\Mafia2.exe", m_strGameDirectory.Get() ) );

	CLogFile::Printf( "EXE signature: 0x%p", pChecksum.GetChecksum() );

	if (SharedUtility::IsEXESignatureAllowed(pChecksum.GetChecksum()) == false)
	{
		if ( MessageBox ( NULL, "Invalid game version detected.\n\nCan't start Mafia 2 Online\n\nPlease contact Tyldar with your EXE Signature (In core file in logs directory)", "Mafia2-Online - Error", MB_OK|MB_ICONERROR ) == MB_OK ) {
			ShellExecute ( NULL, "open", "http://forum.mafia2-online.com", NULL, NULL, SW_SHOWNORMAL );
		}
		return false;
	}

	if( !CGameFiles::CheckFiles() )
	{
		MessageBox( NULL, String( "%s\n\nPlease reinstall Mafia 2 or Mafia2-Online", CGameFiles::GetLastError().Get() ).Get(), "Mafia2-ONline - Error", MB_OK|MB_ICONERROR );
		return false;
	}

	CSettings::Load( "settings.xml" );
	CSettings::ParseCommandLine ( GetCommandLine() );

	m_uiBaseAddress = (unsigned int)GetModuleHandle( NULL );
	m_uiBaseAddress -= 0x400000;

	m_pGame = new CMafia;

	COffsets::Initialise( 0, m_uiBaseAddress );
	CPatches::Initialise ();

	CLua::Initialise ();

	m_pGraphics = new CGraphics;
	m_pFPSCounter = new CFPSCounter;
	m_pStreamer = new CStreamer;
	m_pModelManager = new CModelManager;
	m_pScreenshotManager = new CScreenShot;

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
	if( CCore::Instance()->GetNetworkModule()->IsConnected() )
	{
		CCore::Instance()->GetNetworkModule()->Disconnect( false );
	}
	delete this;
}

void CCore::OnGameLoad( void )
{
#ifdef _DEBUG
	CLogFile::Printf ( "CCore::OnGameLoad" );
#endif

	if( IsGameLoaded() )
		return;

	IE::LoadPointers ();

	CCore::Instance()->SetCamera( new CM2Camera( *(M2Camera **)COffsets::VAR_CCamera ) );
	CCore::Instance()->SetHud( new CM2Hud( *(M2Hud **)COffsets::VAR_CHud ) );
	CCore::Instance()->GetGame()->LoadPointers ();

#ifdef DEBUG
	CLogFile::Printf ( "LocalPed: 0x%p", IE::GetGame()->m_pLocalPed);
#endif

	// Disable the game over writting the localplayer model manager pointer
	// Don't move this into CPatches or else the localplayer model manager won't be set!
	CPatcher::InstallNopPatch ( 0x5A650C, 6 );

	StartMultiplayer ();

	m_pNetworkModule->Startup ();

	m_pGame->OnGameStart ();
	SetGameLoaded( true );
}

void CCore::OnGamePreLoad( void )
{
	if( IsGameLoaded() )
		return;

	CLoadingScreen::Finish();
	CGameFiles::CleanFiles();

	m_pNetworkModule = new CNetworkModule;

	m_pChat->SetVisible( false );
	m_pGUI->GetMainMenu()->SetVisible( true );
	m_pGUI->GetSettings()->LoadSettings ();
}

void CCore::OnDeviceCreate( IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters )
{
	m_pGraphics->Initialise( pDevice );

	m_pGUI = new CGUI( pDevice );

	m_pGUI->SetupGUI();
	CLoadingScreen::Start();

	m_pChat = new CChat( m_pGUI->GetCEGUI(), 30.0f, 25.0f );
	m_pChat->AddInfoMessage( "%s %s started.", MOD_NAME, MOD_VERS_STR );

	m_pGUI->GetServerBrowser()->Refresh ();

	m_pAudioManager = new CAudioManager;
	if (m_pAudioManager)
		m_pAudioManager->Initialize();

#ifdef _DEBUG
	CLogFile::Printf ( "CCore::OnDeviceCreate - Done" );
#endif
}

void CCore::OnDeviceLost( IDirect3DDevice9 * pDevice )
{
	if ( bDeviceLost )
	{
		CLogFile::Printf ( "ERROR - Direct3D device lost, but some objects were not released. Game is in an unrecoverable state now. Exiting." );
		MessageBox ( NULL, "Direct3D device lost, but some objects were not released. Game is in an unrecoverable state now. Exiting.", "Direct3D Device Error", MB_ICONERROR|MB_OK );
		TerminateProcess ( GetCurrentProcess(), 0 );
	}

	if( pStateBlock )
		SAFE_RELEASE( pStateBlock );

	bDeviceLost = true;
	if( m_pGraphics )
		m_pGraphics->OnLostDevice( pDevice );
	if( m_pGUI )
	{
		m_pGUI->GetMainMenu()->OnDeviceLost ();
		m_pGUI->OnDeviceLost ();
	}
	CM2VideoSettings::SetWaitingForDeviceReset ( true );
}

void CCore::OnDeviceReset( IDirect3DDevice9 * pDevice )
{
	if( m_pGraphics )
		m_pGraphics->OnRestoreDevice( pDevice );

	if( m_pGUI )
		m_pGUI->OnDeviceReset();

	CM2VideoSettings::SetWaitingForDeviceReset ( false );

	if( m_pClientScriptingManager )
		m_pClientScriptingManager->GetEvents()->Call( "onClientDeviceReset" );

	bDeviceLost = false;
}

void CCore::OnDevicePreRender( void )
{
	if ( m_pVehicleManager )
		m_pVehicleManager->PreProcess ();

	if( m_pClientScriptingManager && !m_pGUI->GetMainMenu()->IsVisible () )
		m_pClientScriptingManager->GetEvents()->Call( "onClientFramePreRender" );
}

void CCore::OnDeviceRender( void )
{
	if( bDeviceLost )
		return;

	m_pGraphics->GetDevice()->BeginScene ();

	if( FAILED( m_pGraphics->GetDevice()->CreateStateBlock( D3DSBT_ALL, &pStateBlock ) ) )
		return;
	pStateBlock->Capture ();
	if ( m_pGraphics )
		m_pGraphics->OnSceneBegin ();

	if( m_pClientScriptingManager && !m_pGUI->GetMainMenu()->IsVisible () )
	{
		pArguments.push( false );
		m_pClientScriptingManager->GetEvents()->Call( "onClientFrameRender", &pArguments );
		pArguments.clear();
	}
	CLoadingScreen::Render ();
	if ( m_pChat->IsVisible () )
		m_pChat->Render ();
	if ( m_pGUI->GetMainMenu () && m_pGUI->GetMainMenu()->IsVisible () )
		m_pGUI->GetMainMenu()->Render ();
	if ( m_pGraphics )
		m_pGraphics->ExpireCachedTextures ();
	if ( m_pGUI )
		m_pGUI->Render ();
	if ( IsConnectionProblem () )
		CCore::Instance()->GetGraphics()->DrawText ( (CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fX - CCore::Instance()->GetGraphics()->GetTextWidth("Connection Problem", 1.0f, "tahoma-bold") - 5), 5, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, "tahoma-bold", true, "Connection Problem" );
	if ( m_pVehicleManager )
		m_pVehicleManager->Process ();
	if( m_pPedManager )
		m_pPedManager->Pulse();

#ifdef DEBUG
	if (m_pPlayerManager && m_pPlayerManager->GetLocalPlayer() && m_pPlayerManager->GetLocalPlayer()->IsSpawned()) {
		CM2Ped * pPlayerPed = m_pPlayerManager->GetLocalPlayer()->GetPlayerPed();
		M2PlayerControls playerControls = pPlayerPed->GetPed()->m_playerControls;

		CCore::Instance()->GetGraphics()->DrawText(300, 300, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, "tahoma-bold", true, "Is shooting: %s\nMovement State: %d\nModifiers: %d\nMouse Flags: %d\nKeyboard Flags: %d\nIs Aiming: %s\n", playerControls.m_bIsShooting ? "Yes" : "No", playerControls.m_playerMovementState, playerControls.m_byteModifiers, playerControls.m_byteMouseFlags, playerControls.m_byteKeyboardFlags, playerControls.m_bIsAiming ? "Yes" : "No");
	}
#endif

	if ( GetAsyncKeyState ( VK_F10 ) & 0x1 )
	{
		m_pHud->Show ( !m_pHud->IsShowing () );
		m_pChat->SetVisible ( !m_pChat->IsVisible () );
		m_pPlayerManager->GetLocalPlayer()->GetPlayerPed()->ShowModel ( !m_pPlayerManager->GetLocalPlayer()->GetPlayerPed()->IsModelShowing () );
	}
	if( GetAsyncKeyState( VK_F11 ) & 0x1 )
	{
		m_bRenderNetworkStats = !m_bRenderNetworkStats;
	}
	if( GetAsyncKeyState( VK_F12 ) & 0x1 )
	{
		TakeScreenshot ();
	}
	if (m_bCaptureScreenshot && !m_pScreenshotManager->IsSaving())
	{
		unsigned long ulScreenWidth = m_pCamera->GetGameCamera()->m_iWindowWidth;
		unsigned long ulScreenHeight = m_pCamera->GetGameCamera()->m_iWindowHeight;
		unsigned char * ucData = new unsigned char [ ulScreenHeight * (ulScreenWidth * 4) ];

		if( m_pGraphics->GetFrontBufferPixels( &ucData ) )
		{
			m_pScreenshotManager->BeginWrite(ucData);
		}
		m_bCaptureScreenshot = false;
	}
	if( m_bRenderNetworkStats )
	{
		m_pGraphics->DrawText( (m_pGUI->GetCEGUI()->GetResolution().fX - 275), 30, 0xFFFFFFFF, 1.0f, "tahoma-bold", false, DT_NOCLIP, CNetworkStats::GetStats().Get() );
	}
	if (m_pNameTag)
		m_pNameTag->Draw();
	if (m_p3DTextLabelManager && CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsSpawned() == true){
		m_p3DTextLabelManager->Render();
	}
	if( m_pClientScriptingManager && !m_pGUI->GetMainMenu()->IsVisible () )
	{
		pArguments.push( true );
		m_pClientScriptingManager->GetEvents()->Call( "onClientFrameRender", &pArguments );
		pArguments.clear();
	}
	if ( pStateBlock )
		pStateBlock->Apply();
	SAFE_RELEASE( pStateBlock );

	if ( m_pGraphics && m_pGraphics->GetDevice () )
		m_pGraphics->GetDevice()->EndScene();

	if ( m_pFPSCounter )
		m_pFPSCounter->Pulse ();
}

void CCore::OnGameProcess( void )
{
	if( m_pNetworkModule )
		m_pNetworkModule->Pulse ();

	if( m_pTimerManager )
		m_pTimerManager->Pulse ();

	if( m_pFileTransferManager )
		m_pFileTransferManager->Pulse ();

	if( m_pStreamer )
		m_pStreamer->Process ();

	if (m_pAudioManager)
		m_pAudioManager->Process();

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

	if( m_pClientScriptingManager )
		m_pClientScriptingManager->GetEvents()->Call( "onClientProcess" );
}

void CCore::HandleAntiCheatEvent( DWORD dwMessage, unsigned int uiBaseAddress, size_t nSize )
{
	CLogFile::Printf( "HandleAntiCheatEvent - %d, 0x%p, %d", dwMessage, uiBaseAddress, nSize );
	Shutdown();
}

void CCore::StartMultiplayer( void )
{
	if( IsMultiplayerStarted() )
		return;

	CLogFile::Printf( "CCore::StartMultiplayer" );

	m_pChat->SetVisible( false );
	m_pHud->FadeOut( 0 );
	m_pGame->FadeSound( true, 0 );

	m_pPlayerManager = new CPlayerManager;
	m_pPlayerManager->GetLocalPlayer()->SetNick( GetNick () );
	m_pPlayerManager->GetLocalPlayer()->LockControls(true);

	m_pKeyBinds = new CKeyBinds;
	m_pBlipManager = new CBlipManager;
	m_pVehicleManager = new CVehicleManager;
	m_pPedManager = new CPedManager;
	m_pTimerManager = new CTimerManager;
	m_p3DTextLabelManager = new C3DTextLabelManager;
	m_pClientScriptingManager = new CClientScriptingManager;
	m_pFileTransferManager = new CFileTransferManager;
	m_pNameTag = new CNameTag;

	SetMultiplayerStarted( true );

	CLogFile::Printf( "CCore::StartMultiplayer - Multiplayer started!" );
}

void CCore::StopMultiplayer( void )
{
	if( !IsMultiplayerStarted() )
		return;

	CLogFile::Printf( "CCore::StopMultiplayer" );

	SetConnectionProblem ( false );
	m_pChat->SetVisible( false );
	m_pHud->FadeOut( 0 );
	m_pGame->FadeSound( true, 0 );

	m_pChat->Clear();
	m_pChat->ClearHistory();

	CCore::Instance()->GetAudioManager()->RemoveAll();

	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->LockControls( true );

	SAFE_DELETE( m_pKeyBinds );
	SAFE_DELETE( m_pClientScriptingManager );
	SAFE_DELETE( m_pBlipManager );
	SAFE_DELETE( m_pVehicleManager );
	SAFE_DELETE( m_pPlayerManager );
	SAFE_DELETE( m_pPedManager );
	SAFE_DELETE( m_pTimerManager );
	SAFE_DELETE( m_pFileTransferManager );
	SAFE_DELETE(m_p3DTextLabelManager);

	CNetworkModelManager::Cleanup ();

	SetMultiplayerStarted( false );
	CLogFile::Printf( "CCore::StopMultiplayer - Multiplayer stopped!" );
}

void CCore::TakeScreenshot( void )
{
	if( !CCore::Instance()->GetCamera() )
		return;
	m_bCaptureScreenshot = true;
}