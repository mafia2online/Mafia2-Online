#pragma once

class CM2ObjectTable
{
public:
	int			GetSize					( void );
	int			GetRegisteredSize		( void );

	CM2Object * GetObject				( int nIndex );
	CM2Object * GetRegisteredObject		( int nIndex );

	static CM2ObjectTable * GetInstance( void );
};