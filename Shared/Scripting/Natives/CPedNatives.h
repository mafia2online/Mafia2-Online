/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CPedNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once
#ifdef _CLIENT

class CScriptingManager;

class CPedNatives
{

private:

	static	SQInteger		CreatePed					( SQVM * pVM );
	static	SQInteger		SetPedName					( SQVM * pVM);
	static	SQInteger		GetPedName					( SQVM * pVM);
	static	SQInteger		DestroyPed					( SQVM * pVM );
	static	SQInteger		ShowPedName					( SQVM * pVM );
	static	SQInteger		SetPedPosition				( SQVM * pVM );
	static	SQInteger		GetPedPosition				( SQVM * pVM );
	static	SQInteger		SetPedModel					( SQVM * pVM );
	static	SQInteger		GetPedModel					( SQVM * pVM);

public:

	static	void			Register					( CScriptingManager * pScriptingManager );

};
#endif
