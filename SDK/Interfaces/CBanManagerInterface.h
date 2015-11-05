/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Software Development Kit
* File       : CBanManagerInterface.h
*
***************************************************************/

#pragma once

class CBanManagerInterface
{
public:
	virtual bool							Add( const char * szSerial, const char * szBanner, unsigned long ulBanTime, unsigned long ulUnbanTime, const char * szReason, bool bNewNode ) = 0;
	virtual void							Remove( const char * szSerial ) = 0;
	virtual bool							IsSerialBanned( const char * szSerial ) = 0;
};