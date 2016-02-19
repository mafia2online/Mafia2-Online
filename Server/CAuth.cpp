/*************************************************************
*
* Solution   : Mafia 2 Online
* Project    : Shared Library
* File       : CAuth.cpp
* Developers : Tyldar <tyladr@mafia2-online.com>
*
***************************************************************/

#include	"../Client/BaseInc.h"

#include	"CCore.h"

#include	"CAuth.h"

CAuth::CAuth(String serverKey)
{
	this->serverKey = serverKey;
}

CAuth::~CAuth()
{

}