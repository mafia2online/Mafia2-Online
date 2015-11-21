/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Server
* File       : C3DTextLabelManager.cpp
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#include "StdInc.h"

C3DTextLabelManager::C3DTextLabelManager( void )
{
	// Reset all textLabel pointers
	memset(m_3DText, NULL, sizeof(m_3DText));
}

C3DTextLabelManager::~C3DTextLabelManager(void)
{
	// Loop over all textLabel instances
	for (EntityId i = 0; i < MAX_3DTEXTS; i++)
	{
		// Remove the current textLabel
		Remove(i);
	}
}

EntityId C3DTextLabelManager::Add(const char *text, int color, float X, float Y, float Z, float distance)
{
	EntityId textID = FindFreeSlot();

	// Is the textLabel invalid ?
	if (textID < 0 || textID > MAX_3DTEXTS)
		return INVALID_ENTITY_ID;

	// Prepare variables
	CVector3 vec;
	vec.fX = X;
	vec.fY = Y;
	vec.fZ = Z;

	// Create the textLabel instance
	m_3DText[textID] = new C3DTextLabel(textID, text, vec, color, distance);

	// Did the textLabel fail to create?
	if (!IsActive(textID))
		return INVALID_ENTITY_ID;

	// Add this textLabel for everyone
	m_3DText[textID]->AddForWorld();

	return (textID);
}

bool C3DTextLabelManager::Remove(EntityId textID)
{
	// Is the textLabel invalid?
	if (!IsActive(textID))
		return false;

	// Remove this textLabel for everyone
	m_3DText[textID]->RemoveForWorld();

	// Delete the textLabel instance
	SAFE_DELETE(m_3DText[textID]);

	return (true);
}

EntityId C3DTextLabelManager::FindFreeSlot(void)
{
	// Create text entity ID
	EntityId textID = MAX_3DTEXTS;

	// Loop over all textLabel instances
	for (EntityId i = 0; i < MAX_3DTEXTS; i++)
	{
		// Is the current textLabel not active?
		if (!IsActive(i))
		{
			// Set the textLabel id to this textLabel
			textID = i;

			break;
		}
	}

	return (textID);
}

bool C3DTextLabelManager::IsActive(EntityId textID)
{
	// Is the textLabel id invalid?
	if (textID < 0 || textID > MAX_3DTEXTS)
		return false;

	return (m_3DText[textID] != NULL);
}