/*************************************************************
*
* Solution   : Mafia 2 Online
* Project    : Client
* File       : C3DTextLabelRPC.h
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#pragma once

class C3DTextLabelRPC
{

private:

	static	bool							m_bRegistered;

public:

	static	void		Register(RakNet::RPC4 * pRPC);
	static	void		Unregister(RakNet::RPC4 * pRPC);

};