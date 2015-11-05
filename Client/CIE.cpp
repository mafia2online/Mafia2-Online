/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CIE.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			   h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#include	"StdInc.h"
#include	"CModelManager.h"
#include	"engine/CM2Misc.h"

extern	CCore		* pCore;

typedef void * ( __cdecl * CreateObjectByType_t )( int );
CreateObjectByType_t CreateObjectByType = ( CreateObjectByType_t )( 0x0115F840 );

typedef int ( __thiscall * CModel__AssignModel_t )( M2Model *, M2Model *);
CModel__AssignModel_t CModel__AssignModel = ( CModel__AssignModel_t )( 0x014E2970 );

typedef void ( __thiscall * CModel__SetNameHashByName_t )( M2Model *, char * );
CModel__SetNameHashByName_t CModel__SetNameHashByName = ( CModel__SetNameHashByName_t )( 0x014AEA40 );

typedef void ( __thiscall * CModel__SetFlag_t )( M2Model *, int );
CModel__SetFlag_t CModel__SetFlag = ( CModel__SetFlag_t )( 0x014AEAC0 );

typedef void * ( __thiscall * CEntity__SetModel_t )( M2Entity *, M2Model *);
CEntity__SetModel_t CEntity__SetModel = ( CEntity__SetModel_t )( 0x0115CB10 );

typedef void * ( __thiscall * CEntity__UnkFunc_t )( M2Entity* );
CEntity__UnkFunc_t CEntity__UnkFunc = ( CEntity__UnkFunc_t )( 0x0098DA20 );

typedef void * ( __cdecl * GetHumanParamsByTableIndex_t )( int, int );
GetHumanParamsByTableIndex_t GetTrafficHumanParams = ( GetHumanParamsByTableIndex_t )( 0x00A2FD60 );

typedef bool ( __thiscall * CEntity__SetHumanParams_t )( M2Entity *, void * );
CEntity__SetHumanParams_t CEntity__SetHumanParams = ( CEntity__SetHumanParams_t )( 0x0093E6A0 );

typedef void * ( __thiscall * GetHashedString_t )( void **, char * );
GetHashedString_t GetHashedString = ( GetHashedString_t )( 0x180F3E0 );

typedef M2Model * ( __thiscall * LoadModelFromHashName_t )( void * pszModelHashName );
LoadModelFromHashName_t LoadModelFromHashName = ( LoadModelFromHashName_t )( 0x124F2F0 );

typedef void ( __thiscall * CObject__Setup_t )( M2Entity * );
CObject__Setup_t CCar__Setup = ( CObject__Setup_t )( 0x00A04B40 );

typedef bool ( __thiscall * CObject__SetParams_t )( M2Entity *, void * );
CObject__SetParams_t CObject__SetCarParams = ( CObject__SetParams_t )( 0x009906B0 );

M2Window * M2Window::GetInstance( void )
{
	static M2Window * pWindow;

	if( !pWindow )
		pWindow = *(M2Window **)COffsets::VAR_CWindow;

	return pWindow;
}

M2Game * M2Game::GetInstance( void )
{
	static M2Game * pGame;

	if( !pGame )
		pGame = *(M2Game **)COffsets::VAR_CGame;

	return pGame;
}

M2ScriptEngine * M2ScriptEngine::GetInstance( void )
{
	static M2ScriptEngine * pScriptEngine;

	if( !pScriptEngine )
		pScriptEngine = *(M2ScriptEngine **)COffsets::VAR_CGame__ScriptEngine;

	return pScriptEngine;
}

M2WrapperList * M2WrapperList::GetInstance( void )
{
	static M2WrapperList * pWrapperList;

	if( !pWrapperList )
		pWrapperList = *(M2WrapperList **)COffsets::VAR_CWrapperList;

	return pWrapperList;
}

M2VehiclePool * M2VehiclePool::GetInstance( void )
{
	static M2VehiclePool * pVehiclePool;

	if( !pVehiclePool )
		pVehiclePool = *(M2VehiclePool **)COffsets::VAR_VehiclePool;

	return pVehiclePool;
}

namespace IE
{
	M2Game * pGame = NULL;
	M2Window * pWindow = NULL;
	M2ScriptEngine * pScriptEngine = NULL;
	M2WrapperList * pWrapperList = NULL;
	M2VehiclePool * pVehiclePool = NULL;

	M2Game * GetGame( void ) { return pGame; }
	M2Window * GetWindow( void ) { return pWindow; }
	M2ScriptEngine * GetScriptEngine( void ) { return pScriptEngine; }
	M2WrapperList * GetWrapperList( void ) { return pWrapperList; }
	M2VehiclePool * GetVehiclePool( void ) { return pVehiclePool; }

	M2Ped * CreatePlayerPed ( void )
	{
		// Get the model name and directory
		String strModel, strDirectory;
		Game::GetPlayerModelFromId( 0, &strModel, &strDirectory );

		// Load the model
		M2ModelMgr* pModelMgr = CNetworkModelManager::Load ( strDirectory.Get(), strModel.Get() );

		// Create the player entity
		M2Ped* pPed = (M2Ped *)CreateObjectByType ( OBJTYPE_Player );

		// Allocate the model memory
		M2Model* pModel = CM2Core::GetInstance()->AllocateModel ();

		// Assign the model
		CModel__AssignModel ( pModel, pModelMgr->GetModel () );

		// Set the model name hash
		CModel__SetNameHashByName ( pModel, "m2mp_playerped" );

		// Set model flags
		CModel__SetFlag ( pModel, 2 );

		// Set the entity model
		CEntity__SetModel ( pPed, pModel );

		// Set the entity flags
		pPed->m_dwFlags &= 0xFFFFFFBF;
		pPed->m_dwFlags |= 0x10000;

		// Set slot SDS number
		pPed->m_iUnkNum = pModelMgr->GetSlot()->GetNum ();

		// Setup player
		CEntity__UnkFunc ( pPed );

		return pPed;
	}

	bool LoadPointers( void )
	{
		pGame = M2Game::GetInstance ();
		pWindow = M2Window::GetInstance ();
		pScriptEngine = M2ScriptEngine::GetInstance ();
		pWrapperList = M2WrapperList::GetInstance ();
		pVehiclePool = M2VehiclePool::GetInstance ();

		return (pGame != NULL && pWindow != NULL && pScriptEngine != NULL && pWrapperList != NULL && pVehiclePool != NULL);
	}

	HWND GetWindowHandle( void )
	{
		if( pWindow )
			return pWindow->m_hWnd;

		return NULL;
	}

	bool HasWindowFocus( void )
	{
		if( pWindow )
			return pWindow->m_bFocus;

		return false;
	}

	lua_State * GetState( void )
	{
		if ( !pScriptEngine ) return NULL;
		if ( !pScriptEngine->m_pScriptHandler ) return NULL;
		if ( !pScriptEngine->m_pScriptHandler->m_pScriptMachinePool ) return NULL;
		if ( !pScriptEngine->m_pScriptHandler->m_pScriptMachinePool->m_pScriptMachine ) return NULL;
		if ( !pScriptEngine->m_pScriptHandler->m_pScriptMachinePool->m_pScriptMachine[0] ) return NULL;

		return pScriptEngine->m_pScriptHandler->m_pScriptMachinePool->m_pScriptMachine[0]->m_L;
	}

	M2BaseEntity * GetEntityByName( const char * szEntityName )
	{
		if( !pWrapperList )
			return NULL;

		M2BaseEntity * pBaseEntity = NULL;
		_asm
		{
			push		szEntityName
			mov			ecx, pWrapperList
			call		COffsets::FUNC_CWrapperList__GetEntityByName
			mov			pBaseEntity, eax
		}

		return pBaseEntity;
	}

	M2Entity * CreateItem( CVector3 vecPosition )
	{
		M2Entity * pEntity;

		// Create entity
		DWORD dwFunc = 0x992E50;
		_asm push 4;
		_asm call dwFunc;
		_asm mov pEntity, eax;

		// Set Position
		dwFunc = pEntity->m_pVFTable->SetPosition;
		_asm push vecPosition;
		_asm mov ecx, pEntity;
		_asm call dwFunc;

		// Activate
		dwFunc = 0x1189E20;
		_asm mov ecx, pEntity;
		_asm call dwFunc;

		return pEntity;
	}

	// FIXME: random cloth color, etc. (apply params from table?)
	M2Ped * CreatePed( M2ModelMgr * pModelMgr, CVector3 vecPosition )
	{
		// Create the ped human object
		M2Ped * pPed = ( M2Ped *)( CreateObjectByType( OBJTYPE_Human ) );

		// Did the ped fail to create?
		if( !pPed )
			return NULL;

		// Allocate the model memory
		M2Model * pModel = CM2Core::GetInstance()->AllocateModel();

		// Did the model fail to allocate?
		if( !pModel )
		{
			// Delete the ped memory
			SAFE_DELETE( pPed );
			return NULL;
		}

		// Assign the model
		CModel__AssignModel( pModel, pModelMgr->GetModel() );

		// Set the model name hash
		CModel__SetNameHashByName( pModel, "m2mp_player" );

		// Set the initial position
		*( float *)( ( DWORD )( pModel ) + 0x44 ) = vecPosition.fX;
		*( float *)( ( DWORD )( pModel ) + 0x54 ) = vecPosition.fY;
		*( float *)( ( DWORD )( pModel ) + 0x64 ) = vecPosition.fZ;

		// Set model flags
		CModel__SetFlag( pModel, 2 );

		// Set the entity model
		CEntity__SetModel( pPed, pModel );

		// Unknown
		CEntity__UnkFunc( pPed );

		// Set the entity flags
		pPed->m_dwFlags &= 0xFFFFF7FF | 0x80000;

#ifdef DEBUG
		CLogFile::Printf ( "CreatePed - Model slot number: %d", pPed->m_iUnkNum );
#endif

		// Has the entity created successfully?
		if ( pPed->m_dwFlags & 0x20 )
			return pPed;

		// Delete memory
		SAFE_DELETE( pModel );
		SAFE_DELETE( pPed );

		return NULL;
	}

	M2Vehicle * CreateVehicle( M2ModelMgr * pModelMgr, CVector3 vecPosition )
	{
		if ( !pModelMgr || !pModelMgr->GetModel( ) )
			return NULL;

		// Create the vehicle object
		M2Vehicle * pVehicle = ( M2Vehicle *)( CreateObjectByType( OBJTYPE_Car ) );

		// Did the ped fail to create?
		if( !pVehicle )
			return NULL;

		// Allocate the model memory
		M2Model * pModel = CM2Core::GetInstance()->AllocateModel();

		// Did the model fail to allocate?
		if( !pModel )
		{
			// Delete the vehicle memory
			SAFE_DELETE( pVehicle );
			return NULL;
		}

		// Assign new model
		CModel__AssignModel( pModel, pModelMgr->GetModel() );

		// Set name hash
		CModel__SetNameHashByName( pModel, "m2mp_vehicle" );

		// Set the initial position
		*( float *)( ( DWORD )( pModel ) + 0x44 ) = vecPosition.fX;
		*( float *)( ( DWORD )( pModel ) + 0x54 ) = vecPosition.fY;
		*( float *)( ( DWORD )( pModel ) + 0x64 ) = vecPosition.fZ;

 		// Set flag
 		CModel__SetFlag( pModel, 2 );

		// Set model
		CEntity__SetModel( pVehicle, pModel );

		// set car params
		if ( !CObject__SetCarParams( pVehicle, NULL ) )
		{
			// Delete the memory
			SAFE_DELETE( pModel );
			SAFE_DELETE( pVehicle );
			return NULL;
		}

		// Set SlotSDS
		pVehicle->m_nSlotSDS = pModelMgr->GetSlot()->GetNum();

		// Setup
		CCar__Setup( pVehicle );

		// Set entity flags
		DWORD dwFlags = pVehicle->m_dwFlags & 0xFFFFFFBF | 0x4800;
		pVehicle->m_dwFlags = dwFlags;

		// Has the entity created successfully?
		if ( pVehicle->m_dwFlags & 0x20 )
			return pVehicle;

		// Delete memory
		SAFE_DELETE( pModel );
		SAFE_DELETE( pVehicle );

		return NULL;
	}

	void * Malloc( size_t uiSize )
	{
		__asm
		{
			push uiSize
			call COffsets::FUNC_M2malloc
			add esp, 4
		}
	}
};