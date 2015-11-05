/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2PhysFS.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

CM2PhysFS::CM2PhysFS( M2PhysFS * pFileSystem )
{
	// Set the physfs
	SetPhysFS( pFileSystem );

	// Reset the open file pointer
	m_pOpenFile = NULL;
}

CM2PhysFS::~CM2PhysFS( void )
{
	// Do we have an open file?
	if( m_pOpenFile )
		close( m_pOpenFile );
}

void CM2PhysFS::setWriteDir( const char * szDir )
{
	// Get the vtable function
	M2PhysFS * pFileSystem = m_pFileSystem;
	DWORD dwFunc = pFileSystem->m_pVFTable->SetWriteDir;

	// Set the write dir
	_asm push szDir;
	_asm mov ecx, pFileSystem;
	_asm call dwFunc;
}

const char * CM2PhysFS::getWriteDir( void )
{
	const char * szReturn = "";

	// Get the vtable function
	M2PhysFS * pFileSystem = m_pFileSystem;
	DWORD dwFunc = pFileSystem->m_pVFTable->GetWriteDir;

	// Get the write dir
	_asm mov ecx, pFileSystem;
	_asm call dwFunc;
	_asm mov szReturn, eax;

	return szReturn;
}

CM2PhysFSFile * CM2PhysFS::addToSearchPath( const char * szFileName, int iUnknown )
{
	// Do we already have an open file?
	if( m_pOpenFile )
		return NULL;

	// Get the vtable function
	M2PhysFS * pFileSystem = m_pFileSystem;
	DWORD dwFunc = pFileSystem->m_pVFTable->AddToSearchPath;

	// Create a physfs file
	M2PhysFSFile * pFile = NULL;

	CLogFile::Printf( "Adding file to search path '%s'... (AddToSearchPath: 0x%p)", szFileName, dwFunc );

	// Open the file
	_asm push iUnknown;
	_asm push szFileName;
	_asm mov ecx, pFileSystem;
	_asm call dwFunc;
	_asm mov pFile, eax;

	// Create the open file instance
	m_pOpenFile = new CM2PhysFSFile( pFile );

	CLogFile::Printf( "File opened! (0x%p)", m_pOpenFile->GetPhysFSFile() );

	return m_pOpenFile;
}

void CM2PhysFS::close( CM2PhysFSFile * pFile )
{
	// Is the file instance invalid?
	if( !pFile )
		return;

	// Get the vtable function
	M2PhysFS * pFileSystem = m_pFileSystem;
	M2PhysFSFile * pFSFile = pFile->GetPhysFSFile();
	DWORD dwFunc = pFileSystem->m_pVFTable->Close;

	CLogFile::Printf( "Closing file 0x%p. (Func: 0x%p)", pFSFile, dwFunc );

	// Close the file
	_asm mov ecx, pFSFile;
	_asm call dwFunc;

	CLogFile::Print( "Closed!" );

	// Destory the open file instance
	SAFE_DELETE( m_pOpenFile );

	CLogFile::Print( "Cleaned" );
}

const char * CM2PhysFS::loadReadAndClose( const char * szDir, const char * szFile )
{
	// Set the write directory
	setWriteDir( szDir );

	// Load the file
	m_pOpenFile = addToSearchPath( szFile, 1 );

	// Read the file
	//const char * szBuffer = "";
	//m_pOpenFile->read( &szBuffer );

	// Close the file
	//m_pOpenFile->closeHandle();

	// Reset the open file handle
	//SAFE_DELETE( m_pOpenFile );

	return "";
}

void CM2PhysFS::loadWriteAndClose( const char * szDir, const char * szFile, const char * pBuffer )
{
	// Set the write directory
	setWriteDir( szDir );

	// Load the file
	m_pOpenFile = addToSearchPath( szFile, 1 );

	// Read the file
	m_pOpenFile->write( pBuffer );

	// Close the file
	//m_pOpenFile->closeHandle();

	// Reset the open file handle
	//SAFE_DELETE( m_pOpenFile );
}