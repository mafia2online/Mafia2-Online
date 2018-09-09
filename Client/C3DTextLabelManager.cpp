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
	if (textID == INVALID_ENTITY_ID)
		return false;

	m_p3DTextLabels[textID] = new C3DTextLabel(textID, fX, fY, fZ, text, color, distance);
	m_count++;
	return true;
}

bool C3DTextLabelManager::Remove(EntityId textID)
{
	if (IsActive(textID)) {
		SAFE_DELETE(m_p3DTextLabels[textID]);
		m_count--;
		return true;
	}
	return false;
}

bool C3DTextLabelManager::IsActive(EntityId textID) const
{
	if (textID >= MAX_3DTEXTS) {
		return false;
	}

	return m_p3DTextLabels[textID] != nullptr;
}

bool C3DTextLabelManager::IsOnScreen(EntityId textID) const
{
	if (IsActive(textID)) {
		return false;
	}

	CVector3 textPos;
	CVector3 vecScreen;

	m_p3DTextLabels[textID]->GetPosition(&textPos);
	CCore::Instance()->GetGraphics()->WorldToScreen(textPos, &vecScreen);
	return CCore::Instance()->GetCamera()->IsOnScreen(textPos);
}

int	 C3DTextLabelManager::GetCount(void) const
{
	return m_count;
}

EntityId C3DTextLabelManager::FindFreeSlot(void) const
{
	for (EntityId i = 0; i < MAX_3DTEXTS; i++)
	{
		if (IsActive(i) == false)
			return i;
	}

	return INVALID_ENTITY_ID;
}

void C3DTextLabelManager::Render(void)
{
	CVector3 vecScreen;
	CVector3 localPos;
	CVector3 textPos;

	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPosition(&localPos);

	CGraphics* Graphics = CCore::Instance()->GetGraphics();
	for (EntityId i = 0; i < MAX_3DTEXTS; ++i)
	{
		if (!IsActive(i))
			continue;

		if (!IsOnScreen(i))
			return;

		C3DTextLabel* Label = m_p3DTextLabels[i];

		Label->GetPosition(&textPos);

		float fDistance = Math::GetDistanceBetweenPoints(localPos, textPos);
		if (fDistance > m_p3DTextLabels[i]->GetDrawDistance())
			continue;

		float fScale = 1.0f;
		float dimensionWidth = Graphics->GetTextWidth(Label->GetText().Get(), fScale, "tahoma-bold");
		float dimensionHeight = Graphics->GetFontHeight(fScale, "tahoma-bold");

		Graphics->WorldToScreen(textPos, &vecScreen);
		Graphics->DrawText(vecScreen.fX - (dimensionWidth / 2), vecScreen.fY, 0xFFFFFFFF, fScale, "tahoma-bold", true, Label->GetText().Get());
	}
}