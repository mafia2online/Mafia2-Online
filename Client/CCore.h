/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CCore.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <Windows.h>
#include <atomic>
#include <memory>

#include "d3d9.h"
#include "d3dx9.h"

#include "ExplicitSingleton.h"

#include "CString.h"

#include "GUI/ChatBox.h"

class CMafia;
class CNetworkModule;
class CGraphics;
class CPlayerManager;
class CFPSCounter;
class CBlipManager;
class CVehicleManager;
class CPedManager;
class CGUI;
class CKeyBinds;
class CStreamer;
class CModelManager;
class CNameTag;
class CAudioManager;
class C3DTextLabelManager;
class M2Game;
class CM2Camera;
class CM2Hud;
class CClientScriptingManager;
class CTimerManager;
class CFileTransferManager;
class String;
class CScreenShot;
class CM2TextDatabase;

class CCore : public ExplicitSingleton<CCore>
{

private:

	bool							m_bInitialised;
	bool							m_bGameLoaded;
	unsigned int					m_uiBaseAddress;
	bool							m_bMultiplayerStarted;
	bool							m_bCaptureScreenshot;
	bool							m_bRenderNetworkStats;
	bool							m_bConnectionProblem;

	/** Is shutdown request received from any thread? */
	std::atomic_bool				m_bShutdownRequest;

	/** Is render thread rendering stuff? */
	std::atomic_bool				m_bRenderInProgress;

	CMafia							* m_pGame;
	CNetworkModule					* m_pNetworkModule;
	CGraphics						* m_pGraphics;
	std::unique_ptr<ChatBox>		m_chatBox;
	CPlayerManager					* m_pPlayerManager;
	CFPSCounter						* m_pFPSCounter;
	CBlipManager					* m_pBlipManager;
	CVehicleManager					* m_pVehicleManager;
	CPedManager						* m_pPedManager;
	CGUI							* m_pGUI;
	CKeyBinds						* m_pKeyBinds;
	CStreamer						* m_pStreamer;
	CModelManager					* m_pModelManager;
	CNameTag						* m_pNameTag;
	CAudioManager					* m_pAudioManager;
	C3DTextLabelManager				* m_p3DTextLabelManager;
	CScreenShot						* m_pScreenshotManager;
	CClientScriptingManager			* m_pClientScriptingManager;
	CTimerManager					* m_pTimerManager;
	CFileTransferManager			* m_pFileTransferManager;

	CM2Camera						* m_pCamera;
	CM2Hud							* m_pHud;
	CM2TextDatabase					* m_pTextDatabase;

	HWND							m_gameHwnd;

	String							m_strNick;
	String							m_strHost;
	unsigned long					m_ulPort;
	String							m_strPass;

	String							m_strServerName;
	unsigned						m_uServerMaxPlayers;
	bool							m_bGlobalSpeedLimited;
	bool							m_bSummer;

	String							m_strGameDirectory;
	String							m_strModDirectory;

	void							DoRender					( void );

public:

									CCore						( void );
									~CCore						( void );

	bool							Initialise					( void );
	void							Shutdown					( void );

	void							OnGameLoad					( void );
	void							OnGamePreLoad				( void );
	void							OnGameProcess				( void );

	void							HandleAntiCheatEvent		( DWORD dwMessage, unsigned int uiBaseAddress, size_t nSize );

	void							SetGameLoaded				( bool bLoaded ) { m_bGameLoaded = bLoaded; }
	bool							IsGameLoaded				( void ) { return m_bGameLoaded; }

	void							SetMultiplayerStarted		( bool bStarted ) { m_bMultiplayerStarted = bStarted; }
	bool							IsMultiplayerStarted		( void ) { return m_bMultiplayerStarted; }

	void							OnDeviceCreate				( IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters );
	void							OnDeviceLost				( IDirect3DDevice9 * pDevice );
	void							OnDeviceReset				( IDirect3DDevice9 * pDevice );
	void							OnDevicePreRender			( void );
	void							OnDeviceRender				( void );

	unsigned int					GetBaseAddress				( void ) { return m_uiBaseAddress; }

	CMafia							* GetGame					( void ) { return m_pGame; }
	CNetworkModule					* GetNetworkModule			( void ) { return m_pNetworkModule; }
	CGraphics						* GetGraphics				( void ) { return m_pGraphics; }
	CPlayerManager					* GetPlayerManager			( void ) { return m_pPlayerManager; }
	CFPSCounter						* GetFPSCounter				( void ) { return m_pFPSCounter; }
	CBlipManager					* GetBlipManager			( void ) { return m_pBlipManager; }
	CVehicleManager					* GetVehicleManager			( void ) { return m_pVehicleManager; }
	CPedManager						* GetPedManager				( void ) { return m_pPedManager; }
	CGUI							* GetGUI					( void ) { return m_pGUI; }
	CKeyBinds						* GetKeyBinds				( void ) { return m_pKeyBinds; }
	CStreamer						* GetStreamer				( void ) { return m_pStreamer; }
	CModelManager					* GetModelManager			( void ) { return m_pModelManager; }
	CAudioManager					* GetAudioManager			( void ) { return m_pAudioManager; }
	C3DTextLabelManager				* Get3DTextLabelManager		( void ) { return m_p3DTextLabelManager; }
	CScreenShot						* GetScreenshotManager		( void ) { return m_pScreenshotManager; }

	void							SetGameHwnd					( HWND hwnd ) { m_gameHwnd = hwnd; }
	HWND							GetGameHwnd					( void ) { return m_gameHwnd; }

	void							SetCamera					( CM2Camera * pCamera ) { m_pCamera = pCamera; }
	CM2Camera						* GetCamera					( void ) { return m_pCamera; }

	void							SetHud						( CM2Hud * pHud ) { m_pHud = pHud; }
	CM2Hud							* GetHud					( void ) { return m_pHud; }

	void							SetTextDatabase				(CM2TextDatabase *pTextDB){ m_pTextDatabase = pTextDB; }
	CM2TextDatabase					* GetTextDatabase			(){ return m_pTextDatabase; }

	void							SetNick						( String strNick ) { m_strNick = strNick; }
	String							GetNick						( void ) { return m_strNick; }
	void							SetHost						( String strHost ) { m_strHost = strHost; }
	String							GetHost						( void ) { return m_strHost; }
	void							SetPort						( unsigned long ulPort ) { m_ulPort = ulPort; }
	unsigned long					GetPort						( void ) { return m_ulPort; }
	void							SetPass						( String strPass ) { m_strPass = strPass; }
	String							GetPass						( void ) { return m_strPass; }

	void							SetGameDirectory			( String strDir ) { m_strGameDirectory = strDir; }
	String							GetGameDirectory			( void ) { return m_strGameDirectory; }

	void							SetModDirectory				( String strDir ) { m_strModDirectory = strDir; }
	String							GetModDirectory				( void ) { return m_strModDirectory; }

	void							SetServerName				( String strServerName ) { m_strServerName = strServerName; }
	String							GetServerName				( void ) { return m_strServerName; }

	void							SetServerMaxPlayers			( unsigned iMaxPlayers ) { m_uServerMaxPlayers = iMaxPlayers; }
	unsigned						GetServerMaxPlayers			( void ) { return m_uServerMaxPlayers; }

	void							SetGlobalSpeedLimited		( bool bLimited ) { m_bGlobalSpeedLimited = bLimited; }
	bool							IsGlobalSpeedLimited		( void ) { return m_bGlobalSpeedLimited; }

	void							SetSummer					( bool bSummer ) { m_bSummer = bSummer; }
	bool							IsSummer					( void ) { return m_bSummer; }

	CClientScriptingManager			* GetClientScriptingManager	( void ) { return m_pClientScriptingManager; }
	CTimerManager					* GetTimerManager			( void ) { return m_pTimerManager; }
	CFileTransferManager			* GetFileTransferManager	( void ) { return m_pFileTransferManager; }

	void							StartMultiplayer			( void );
	void							StopMultiplayer				( void );

	void							TakeScreenshot				( void );

	void							SetConnectionProblem		( bool bConnectionProblem ) { m_bConnectionProblem = bConnectionProblem; }
	bool							IsConnectionProblem			( void ) { return m_bConnectionProblem; }

	void							OnFocusChange				( bool bFocus );

	bool							HandleMessage				( UINT uMsg, DWORD wParam, DWORD lParam );
};
