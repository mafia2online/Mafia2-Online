/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2PhysFSFile.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

CM2PhysFSFile::CM2PhysFSFile( M2PhysFSFile * pFile )
{
	// Set the file instance
	m_pFile = pFile;
}

CM2PhysFSFile::~CM2PhysFSFile( void )
{
}

signed int CM2PhysFSFile::read( void* pBuffer, int iObjSize, int iObjCount )
{
	//
	signed int iObjReadCount = 0;

	// Get the vtable function
	M2PhysFSFile * pFile = m_pFile;
	DWORD dwFunc = pFile->m_pVFTable->Read;

	// Call the function
	_asm push iObjCount;
	_asm push iObjSize;
	_asm push pBuffer;
	_asm mov ecx, pFile;
	_asm mov iObjReadCount, eax;

	return iObjReadCount;
}

int CM2PhysFSFile::write( const char * szBuffer )
{
	//
	int iObjWrittenCount = 0;
	int iObjSize = strlen( szBuffer );

	// Get the vtable function
	M2PhysFSFile * pFile = m_pFile;
	DWORD dwFunc = pFile->m_pVFTable->Write;

	CLogFile::Printf( "FileHandle: 0x%p, Func: 0x%p", pFile, dwFunc );

	// Call the function
	_asm push iObjSize;
	_asm push 1;
	_asm push szBuffer;
	_asm mov ecx, pFile;
	_asm mov iObjWrittenCount, eax;

	CLogFile::Printf( "Written %d bytes!", iObjWrittenCount );

	return iObjWrittenCount;
}

void CM2PhysFSFile::flush( void )
{
	// Get the vtable function
	M2PhysFSFile * pFile = m_pFile;
	DWORD dwFunc = pFile->m_pVFTable->Flush;

	// Call the function
	_asm mov ecx, pFile;
	_asm call dwFunc;
}

int CM2PhysFSFile::fileLength( void )
{
	//
	int iLen = 0;

	// Get the vtable function
	M2PhysFSFile * pFile = m_pFile;
	DWORD dwFunc = pFile->m_pVFTable->FileLength;

	// Call the function
	_asm mov ecx, pFile;
	_asm call dwFunc;
	_asm mov iLen, eax;

	return iLen;
}

void CM2PhysFSFile::closeHandle( void )
{
	// Get the vtable function
	M2PhysFSFile * pFile = m_pFile;
	DWORD dwFunc = pFile->m_pVFTable->CloseHandle;

	// Call the function
	_asm mov ecx, pFile;
	_asm call dwFunc;
}