/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPed.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"BaseInc.h"

#include	"CCore.h"

#include	"Math\CVector3.h"
#include	"Math\CQuaternion.h"
#include	"CString.h"

#include	"Game\CGame.h"

#include	"CNetworkModelManager.h"

#include	"engine\CM2Entity.h"
#include	"engine\CM2Ped.h"
#include	"CIE.h"

#include	"CPed.h"

CPed::CPed( unsigned int uiModelIndex, CVector3 vecPosition, CVector3 vecRotation )
{
	// Store the model
	m_uiModelIndex = uiModelIndex;

	// Create ped
	Create(vecPosition, vecRotation);
}

CPed::~CPed( void )
{
	// Destroy ped
	Destroy();
}

void CPed::Create(CVector3 vecPosition, CVector3 vecRotation)
{
	/* This causes crash */
	/*if (m_pPed)
		Destroy();*/

	CLogFile::Print("Loading ped model manager...");

	// Get the model name and directory
	String strModel, strDirectory;
	Game::GetPlayerModelFromId(m_uiModelIndex, &strModel, &strDirectory);

	// Try load the ped model
	m_pPedModelManager = CNetworkModelManager::Load(strDirectory.Get(), strModel.Get());

	CLogFile::Printf("Loaded! 0x%p", m_pPedModelManager);

	// Create the ped
	m_pPed = new CM2Ped(IE::CreatePed(m_pPedModelManager, vecPosition));

	CLogFile::Printf("Ped created. 0x%p", m_pPed);

	// Activate the ped
	m_pPed->Activate();
}

void CPed::Destroy()
{
	if (m_pPed){
		// Deactivate the entity
		m_pPed->Deactivate();
	}
	// Free the model
	CNetworkModelManager::Unload(m_pPedModelManager);

	// Destroy the ped
	SAFE_DELETE(m_pPed);
}

void CPed::SetModel( unsigned int uiModelIndex )
{
	// Store the model
	m_uiModelIndex = uiModelIndex;

	// Get the model name and directory
	String strModel, strDirectory;
	Game::GetPlayerModelFromId(m_uiModelIndex, &strModel, &strDirectory);

	CVector3	pos;
	Quaternion	rot;
	CVector3	tempRot;

	// Save position
	m_pPed->GetPosition(&pos);
	m_pPed->GetRotation(&rot);

	// Store temp rotation
	tempRot.fX = rot.fX;
	tempRot.fY = rot.fY;
	tempRot.fZ = rot.fZ;

	// Set model by resetting instance - BUGGED
	//Create(pos, tempRot);

}

void CPed::SetNick(const char *szNick)
{
	// Store the nick
	m_pNick.Set(szNick);
}