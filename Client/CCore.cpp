/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CCore.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include <ShellAPI.h>

#include "Scripting/CSquirrelArguments.h"

#include "CString.h"

#include "SharedUtility.h"

#include "CRC.h"

#include "CGameFiles.h"
#include "CSettings.h"

#include "CMafia.h"
#include "CCommands.h"
#include "CEvents.h"
#include "CGraphics.h"
#include "CStreamer.h"
#include "CFPSCounter.h"
#include "CLua.h"
#include "CModelManager.h"
#include "CM2Camera.h"

#include "CNetworkModule.h"
#include "CIE.h"
#include "CM2Hud.h"

#include "CLoadingScreen.h"

#include "CGUI.h"
#include "CEGUI.h"
#include "CDirect3D9Hook.h"
#include "CDirectInput8Hook.h"
#include "gui_impl/CGUI_Impl.h"

#include "CMainMenu.h"
#include "CServerBrowser.h"
#include "CMenuSettings.h"

#include "CAudioManager.h"

#include "CM2VideoSettings.h"

#include "CClientScriptingManager.h"

#include "CVehicleManager.h"

#include "CPedManager.h"

#include "COffsets.h"
#include "CPatcher.h"
#include "CPatches.h"

#include "CScreenShot.h"

#include "CNameTag.h"
#include "Network/CNetworkStats.h"

#include "CPlayerManager.h"
#include "CVehicleManager.h"
#include "CLocalPlayer.h"

#include "engine/CM2Entity.h"
#include "engine/CM2Ped.h"
#include "engine/CM2TextDatabase.h"

#include "Timers/CTimerManager.h"
#include "CFileTransferManager.h"

#include "C3DTextLabelManager.h"
#include "CKeyBinds.h"
#include "CModelManager.h"
#include "CBlipManager.h"

#include "CNetworkModelManager.h"

#include "CCore.h"

#include "CLogFile.h"

#include "ExceptionHandler.h"
#include "SharedUtility.h"

CSquirrelArguments		pArguments;
bool					bDeviceLost = false;

extern					DWORD sub_CD00A0__ECX;

DWORD					GetModelHasFromModelName_ECX;

int						iIndex	 = 0;
CNetworkPlayer			* pTestPlayer = nullptr;
CNetworkVehicle			* pTestVehicle = nullptr;
CM2Ped					* pPlayerPed = nullptr;

CCore::CCore( void )
	: m_bInitialised(false)
	, m_bGameLoaded(false)
	, m_uiBaseAddress(0)
	, m_bMultiplayerStarted(false)
	, m_bCaptureScreenshot(false)
	, m_bRenderNetworkStats(false)
	, m_bConnectionProblem(false)

	, m_bShutdownRequest(false)
	, m_bRenderInProgress(false)

	, m_pGame(nullptr)
	, m_pNetworkModule(nullptr)
	, m_pGraphics(nullptr)
	, m_chatBox()
	, m_pPlayerManager(nullptr)
	, m_pFPSCounter(nullptr)
	, m_pBlipManager(nullptr)
	, m_pVehicleManager(nullptr)
	, m_pPedManager(nullptr)
	, m_pGUI(nullptr)
	, m_pKeyBinds(nullptr)
	, m_pStreamer(nullptr)
	, m_pModelManager(nullptr)
	, m_pNameTag(nullptr)
	, m_pAudioManager(nullptr)
	, m_p3DTextLabelManager(nullptr)
	, m_pScreenshotManager(nullptr)
	, m_pClientScriptingManager(nullptr)
	, m_pTimerManager(nullptr)
	, m_pFileTransferManager(nullptr)

	, m_pCamera(nullptr)
	, m_pHud(nullptr)

	, m_gameHwnd(0)

	, m_strNick()
	, m_strHost()
	, m_ulPort(0)
	, m_strPass()

	, m_strServerName()
	, m_uServerMaxPlayers(0)
	, m_bGlobalSpeedLimited(false)
	, m_bSummer(true)

	, m_strGameDirectory()
	, m_strModDirectory()
{
	// Set the game directory to current exe path.
	String gameDirectory(SharedUtility::GetExePath());

	// Cut the pc\\ from the path.
	gameDirectory.Substitute("\\pc\\", "");

	SetGameDirectory( gameDirectory );

	// Set the mod directory to the directory where DLL is placed.
	SetModDirectory( SharedUtility::GetAppPath() );

}

CCore::~CCore( void )
{
	DEBUG_LOG ( "CCore::~CCore" );

	// Destroy all entity managers.
	SAFE_DELETE(m_pVehicleManager);

	// Destroy after vehicle manager as vehicle manager depends on it. (removal of peds from car on destroy)
	SAFE_DELETE(m_pPlayerManager);

	SAFE_DELETE(m_pPedManager);
	SAFE_DELETE(m_pGraphics);
	m_chatBox.reset();
	SAFE_DELETE(m_pFPSCounter);
	SAFE_DELETE(m_pBlipManager);
	SAFE_DELETE(m_pKeyBinds);
	SAFE_DELETE(m_pStreamer);
	SAFE_DELETE(m_pModelManager);
	SAFE_DELETE(m_pNameTag);
	SAFE_DELETE(m_pAudioManager);
	SAFE_DELETE(m_p3DTextLabelManager);
	SAFE_DELETE(m_pScreenshotManager);
	SAFE_DELETE(m_pClientScriptingManager);
	SAFE_DELETE(m_pTimerManager);
	SAFE_DELETE(m_pFileTransferManager);

	// Clean gui first before deleting pointer
	CCore::Instance()->GetGUI()->DeleteAllClientScriptGUI();
	SAFE_DELETE(m_pGUI);

	SAFE_DELETE(m_pGame);
	SAFE_DELETE(m_pNetworkModule);

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pHud);
}

bool CCore::Initialise( void )
{
#ifdef _DEBUG
	//MessageBox(NULL, "Attach debugger", NULL, NULL);
#endif

	if( m_bInitialised )
		return false;

	char exceptionPath[MAX_PATH + 1] = { 0 };
	strcpy(exceptionPath, SharedUtility::GetAppPath());
	strcat(exceptionPath, "crashes");

	if (!ExceptionHandler::Install(exceptionPath))
	{
		MessageBox(NULL, "Mafia2-Online", "Unable to initialize exception handler!", NULL);
		return FALSE;
	}

	CLogFile::Open( SharedUtility::GetAbsolutePath( "logs\\core.log" ).Get() );

	CFileChecksum pChecksum;
	String strExePath;
	strExePath.Format("%s\\pc\\Mafia2.exe", m_strGameDirectory.Get());
	if (!pChecksum.Calculate(strExePath))
	{
		String strErrorMessage;
		strErrorMessage.Format("Failed to calculate checksum of the Mafia2 executable.\nPath: %s", strExePath.Get());
		MessageBox(NULL, strErrorMessage, "Mafia2-Online - Fatal error", MB_ICONERROR);
		return false;
	}

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
		String errorMessage;
		errorMessage.Format( "%s\n\nPlease reinstall Mafia 2 or Mafia2-Online", CGameFiles::GetLastError().Get() );
		MessageBox( NULL, errorMessage, "Mafia2-Online - Error", MB_OK|MB_ICONERROR );
		return false;
	}

	CSettings::Load( "settings.xml" );
	CSettings::ParseCommandLine ( GetCommandLine() );

	m_uiBaseAddress = (unsigned int)GetModuleHandle( NULL );
	m_uiBaseAddress -= 0x400000;

	m_pGame = new CMafia();

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
		CLogFile::Print("[GFX] Using Intel chipset");
	} else if (m_pGraphics->IsUsingAMD()){
		CLogFile::Print("[GFX] Using AMD");
	} else if (m_pGraphics->IsUsingNVIDIA()){
		CLogFile::Print("[GFX] Using NVIDIA");
	} else {
		CLogFile::Print("[GFX] Unknow graphic card");
	}

	return true;
}

void CCore::Shutdown( void )
{
	m_bShutdownRequest = true;
}

void CCore::OnGameLoad( void )
{
	DEBUG_LOG ( "CCore::OnGameLoad" );

	if( IsGameLoaded() )
		return;

	IE::LoadPointers ();

	CCore::Instance()->SetCamera( new CM2Camera( *(M2Camera **)COffsets::VAR_CCamera ) );
	CCore::Instance()->SetHud( new CM2Hud( *(M2Hud **)COffsets::VAR_CHud ) );
	CCore::Instance()->SetTextDatabase( new CM2TextDatabase( *(M2TextDatabase **)COffsets::VAR_CTextDatabase));
	CCore::Instance()->GetGame()->LoadPointers ();

	DEBUG_LOG ( "LocalPed: 0x%p", IE::GetGame()->m_pLocalPed);

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

	m_chatBox->SetVisible( false );
	m_pGUI->GetMainMenu()->SetVisible( true );
	m_pGUI->SetCursorVisible(true);
	m_pCamera->LockControl(false);
	m_pGUI->GetSettings()->LoadSettings ();
}

void CCore::OnDeviceCreate( IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters )
{
	m_pGraphics->Initialise( pDevice );

	m_pGUI = new CGUI( pDevice );

	m_pGUI->SetupGUI();
	CLoadingScreen::Start();

	m_chatBox = std::make_unique<ChatBox>( m_pGUI );
	m_chatBox->OutputF(ChatBox::INFO_MESSAGE_COLOR, "%s %s started.", MOD_NAME, MOD_VERS_STR);

	m_pGUI->GetServerBrowser()->Refresh ();

	m_pAudioManager = new CAudioManager;
	if (m_pAudioManager)
		m_pAudioManager->Initialize();

	DEBUG_LOG ( "CCore::OnDeviceCreate - Done" );
}

void CCore::OnDeviceLost( IDirect3DDevice9 * pDevice )
{
	if ( bDeviceLost )
	{
		CLogFile::Printf ( "ERROR - Direct3D device lost, but some objects were not released. Game is in an unrecoverable state now. Exiting." );
		MessageBox ( NULL, "Direct3D device lost, but some objects were not released. Game is in an unrecoverable state now. Exiting.", "Direct3D Device Error", MB_ICONERROR|MB_OK );
		TerminateProcess ( GetCurrentProcess(), 0 );
	}

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
	m_bRenderInProgress = true;

	if (!m_pGUI)
		return;

	CMainMenu *pMenu = m_pGUI->GetMainMenu();
	if( pMenu && m_pClientScriptingManager && !pMenu->IsVisible () )
		m_pClientScriptingManager->GetEvents()->Call( "onClientFramePreRender" );
}

void CCore::DoRender( void )
{
	if ( m_pScreenshotManager )
		m_pScreenshotManager->ProcessRenderThread();

	if( bDeviceLost )
		return;

	if (! m_pGraphics)
		return;

	IDirect3DDevice9 *pDevice = m_pGraphics->GetDevice();
	if (! pDevice)
		return;

	m_pGraphics->BeginRender();

	if( m_pClientScriptingManager && !m_pGUI->GetMainMenu()->IsVisible () )
	{
		pArguments.push( false );
		m_pClientScriptingManager->GetEvents()->Call( "onClientFrameRender", &pArguments );
		pArguments.clear();
	}

	if (!m_bGameLoaded) {
		CLoadingScreen::Render ();
	}

	// Draw 3D texts and nametags before any UI element.
	if (m_pPlayerManager) {
		CLocalPlayer *pLocalPlayer = m_pPlayerManager->GetLocalPlayer();
		if (pLocalPlayer && pLocalPlayer->IsSpawned()) {
			if (m_pNameTag)
				m_pNameTag->Draw();

			if (m_p3DTextLabelManager)
				m_p3DTextLabelManager->Render();
		}
	}

	m_chatBox->Draw();

	if ( m_pGUI->GetMainMenu () && m_pGUI->GetMainMenu()->IsVisible () )
		m_pGUI->GetMainMenu()->Render ();

	m_pGraphics->ExpireCachedTextures ();

	if ( m_pGUI )
		m_pGUI->Render ();

	if ( IsConnectionProblem () )
		m_pGraphics->DrawText ( (CCore::Instance()->GetGUI()->GetCEGUI()->GetResolution().fX - CCore::Instance()->GetGraphics()->GetTextWidth("Connection Problem", 1.0f, "tahoma-bold") - 5), 5, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, "tahoma-bold", true, "Connection Problem" );

	if (m_pGame && IsGameLoaded())
	{
#ifdef DEBUG
		if (m_pPlayerManager && m_pPlayerManager->GetLocalPlayer() && m_pPlayerManager->GetLocalPlayer()->IsSpawned()) {
			CM2Ped * pPlayerPed = m_pPlayerManager->GetLocalPlayer()->GetPlayerPed();
			M2PlayerControls playerControls = pPlayerPed->GetPed()->m_playerControls;

			CCore::Instance()->GetGraphics()->DrawText(300, 300, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, "tahoma-bold", true, "Is shooting: %s\nMovement State: %d\nModifiers: %d\nMouse Flags: %d\nKeyboard Flags: %d\nIs Aiming: %s\n", playerControls.m_bIsShooting ? "Yes" : "No", playerControls.m_playerMovementState, playerControls.m_byteModifiers, playerControls.m_byteMouseFlags, playerControls.m_byteKeyboardFlags, playerControls.m_bIsAiming ? "Yes" : "No");
		}
#endif

		if ( GetAsyncKeyState ( VK_F10 ) & 0x1 && m_pHud && m_chatBox && m_pPlayerManager )
		{
			m_pHud->Show ( !m_pHud->IsShowing () );
			m_chatBox->SetVisible ( !m_chatBox->IsVisible () );
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
			std::unique_ptr<uint8_t[]> pixels;
			unsigned imageWidth=0,imageHeight=0;

			if( m_pGraphics->GetImage( pixels, imageWidth, imageHeight ) )
			{
				m_pScreenshotManager->BeginWrite( pixels, imageWidth, imageHeight );
			}
			else {
				const char *const errorMessage = "Failed to capture screenshot. (Image capture failed)";
				m_chatBox->Output(CColor(255, 0, 0, 255), errorMessage);
				CLogFile::Printf(errorMessage);
			}
			m_bCaptureScreenshot = false;
		}
		if( m_bRenderNetworkStats )
		{
			m_pGraphics->DrawText( (m_pGUI->GetCEGUI()->GetResolution().fX - 275), 30, 0xFFFFFFFF, 1.0f, "tahoma-bold", false, DT_NOCLIP, CNetworkStats::GetStats().Get() );
		}

		if( m_pClientScriptingManager && !m_pGUI->GetMainMenu()->IsVisible () )
		{
			pArguments.push( true );
			m_pClientScriptingManager->GetEvents()->Call( "onClientFrameRender", &pArguments );
			pArguments.clear();
		}
	}

	m_pGraphics->EndRender();

	if ( m_pFPSCounter )
		m_pFPSCounter->Pulse ();
}

void CCore::OnGameProcess( void )
{
	if (m_bShutdownRequest) {
		if( CCore::Instance()->GetNetworkModule()->IsConnected() )
		{
			CCore::Instance()->GetNetworkModule()->Disconnect( false );
		}

		// Wait for render thread to finish job.
		while (m_bRenderInProgress) {
			Sleep(5);
		}

		// This is not correct as game is "killed" not shutdown with all systems being shutdown
		// likely leaving a lot of memory leaks in game code. This should be done better.

		delete this;
		TerminateProcess(GetCurrentProcess(), 0);
		return;
	}

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

	if( m_pClientScriptingManager )
		m_pClientScriptingManager->GetEvents()->Call( "onClientProcess" );
}


void CCore::OnDeviceRender( void )
{
	DoRender();
	m_bRenderInProgress = false;
}

void CCore::HandleAntiCheatEvent( DWORD dwMessage, unsigned int uiBaseAddress, size_t nSize )
{
	DEBUG_LOG( "HandleAntiCheatEvent - %d, 0x%p, %d", dwMessage, uiBaseAddress, nSize );
	Shutdown();
}

void CCore::StartMultiplayer( void )
{
	if( IsMultiplayerStarted() )
		return;

	DEBUG_LOG( "CCore::StartMultiplayer" );

	m_chatBox->SetVisible( true );
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

	DEBUG_LOG( "CCore::StartMultiplayer - Multiplayer started!" );
}

void CCore::StopMultiplayer( void )
{
	if( !IsMultiplayerStarted() )
		return;

	DEBUG_LOG( "CCore::StopMultiplayer" );

	SetConnectionProblem ( false );
	m_chatBox->SetVisible( false );
	m_pHud->FadeOut( 0 );
	m_pGame->FadeSound( true, 0 );

	//m_chatBox->Clear();
	//m_chatBox->ClearHistory();

	m_pAudioManager->RemoveAll();

	m_pPlayerManager->GetLocalPlayer()->LockControls( true );

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
	DEBUG_LOG( "CCore::StopMultiplayer - Multiplayer stopped!" );
}

void CCore::TakeScreenshot( void )
{
	if( !CCore::Instance()->GetCamera() )
		return;
	m_bCaptureScreenshot = true;
}

/**
 * Called when focus change is detected.
 *
 * @param bFocus @c true if game window is now focused @c false if focus was lost.
 */
void CCore::OnFocusChange( bool bFocus )
{
	if ( bFocus ) {
		if( m_pClientScriptingManager )
		{
			CSquirrelArguments args;
			args.push( false );
			m_pClientScriptingManager->GetEvents()->Call( "onClientFocusChange", &args );
		}

		if (m_pAudioManager) {
			m_pAudioManager->UnmuteAll();
		}
	}
	else {
		if( m_pClientScriptingManager )
		{
			CSquirrelArguments args;
			args.push( true );
			m_pClientScriptingManager->GetEvents()->Call( "onClientFocusChange", &args );
		}

		if (m_pAudioManager) {
			m_pAudioManager->MuteAll();
		}
	}
}

/**
 * Handle the main game window WINAPI message.
 *
 * @param uMsg    The message to handle.
 * @param wParam  Additional message-specific information.
 * @param lParam  Additional message-specific information.
 * @return @c true if message has been processed @c false otherwise.
 */
bool CCore::HandleMessage( UINT uMsg, DWORD wParam, DWORD lParam )
{
	const bool bFocus = (GetForegroundWindow() == m_gameHwnd);

	if (bFocus != m_pGame->Focused()) {
		if (! bFocus) {
			ReleaseCapture();
		}
		else {
			SetCapture(m_gameHwnd);
		}

		OnFocusChange(bFocus);

		m_pGame->SetFocus(bFocus);
	}

	if( bFocus && m_bGameLoaded )
	{
		if (m_chatBox->HandleMessage(uMsg, wParam, lParam)) {
			return true;
		}

		if (m_pGUI->ProcessInput( uMsg, wParam, lParam )) {
			return true;
		}

		if( (m_pNetworkModule && m_pNetworkModule->IsConnected()) && !m_pGUI->GetCEGUI()->IsInputEnabled() )
		{
			if( uMsg == WM_KEYDOWN && (DWORD)wParam == VK_ESCAPE )
			{
				CMainMenu *const pMainMenu = m_pGUI->GetMainMenu();
				pMainMenu->SetVisible( !pMainMenu->IsVisible() );
				return true;
			}

			m_pKeyBinds->ProcessInput( uMsg, wParam, lParam );
		}
	}
	return false;
}
