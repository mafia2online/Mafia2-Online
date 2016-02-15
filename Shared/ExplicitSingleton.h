/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : ExplicitSingleton.h
*
***************************************************************/

#pragma once

#include <assert.h>

template <typename CLASS_TYPE>
class ExplicitSingleton
{
private:
	static CLASS_TYPE*		ms_pInstance;

public:

	ExplicitSingleton ( void )
	{
		ms_pInstance = static_cast<CLASS_TYPE*>(this);
	}

	virtual ~ExplicitSingleton ( void )
	{
		ms_pInstance = 0;
	}

	static CLASS_TYPE *Instance ( void )
	{
		assert(ms_pInstance);
		return ms_pInstance;
	}
};

template <typename CLASS_TYPE>
CLASS_TYPE* ExplicitSingleton<CLASS_TYPE>::ms_pInstance = 0;
