/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CNetworkModule.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"BaseInc.h"

#include	"CCore.h"

#include	<RPC4Plugin.h>
#include	"RakPeerInterface.h"
#include	"CNetworkRPC.h"
#include	"CScriptingRPC.h"

#include	"SharedUtility.h"

#include	"CChat.h"

#include	"CPlayerManager.h"
#include	"CVehicleManager.h"

#include	"CGUI.h"
#include	"CServerBrowser.h"

#include	"../Shared/CNetworkRPC.h"

#include	"../Libraries/RakNet/Source/MessageIdentifiers.h"

#include	"CRemotePlayer.h"
#include	"CLocalPlayer.h"

#include	"CNetworkModule.h"

RakNet::RPC4			* CNetworkModule::m_pRPC = NULL;

CNetworkModule::CNetworkModule(void)
{
	m_pRakPeer = RakNet::RakPeerInterface::GetInstance();

	m_pRPC = RakNet::RPC4::GetInstance();
	m_pRakPeer->AttachPlugin(m_pRPC);

	CNetworkRPC::Register(m_pRPC);
	CScriptingRPC::Register(m_pRPC);

	SetNetworkState(NETSTATE_NONE);

	m_uiLastConnectionTry = (unsigned int)SharedUtility::GetTime();
}

CNetworkModule::~CNetworkModule(void)
{
	if (IsConnected())
	{
		Disconnect();
	}

	CNetworkRPC::Unregister(m_pRPC);
	CScriptingRPC::Unregister(m_pRPC);
	m_pRakPeer->DetachPlugin(m_pRPC);
	RakNet::RPC4::DestroyInstance(m_pRPC);
	RakNet::RakPeerInterface::DestroyInstance(m_pRakPeer);
}

bool CNetworkModule::Startup(void)
{
	return (m_pRakPeer->Startup(1, &RakNet::SocketDescriptor(), 1, THREAD_PRIORITY_NORMAL) == RakNet::RAKNET_STARTED);
}

void CNetworkModule::Shutdown(void)
{
	if (IsConnected())
		Disconnect(false);

	m_pRakPeer->Shutdown(500, 0, HIGH_PRIORITY);
}

eNetworkResponse CNetworkModule::Connect(String strHost, unsigned short usPort, String strPass)
{
	if (IsConnected())
	{
		Disconnect();
	}

	SetLastConnection(strHost, usPort, strPass);

	int iConnectionResult = m_pRakPeer->Connect(strHost.Get(), usPort, strPass.Get(), strPass.GetLength());
	SetNetworkState(NETSTATE_CONNECTING);
	if (iConnectionResult != RakNet::INVALID_PARAMETER)
	{
		SetNetworkState(NETSTATE_NONE);
		m_uiLastConnectionTry = (unsigned int)SharedUtility::GetTime();
	}

	return (eNetworkResponse)iConnectionResult;
}

void CNetworkModule::Disconnect(bool bRestart)
{
	if (!IsConnected())
		return;

	CCore::Instance()->GetGUI()->DeleteAllClientScriptGUI();
	m_pRakPeer->CloseConnection(RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	SetNetworkState(NETSTATE_DISCONNECTED);

	Shutdown();
	if (bRestart)
	{
		Startup();

		CCore::Instance()->SetServerName("M2Online Server");
		CCore::Instance()->SetServerMaxPlayers(0);

		CCore::Instance()->GetChat()->Clear();
		CCore::Instance()->GetChat()->ClearHistory();
		CCore::Instance()->GetChat()->ClearSelectText();
		CCore::Instance()->GetChat()->ClearInputText();

		//pCore->GetPlayerManager()->GetLocalPlayer()->SetModel ( 10 );
	}
}

void CNetworkModule::Pulse(void)
{
	if (!CCore::Instance()->IsGameLoaded())
		return;

	if (GetNetworkState() == NETSTATE_DISCONNECTED)
		return;

	UpdateNetwork();
	if (IsConnected())
	{
		CCore::Instance()->GetPlayerManager()->Pulse();
		CCore::Instance()->GetVehicleManager()->Pulse();
	}
}

void CNetworkModule::Call(const char * szIdentifier, RakNet::BitStream * pBitStream, PacketPriority priority, PacketReliability reliability, bool bBroadCast)
{
	if (!IsConnected())
		return;

	m_pRPC->Call(szIdentifier, pBitStream, priority, reliability, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, bBroadCast);
}

void Packet_PlayerSync(RakNet::Packet * pPacket)
{
	RakNet::BitStream bsSyncData(pPacket->data, pPacket->length, false);
	bsSyncData.IgnoreBytes(sizeof(RakNet::MessageID));

	EntityId playerId;
	bsSyncData.ReadCompressed(playerId);

	unsigned short usPing;
	bsSyncData.ReadCompressed(usPing);

	OnFootSync onFootSync;
	bsSyncData.Read((PCHAR)&onFootSync, sizeof(OnFootSync));


	CRemotePlayer * pRemotePlayer = CCore::Instance()->GetPlayerManager()->Get(playerId);
	if (pRemotePlayer)
	{
		pRemotePlayer->SetPing(usPing);
		if (CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->IsSpawned())
		{
			if (playerId == CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetId())
				return;
			pRemotePlayer->StoreOnFootSync(&onFootSync);
		}
	}
}

void CNetworkModule::UpdateNetwork(void)
{
	RakNet::Packet * pPacket = NULL;

	bool bDisconnect = false;

	while (pPacket = m_pRakPeer->Receive())
	{
		if (!pPacket)
			continue;
		if (pPacket->data[0] == ID_PLAYERSYNC)
		{
			Packet_PlayerSync(pPacket);
		}
		if (pPacket->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
		{
			ConnectionAccepted(pPacket);
			CCore::Instance()->GetGUI()->GetServerBrowser()->ProcessNetworkPacket((DefaultMessageIDTypes)pPacket->data[0]);
		}
		else if (pPacket->data[0] == ID_DISCONNECTION_NOTIFICATION || pPacket->data[0] == ID_CONNECTION_LOST ||
			pPacket->data[0] == ID_NO_FREE_INCOMING_CONNECTIONS || pPacket->data[0] == ID_INVALID_PASSWORD ||
			pPacket->data[0] == ID_CONNECTION_BANNED || pPacket->data[0] == ID_CONNECTION_ATTEMPT_FAILED || pPacket->data[0] == ID_ALREADY_CONNECTED)
		{
			if (pPacket->data[0] == ID_DISCONNECTION_NOTIFICATION || pPacket->data[0] == ID_CONNECTION_LOST)
			{
				CCore::Instance()->GetGUI()->DeleteAllClientScriptGUI();
				CCore::Instance()->StopMultiplayer();
				CCore::Instance()->StartMultiplayer();
			}

			SetNetworkState(NETSTATE_NONE);
			CCore::Instance()->GetGUI()->GetServerBrowser()->ProcessNetworkPacket((DefaultMessageIDTypes)pPacket->data[0]);
		}

		m_pRakPeer->DeallocatePacket(pPacket);
		if (bDisconnect)
		{
			Disconnect();
			bDisconnect = false;
		}
	}
}

void CNetworkModule::ConnectionAccepted(RakNet::Packet * pPacket)
{
	SetNetworkState(NETSTATE_CONNECTED);

	RakNet::BitStream pBitStream;
	pBitStream.Write(NETWORK_VERSION);
	pBitStream.Write(RakNet::RakString(CCore::Instance()->GetNick().Get()));
	pBitStream.Write(RakNet::RakString(SharedUtility::GetSerialHash().Get()));
	Call(RPC_INITIAL_DATA, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, true);
}