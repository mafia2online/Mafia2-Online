/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CWebServer.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../Libraries/mongoose/mongoose.h"
#include	"../Shared/CRC.h"

class CWebServer
{

private:

	mg_context				* m_pMongooseContext;
	static	void			* MongooseEventHandler			( mg_event event, mg_connection * connection, const mg_request_info * request_info );

public:

							CWebServer						( void );
							~CWebServer						( void );

	bool					FileCopy						( String strDirectroy, String strFileName, CFileChecksum * pChecksum, bool bIsScript = true );

};
