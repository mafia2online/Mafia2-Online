/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2PhysFSFile.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CM2PhysFSFile.h"

#include "CLogFile.h"

CM2PhysFSFile::CM2PhysFSFile( M2PhysFSFile * pFile )
{
	m_pFile = pFile;
}

CM2PhysFSFile::~CM2PhysFSFile( void )
{
}

signed int CM2PhysFSFile::read( void* pBuffer, int iObjSize, int iObjCount )
{
	signed int iObjReadCount = 0;

	M2PhysFSFile * pFile = m_pFile;
	DWORD dwFunc = pFile->m_pVFTable->Read;

	_asm
	{
		push iObjCount;
		push iObjSize;
		push pBuffer;
		mov ecx, pFile;
		mov iObjReadCount, eax;
	}

	return iObjReadCount;
}

int CM2PhysFSFile::write( const char * szBuffer )
{
	int iObjWrittenCount = 0;
	int iObjSize = strlen( szBuffer );

	M2PhysFSFile * pFile = m_pFile;
	DWORD dwFunc = pFile->m_pVFTable->Write;

	DEBUG_LOG( "FileHandle: 0x%p, Func: 0x%p", pFile, dwFunc );

	_asm
	{
		push iObjSize;
		push 1;
		push szBuffer;
		mov ecx, pFile;
		mov iObjWrittenCount, eax;
	}

	DEBUG_LOG( "Written %d bytes!", iObjWrittenCount );

	return iObjWrittenCount;
}

void CM2PhysFSFile::flush( void )
{
	M2PhysFSFile * pFile = m_pFile;
	DWORD dwFunc = pFile->m_pVFTable->Flush;

	_asm
	{
		mov ecx, pFile;
		call dwFunc;
	}
}

int CM2PhysFSFile::fileLength( void )
{
	int iLen = 0;

	M2PhysFSFile * pFile = m_pFile;
	DWORD dwFunc = pFile->m_pVFTable->FileLength;

	_asm
	{
		mov ecx, pFile;
		call dwFunc;
		mov iLen, eax;
	}

	return iLen;
}

void CM2PhysFSFile::closeHandle( void )
{
	M2PhysFSFile * pFile = m_pFile;
	DWORD dwFunc = pFile->m_pVFTable->CloseHandle;

	_asm
	{
		mov ecx, pFile;
		call dwFunc;
	}
}