/*************************************************************
*
* Solution   : Mafia 2 Online
* Project    : Shared Library
* File       : CAuth.h
* Developers : Tyldar <tyladr@mafia2-online.com>
*
***************************************************************/

#pragma once

#include	"CString.h"

class CAuth
{
private:
	String	serverKey;
public:
	CAuth(String serverKey);
	~CAuth();
};