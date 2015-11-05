/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkModelManager.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"

extern	CCore		* pCore;
std::list< CNetworkModel* > CNetworkModelManager::m_loadedModels;

CNetworkModel::CNetworkModel( M2ModelMgr * pModelMgr, const char * szModel )
{
	//
	m_pModelMgr = pModelMgr;
	m_strModelName.Set( szModel );
	m_dwRefCount = 0;
}

CNetworkModel::~CNetworkModel( void )
{
#ifdef DEBUG
	CLogFile::Printf( "CNetworkModel::~CNetworkModel - M2ModelMgr: 0x%p", m_pModelMgr );
#endif

	// Free the model
	if ( m_pModelMgr )
		m_pModelMgr->Free ();

	// Clear the model name
	m_strModelName.clear();
}

M2ModelMgr * CNetworkModelManager::Load( const char * szDir, const char * szModel )
{
	// Try get the network model
	CNetworkModel * pNetModel = Get( szModel );

	// Is the net model invalid?
	if( !pNetModel )
	{
		// Try get the model from the SDS model manager
		M2ModelMgr * pModelMgr = pCore->GetModelManager()->GetModelMgrByName( szModel );

		// Is the model not loaded?
		if( pCore->GetModelManager() && !pModelMgr )
		{
#ifdef DEBUG
			CLogFile::Printf( "CNetworkModelManager::Load() - Loading model '%s' (%s)", szModel, szDir );
#endif
			// Load the model
			pModelMgr = pCore->GetModelManager()->LoadModel( szDir, szModel );
		}

#ifdef DEBUG
		CLogFile::Printf( "CNetworkModelManager::Load() - Done! 0x%p", pModelMgr );
#endif

		// Create the network model instance
		pNetModel = new CNetworkModel ( pModelMgr, szModel );

		// Add the model to the loaded models list
		m_loadedModels.push_back ( pNetModel );
	}

	// Increase the ref count on the model
	pNetModel->m_dwRefCount ++;

#ifdef DEBUG
	CLogFile::Printf( "CNetworkModelManager::Load - Loaded model! 0x%p (RefCount: %d)", pNetModel->m_pModelMgr, pNetModel->m_dwRefCount );
#endif
	return pNetModel->m_pModelMgr;
}

void CNetworkModelManager::Unload( M2ModelMgr * pModelMgr )
{
	// Try get the network model
	CNetworkModel * pNetModel = Get( pModelMgr );

	// Is the net model invalid?
	if( !pNetModel )
		return;

	// Decrease the ref count on the model
	pNetModel->m_dwRefCount --;

#ifdef DEBUG
	CLogFile::Printf( "CNetworkModelManager::Unload - RefCount: %d", pNetModel->m_dwRefCount );
#endif

	// Are we done with the model?
	if( pNetModel->m_dwRefCount == 0 )
	{
		// Remove the network model from the loaded list
		m_loadedModels.remove( pNetModel );

		// Delete the network model
		SAFE_DELETE( pNetModel );

#ifdef DEBUG
		CLogFile::Printf( "CNetworkModelManager::Unload - Unloaded model!" );
#endif
	}
}

CNetworkModel * CNetworkModelManager::Get( const char * szModel )
{
	// Loop over our list
	for( std::list< CNetworkModel* >::iterator iter = m_loadedModels.begin(); iter != m_loadedModels.end(); iter++ )
	{
		// Get a pointer to the current network model
		CNetworkModel * pNetModel = *iter;

		// Is the current network model valid and what we're looking for?
		if( pNetModel && !strcmp( pNetModel->m_strModelName.Get(), szModel ) )
			return pNetModel;
	}

	return NULL;
}

CNetworkModel * CNetworkModelManager::Get( M2ModelMgr * pModelMgr )
{
	// Loop over our list
	for( std::list< CNetworkModel* >::iterator iter = m_loadedModels.begin(); iter != m_loadedModels.end(); iter++ )
	{
		// Get a pointer to the current network model
		CNetworkModel * pNetModel = *iter;

		// Is the current network model valid and what we're looking for?
		if( pNetModel && pNetModel->m_pModelMgr == pModelMgr )
			return pNetModel;
	}

	return NULL;
}

void CNetworkModelManager::Cleanup ( void )
{
	// Delete all model instances
	for( std::list< CNetworkModel* >::iterator iter = m_loadedModels.begin(); iter != m_loadedModels.end(); iter++ ) {
		SAFE_DELETE( *iter );
	}

	// Clear the loaded models list
	m_loadedModels.clear ();

	// Clear the model manager list
	pCore->GetModelManager()->Clear ();
}