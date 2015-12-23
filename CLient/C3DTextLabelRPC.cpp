/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CBlipRPC.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"
#include	"C3DTextLabelRPC.h"

extern	CCore			* pCore;

void NewTextLabel(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	CLogFile::Printf("Received order to create new textLabel");
}

void RemoveTextLabel(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	CLogFile::Printf("Received order to delete textLabel");
}

void C3DTextLabelRPC::Register(RakNet::RPC4 * pRPC)
{
	// TextLabel functions
	pRPC->RegisterFunction(RPC_NEWTEXTLABEL, NewTextLabel);
	pRPC->RegisterFunction(RPC_REMOVETEXTLABEL, RemoveTextLabel);
}

void C3DTextLabelRPC::Unregister(RakNet::RPC4 * pRPC)
{
	// TextLabel functions
	pRPC->UnregisterFunction(RPC_ATTACHBLIPTOPLAYER);
	pRPC->UnregisterFunction(RPC_REMOVETEXTLABEL);
}