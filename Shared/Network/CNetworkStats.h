/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : CNetworkStats.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

struct CNetStats
{
	unsigned int bpsRecv;
	unsigned int bpsSent;
	unsigned int totalRecv;
	unsigned int totalSent;
	unsigned int connectionTime;
	float fPacketLossLastSecond;
	float fTotalPacketLoss;
	bool bIsLimitedByCongestionControl;
	bool bIsLimitedByOutgoingBandwidthLimit;
	unsigned long ulBPSLimitByCongestionControl;
	unsigned long ulBPSLimitByOutgoingBandwidthLimit;
	unsigned int uiMessagesInResendBuffer;
	unsigned long ulBytesInResendBuffer;
};

class CNetworkStats
{

public:

#ifdef _CLIENT
	static	void				GetStats( CNetStats * netStats );
#else
	static	void				GetStats( CNetStats * netStats, EntityId playerId = INVALID_ENTITY_ID );
#endif
	static	String				GetStats( void );

};