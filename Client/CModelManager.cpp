/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game
* File       : CModelManager.cpp
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#include "StdInc.h"

void M2ModelMgr::ChangeModel( const char * pszDir, const char * pszModel, int iHumanColour )
{
	CModelManager::SetDir( pszDir );

	// Change model
	_asm push iHumanColour;
	_asm push pszModel;
	_asm mov ecx, this;
	_asm call COffsets::FUNC_CModelMgr__ChangeModel;

	CModelManager::SetDir( SDS_LOAD_DIR_PLAYER );
}

char * CModelManager::GetDir( void )
{
	return ( char *)( SDS_LOAD_DIR_ADDR );
}

void CModelManager::SetDir( const char * pszDirectory )
{
	char * pszLoadPath = ( char *)( SDS_LOAD_DIR_ADDR );

	DWORD dwOldProtect = 0;
	VirtualProtect( pszLoadPath, 0x20, PAGE_READWRITE, &dwOldProtect );

	// modify path
	sprintf( pszLoadPath, "%s%%s.sds", pszDirectory );

	VirtualProtect( pszLoadPath, 0x20, dwOldProtect, &dwOldProtect );
}

M2ModelMgr * CModelManager::LoadModel( const char * pszDirectory, const char * pszFileName )
{
	if ( m_ModelMgrs.size() >= MODELMGR_MAX )
		return NULL;

	M2ModelMgr * pModelMgr = ( M2ModelMgr *)( IE::Malloc( 0x50 ) );

	if ( !pModelMgr )
	{
		CLogFile::Printf( "Can't allocate memory for model manager!" );
		return NULL;
	}

	// Construct the model manager
	pModelMgr->Construct ();

	// dirty hack :D
	SetDir( pszDirectory );
	bool bLoadResult = pModelMgr->Load( pszFileName );
	SetDir( SDS_LOAD_DIR_PLAYER );

	CLogFile::Printf ( "CModelManager::LoadModel - Result: %s", (bLoadResult ? "true" : "false") );

	// Did the model fail to load?
	if ( !bLoadResult )
	{
		// Delete the model manager
		pModelMgr->Free ();

		return NULL;
	}

	m_ModelMgrs.push_back( pModelMgr );

	return pModelMgr;
}

bool CModelManager::FreeModel( const char * pszModelName )
{
	for ( unsigned int I = 0; I < m_ModelMgrs.size( ); I++ )
	{
		M2ModelMgr * pModelMgr = m_ModelMgrs[I];

		if ( !pModelMgr )
			continue;

		if ( !pModelMgr->GetSlot( ) )
			continue;

		if ( pModelMgr->GetSlot()->GetModelByName( pszModelName ) )
		{
			m_ModelMgrs.erase( m_ModelMgrs.begin() + I );
			pModelMgr->Free ();
			return true;
		}
	}

	return false;
}

M2Model * CModelManager::GetModelByName( const char * pszModelName )
{
	M2ModelMgr * pModelMgr = GetModelMgrByName( pszModelName );

	return ( pModelMgr ? pModelMgr->GetModel( ) : NULL );
}

M2Model * CModelManager::GetModelByIndex( int nIndex )
{
	M2ModelMgr * pModelMgr = GetModelMgrByIndex( nIndex );

	return ( pModelMgr ? pModelMgr->GetModel( ) : NULL );
}

M2ModelMgr * CModelManager::GetModelMgrByName( const char * pszModelName )
{
	// Loop over all loaded models
	for ( std::vector< M2ModelMgr* >::iterator iter = m_ModelMgrs.begin(); iter != m_ModelMgrs.end(); iter++ )
	{
		// Get a pointer to the current model manager
		M2ModelMgr * pModelManager = *iter;

		// Is the current model manager valid?
		if ( *iter && (*iter)->GetSlot () )
		{
			// Is this the model manager we're looking for?
			if ( (*iter)->GetSlot()->GetModelByName ( pszModelName ) )
				return *iter;
		}
	}

	return NULL;
}

M2ModelMgr * CModelManager::GetModelMgrByIndex( int nIndex )
{
	if ( m_ModelMgrs.size() >= MODELMGR_MAX )
		return NULL;

	return m_ModelMgrs[nIndex];
}