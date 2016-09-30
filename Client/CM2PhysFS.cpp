/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2PhysFS.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CM2PhysFS.h"

#include "CLogFile.h"

CM2PhysFS::CM2PhysFS( M2PhysFS * pFileSystem )
{
	SetPhysFS( pFileSystem );

	m_pOpenFile = NULL;
}

CM2PhysFS::~CM2PhysFS( void )
{
	if( m_pOpenFile )
		close( m_pOpenFile );
}

void CM2PhysFS::setWriteDir( const char * szDir )
{
	M2PhysFS * pFileSystem = m_pFileSystem;
	DWORD dwFunc = pFileSystem->m_pVFTable->SetWriteDir;

	_asm
	{
		push szDir;
		mov ecx, pFileSystem;
		call dwFunc;
	}
}

const char * CM2PhysFS::getWriteDir( void )
{
	const char * szReturn = "";

	M2PhysFS * pFileSystem = m_pFileSystem;
	DWORD dwFunc = pFileSystem->m_pVFTable->GetWriteDir;

	_asm
	{
		mov ecx, pFileSystem;
		call dwFunc;
		mov szReturn, eax;
	}

	return szReturn;
}

CM2PhysFSFile * CM2PhysFS::addToSearchPath( const char * szFileName, int iUnknown )
{
	if( m_pOpenFile )
		return NULL;

	M2PhysFS * pFileSystem = m_pFileSystem;
	DWORD dwFunc = pFileSystem->m_pVFTable->AddToSearchPath;

	M2PhysFSFile * pFile = NULL;

	CLogFile::Printf( "Adding file to search path '%s'...", szFileName );
	DEBUG_LOG("AddToSearchPath: 0x%p", dwFunc);

	// Open the file
	_asm
	{
		push iUnknown;
		push szFileName;
		mov ecx, pFileSystem;
		call dwFunc;
		mov pFile, eax;
	}

	m_pOpenFile = new CM2PhysFSFile( pFile );

	DEBUG_LOG( "File opened! (0x%p)", m_pOpenFile->GetPhysFSFile() );

	return m_pOpenFile;
}

void CM2PhysFS::close( CM2PhysFSFile * pFile )
{
	if( !pFile )
		return;

	M2PhysFS * pFileSystem = m_pFileSystem;
	M2PhysFSFile * pFSFile = pFile->GetPhysFSFile();
	DWORD dwFunc = pFileSystem->m_pVFTable->Close;

	DEBUG_LOG( "Closing file 0x%p. (Func: 0x%p)", pFSFile, dwFunc );

	_asm
	{
		mov ecx, pFSFile;
		call dwFunc;
	}

	DEBUG_LOG( "Closed!" );

	SAFE_DELETE( m_pOpenFile );

	DEBUG_LOG( "Cleaned" );
}

const char * CM2PhysFS::loadReadAndClose( const char * szDir, const char * szFile )
{
	setWriteDir( szDir );

	m_pOpenFile = addToSearchPath( szFile, 1 );

	//const char * szBuffer = "";
	//m_pOpenFile->read( &szBuffer );

	//m_pOpenFile->closeHandle();

	//SAFE_DELETE( m_pOpenFile );

	return "";
}

void CM2PhysFS::loadWriteAndClose( const char * szDir, const char * szFile, const char * pBuffer )
{
	setWriteDir( szDir );

	m_pOpenFile = addToSearchPath( szFile, 1 );

	m_pOpenFile->write( pBuffer );

	//m_pOpenFile->closeHandle();

	//SAFE_DELETE( m_pOpenFile );
}