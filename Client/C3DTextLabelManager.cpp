/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabelManager.cpp
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#include	"BaseInc.h"

#include	"CCore.h"

#include	"CString.h"
#include	"Math\CVector3.h"
#include	"Math\CMaths.h"

#include	"CGraphics.h"
#include	"CM2Camera.h"

#include	"CPlayerManager.h"
#include	"CLocalPlayer.h"

#include	"C3DTextLabelManager.h"
C3DTextLabelManager::C3DTextLabelManager(void)
{
	DEBUG_TRACE("C3DTextLabelManager::C3DTextLabelManager");

	// Reset pointers
	for (EntityId textId = 0; textId < MAX_3DTEXTS; textId++)
	{
		m_p3DTextLabels[textId] = NULL;
	}
	//memset(m_p3DTextLabels, NULL, sizeof(m_p3DTextLabels));
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

	// Finished
	m_p3DTextLabels[textID]->SetActive(true);

	if (IsActive(textID) == false){
		if (m_p3DTextLabels[textID] != NULL)
			SAFE_DELETE(m_p3DTextLabels[textID]);
		return (false);
	}
	else {
		return (true);
	}
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
	if (m_p3DTextLabels[textID] != NULL){
		return (m_p3DTextLabels[textID]->IsActive());
	}
	else {
		return (false);
	}
}

bool	C3DTextLabelManager::IsOnScreen(EntityId textID)
{
	// Text position
	CVector3 textPos;
	m_p3DTextLabels[textID]->GetPosition(&textPos);

	// Convert coordinates
	CVector3 vecScreen;
	CCore::Instance()->GetGraphics()->WorldToScreen(textPos, &vecScreen);

	return (CCore::Instance()->GetCamera()->IsOnScreen(textPos));
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
		if (IsActive(i) == true){
			// We get the player pos
			CVector3 localPos;
			CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPosition(&localPos);

			// We get the textLabel pos
			CVector3 textPos;
			m_p3DTextLabels[i]->GetPosition(&textPos);

			// We get the distance
			float fDistance = Math::GetDistanceBetweenPoints(localPos, textPos);

			// We render on distance
			if (fDistance <= m_p3DTextLabels[i]->GetDrawDistance()){

				// If we can see the textLabel on our screen
				if (IsOnScreen(i))
				{
					//Scale
					float fScale = 1.0f;

					//Font
					float dimensionWidth = CCore::Instance()->GetGraphics()->GetTextWidth(m_p3DTextLabels[i]->GetText().Get(), fScale, "tahoma-bold");
					float dimensionHeight = CCore::Instance()->GetGraphics()->GetFontHeight(fScale, "tahoma-bold");

					// Convert position to screen position
					CVector3 vecScreen;
					CCore::Instance()->GetGraphics()->WorldToScreen(textPos, &vecScreen);

					//Draw
					CCore::Instance()->GetGraphics()->DrawTextA(vecScreen.fX - (dimensionWidth / 2), vecScreen.fY, 0xFFFFFFFF, fScale, "tahoma-bold", true, m_p3DTextLabels[i]->GetText().Get());
				}
			}
		}
	}
}