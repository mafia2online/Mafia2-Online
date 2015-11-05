/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Camera.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			   h0pk1nz <>
*
***************************************************************/

#include	"StdInc.h"

#define VectorMA( v, s, b, o ) ( (o).fX = (v).fX + (b).fX * (s), (o).fY = (v).fY + (b).fY * (s), (o).fZ = (v).fZ + (b).fZ * (s) )

extern		CCore				* pCore;

CM2Camera::CM2Camera( void )
{
	// Set the camera
	SetCamera( NULL );

	// Set the game camera
	SetGameCamera( NULL );
}

CM2Camera::CM2Camera( M2Camera * pCamera )
{
	// Set the camera
	SetCamera( pCamera );

	// Get the game camera
	M2GameCamera * pGameCamera = NULL;
	_asm
	{
		push		1
		mov			ecx, pCamera
		call		COffsets::FUNC_CCamera__GetGameCameraFromIndex
		mov			pGameCamera, eax
	}

	// Set the game camera
	SetGameCamera( pGameCamera );

	// Build the projection matrix
	D3DXMatrixPerspectiveFovLH( &m_projectionMatrix, m_pGameCamera->m_fFovAngle, m_pGameCamera->pCameraData->m_fAspect, m_pGameCamera->pCameraData->m_fNear, m_pGameCamera->pCameraData->m_fFar );

#ifdef _DEBUG
	pCore->GetChat()->AddDebugMessage( "CM2Camera::CM2Camera( pCamera: 0x%p, pGameCamera: 0x%p, pCameraData: 0x%p )", pCamera, m_pGameCamera, m_pGameCamera->pCameraData );
#endif
}

CM2Camera::~CM2Camera( void )
{
}

void CM2Camera::LockControl( bool bLock )
{
	// Store the last sensitivity multiplier if we're locking the camera
	//if( bLock )
	//	m_fLastSensitivityMultiplier = pCore->GetGame()->GetMouseSensitivityMultiplier();

	// Set the camera state
	if ( bLock )
		*(BYTE *)0x1BAF07C = 2;
	else
		*(BYTE *)0x1BAF07C = 0;

	// Set the sensitivity multiplier
	//if( bLock )
	//	pCore->GetGame()->SetMouseSensitivityMultiplier( 0.0f );
	//else
	//	pCore->GetGame()->SetMouseSensitivityMultiplier( m_fLastSensitivityMultiplier );
}

bool CM2Camera::IsLocked( void )
{
	return (*(int *)0x1BAF07C != 0);
}

float CM2Camera::GetNearClip( void )
{
	// Is the camera active?
	if( m_pGameCamera && m_pGameCamera->pCameraData )
		return m_pGameCamera->pCameraData->m_fNear;

	return 0.0f;
}

float CM2Camera::GetFarClip( void )
{
	// Is the camera active?
	if( m_pGameCamera && m_pGameCamera->pCameraData )
		return m_pGameCamera->pCameraData->m_fFar;

	return 0.0f;
}

float CM2Camera::GetAspectRatio( void )
{
	// Is the camera active?
	if( m_pGameCamera && m_pGameCamera->pCameraData )
		return m_pGameCamera->pCameraData->m_fAspect;

	return 0.0f;
}

float CM2Camera::GetFovAngle( void )
{
	// Is the camera active?
	if( m_pGameCamera )
		return m_pGameCamera->m_fFovAngle;

	return 0.0f;
}

int CM2Camera::GetWindowWidth( void )
{
	// Is the camera active?
	if( m_pGameCamera )
		return m_pGameCamera->m_iWindowWidth;

	return 0;
}

int CM2Camera::GetWindowHeight( void )
{
	// Is the camera active?
	if( m_pGameCamera )
		return m_pGameCamera->m_iWindowHeight;

	return 0;
}

void CM2Camera::GetPosition( CVector3 * vecPosition )
{
	// Is the camera active?
	if( m_pGameCamera )
	{
		// Get the camera world matrix
		Matrix43 worldMatrix;
		GetWorldMatrix( &worldMatrix );

		// Copy the camera position vector
		memcpy( vecPosition, &CVector3( worldMatrix._21, worldMatrix._32, worldMatrix._43 ), sizeof(CVector3) );
	}
}

void CM2Camera::GetRight( CVector3 * vecRight )
{
	// Is the camera active?
	if( m_pGameCamera )
	{
		// Get the camera world matrix
		Matrix43 worldMatrix;
		GetWorldMatrix( &worldMatrix );

		// Copy the camera right vector
		memcpy( vecRight, &CVector3( worldMatrix._33, worldMatrix._41, worldMatrix._42 ), sizeof(CVector3) );
	}
}

void CM2Camera::GetUp( CVector3 * vecUp )
{
}

void CM2Camera::GetForward( CVector3 * vecForward )
{
	// Is the camera active?
	if( m_pGameCamera )
	{
		// Get the camera world matrix
		Matrix43 worldMatrix;
		GetWorldMatrix( &worldMatrix );

		// Copy the matrix forward vector
		memcpy( vecForward, &CVector3( worldMatrix._22, worldMatrix._23, worldMatrix._31 ), sizeof(CVector3) );
	}
}

void CM2Camera::GetWorldViewProjection( D3DXMATRIX * mat )
{
	// Is the camera active?
	if( m_pGameCamera && m_pGameCamera->pCameraData )
	{
		// Copy the matrix
		memcpy( mat, &m_pGameCamera->pCameraData->m_worldViewProjection, sizeof(D3DXMATRIX) );

		// Transpose the matrix
		D3DXMatrixTranspose( mat, mat );
	}
}

void CM2Camera::GetWorldMatrix( Matrix43 * mat )
{
	// Is the camera active?
	if( m_pGameCamera && m_pGameCamera->pCameraData )
	{
		// Copy the matrix
		memcpy( mat, &m_pGameCamera->pCameraData->m_worldMatrix, sizeof(Matrix43) );
	}
}

void CM2Camera::GetViewMatrix( D3DXMATRIX * mat )
{
	// Is the camera active?
	if( m_pGameCamera && m_pGameCamera->pCameraData )
	{
		// Copy the matrix
		memcpy( mat, &m_pGameCamera->pCameraData->m_viewMatrix, sizeof(D3DXMATRIX) );

		// Transpose the matrix
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
	// Is the camera active?
	if( m_pGameCamera )
	{
		CVector3 vCamPos = m_pGameCamera->m_vecCamPos;
		CVector3 vCamUp = m_pGameCamera->m_vecCamUp;
		CVector3 vDir = ( vCamUp - vCamPos );

		CVector3 vecLook;
		VectorMA( vCamPos, 8192.0f, vDir, vecLook );
		
		// Copy
		memcpy( vecLookAt, &vecLook, sizeof(CVector3) );
	}
}