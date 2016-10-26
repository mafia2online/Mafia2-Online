/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPed.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "Math/CVector3.h"
#include "Math/CQuaternion.h"
#include "CString.h"

#include "Game/CGame.h"

#include "CNetworkModelManager.h"

#include "engine/CM2Entity.h"
#include "engine/CM2Ped.h"
#include "CIE.h"

#include "CPed.h"

#include "CLogFile.h"

CPed::CPed( unsigned int uiModelIndex, CVector3 vecPosition, CVector3 vecRotation )
{
	m_uiModelIndex = uiModelIndex;

	Create(vecPosition, vecRotation);
}

CPed::~CPed( void )
{
	Destroy(true);
}

void CPed::Create(CVector3 vecPosition, CVector3 vecRotation)
{
	DEBUG_LOG("Loading ped model manager...");

	String strModel, strDirectory;
	Game::GetPlayerModelFromId(m_uiModelIndex, &strModel, &strDirectory);

	m_pPedModelManager = CCore::Instance()->GetModelManager()->Load(strDirectory.Get(), strModel.Get());

	DEBUG_LOG("Loaded! 0x%p", m_pPedModelManager);

	m_pPed = IE::CreateWrapperPed(m_pPedModelManager);

	if (!m_pPed) {
		CLogFile::Print("Failed to create ped");
		return;
	}

	DEBUG_LOG("Ped created. 0x%p", m_pPed);

	m_pPed->Activate();

	m_pPed->SetPosition(vecPosition);
	m_pPed->SetRotation(vecRotation);
}

void CPed::Destroy(bool del = true)
{
	if (m_pPed){
		m_pPed->Deactivate();
	}

	CCore::Instance()->GetModelManager()->Free(m_pPedModelManager);

	if (del) {
		SAFE_DELETE(m_pPed);
	}
}

void CPed::SetModel( unsigned int uiModelIndex )
{
	CVector3	pos;
	Quaternion	rot;
	CVector3	tempRot;

	m_pPed->GetPosition(&pos);
	m_pPed->GetRotation(&rot);

	tempRot.fX = rot.fX;
	tempRot.fY = rot.fY;
	tempRot.fZ = rot.fZ;

	Destroy(false);

	m_uiModelIndex = uiModelIndex;

	Create(pos, tempRot);
}

void CPed::SetNick(const char *szNick)
{
	if (strlen(szNick) > 0)
		m_pNick.Set(szNick);
}