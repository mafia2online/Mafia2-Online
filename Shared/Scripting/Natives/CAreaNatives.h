/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CAreaNatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include	"../CScriptingManager.h"

class CAreaNatives
{

private:

	static	SQInteger		DistanceBetweenPoints3D( SQVM * pVM );
	static	SQInteger		DistanceBetweenPoints2D( SQVM * pVM );
	static	SQInteger		PointInArea3D( SQVM * pVM );
	static	SQInteger		PointInArea2D( SQVM * pVM );
	static	SQInteger		PointInCircle2D( SQVM * pVM );
	static	SQInteger		PointInCircle3D( SQVM * pVM );
	static	SQInteger		PointInTube( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};