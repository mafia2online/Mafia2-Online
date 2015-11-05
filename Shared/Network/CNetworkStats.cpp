/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CNetworkStats.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../SharedUtility.h"
#include	"CNetworkStats.h"
#include	"../../Libraries/RakNet/Source/RakNetStatistics.h"
#include	"../../Libraries/RakNet/Source/GetTime.h"

#ifdef _CLIENT
#include	"../../Client/StdInc.h"
#else
#include	"../../Server/StdInc.h"
#endif

extern	CCore				* pCore;

#ifdef _CLIENT
void CNetworkStats::GetStats( CNetStats * netStats )
#else
void CNetworkStats::GetStats( CNetStats * netStats, EntityId playerId )
#endif
{
	//
	RakNet::RakNetStatistics * pNetworkStats = NULL;

	// Get the statistics
#ifdef _CLIENT
	pNetworkStats = pCore->GetNetworkModule()->GetRakPeer()->GetStatistics( RakNet::UNASSIGNED_SYSTEM_ADDRESS );
#else
	if( playerId == INVALID_ENTITY_ID )
		pNetworkStats = pCore->GetNetworkModule()->GetRakPeer()->GetStatistics( pCore->GetNetworkModule()->GetRakPeer()->GetMyBoundAddress () );
	else
		pNetworkStats = pCore->GetNetworkModule()->GetRakPeer()->GetStatistics( pCore->GetNetworkModule()->GetRakPeer()->GetSystemAddressFromIndex( playerId ) );
#endif

	// Copy the stats
	netStats->bpsRecv = (unsigned int)pNetworkStats->valueOverLastSecond [ RakNet::ACTUAL_BYTES_RECEIVED ];
	netStats->bpsSent = (unsigned int)pNetworkStats->valueOverLastSecond [ RakNet::ACTUAL_BYTES_SENT ];
	netStats->totalRecv = (unsigned int)pNetworkStats->runningTotal [ RakNet::ACTUAL_BYTES_RECEIVED ];
	netStats->totalSent = (unsigned int)pNetworkStats->runningTotal [ RakNet::ACTUAL_BYTES_SENT ];
	netStats->connectionTime = (unsigned int)((RakNet::GetTimeUS() - pNetworkStats->connectionStartTime) / 1000000);
	netStats->fPacketLossLastSecond = pNetworkStats->packetlossLastSecond;
	netStats->fTotalPacketLoss = pNetworkStats->packetlossTotal;
	netStats->bIsLimitedByCongestionControl = pNetworkStats->isLimitedByCongestionControl;
	netStats->bIsLimitedByOutgoingBandwidthLimit = pNetworkStats->isLimitedByOutgoingBandwidthLimit;
	netStats->ulBPSLimitByCongestionControl = (unsigned int)pNetworkStats->BPSLimitByCongestionControl;
	netStats->ulBPSLimitByOutgoingBandwidthLimit = (unsigned int)pNetworkStats->BPSLimitByOutgoingBandwidthLimit;
	netStats->uiMessagesInResendBuffer = pNetworkStats->messagesInResendBuffer;
	netStats->ulBytesInResendBuffer = (unsigned int)pNetworkStats->bytesInResendBuffer;
}

String CNetworkStats::GetStats( void )
{
	// Get the stats
	CNetStats netStats;
	CNetworkStats::GetStats( &netStats );

	//
	String bytesReceived = SharedUtility::BytesToString ( netStats.bpsRecv );
	String bytesSent = SharedUtility::BytesToString ( netStats.bpsSent );
	String totalBytesReceived = SharedUtility::BytesToString ( netStats.totalRecv );
	String totalBytesSent = SharedUtility::BytesToString ( netStats.totalSent );
	String BPSLimitedByCongestionControl = SharedUtility::BytesToString ( netStats.ulBPSLimitByCongestionControl );
	String BPSLImitedByOutgoingBandwidth = SharedUtility::BytesToString ( netStats.ulBPSLimitByOutgoingBandwidthLimit );
	String bytesInResendBuffer = SharedUtility::BytesToString ( netStats.ulBytesInResendBuffer );

#ifdef _CLIENT
	return String( "Data Receive Rate: %s/s\n"
				"Data Send Rate: %s/s\n"
				"Total Data Received: %s\n"
				"Total Data Sent: %s\n"
				"Current Packet Loss: %.02f\n"
				"Total Packet Loss: %.02f\n"
				"Connection Time: %ds\n"
				"Is Limited By Congestion Control: %s\n"
				"Is Limited By Outgoing Bandwidth Limit: %s\n"
				"Congestion Control BPS Limit: %s/s\n"
				"Outgoing Bandwidth BPS Limit: %s/s\n"
				"Messages In Resend Buffer: %d (%s)\n"
				"Netcode Version: %s (%d)",
				bytesReceived.Get(), bytesSent.Get(), totalBytesReceived.Get(), totalBytesSent.Get(),
				netStats.fPacketLossLastSecond, netStats.fTotalPacketLoss, netStats.connectionTime,
				(netStats.bIsLimitedByCongestionControl ? "Yes" : "No"), (netStats.bIsLimitedByOutgoingBandwidthLimit ? "Yes" : "No"),
				BPSLimitedByCongestionControl.Get(), BPSLImitedByOutgoingBandwidth.Get(),
				netStats.uiMessagesInResendBuffer, bytesInResendBuffer.Get(),
				MOD_VERS_STR, NETWORK_VERSION );
#else
	CLogFile::Printf( "= Data Receive Rate: %s/s", bytesReceived.Get() );
	CLogFile::Printf( "= Data Send Rate: %s/s", bytesSent.Get() );
	CLogFile::Printf( "= Total Data Received: %s", totalBytesReceived.Get() );
	CLogFile::Printf( "= Total Data Sent: %s", totalBytesSent.Get() );
	CLogFile::Printf( "= Current Packet Loss: %.02f", netStats.fPacketLossLastSecond );
	CLogFile::Printf( "= Total Packet Loss: %.02f", netStats.fTotalPacketLoss );
	CLogFile::Printf( "= Connection Time: %ds", netStats.connectionTime );
	CLogFile::Printf( "= Is Limited By Congestion Control: %s", (netStats.bIsLimitedByCongestionControl ? "Yes" : "No") );
	CLogFile::Printf( "= Is Limited By Outgoing Bandwidth Limit: %s", (netStats.bIsLimitedByOutgoingBandwidthLimit ? "Yes" : "No") );
	CLogFile::Printf( "= Congestion Control BPS Limit: %s/s", BPSLimitedByCongestionControl.Get() );
	CLogFile::Printf( "= Outgoing Bandwidth BPS Limit: %s/s", BPSLImitedByOutgoingBandwidth.Get() );
	CLogFile::Printf( "= Messages In Resend Buffer: %d (%s)", netStats.uiMessagesInResendBuffer, bytesInResendBuffer.Get() );
	CLogFile::Printf( "= Netcode Version: %s (%d)", MOD_VERS_STR, NETWORK_VERSION );
	return String();
#endif
}