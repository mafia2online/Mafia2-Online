/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMasterList.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CServerList.h"

enum eRefreshType
{
	E_REFRESH_NONE,
	E_REFRESH_HOSTED
};

class CMasterList
{
	typedef void (* QueryHandler_t)( const std::vector<String>& servers );

	enum RefreshState
	{
		RefreshState_Ready,
		RefreshState_InProgress,
		RefreshState_UpdateRequired
	};

public:
	 CMasterList( QueryHandler_t handler );
	~CMasterList();

	bool Refresh( eRefreshType type = eRefreshType::E_REFRESH_NONE );

private:
	void WorkerThread( eRefreshType type );

	void OnRefreshFailed( const char *errorMessage );
	void OnRefreshSuccess( String *serverListData );

private:
	CHttpClient                 *m_pHttpClient;
	
	QueryHandler_t               m_queryHandler;
	 
	unsigned long                m_lastRefreshTime;
	
	std::thread                 *m_refreshThread;
	std::atomic<RefreshState>    m_refreshState;

	CGUIMessageBox_Impl         *m_pMessageBox;
};