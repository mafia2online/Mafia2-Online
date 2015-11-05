/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkModelManager.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CNetworkModelManager;
class CNetworkModel
{

	friend class CNetworkModelManager;

private:

	DWORD								m_dwRefCount;
	M2ModelMgr							* m_pModelMgr;
	String								m_strModelName;

public:

										CNetworkModel		( M2ModelMgr * pModelMgr, const char * szModelName );
										~CNetworkModel		( void );

	M2ModelMgr							* GetModelMgr		( void ) { return m_pModelMgr; }

};

class CNetworkModelManager
{

private:

	static	std::list< CNetworkModel* >	m_loadedModels;

	static	CNetworkModel				* Get				( const char * szModel );
	static	CNetworkModel				* Get				( M2ModelMgr * pModelMgr );

public:

	static	M2ModelMgr					* Load				( const char * szDir, const char * szModel );
	static	void						Unload				( M2ModelMgr * pModelMgr );

	static	void						Cleanup				( void );

};