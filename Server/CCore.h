/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CCore.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CCore : public CCoreInterface
{

private:

	bool														m_bActive;

	int															m_iResourcesLoaded;
	int															m_iFailedResources;

	CNetworkModule												* m_pNetworkModule;
	CPlayerManager												* m_pPlayerManager;
	CScriptingManager											* m_pScriptingManager;
	CEvents														* m_pEvents;
	CCommands													* m_pCommands;
	CMasterList													* m_pMasterList;
	CQuery														* m_pQuery;
	CWebServer													* m_pWebServer;
	CClientScriptingManager										* m_pClientScriptingManager;
	CTimerManager												* m_pTimerManager;
	CBanManager													* m_pBanManager;
	CModuleManager												* m_pModuleManager;
	CBlipManager												* m_pBlipManager;
	CVehicleManager												* m_pVehicleManager;
	CResourceManager											* m_pResourceManager;
	CWebRequest													* m_pWebRequest;
	CPedManager													* m_pPedManager;

	String														m_strGameModeText;
	String														m_strMapName;
	String														m_strWeather;
	bool														m_bSummer;

	String														m_strMasterCode;

public:

									CCore						( void );
									~CCore						( void );

	bool							Initialise					( int argc, char * argv[] );
	void							Deinitialise				( void );

	void							Pulse						( void );

	void							SetActive					( bool bActive ) { m_bActive = bActive; }
	bool							IsActive					( void ) { return m_bActive; }

	void							UpdateResourceTotals		( int iLoaded, int iFailed ) { m_iResourcesLoaded += iLoaded; m_iFailedResources += iFailed; }

	CNetworkModule					* GetNetworkModule			( void ) { return m_pNetworkModule; }
	CPlayerManager					* GetPlayerManager			( void ) { return m_pPlayerManager; }
	CScriptingManager				* GetScriptingManager		( void ) { return m_pScriptingManager; }
	CEvents							* GetEvents					( void ) { return m_pEvents; }
	CCommands						* GetCommands				( void ) { return m_pCommands; }
	CMasterList						* GetMasterList				( void ) { return m_pMasterList; }
	CQuery							* GetQuery					( void ) { return m_pQuery; }
	CWebServer						* GetWebServer				( void ) { return m_pWebServer; }
	CClientScriptingManager			* GetClientScriptingManager	( void ) { return m_pClientScriptingManager; }
	CTimerManager					* GetTimerManager			( void ) { return m_pTimerManager; }
	CBanManager						* GetBanManager				( void ) { return m_pBanManager; }
	CModuleManager					* GetModuleManager			( void ) { return m_pModuleManager; }
	CBlipManager					* GetBlipManager			( void ) { return m_pBlipManager; }
	CVehicleManager					* GetVehicleManager			( void ) { return m_pVehicleManager; }
	CResourceManager				* GetResourceManager		( void ) { return m_pResourceManager; }
	CWebRequest						* GetWebRequest				( void ) { return m_pWebRequest; }
	CPedManager						* GetPedManager				( void ) { return m_pPedManager; }

	void							SetGameModeText				( const char * szGameModeText ) { m_strGameModeText.Set ( szGameModeText ); }
	const char						* GetGameModeText			( void ) { return m_strGameModeText.Get(); }

	void							SetMapName					( const char * szMapName ) { m_strMapName.Set ( szMapName ); }
	const char						* GetMapName				( void ) { return m_strMapName.Get(); }

	void							SetWeather					( const char * szWeather ) { m_strWeather.Set ( szWeather ); }
	const char						* GetWeather				( void ) { return m_strWeather.Get(); }

	void							SetSummer					( bool bSummer ) { m_bSummer = bSummer; }
	bool							IsSummer					( void ) { return m_bSummer; }

	bool							IsPasswordProtected			( void ) { return (strlen(CVAR_GET_STRING( "password" )) > 0); }

	void							SetMasterCode				( String strCode ) { m_strMasterCode = strCode; }
	String							GetMasterCode				( void ) { return m_strMasterCode; }

};