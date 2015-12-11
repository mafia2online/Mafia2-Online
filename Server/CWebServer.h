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
	struct					mg_mgr							mgr;
	struct					mg_connection					*nc;
	cs_stat_t												st;

public:

							CWebServer						( void );
							~CWebServer						( void );

	void					Pulse							( void );

	bool					FileCopy						( String strDirectroy, String strFileName, CFileChecksum * pChecksum, bool bIsScript = true );
	static void				ev_handler						(struct mg_connection *nc, int ev, void *ev_data);

};
