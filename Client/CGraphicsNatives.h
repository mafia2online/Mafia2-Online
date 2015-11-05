/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGraphicsNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGraphicsNatives
{

private:

	static	SQInteger		GetFPS( SQVM * pVM );
	static	SQInteger		GetScreenSize( SQVM * pVM );
	static	SQInteger		WorldToScreen( SQVM * pVM );
	static	SQInteger		ScreenToWorld( SQVM * pVM );
	static	SQInteger		ShowChat( SQVM * pVM );
	static	SQInteger		IsChatVisible( SQVM * pVM );
	static	SQInteger		IsInputVisible( SQVM * pVM );
	static	SQInteger		SendMessage( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};

