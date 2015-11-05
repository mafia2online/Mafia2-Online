/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CCoreInterface.h
*
***************************************************************/

#pragma once

class CCoreInterface
{

public:

	virtual	void			SetActive					( bool bActive ) = 0;
	virtual	bool			IsActive					( void ) = 0;

	virtual	void			SetGameModeText				( const char * szGameModeText ) = 0;
	virtual	const char		* GetGameModeText			( void ) = 0;

	virtual	void			SetMapName					( const char * szMapName ) = 0;
	virtual	const char		* GetMapName				( void ) = 0;

	virtual	bool			IsPasswordProtected			( void ) = 0;

};