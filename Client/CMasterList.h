/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMasterList.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	<thread>

#include "CServerList.h"
#include "Network/CHttpClient.h"

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
	void WorkerThread();

	void OnRefreshFailed( const char *errorMessage );
	void OnRefreshSuccess( String *serverListData );

private:
	std::unique_ptr<CHttpClient> m_pHttpClient;

	QueryHandler_t               m_queryHandler;

	std::thread					m_refreshThread;
	mutable bool				m_forceWorkerShutdown;
	mutable RefreshState		m_refreshState;
	mutable eRefreshType		m_refreshType;

	std::shared_ptr<CGUIMessageBox_Impl> m_pMessageBox;
};