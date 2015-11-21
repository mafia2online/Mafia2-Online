/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Server
* File       : C3DtextLabel.cpp
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

C3DTextLabel::C3DTextLabel(EntityId textID , const char *text, CVector3 vecPosition, int color, int drawDistance)
{
	// Save the variables
	m_textID = textID;
	m_text = text;
	m_position = vecPosition;
	m_color = color;
	m_distance = drawDistance;
}

C3DTextLabel::~C3DTextLabel(void)
{
}

void C3DTextLabel::AddForPlayer(EntityId playerId)
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the textLabel id
	pBitStream.WriteCompressed(m_text);

	// Write the x coordinate
	pBitStream.Write(m_position.fX);

	// Write the y coordinate
	pBitStream.Write(m_position.fY);

	// Write the z coordinate
	pBitStream.Write(m_position.fZ);

	// Write the color
	pBitStream.WriteCompressed(m_color);

	// Write the drawDistance
	pBitStream.WriteCompressed(m_distance);

	// Write the text
	pBitStream.WriteCompressed(m_text);

	// Send it to the player
	pCore->GetNetworkModule()->Call(RPC_NEWTEXTLABEL, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false);
}

void C3DTextLabel::AddForWorld(void)
{
	// Loop over all players
	for (EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		// Is the current player active?
		if (pCore->GetPlayerManager()->IsActive(i))
		{
			// Add the blip for this player
			AddForPlayer(i);
		}
	}
}

void C3DTextLabel::RemoveForPlayer(EntityId playerId)
{
	// Construct a new bitstream
	RakNet::BitStream pBitStream;

	// Write the textLabel id
	pBitStream.WriteCompressed(m_textID);

	// Send it to the player
	pCore->GetNetworkModule()->Call(RPC_REMOVETEXTLABEL, &pBitStream, HIGH_PRIORITY, RELIABLE, playerId, false);
}

void C3DTextLabel::RemoveForWorld(void)
{
	// Loop over all players
	for (EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		// Is the current player active?
		if (pCore->GetPlayerManager()->IsActive(i))
		{
			// Remove the blip for this player
			RemoveForPlayer(i);
		}
	}
}