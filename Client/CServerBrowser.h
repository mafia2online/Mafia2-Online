/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CServerBrowser.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#define MAX_SERVER_LISTS							3

enum ServerBrowserType
{
	INTERNET,
	HOSTED,
	LAN,
	FAVOURITES,
	HISTORY
};

class CMasterList;
class CServerList;
class CServerListItem;

enum eConnectionState
{
	CONNECTION_STATE_NONE = 0,
	CONNECTION_STATE_WAITING_FOR_GAME = 1,
	CONNECTION_STATE_CONNECTING = 2,
	CONNECTION_STATE_CONNECTED = 3,
	CONNECTION_STATE_FAILED = 4
};

class CServerBrowser
{

	friend class CMainMenu;

private:

	CGUI_Impl										* m_pGUI;
	CMasterList										* m_pMasterList;
	CServerQuery									* m_pServerQuery;

	//
	unsigned long									m_ulLastRefreshTime;

	// Gui Widgets
	CGUIWindow_Impl									* m_pWindow;
	CGUITabPanel_Impl								* m_pTabs;
	CGUITab_Impl									* m_pTab[MAX_SERVER_LISTS];
	CGUIGridList_Impl								* m_pServerGridList[MAX_SERVER_LISTS];
	CGUIGridList_Impl								* m_pPlayersGridList;
	CGUITabPanel_Impl								* m_pPlayers;
	CGUITab_Impl									* m_pPlayersTab;
	CGUITab_Impl									* m_pRulesTab;
	CGUIButton_Impl									* m_pRefresh;
	CGUIButton_Impl									* m_pConnect;
	CGUIButton_Impl									* m_pFavourite;
	CGUILabel_Impl									* m_pStatus;
	CGUIStaticImage_Impl							* m_pLockedImage;
	CGUILabel_Impl									* m_pStatusLabel;

	// Counts
	int												m_iTotalServers;
	int												m_iTotalPlayers;
	int												m_iAvailableSlots;

	// Quick Connect
	CGUIWindow_Impl									* m_pQuickConnectWindow;
	CGUILabel_Impl									* m_pQuickConnectLabel;
	CGUIEdit_Impl									* m_pQuickConnectAddress;
	CGUIEdit_Impl									* m_pQuickConnectPort;
	CGUIEdit_Impl									* m_pQuickConnectPassword;
	CGUIButton_Impl									* m_pQuickConnectSubmit;
	CGUIButton_Impl									* m_pQuickConnectCancel;

	// Quick connect events
	bool				Event_QuickConnectSubmitClick( CGUIElement_Impl * pElement );
	bool				Event_QuickConnectCancelClick( CGUIElement_Impl * pElement );
	bool				Event_QuickConnectInputFocus ( CGUIElement_Impl * pElement );

	// Server lists
	CServerList										* m_pServerList[MAX_SERVER_LISTS];

	// Gui Events
	bool				Event_OnMouseClick			( CGUIElement_Impl * pElement );
	static bool			Event_OnMouseDoubleClick	( CGUIMouseEventArgs args );

	// Masterlist Events
	static void			Event_MasterListQueryHandler( const std::vector<String>& servers );

	// Connection messages
	eConnectionState								m_connectionState;
	String											m_strServerIP;
	int												m_iServerPort;
	String											m_strServerPassword;
	unsigned long									m_ulFailedTime;
	int												m_iSelectedServer[MAX_SERVER_LISTS];

	//
	CGUIMessageBox_Impl								* m_pMessageBox;
	static void			Button1ClickHandler			( void * pUserData );

	//
	CServerPassword									* m_pServerPassword;
	CServerListItem									* m_pSelectedServer;

	static	void		ServerPasswordHandler		( String strPassword, void * pUserData );

public:

						CServerBrowser				( CGUI_Impl * pGUI );
						~CServerBrowser				( void );

	void				Pulse						( void );

	void				SetupUI						( float fX, float fY, float fWidth, float fHeight );

	void				CreateTab					( ServerBrowserType type, const char * szName, float fWidth, float fHeight );
	void				DeleteTab					( ServerBrowserType type );

	void				AddServer					( ServerBrowserType type, CServerListItem * pServer );
	void				ConnectToSelectedServer		( void );
	void				StartConnection				( void );
	void				ProcessConnection			( void );

	void				Refresh						( void );

	void				OnClick						( CGUIElement_Impl * pElement );
	void				OnDoubleClick				( CGUIElement_Impl * pElement );

	void				SetVisible					( bool bVisible );
	bool				IsVisible					( void );

	void				RefreshFromFile				( const char * szFile );

	CGUILabel_Impl		* GetStatusLabel			( void ) { return m_pStatus; }
	CServerList			* GetServerList				( ServerBrowserType type );
	CGUIGridList_Impl	* GetServerGridList			( ServerBrowserType type ) { return m_pServerGridList[ type ]; }

	ServerBrowserType	GetCurrentServerBrowserType	( void );

	void				ProcessNetworkPacket		( DefaultMessageIDTypes packet );
	void				SetConnectionState			( eConnectionState connection ) { m_connectionState = connection; }
	eConnectionState	GetConnectionState			( void ) { return m_connectionState; }

	void				SetDisconnectReason			( bool bDisconnect, const char * szReason, ... );
	void				SetMessageBox				( const char * szTitle, const char * szCaption );

	CMasterList			* GetMasterList				( void ) { return m_pMasterList; }
	CServerQuery		* GetServerQuery			( void ) { return m_pServerQuery; }

	void				OnScreenSizeChange			( float fX, float fY );

	CGUIMessageBox_Impl	* GetMessageBox				( void ) { return m_pMessageBox; }

};