/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2PhysFS.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CM2PhysFSFile.h"

#define	VAR__PHYSFS_baseDir							0x1BA6AFC
#define	VAR__PHYSFS_allocator						0x1BBDE80
#define	PHYSFS_init									0x829B20
#define	PHYSFS_deinit								0x829C70
#define	PHYSFS_openWrite							0x828E90
#define	PHYSFS_openAppend							0x828EA0
#define	PHYSFS_openRead								0x828EB0
#define	PHYSFS_fileLength							0x829270
#define	PHYSFS_isDirectory							0x828A80
#define PHYSFS_exists								0x828940
#define PHYSFS_delete								0x828480
#define	PHYSFS_mkdir								0x828320
#define	PHYSFS_mount								0x827CC0
#define PHYSFS_setWriteDir							0x827BF0
#define	PHYSFS_getWriteDir							0x827B60
#define	PHYSFS_getBaseDir							0x827B40
#define	PHYSFS_getDirSeparator						0x827B20
#define	PHYSFS_getLastError							0x827380
#define	PHYSFS_write								0x82A040
#define	PHYSFS_read									0x829190
#define	PHYSFS_EOF									0x829200
#define	PHYSFS_tell									0x829230
#define	PHYSFS_flush								0x829290
#define	PHYSFS_setAllocator							0x8292F0
#define	PHYSFS_getAllocator							0x829350
#define	PHYSFS_getFolderSize						0x8294C0
#define	PHYSFS_mount2								0x8295E0
#define PHYSFS_addToSearchPath						0x827DE0
#define	PHYSFS_getSearchPath						0x829D90
#define	PHYSFS_enumerateFiles						0x829DA0
#define	PHYSFS_close								0x829EF0
#define	PHYSFS_seek									0x82A0A0
#define	PHYSFS_setBuffer							0x82A140
#define	PHYSFS_setSaneConfig						0x82A240
#define	PHYSFS_enumerateFilesWildcard				0x82A6B0
#define	PHYSFS_freeList								0x827AE0

class M2PhysFS__vftable
{
public:
	PAD(M2PhysFS__vftable, pad0, 0x10);				// 0000 - 0010
	DWORD SetWriteDir;								// 0010 - 0014
	PAD(M2PhysFS__vftable, pad1, 0x4);				// 0014 - 0018
	DWORD GetWriteDir;								// 0018 - 001C
	PAD(M2PhysFS__vftable, pad2, 0x78);				// 001C - 0094
	DWORD AddToSearchPath;							// 0094 - 0098
	PAD(M2PhysFS__vftable, pad3, 0x1C);				// 0098 - 00B4
	DWORD Close;									// 00B4 - 00B8
};

class M2PhysFS // size: 0xE0
{
public:
	M2PhysFS__vftable * m_pVFTable;					// 0000 - 0004
	// 0x4 = M2PhysFSFile * m_pFile;
	// 0x10 = const char * m_szFilename;
};

class CM2PhysFS
{

private:

	M2PhysFS										* m_pFileSystem;
	CM2PhysFSFile									* m_pOpenFile;

public:

						CM2PhysFS					( M2PhysFS * pFileSystem );
						~CM2PhysFS					( void );

	void				SetPhysFS					( M2PhysFS * pFileSystem ) { m_pFileSystem = pFileSystem; }
	M2PhysFS			* GetPhysFS					( void ) { return m_pFileSystem; }

	void				setWriteDir					( const char * szDir );
	const char			* getWriteDir				( void );

	CM2PhysFSFile		* addToSearchPath			( const char * szFilename, int iUnknown );
	void				close						( CM2PhysFSFile * pFile );

	CM2PhysFSFile		* GetOpenFile				( void ) { return m_pOpenFile; }

	const char			* loadReadAndClose			( const char * szDir, const char * szFile );
	void				loadWriteAndClose			( const char * szDir, const char * szFile, const char * pBuffer );

};