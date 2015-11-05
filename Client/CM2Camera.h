/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Camera.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class C_RenderCamera // size: 0x1E8
{
public:
	PAD(C_RenderCamera, pad0, 0x1E8);		// 0000 - 01E8
};

class M2CameraData // C_Camera is real name
{
public:
	PAD(M2CameraData, pad0, 0x38);			// 0000 - 0038
	Matrix43 m_worldMatrix;					// 0038 - 0068
	PAD(M2CameraData, pad1, 0x4);			// 0068 - 006C
	C_RenderCamera* m_pRenderCamera;		// 006C - 0070
	PAD(M2CameraData, pad2, 0x14);			// 0070 - 0084
	float m_fUnknown1;						// 0084 - 0088
	float m_fNear;							// 0088 - 008C
	float m_fFar;							// 008C - 0090
	float m_fAspect;						// 0090 - 0094
	PAD(M2CameraData, pad3, 0x18);			// 0094 - 00AC
	Matrix43 m_viewMatrix;					// 00AC - 00DC
	D3DXMATRIX m_projectionMatrix;			// 00DC - 011C
	D3DXMATRIX m_worldViewProjection;		// 011C - 015C
};

class M2CameraUnk001
{
public:
	// 0x5C
	// 0x5D
	// 0x5E
	// 0x5F
	// 0x60
	// 0x61
	// ^		THINK it's a RECT
};

class M2GameCamera // (size: 0x15C) - real name: C_GameCameraMafia
{
public:
	// 0x0 = VFTable
	PAD(M2GameCamera, pad0, 0x8);			// 0000 - 0008
	Matrix43 m_matWorld;					// 0008 - 0038
	CVector3 m_vecCamPos;					// 0038 - 0044
	CVector3 m_vecCamUp;					// 0044 - 0050
	PAD(M2GameCamera, pad2, 0xC);			// 0000 - 005C
	float m_fFovAngle;						// 005C - 0060
	PAD(M2GameCamera, pad3, 0x8C);			// 0060 - 00EC
	int m_iUnknown1;						// 00EC - 00F0
	PAD(M2GameCamera, pad4, 0x34);			// 00F0 - 0124
	int m_iWindowWidth;						// 0124 - 0128
	int m_iWindowHeight;					// 0128 - 012C
	M2CameraData * pCameraData;				// 012C - 0130
	// 0x134 = Class (M2CameraUnk001)
	//  + 0x4 = BYTE
	//  + 0x8 = BYTE (flags)
	//	+ 0x5C = BYTE
};

class M2Camera
{
public:
	PAD(M2Camera, pad0, 0x8);				// 0000 - 0008
	M2GameCamera * pCamera1;				// 0008 - 000C
	M2GameCamera * pCamera2;				// 000C - 0010
};

class CM2Camera
{

private:

	M2Camera					* m_pCamera;
	M2GameCamera				* m_pGameCamera;
	D3DXMATRIX					m_projectionMatrix;
	float						m_fLastSensitivityMultiplier;

public:

	CM2Camera( void );
	CM2Camera( M2Camera * pCamera );
	~CM2Camera( void );

	void						SetCamera( M2Camera * pCamera ) { m_pCamera = pCamera; }
	M2Camera					* GetCamera( void ) { return m_pCamera; }

	void						SetGameCamera( M2GameCamera * pGameCamera ) { m_pGameCamera = pGameCamera; }
	M2GameCamera				* GetGameCamera( void ) { return m_pGameCamera; }

	void						LockControl( bool bLocked );
	bool						IsLocked( void );

	float						GetNearClip( void );
	float						GetFarClip( void );
	float						GetAspectRatio( void );
	float						GetFovAngle( void );
	int							GetWindowWidth( void );
	int							GetWindowHeight( void );

	void						GetPosition( CVector3 * vecPosition );
	void						GetRight( CVector3 * vecRight );
	void						GetUp( CVector3 * vecUp );
	void						GetForward( CVector3 * vecForward );
	void						GetWorldViewProjection( D3DXMATRIX * mat );
	void						GetWorldMatrix( Matrix43 * mat );
	void						GetViewMatrix( D3DXMATRIX * mat );
	void						GetProjectionMatrix( D3DXMATRIX * mat );
	void						GetLookAt( CVector3 * vecLookAt );

};