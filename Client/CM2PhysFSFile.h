/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2PhysFSFile.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

class M2PhysFSFile__vftable
{
public:
	DWORD Read;										// 0000 - 0004
	DWORD Write;									// 0004 - 0008
	PAD( M2PhysFSFile__vftable, pad0, 0x4 );		// 0008 - 000C
	DWORD Tell;										// 000C - 0010
	DWORD EndOfFile;								// 0010 - 0014
	DWORD Flush;									// 0014 - 0018
	DWORD FileLength;								// 0018 - 001C
	DWORD CloseHandle;								// 001C - 0020
	PAD( M2PhysFSFile__vftable, pad1, 0x8 );		// 0020 - 0028
	DWORD SetBuffer;								// 0028 - 002C
};

class M2PhysFSFile
{
public:
	M2PhysFSFile__vftable * m_pVFTable;				// 0000 - 0004
	void * m_pFileHandle;							// 0004 - 0008
};

class CM2PhysFSFile
{

private:

	M2PhysFSFile									* m_pFile;

public:

						CM2PhysFSFile				( M2PhysFSFile * pFile );
						~CM2PhysFSFile				( void );

	signed int			read						( void* pBuffer, int iObjSize, int iObjCount );
	int					write						( const char * szBuffer );

	void				flush						( void );

	int					fileLength					( void );
	void				closeHandle					( void );

	M2PhysFSFile		* GetPhysFSFile				( void ) { return m_pFile; }

};