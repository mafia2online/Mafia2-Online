/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabelManager.cpp
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "CString.h"
#include "Math/CVector3.h"
#include "Math/CMaths.h"

#include "CGraphics.h"
#include "CM2Camera.h"

#include "CPlayerManager.h"
#include "CLocalPlayer.h"

#include "C3DTextLabelManager.h"

#include "CLogFile.h"

C3DTextLabelManager::C3DTextLabelManager(void) :
	m_count(0)
{
	for (EntityId textId = 0; textId < MAX_3DTEXTS; textId++)
	{
		m_p3DTextLabels[textId] = nullptr;
	}
}

C3DTextLabelManager::~C3DTextLabelManager(void)
{
	for (EntityId textId = 0; textId < MAX_3DTEXTS; textId++)
	{
		if (!IsActive(textId))
			continue;

		SAFE_DELETE(m_p3DTextLabels[textId]);
	}
}

bool	C3DTextLabelManager::Add(float fX, float fY, float fZ, String text, int color, float distance)
{
	EntityId textID = FindFreeSlot();
	if (textID < 0 || textID > MAX_3DTEXTS)
		return (false);

	m_p3DTextLabels[textID] = new C3DTextLabel(textID, fX, fY, fZ, text, color, distance);
	m_p3DTextLabels[textID]->SetActive(true);

	if (IsActive(textID) == false){
		if (m_p3DTextLabels[textID] != nullptr)
			SAFE_DELETE(m_p3DTextLabels[textID]);
		return (false);
	}
	else {
		m_count++;
		return (true);
	}
}

bool	C3DTextLabelManager::Remove(EntityId textID)
{
	if (IsActive(textID) == false)
		return (false);

	m_p3DTextLabels[textID]->SetActive(false);

	SAFE_DELETE(m_p3DTextLabels[textID]);

	if (IsActive(textID) == false){
		m_count--;
		return (true);
	}
	else {
		m_p3DTextLabels[textID]->SetActive(true);
		return (false);
	}
}

bool	C3DTextLabelManager::IsActive(EntityId textID)
{
	if (textID < 0 || textID == INVALID_ENTITY_ID)
		return (false);

	if (m_p3DTextLabels[textID] != nullptr){
		return (m_p3DTextLabels[textID]->IsActive());
	}
	else {
		return (false);
	}
}

bool	C3DTextLabelManager::IsOnScreen(EntityId textID)
{
	CVector3 textPos;
	CVector3 vecScreen;

	m_p3DTextLabels[textID]->GetPosition(&textPos);
	CCore::Instance()->GetGraphics()->WorldToScreen(textPos, &vecScreen);
	return (CCore::Instance()->GetCamera()->IsOnScreen(textPos));
}

int		C3DTextLabelManager::GetCount(void)
{
	return (m_count);
}

EntityId C3DTextLabelManager::FindFreeSlot(void)
{
	for (EntityId i = 0; i < MAX_3DTEXTS; i++)
	{
		if (IsActive(i) == false)
			return i;
	}

	return INVALID_ENTITY_ID;
}

void	C3DTextLabelManager::Render(void)
{
	CVector3 vecScreen;
	CVector3 localPos;
	CVector3 textPos;

	for (EntityId i = 0; i < MAX_3DTEXTS; i++)
	{
		if (!IsActive(i))
			continue;
		
		CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPosition(&localPos);
		
		m_p3DTextLabels[i]->GetPosition(&textPos);

		float fDistance = Math::GetDistanceBetweenPoints(localPos, textPos);
		if (fDistance > m_p3DTextLabels[i]->GetDrawDistance())
			continue;

		if (!IsOnScreen(i))
			return;

		float fScale = 1.0f;
		float dimensionWidth = CCore::Instance()->GetGraphics()->GetTextWidth(m_p3DTextLabels[i]->GetText().Get(), fScale, "tahoma-bold");
		float dimensionHeight = CCore::Instance()->GetGraphics()->GetFontHeight(fScale, "tahoma-bold");

		CCore::Instance()->GetGraphics()->WorldToScreen(textPos, &vecScreen);
		CCore::Instance()->GetGraphics()->DrawText(vecScreen.fX - (dimensionWidth / 2), vecScreen.fY, 0xFFFFFFFF, fScale, "tahoma-bold", true, m_p3DTextLabels[i]->GetText().Get());
	}
}