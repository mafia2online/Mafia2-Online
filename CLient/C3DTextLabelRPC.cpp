/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabelRPC.cpp
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#include	"StdInc.h"
#include	"C3DTextLabelRPC.h"

extern	CCore			* pCore;

void NewTextLabel(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	pCore->GetChat->AddDebugMessage("Received order to create new textLabel");
}

void RemoveTextLabel(RakNet::BitStream * pBitStream, RakNet::Packet * pPacket)
{
	pCore->GetChat()->AddDebugMessage("Received order to delete textLabel");
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
	pRPC->UnregisterFunction(RPC_NEWTEXTLABEL);
	pRPC->UnregisterFunction(RPC_REMOVETEXTLABEL);
}