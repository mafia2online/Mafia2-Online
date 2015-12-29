/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabelManager.cpp
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

C3DTextLabelManager::C3DTextLabelManager(void)
{
	DEBUG_TRACE("C3DTextLabelManager::C3DTextLabelManager");

	// Reset pointers
	memset(m_p3DTextLabels, NULL, sizeof(m_p3DTextLabels));
}

C3DTextLabelManager::~C3DTextLabelManager(void)
{
	DEBUG_TRACE("C3DTextLabelManager::~C3DTextLabelManager");

	// Loop and delete all active 3DTextLabel
	for (EntityId textId = 0; textId < MAX_3DTEXTS; textId++)
	{
		if (IsActive(textId) == true){
			SAFE_DELETE(m_p3DTextLabels[textId]);
		}
	}
}

bool	C3DTextLabelManager::Add(float fX, float fY, float fZ, String text, int color, float distance)
{
	// Find free entityId
	EntityId textID = FindFreeSlot();

	// Incorrect 3DText ?
	if (textID < 0 || textID > MAX_3DTEXTS)
		return (false);

	// Create the 3DText
	m_p3DTextLabels[textID] = new C3DTextLabel(textID, fX, fY, fZ, text, color, distance);

	// Failed ?
	if (IsActive(textID) == false)
		return (false);

	// Finished
	m_p3DTextLabels[textID]->SetActive(true);
	return (true);
}

bool	C3DTextLabelManager::Remove(EntityId textID)
{
	// Active 3DText ?
	if (IsActive(textID) == false)
		return (false);

	// Mark as inactive
	m_p3DTextLabels[textID]->SetActive(false);

	// Try to delete
	SAFE_DELETE(m_p3DTextLabels[textID]);

	// Deleted ?
	if (IsActive(textID) == false){
		return (true);
	}
	else {
		// Restore state if cannot delete
		m_p3DTextLabels[textID]->SetActive(true);
		return (false);
	}
}

bool	C3DTextLabelManager::IsActive(EntityId textID)
{
	// Invalid 3DText ?
	if (textID < 0 || textID == INVALID_ENTITY_ID)
		return (false);

	// Return state
	return(m_p3DTextLabels[textID]->IsActive());
}

bool	C3DTextLabelManager::IsOnScreen(EntityId textID)
{
	return (true);
}

int		C3DTextLabelManager::GetCount(void)
{
	// Declare counter
	int count = 0;

	// Loop over all entities
	for (EntityId i = 0; i < MAX_3DTEXTS; i++)
	{
		// Is the current label active ?
		if (IsActive(i) == true)
			count++;
	}

	// Return counter
	return (count);
}

EntityId C3DTextLabelManager::FindFreeSlot(void)
{
	// Loop over all textlabel instances
	for (EntityId i = 0; i < MAX_3DTEXTS; i++)
	{
		// Is the current textLabel not active?
		if (IsActive(i) == false)
			return i;
	}

	return INVALID_ENTITY_ID;
}

void	C3DTextLabelManager::PreRender(void)
{
	//Todo
}

void	C3DTextLabelManager::Render(void)
{
	for (EntityId i = 0; i < MAX_3DTEXTS; i++)
	{
		// Is active 3DTextLabel
		if (m_p3DTextLabels[i]->IsActive() == true){
			// If we can see the textLabel on our screen
			if (IsOnScreen(i))
			{
				// We get the player pos
				CVector3 localPos;
				pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition(&localPos);

				// We get the textLabel pos
				CVector3 textPos;
				m_p3DTextLabels[i]->GetPosition(&textPos);

				// We get the distance
				float fDistance = Math::GetDistanceBetweenPoints(localPos, textPos);

				// We render on distance
				if (fDistance <= m_p3DTextLabels[i]->GetDrawDistance()){
					//Scale
					float fScale = 1.0f;

					//Font
					float dimensionWidth = pCore->GetGraphics()->GetTextWidth(m_p3DTextLabels[i]->GetText().Get(), fScale, "tahoma-bold");
					float dimensionHeight = pCore->GetGraphics()->GetFontHeight(fScale, "tahoma-bold");

					// Convert position to screen position
					CVector3 vecScreen;
					pCore->GetGraphics()->WorldToScreen(textPos, &vecScreen);

					//Draw
					pCore->GetGraphics()->DrawTextA(vecScreen.fX - (dimensionWidth / 2), vecScreen.fY, 0xFFFFFFFF, fScale, "tahoma-bold", true, m_p3DTextLabels[i]->GetText().Get());
				}
			}
		}
	}
}