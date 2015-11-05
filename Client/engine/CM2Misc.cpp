/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Misc.cpp
* Developers : h0pk1nz <>
*
***************************************************************/

#include "../StdInc.h"
#include "CM2Misc.h"

extern	CCore				* pCore;

typedef C_TrafficModelDescriptor * ( __thiscall * CTrafficModelDesc__constructor_t )( void *, void *, int, char * );
CTrafficModelDesc__constructor_t CTrafficModelDesc__constructor = ( CTrafficModelDesc__constructor_t )( 0x00A16980 );

typedef bool ( __thiscall * CTrafficModelDesc__LoadModel_t )( void *, char * );
CTrafficModelDesc__LoadModel_t CTrafficModelDesc__LoadModel = ( CTrafficModelDesc__LoadModel_t )( 0x00AD6790 );

C_TrafficActorCache * GetTrafficActorCache( void )
{
	return *( C_TrafficActorCache **)( 0x01BACA94 );
}

C_TrafficModelDescriptor * LoadHumanModel( char * szModelFileName )
{
	// Allocate memory
	C_TrafficModelDescriptor * pDescriptor = (C_TrafficModelDescriptor *)pCore->GetGame()->allocate( 0x1C );

	// Did we fail to allocate enough memory?
	if( !pDescriptor )
		return NULL;

	// Call the traffic model descriptor constructor
	CTrafficModelDesc__constructor( pDescriptor, NULL, 2, "NORMAL_CHARACTER" );

	// Try and load the model
	if( !CTrafficModelDesc__LoadModel( pDescriptor, szModelFileName ) )
	{
		CLogFile::Printf( "[ERROR] - CTrafficModelDesc__LoadModel - Failed to load model '%s'.", szModelFileName );
		
		// Delete the descriptor memory
		SAFE_DELETE( pDescriptor );
		
		return NULL;
	}

	CLogFile::Printf( "[ERROR] - CTrafficModelDesc__LoadModel - Loaded model '%s'.", szModelFileName );
	return pDescriptor;
}

C_TrafficCacheCharacter * GetCacheObjectByModelName( const char * pszInputModelName )
{
	C_TrafficActorCache * pActorCache = GetTrafficActorCache( );

	if ( !pActorCache )
		return NULL;

	for ( int I = 0; I < pActorCache->GetActorCacheSize(); I++ )
	{
		C_TrafficCacheObject * pCacheObject = pActorCache->GetActorCache( I );

		if ( pCacheObject->GetModelDescriptor( ) )
		{
			if ( pCacheObject->GetActorType( ) == OBJTYPE_Human )
			{
				const char * pszModelName = pCacheObject->GetModelDescriptor()->GetUnkData()->GetModelFilePath();

				char szModelFilePath[255] = { 0 };
				sprintf( szModelFilePath, "/sds/Traffic/%s.sds", pszInputModelName );

				if ( pszModelName && !strcmpi( pszModelName, szModelFilePath ) )
					return ( C_TrafficCacheCharacter *)( pCacheObject );
			}
		}
	}

	return NULL;
}

C_TrafficCacheObject * GetCacheCarObjectByModelName( const char * pszInputModelName )
{
	C_TrafficActorCache * pActorCache = GetTrafficActorCache( );

	if ( !pActorCache )
		return NULL;

	for ( int I = 0; I < pActorCache->GetActorCacheSize(); I++ )
	{
		C_TrafficCacheObject * pCacheObject = pActorCache->GetActorCache( I );

		if ( pCacheObject->GetModelDescriptor( ) )
		{
			if ( pCacheObject->GetActorType( ) == OBJTYPE_Car )
			{
				C_TrafficCarModelDescriptor * pModelDesc = ( C_TrafficCarModelDescriptor *)pCacheObject->GetModelDescriptor( );

				const char * pszModelName = pModelDesc->GetUnkData()->GetModelFilePath();

				char szModelFilePath[255] = { 0 };
				sprintf( szModelFilePath, "%s", pszInputModelName ); // /sds/Cars/%s.sds

				if ( pszModelName && !strcmpi( pszModelName, szModelFilePath ) )
					return pCacheObject;
			}
		}
	}

	return NULL;
}