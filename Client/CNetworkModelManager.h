/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkModelManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CString.h"
#include "engine/CM2ModelManager.h"
#include "CModelManager.h"

class CNetworkModelManager;
class CNetworkModel
{

	friend class CNetworkModelManager;

private:

	DWORD								m_dwRefCount;
	CM2ModelManager						* m_pModelMgr;
	String								m_strModelName;

public:

										CNetworkModel		( CM2ModelManager * pModelMgr, const char * szModelName );
										~CNetworkModel		( void );

	CM2ModelManager						* GetModelMgr		( void ) { return m_pModelMgr; }

};

class CNetworkModelManager
{

private:

	static	std::list< CNetworkModel* >	m_loadedModels;

	static	CNetworkModel				* Get				( const char * szModel );
	static	CNetworkModel				* Get				( CM2ModelManager * pModelMgr );

public:

	static	CM2ModelManager				* Load				( const char * szDir, const char * szModel );
	static	void						Unload				( CM2ModelManager * pModelMgr );

	static	void						Cleanup				( void );

};