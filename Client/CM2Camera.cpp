/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Camera.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			   h0pk1nz <>
*
***************************************************************/

#include "BaseInc.h"

#include "CLua.h"

#include "Math/CVector3.h"
#include "Math/CMatrix.h"

#include "CM2Camera.h"

#include "CGraphics.h"

#include "CCore.h"

#include "COffsets.h"

CM2Camera::CM2Camera( void )
	: m_pCamera(nullptr)
	, m_pGameCamera(nullptr)
	, m_projectionMatrix()
	, m_fLastSensitivityMultiplier(0.0f)
{
	memset(m_projectionMatrix, 0, sizeof(m_projectionMatrix));
}

CM2Camera::CM2Camera( M2Camera * pCamera )
	: CM2Camera()
{
	SetCamera( pCamera );

	M2GameCamera * pGameCamera = pCamera->GetCamera(1);
	SetGameCamera( pGameCamera );

	// Build the projection matrix
	D3DXMatrixPerspectiveFovLH( &m_projectionMatrix, m_pGameCamera->m_fFovAngle, m_pGameCamera->pCameraData->m_fAspect, m_pGameCamera->pCameraData->m_fNear, m_pGameCamera->pCameraData->m_fFar );
}

CM2Camera::~CM2Camera( void )
{
}

void CM2Camera::LockControl( bool bLock )
{
	// Store the last sensitivity multiplier if we're locking the camera
	//if( bLock )
	//	m_fLastSensitivityMultiplier = pCore->GetGame()->GetMouseSensitivityMultiplier();

	if ( bLock )
		*(BYTE *)COffsets::VAR_CCamera__State = 2;
	else
		*(BYTE *)COffsets::VAR_CCamera__State = 0;

	// Set the sensitivity multiplier
	//if( bLock )
	//	pCore->GetGame()->SetMouseSensitivityMultiplier( 0.0f );
	//else
	//	pCore->GetGame()->SetMouseSensitivityMultiplier( m_fLastSensitivityMultiplier );
}

bool CM2Camera::IsLocked( void ) const
{
	return (*(int *)COffsets::VAR_CCamera__State != 0);
}

float CM2Camera::GetNearClip( void )
{
	if( m_pGameCamera && m_pGameCamera->pCameraData )
		return m_pGameCamera->pCameraData->m_fNear;

	return 0.0f;
}

float CM2Camera::GetFarClip( void )
{
	if( m_pGameCamera && m_pGameCamera->pCameraData )
		return m_pGameCamera->pCameraData->m_fFar;

	return 0.0f;
}

float CM2Camera::GetAspectRatio( void )
{
	if( m_pGameCamera && m_pGameCamera->pCameraData )
		return m_pGameCamera->pCameraData->m_fAspect;

	return 0.0f;
}

float CM2Camera::GetFovAngle( void )
{
	if( m_pGameCamera )
		return m_pGameCamera->m_fFovAngle;

	return 0.0f;
}

int CM2Camera::GetWindowWidth( void )
{
	if( m_pGameCamera )
		return m_pGameCamera->m_iWindowWidth;

	return 0;
}

int CM2Camera::GetWindowHeight( void )
{
	if( m_pGameCamera )
		return m_pGameCamera->m_iWindowHeight;

	return 0;
}

void CM2Camera::GetPosition( CVector3 * vecPosition )
{
	if( m_pGameCamera )
	{
		Matrix43 worldMatrix;
		GetWorldMatrix( &worldMatrix );

		memcpy( vecPosition, &CVector3( worldMatrix._21, worldMatrix._32, worldMatrix._43 ), sizeof(CVector3) );
	}
}

void CM2Camera::GetRight( CVector3 * vecRight )
{
	if( m_pGameCamera )
	{
		Matrix43 worldMatrix;
		GetWorldMatrix( &worldMatrix );

		memcpy( vecRight, &CVector3( worldMatrix._33, worldMatrix._41, worldMatrix._42 ), sizeof(CVector3) );
	}
}

void CM2Camera::GetUp( CVector3 * vecUp )
{
}

void CM2Camera::GetForward( CVector3 * vecForward )
{
	if( m_pGameCamera )
	{
		Matrix43 worldMatrix;
		GetWorldMatrix( &worldMatrix );
		memcpy( vecForward, &CVector3( worldMatrix._22, worldMatrix._23, worldMatrix._31 ), sizeof(CVector3) );
	}
}

void CM2Camera::GetWorldViewProjection( D3DXMATRIX * mat )
{
	if( m_pGameCamera && m_pGameCamera->pCameraData )
	{
		memcpy( mat, &m_pGameCamera->pCameraData->m_worldViewProjection, sizeof(D3DXMATRIX) );
		D3DXMatrixTranspose( mat, mat );
	}
}

void CM2Camera::GetWorldMatrix( Matrix43 * mat )
{
	if( m_pGameCamera && m_pGameCamera->pCameraData )
	{
		memcpy( mat, &m_pGameCamera->pCameraData->m_worldMatrix, sizeof(Matrix43) );
	}
}

bool CM2Camera::IsOnScreen(const CVector3 & vecPosition)
{
	CCore *pCore = CCore::Instance();

	if (pCore->GetCamera() == NULL)
		return (false);

	CVector3 vecCamPos;
	pCore->GetCamera()->GetPosition(&vecCamPos);

	CVector3 vecCamLookAt;
	pCore->GetCamera()->GetLookAt(&vecCamLookAt);

	D3DXMATRIX matView;
	D3DXVECTOR3 _vecCamPos(vecCamPos.fX, vecCamPos.fY, vecCamPos.fZ);
	D3DXVECTOR3 _vecCamLookAt(vecCamLookAt.fX, vecCamLookAt.fY, vecCamLookAt.fZ);
	D3DXMatrixLookAtLH(&matView, &_vecCamPos, &_vecCamLookAt, &D3DXVECTOR3(0, 0, 1));

	D3DVIEWPORT9 viewport;
	pCore->GetGraphics()->GetDevice()->GetViewport(&viewport);

	DWORD dwLenX = viewport.Width;
	DWORD dwLenY = viewport.Height;

	D3DXMATRIX matProj;

	D3DXMatrixPerspectiveFovLH(&matProj, pCore->GetCamera()->GetFovAngle(), (float)dwLenX / (float)dwLenY, pCore->GetCamera()->GetNearClip(), pCore->GetCamera()->GetFarClip());

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	D3DXVECTOR3 vecSPos;
	D3DXVECTOR3 _vecPosition(vecPosition.fX, vecPosition.fY, vecPosition.fZ);
	D3DXVec3Project(&vecSPos, &_vecPosition, &viewport, &matProj, &matView, &matWorld);

	return (vecSPos.z < 1.f);
}

void CM2Camera::GetViewMatrix( D3DXMATRIX * mat )
{
	if( m_pGameCamera && m_pGameCamera->pCameraData )
	{
		memcpy( mat, &m_pGameCamera->pCameraData->m_viewMatrix, sizeof(D3DXMATRIX) );
		D3DXMatrixTranspose( mat, mat );
	}
}

void CM2Camera::GetProjectionMatrix( D3DXMATRIX * mat )
{
	// Copy the matrix
	memcpy( mat, &m_projectionMatrix, sizeof(D3DXMATRIX) );
}

void CM2Camera::GetLookAt( CVector3 * vecLookAt )
{
	if( m_pGameCamera )
	{
		CVector3 vCamPos = m_pGameCamera->m_vecCamPos;
		CVector3 vCamUp = m_pGameCamera->m_vecCamUp;
		CVector3 vDir = ( vCamUp - vCamPos );

		// TODO: Find out what is the "8192.0f" if it's some random value big
		//       enough to fit this case or some value from the game.
		static const float DIRECTION_MULTIPLIER = 8192.0f;

		*vecLookAt = (vCamPos + vDir * DIRECTION_MULTIPLIER);
	}
}

M2GameCamera _declspec(naked) * M2Camera::GetCamera(int index)
{
	_asm jmp COffsets::FUNC_CCamera__GetGameCameraFromIndex
}

DWORD CCamera_Look = 0x0B37BF0;
void _declspec(naked) M2GameCamera::LockLookAt(const char *unk1, const char *unk2, double unk3)
{
	_asm jmp CCamera_Look;
}

DWORD CCamera_Rotation = 0x0B4FE70;
void _declspec(naked) M2GameCamera::RotationTowards(CVector3 vec)
{
	_asm jmp CCamera_Rotation;
}

DWORD CCamera_ModeChange = 0x107BB70;
void _declspec(naked) M2GameCamera::ModeChange(int a2, void *a3, int a4, int modeID, int a6, int a7, int a8, int a9, int a10)
{
	_asm jmp CCamera_ModeChange;
}

DWORD CCamera_BroadcastCommand = 0x107A010;
void _declspec(naked) M2GameCamera::BroadcastCommand(eCameraCommand command, void *data, void *unknow)
{
	_asm jmp CCamera_BroadcastCommand;
}

DWORD CCamera_ModePop = 0x107A5B0;
void _declspec(naked) M2GameCamera::ModePop(int unk1, int unk2)
{
	_asm jmp CCamera_ModePop;
}

void CM2Camera::LockLookAt(const char *unk1, const char *unk2, double unk3)
{
	if (!m_pGameCamera)
		return;

	m_pGameCamera->LockLookAt(unk1, unk2, unk3);
}

void CM2Camera::SetRotation(CVector3 vec)
{
	if (!m_pGameCamera)
		return;

	m_pGameCamera->RotationTowards(vec);
}

void CM2Camera::SimpleShake(float speed, float strength, float duration)
{
	if (!m_pGameCamera)
		return;

	ShakeCommandData data;
	data.speed = speed;
	data.strength = strength;
	data.duration = duration;

	m_pGameCamera->BroadcastCommand(CAMCOMMAND_SHAKE, &data, 0);
}

void CM2Camera::EnableFPV(bool enable)
{
	if (!m_pGameCamera)
		return;

	if (enable)
	{
		CLua::Execute("game.cameramanager:GetPlayerMainCamera(0):EnableFPV(game.game:GetActivePlayer(), true)");
	}
	else
	{
		m_pGameCamera->ModePop(1, 1);
	}
}