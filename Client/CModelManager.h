/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game
* File       : CModelManager.h
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#pragma once

#define MODELMGR_MAX			1000
#define SDS_LOAD_DIR_ADDR		0x18F6FDC
#define SDS_LOAD_DIR_CARS		"/sds/cars/"
#define SDS_LOAD_DIR_HCHAR		"/sds/hchar/"
#define SDS_LOAD_DIR_PLAYER		"/sds/player/"
#define SDS_LOAD_DIR_TRAFFIC	"/sds/traffic/"

class CModelManager;
class M2ModelMgr
{
public:
	CM2Slot * GetSlot( void )
	{
		return *( CM2Slot **)( ( DWORD )( this ) + 0x4 );
	}

	M2Entity * GetEntity( void )
	{
		return *( M2Entity **)( ( DWORD )( this ) + 0x10 );
	}

	M2Model * GetModel( void )
	{
		return *( M2Model **)( ( DWORD )( this ) + 0x14 );
	}

	const char * GetModelName( void )
	{
		return (const char *)( ( DWORD )( this ) + 0x2C );
	}

	bool Load( const char * pszFilePath )
	{
		bool bResult = false;

		__asm
		{
			push pszFilePath
			mov ecx, this
			call COffsets::FUNC_CModelMgr__Load
			mov bResult, al
		}

		return bResult;
	}

	void Free( void )
	{
		__asm
		{
			mov ecx, this
			call COffsets::FUNC_CModelMgr__Free
		}
	}

	void Construct( void )
	{
		__asm
		{
			mov ecx, this
			call COffsets::FUNC_CModelMgr__Construct
		}
	}

	void ChangeModel( const char * pszDir, const char * pszModel, int iHumanColour = -1 );
};

class CModelManager
{
	friend class M2ModelMgr;

private:
	std::vector< M2ModelMgr *> m_ModelMgrs;

protected:
	static char		* GetDir			( void );
	static void		SetDir				( const char * pszDirectory );

public:
	int			Size					( void ) { return m_ModelMgrs.size(); }
	void		Clear					( void ) { m_ModelMgrs.clear(); }

	M2ModelMgr	* LoadModel				( const char * pszDir, const char * pszFileName );
	bool		FreeModel				( const char * pszModelName );

	M2Model		* GetModelByName		( const char * pszModelName );
	M2Model		* GetModelByIndex		( int nIndex );
	M2ModelMgr	* GetModelMgrByName		( const char * pszModelName );
	M2ModelMgr	* GetModelMgrByIndex	( int nIndex );
};