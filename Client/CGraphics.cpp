/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGraphics.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include <iostream>
#include <algorithm>

#include <map>

#include "CString.h"
#include "Math/CVector3.h"

#include "CCore.h"
#include "CGraphics.h"

#include "CFont.h"

#include "CDirect3D9Hook.h"
#include "CDirectInput8Hook.h"

#include "CPlayerManager.h"
#include "CLocalPlayer.h"

#include "CM2Camera.h"

#include "CIE.h"

#include "SharedUtility.h"

#include "CLogFile.h"

CGraphics::CGraphics( void )
	: m_pSprite(nullptr)
	, m_pDevice(nullptr)
	, m_pOriginalTarget(nullptr)
	, m_pStateBlock(nullptr)

	, m_pDirect(nullptr)
	, m_pD3Identifier()

	, m_fonts()
	, m_bigFonts()
	, m_customFonts()

	, m_cachedTextureMap()
{
	CDirect3D9Hook::Install ();
	CDirectInput8Hook::Install (); // TODO: Move? Does not fit into graphics imho.

	m_pDirect = Direct3DCreate9(D3D_SDK_VERSION);
}

CGraphics::~CGraphics( void )
{
	// Expire the texture cache
	ExpireCachedTextures( true );

	// Clear the fonts list
	m_fonts.clear();

	// Clear the large fonts list
	m_bigFonts.clear();

	// Clear the custom fonts list
	m_customFonts.clear();

	// Delete device
	SAFE_DELETE(m_pDirect);
}

void CGraphics::Initialise( IDirect3DDevice9 * pDevice )
{
	// Is the device pointer valid?
	if( pDevice )
	{
		// Store the device pointer
		m_pDevice = pDevice;

		// Load the font's
		if( !LoadFonts() )
		{
			CLogFile::Printf( "Failed to load default fonts." );
			TerminateProcess( GetCurrentProcess(), 0 );
		}

		// Get the original render target
		m_pDevice->GetRenderTarget( 0, &m_pOriginalTarget );

		CreateStateBlock();
	}
}


/**
 * Create state block if not exists.
 */
void CGraphics::CreateStateBlock(void)
{
	if ( m_pStateBlock )
		return;

	assert(m_pDevice);
	if( FAILED( m_pDevice->CreateStateBlock( D3DSBT_ALL, &m_pStateBlock ) ) ) {
		CLogFile::Print("Failed to initialize state block.");
		return;
	}
}

bool CGraphics::IsUsingNVIDIA(void)
{
	m_pDirect->GetAdapterIdentifier(0, 0, &m_pD3Identifier);
	if (strstr(m_pD3Identifier.Description, "NVIDIA") != NULL){
		return (true);
	}
	else {
		return (false);
	}
}

bool CGraphics::IsUsingAMD(void)
{
	m_pDirect->GetAdapterIdentifier(0, 0, &m_pD3Identifier);
	if (strstr(m_pD3Identifier.Description, "AMD") != NULL){
		return (true);
	}
	else {
		return (false);
	}
}

bool CGraphics::IsUsingChipset( void )
{
	m_pDirect->GetAdapterIdentifier(0, 0, &m_pD3Identifier);
	if (strstr(m_pD3Identifier.Description, "Intel(R) HD Graphics Family") != NULL){
		return (true);
	} else {
		return (false);
	}
}

bool CGraphics::LoadFonts( void )
{
	bool bSuccess = true;

	// Load normal size fonts
	bSuccess &= LoadFont( "tahoma-bold", 15, true, &m_fonts );
	bSuccess &= LoadFont( "verdana-bold", 15, true, &m_fonts );

	// Load larger size fonts
	bSuccess &= LoadFont ( "tahoma-bold", 60, true, &m_bigFonts );
	bSuccess &= LoadFont ( "verdana-bold", 60, true, &m_bigFonts );

	return (bSuccess && SUCCEEDED( D3DXCreateSprite( m_pDevice, &m_pSprite ) ));
}

bool CGraphics::LoadFont( String strFont, unsigned int uiSize, bool bBold, std::list< CFont* > * list )
{
	// Create the font
	CFont * pFont = new CFont( strFont, uiSize, bBold );

	// Try load the font
	if( !pFont->Load() )
	{
		CLogFile::Printf( "Failed to load font '%s'.", strFont.Get() );
		SAFE_DELETE( pFont );
		return false;
	}

	CLogFile::Printf( "Loaded font '%s-%d'.", strFont.Get(), uiSize );

	// Add the font to the list
	list->push_back( pFont );

	return true;
}

bool CGraphics::LoadFont( String strFont, unsigned int uiSize, bool bBold )
{
	// Try add the font resource
	int iLoaded = AddFontResourceEx( strFont.Get(), FR_PRIVATE, 0 );

	// Did the font fail to add?
	if( !iLoaded )
		return false;

	// Load the font
	return LoadFont( strFont, uiSize, bBold, &m_customFonts );
}

void CGraphics::DrawText( float uiLeft, float uiTop, float uiRight, float uiBottom, unsigned long ulColor, const char * szText, float fScaleX, float fScaleY, unsigned long ulFormat, String strFont, bool bShadow )
{
	// Do not accept invalid text strings or null sprites
	if( !szText || !m_pSprite )
		return;

	bool bBigFont = false;

	// Get the font
	ID3DXFont * m_pFont = GetFont( strFont );

	// We're using a big font so keep it looking nice, so get the actual scale
    if( fScaleX > 1.1f || fScaleY > 1.1f )
    {
		m_pFont = GetBigFont( strFont );
        fScaleX /= 4.0f;
        fScaleY /= 4.0f;
		bBigFont = true;
    }

	// Is the font unavailable?
	if( m_pFont == NULL )
	{
		// Get the default font
		m_pFont = (bBigFont ? GetBigFont( "tahoma-bold" ) : GetFont( "tahoma-bold" ));

		// Font still doesn't exist? What the fuck happend?
		if( !m_pFont )
		{
			CLogFile::Printf( "Failed to load default font. Exiting." );
			TerminateProcess( GetCurrentProcess(), 0 );
			return;
		}
	}

	// Prevent the rect from getting scaled along with the size
    uiLeft = uiLeft * (1.0f / fScaleX);
    uiTop = uiTop * (1.0f / fScaleY);
    uiRight = uiRight * (1.0f / fScaleX);
    uiBottom = uiBottom * (1.0f / fScaleY);

	// Set the rect
	RECT rect;
    SetRect( &rect, uiLeft, uiTop, uiRight, uiBottom );

	// Set the scale
	D3DXMATRIX matrix;
	D3DXVECTOR2 scalingCentre( 0.5f, 0.5f );
	D3DXVECTOR2 scaling( fScaleX, fScaleY );

	// Begin the sprite
	m_pSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	// Set the sprite transform
	D3DXMatrixTransformation2D( &matrix, NULL, 0.0f, &scaling, NULL, 0.0f, NULL );
	m_pSprite->SetTransform( &matrix );

	// If we need to draw shadow do it now
	if ( bShadow )
	{
		// Create the shadow rect
		RECT shadow_rect;
		SetRect( &shadow_rect, uiLeft + 1, uiTop + 1, uiRight + 1, uiBottom + 1 );

		// Draw the text shadow
		m_pFont->DrawTextA( m_pSprite, szText, -1, &shadow_rect, ulFormat, D3DCOLOR_ARGB( 255, 0, 0, 0 ) );
	}

	// Draw the text
	m_pFont->DrawTextA( m_pSprite, szText, -1, &rect, ulFormat, ulColor );

	// End the sprite
    m_pSprite->End( );
}

void CGraphics::DrawText( float uiLeft, float uiTop, unsigned long ulColor, const char * szText, float fScale, String strFont, bool bShadow )
{
	DrawText( uiLeft, uiTop, uiLeft, uiTop, ulColor, szText, fScale, fScale, DT_NOCLIP, strFont, bShadow );
}

void CGraphics::DrawText( float uiLeft, float uiTop, unsigned long ulColor, float fScale, String strFont, bool bShadow, const char * szText, ... )
{
	// Get the string arguments
	char szBuffer[ 2048 ];
	va_list vArgs;
	va_start( vArgs, szText );
	vsprintf_s( szBuffer, szText, vArgs );
	va_end( vArgs );

	// Draw the text
	DrawText( uiLeft, uiTop, ulColor, szBuffer, fScale, strFont, bShadow );
}

void CGraphics::DrawText( float uiLeft, float uiTop, unsigned long ulColor, float fScale, String strFont, bool bShadow, unsigned long ulFormat, const char * szText, ... )
{
	// Get the string arguments
	char szBuffer[ 2048 ];
	va_list vArgs;
	va_start( vArgs, szText );
	vsprintf_s( szBuffer, szText, vArgs );
	va_end( vArgs );

	// Draw the text
	DrawText( uiLeft, uiTop, uiLeft, uiTop, ulColor, szBuffer, fScale, fScale, ulFormat, strFont, bShadow );
}

void CGraphics::DrawText( CVector3 vecPosition, float fDistance, unsigned long ulColor, float fScale, String strFont, bool bShadow, String strText )
{
	// Convert the world coordinates to screen space coordinates
	CVector3 vecScreen;
	WorldToScreen( vecPosition, &vecScreen );

	// Is the position behind the camera?
	if( vecScreen.fZ < 0 )
		return;

	// Get the current player position
	CVector3 vecCurrentPosition;
	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecCurrentPosition );

	// Are we too far away?
	if( (vecCurrentPosition - vecPosition).Length() > fDistance )
		return;

	// Fix the position
	vecScreen.fX -= (GetTextWidth( strText.Get(), 1.0f, strFont ) / 2);

	// Draw the text
	DrawText( vecScreen.fX, vecScreen.fY, ulColor, fScale, strFont, bShadow, strText.Get() );
}

void CGraphics::DrawBox( CVector3 vecPosition, float fWidth, float fHeight, float fDistance, DWORD dwBoxColour )
{
	// Convert the world coordinates to screen space coordinates
	CVector3 vecScreen;
	WorldToScreen( vecPosition, &vecScreen );

	// Is the position behind the camera?
	if( vecScreen.fZ < 0 )
		return;

	// Get the current player position
	CVector3 vecCurrentPosition;
	CCore::Instance()->GetPlayerManager()->GetLocalPlayer()->GetPosition(&vecCurrentPosition);

	// Are we too far away?
	if( (vecCurrentPosition - vecPosition).Length() > fDistance )
		return;

	// Draw the box
	DrawBox( (vecScreen.fX - (fWidth / 2)), (vecScreen.fY - (fHeight / 2)), fWidth, fHeight, dwBoxColour );
}

void CGraphics::OnLostDevice( IDirect3DDevice9 * pDevice )
{
	std::list< CFont* >::iterator iter;

	// Notify our small fonts
	for( iter = m_fonts.begin(); iter != m_fonts.end(); iter++ )
		(*iter)->GetFont()->OnLostDevice();

	// Notify our large fonts
	for( iter = m_bigFonts.begin(); iter != m_bigFonts.end(); iter++ )
		(*iter)->GetFont()->OnLostDevice();

	// Notify our custom fonts
	for( iter = m_customFonts.begin(); iter != m_customFonts.end(); iter++ )
		(*iter)->GetFont()->OnLostDevice();

	// Notify our sprite
	if( m_pSprite )
		m_pSprite->OnLostDevice ();

	// Release the original surface
	m_pOriginalTarget->Release ();

	SAFE_RELEASE(m_pStateBlock);
}

void CGraphics::OnRestoreDevice( IDirect3DDevice9 * pDevice )
{
	std::list< CFont* >::iterator iter;

	// Notify our small fonts
	for( iter = m_fonts.begin(); iter != m_fonts.end(); iter++ )
		(*iter)->GetFont()->OnResetDevice ();

	// Notify our large fonts
	for( iter = m_bigFonts.begin(); iter != m_bigFonts.end(); iter++ )
		(*iter)->GetFont()->OnResetDevice ();

	// Notify our custom fonts
	for( iter = m_customFonts.begin(); iter != m_customFonts.end(); iter++ )
		(*iter)->GetFont()->OnResetDevice ();

	// Notify our sprite
	if( m_pSprite )
		m_pSprite->OnResetDevice ();

	// Get the original render target
	m_pDevice->GetRenderTarget ( 0, &m_pOriginalTarget );

	CreateStateBlock();
}

ID3DXFont * CGraphics::GetFont( String strFont )
{
	// Loop through all small fonts
	for( std::list< CFont* >::iterator iter = m_fonts.begin(); iter != m_fonts.end(); iter++ )
	{
		// Is this the font we're looking for?
		if( (*iter)->GetName() == strFont )
			return (*iter)->GetFont();
	}

	return NULL;
}

ID3DXFont * CGraphics::GetBigFont( String strFont )
{
	// Loop through all large fonts
	for( std::list< CFont* >::iterator iter = m_bigFonts.begin(); iter != m_bigFonts.end(); iter++ )
	{
		// Is this the font we're looking for?
		if( (*iter)->GetName() == strFont )
			return (*iter)->GetFont();
	}

	return NULL;
}

float CGraphics::GetTextWidth( const char * szText, float fScale, String strFont )
{
	LPD3DXFONT pFont = GetFont( strFont );

	if( pFont )
	{
		HDC dc = pFont->GetDC();
		SIZE size;
		GetTextExtentPoint32( dc, szText, strlen(szText), &size );
		return ((float)size.cx * fScale);
	}

	return 0.0f;
}

float CGraphics::GetFontHeight( float fScale, String strFont )
{
	// Get the font
	LPD3DXFONT pFont = GetFont( strFont );

	// Is the font scale big?
	if( fScale > 1.1f )
	{
		pFont = GetBigFont( strFont );
		fScale /= 4.0f;
	}

	if( pFont )
	{
		D3DXFONT_DESC desc;
		pFont->GetDesc( &desc );
		return ( (float) desc.Height * fScale );
	}

	return 0.0f;
}

float CGraphics::GetCharacterWidth( char c, float fScale, String strFont )
{
	// Get the font
	LPD3DXFONT pFont = GetFont( strFont );

	if( fScale > 1.1f )
	{
		pFont = GetBigFont( strFont );
		fScale /= 4.0f;
	}

	if( pFont )
	{
		HDC dc = pFont->GetDC ();
        SIZE size;
        GetTextExtentPoint32 ( dc, &c, 1, &size );
		return ( ( float ) size.cx * fScale );
	}

	return 0.0f;
}

void CGraphics::WorldToScreen( CVector3 vecWorld, CVector3 * vecScreen )
{
	// Get the world view projection matrix
	D3DXMATRIX worldViewProjection;
	CCore::Instance()->GetCamera()->GetWorldViewProjection(&worldViewProjection);

	// Get the viewport
	D3DVIEWPORT9 viewport;
	m_pDevice->GetViewport( &viewport );

	// Transform the world coordinate by the worldViewProjection matrix
	D3DXVECTOR3 vec;
	D3DXVec3TransformCoord( &vec, &D3DXVECTOR3( vecWorld.fX, vecWorld.fY, vecWorld.fZ ), &worldViewProjection );

	vecScreen->fX = viewport.X + (1.0f + vec.x) * viewport.Width / 2.0f;
	vecScreen->fY = viewport.Y + (1.0f - vec.y) * viewport.Height / 2.0f;
	vecScreen->fZ = viewport.MinZ + vec.z * (viewport.MaxZ - viewport.MinZ);
}

void CGraphics::ScreenToWorld( CVector3 vecScreen, CVector3 * vecWorld )
{
	// Get the world view projection matrix
	D3DXMATRIX worldViewProjection;
	CCore::Instance()->GetCamera()->GetWorldViewProjection(&worldViewProjection);

	// Get the viewport
	D3DVIEWPORT9 viewport;
	m_pDevice->GetViewport( &viewport );

	// Inverse the worldViewProjection matrix
	D3DXMATRIX invWorldViewProjectionMatrix;
	D3DXMatrixInverse( &invWorldViewProjectionMatrix, NULL, &worldViewProjection );

	// Reverse screen coordinates
	D3DXVECTOR3 out, vec;
	vec.x = 2.0f * (vecScreen.fX - viewport.X) / viewport.Width - 1.0f;
	vec.y = 1.0f - 2.0f * (vecScreen.fY - viewport.Y) / viewport.Height;
	vec.z = (vecScreen.fZ - viewport.MinZ) / (viewport.MaxZ - viewport.MinZ);

	// Transform the coord
	D3DXVec3TransformCoord( &out, &vec, &invWorldViewProjectionMatrix );

	vecWorld->fX = out.x;
	vecWorld->fY = out.y;
	vecWorld->fZ = out.z;
}

struct Vertex2D
{
	float x, y, z, rhw;
	DWORD color;
};

void CGraphics::DrawBox( float fLeft, float fTop, float fWidth, float fHeight, DWORD dwColorBox )
{
	const Vertex2D rect[] = {
		{ fLeft,			fTop,			0.0f, 1.0f,	dwColorBox },
		{ fLeft + fWidth,	fTop,			0.0f, 1.0f,	dwColorBox },
		{ fLeft,			fTop + fHeight, 0.0f, 1.0f,	dwColorBox },
		{ fLeft + fWidth,	fTop + fHeight, 0.0f, 1.0f,	dwColorBox },
	};

	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->SetPixelShader(NULL);
	m_pDevice->SetVertexShader(NULL);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetTexture(0, NULL);

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &rect, sizeof(Vertex2D));
}

void CGraphics::DrawLine( float fLeft, float fTop, float fRight, float fBottom, DWORD dwColour )
{
	const Vertex2D line[] = {
		{ fLeft,	   fTop,	0.0f, 1.0f,	dwColour },
		{ fRight,	fBottom,	0.0f, 1.0f,	dwColour },
	};

	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->SetPixelShader(NULL);
	m_pDevice->SetVertexShader(NULL);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pDevice->SetTexture(0, NULL);

	m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &line, sizeof(Vertex2D));
}

IDirect3DTexture9 * CGraphics::CreateTexture( DWORD * dwBitMap, unsigned int uiWidth, unsigned int uiHeight )
{
	// Create the texture
	IDirect3DTexture9 * pTexture = NULL;
	D3DXCreateTexture( m_pDevice, uiWidth, uiHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture );

	// Lock the rect
	D3DLOCKED_RECT lockedRect;
	pTexture->LockRect( 0, &lockedRect, 0, 0 );

	BYTE * bTexture;
	bTexture = static_cast< BYTE* >( lockedRect.pBits );

	DWORD dwColour, dwPitch;
	dwPitch = lockedRect.Pitch;

	for( unsigned int c = 0; c < uiHeight; c++ )
	{
		for( unsigned int r = 0; r < uiWidth; r++ )
		{
			dwColour = dwBitMap[ uiWidth * c + r ];
			bTexture[ r * 4 + c * dwPitch + 0 ] = static_cast< BYTE >( dwColour );
			bTexture[ r * 4 + c * dwPitch + 1 ] = static_cast< BYTE >( dwColour >> 8 );
			bTexture[ r * 4 + c * dwPitch + 2 ] = static_cast< BYTE >( dwColour >> 16 );
			bTexture[ r * 4 + c * dwPitch + 3 ] = static_cast< BYTE >( dwColour >> 24 );
		}
	}

	lockedRect.pBits = bTexture;

	// Unlock the locked rect
	pTexture->UnlockRect( 0 );
	return pTexture;
}

IDirect3DTexture9 * CGraphics::LoadTexture( const char * szFile )
{
	IDirect3DTexture9 * pTexture = NULL;

	// Attempt to create the texture
	if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFile, &pTexture))) {
		CLogFile::Printf("ERROR - %s - Unable to create texture! (Error: %d)", szFile, GetLastError());
	}

	return pTexture;
}

IDirect3DTexture9 * CGraphics::LoadTexture( const char * szFile, unsigned int uiWidth, unsigned int uiHeight )
{
	IDirect3DTexture9 * pTexture = NULL;

	// Attempt to create the texture
	if ( FAILED ( D3DXCreateTextureFromFileEx( m_pDevice, szFile, uiWidth, uiHeight, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &pTexture ) ) )
		CLogFile::Printf ( "ERROR - %s - Unable to create texture! (Error: %d)", szFile, GetLastError() );

	return pTexture;
}

void CGraphics::DrawTexture( IDirect3DTexture9 * pTexture, float fLeft, float fTop, float fScaleX, float fScaleY, float fRotation, float fCenterX, float fCenterY, unsigned char ucAlpha )
{
	// Is the texture invalid?
	if ( !pTexture )
		return;

	// Begin the sprite
	m_pSprite->Begin ( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	// Get the texture surface desc
	D3DSURFACE_DESC textureDesc;
	pTexture->GetLevelDesc ( 0, &textureDesc );

	// Get the texture scale
	D3DXVECTOR2 scaling ( fScaleX, fScaleY );

	// Get the rotation center
	D3DXVECTOR2 rotationCenter ( (float)textureDesc.Width * fCenterX, (float)textureDesc.Height * fCenterY );

	// Get the texture position
	D3DXVECTOR2 position ( ( fLeft - (float)textureDesc.Width * fScaleX * fCenterX ), ( fTop - (float)textureDesc.Height * fScaleY * fCenterY ) );

	// Generate the matrix
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D ( &matrix, NULL, NULL, &scaling, &rotationCenter, ((fRotation * 6.2832f) / 360.f), &position );

	// Set the sprite transform
	m_pSprite->SetTransform ( &matrix );

	// Draw the texture
	m_pSprite->Draw ( pTexture, NULL, NULL, NULL, D3DCOLOR_ARGB( ucAlpha, 255, 255, 255 ) );

	// End the sprite
	m_pSprite->End ();
}

IDirect3DTexture9 * CGraphics::CacheTexture( const char * szFile )
{
	// Find the existing texture if it's there
	std::map< std::string, sCachedTextureInfo >::iterator iter = m_cachedTextureMap.find( szFile );

	// Was the texture not found?
	if( iter == m_cachedTextureMap.end() )
	{
		// Add the texture to the cache
		m_cachedTextureMap[ szFile ] = sCachedTextureInfo ();
		iter = m_cachedTextureMap.find( szFile );

		sCachedTextureInfo& info = iter->second;
		info.pTexture = LoadTexture( szFile );
	}

	sCachedTextureInfo& info = iter->second;
	info.ulLastTimeUsed = SharedUtility::GetTime();

	return info.pTexture;
}

void CGraphics::ExpireCachedTextures( bool bErase )
{
	// 1 cached texture = 15 seconds
	// 50 cached textures = 8 seconds
	// 100 cached textures = 1 second
	long ulNumTextures = m_cachedTextureMap.size( );
	unsigned long ulMaxAgeSeconds = std::max<int>( 1, 15 - (ulNumTextures * 15 / 100) );

	std::map< std::string, sCachedTextureInfo >::iterator iter = m_cachedTextureMap.begin();
	while( iter != m_cachedTextureMap.end() )
	{
		sCachedTextureInfo& info = iter->second;
		unsigned long ulAge = (SharedUtility::GetTime() - info.ulLastTimeUsed);

		// Has enough time passed to expire the current cached texture?
		if( ulAge > ulMaxAgeSeconds * 1000 || bErase )
		{
			// Release the texture
			SAFE_RELEASE( info.pTexture );
			iter = m_cachedTextureMap.erase( iter );
		}
		else
			iter++;
	}
}

void CGraphics::BeginRender( void )
{
	m_pStateBlock->Capture ();

	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG0, D3DTA_DIFFUSE );
	m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG0, D3DTA_DIFFUSE );
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

	m_pDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
	m_pDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );

	m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	m_pDevice->SetRenderState( D3DRS_LIGHTING, 0 );
	m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	m_pDevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
}

void CGraphics::EndRender( void )
{
	m_pStateBlock->Apply();
}

unsigned int CGraphics::GetFontIndex( const char * szFont )
{
	if( !strcmp( szFont, "tahoma" ) ) return FONT_INDEX_TAHOMA;
	else if( !strcmp( szFont, "tahoma-bold" ) ) return FONT_INDEX_TAHOMA_BD;
	else if( !strcmp( szFont, "verdana" ) ) return FONT_INDEX_VERDANA;
	else if( !strcmp( szFont, "arial" ) ) return FONT_INDEX_ARIAL;

	return FONT_INDEX_TAHOMA;
}

/** "Smart" pointer for surface. */
struct IDirect3DSurface9Ptr
{
private:
	IDirect3DSurface9 *m_surface;

public:
	IDirect3DSurface9Ptr() : m_surface(nullptr)
	{
	}

	~IDirect3DSurface9Ptr()
	{
		if (m_surface) {
			m_surface->Release();
			m_surface = nullptr;
		}
	}

	IDirect3DSurface9 *operator->()
	{
		assert(m_surface);
		return m_surface;
	}

	IDirect3DSurface9 *&data()
	{
		return m_surface;
	}
};

/**
 * Copy back buffer contents into the pixels buffer.
 *
 * @param[out] pixels The unique ptr where the pixels buffer will be stored.
 * @param[out] width The width of the image in pixels.
 * @param[out] height The height of image in pixels.
 * @return @c true if image was copied successfully and can be used @c false otherwise.
 */
bool CGraphics::GetImage( std::unique_ptr<uint8_t[]> &pixels, unsigned &width, unsigned &height )
{
	IDirect3DSurface9Ptr backBuffer;
	if (FAILED(m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer.data()))) {
		return false;
	}

	D3DSURFACE_DESC desc;
	if (FAILED(backBuffer->GetDesc(&desc))) {
		return false;
	}

	IDirect3DSurface9Ptr copySurface;
	if (FAILED(m_pDevice->CreateRenderTarget(desc.Width, desc.Height, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, TRUE, &copySurface.data(), NULL))) {
		return false;
	}

	if (FAILED(m_pDevice->StretchRect(backBuffer.data(), NULL, copySurface.data(), NULL, D3DTEXF_POINT))) {
		return false;
	}

	D3DLOCKED_RECT lockedRect = { 0 };
	if (FAILED(copySurface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY | D3DLOCK_NOSYSLOCK))) {
		return false;
	}

	pixels = std::make_unique<uint8_t[]>(desc.Width * desc.Height * 4);

	if (!pixels) {
		return false;
	}

	width = desc.Width;
	height = desc.Height;

	uint32_t *const targetBuffer = (uint32_t*)pixels.get();
	uint32_t *const sourceBuffer = (uint32_t *)(lockedRect.pBits);

	for (unsigned y = 0; y < height; ++y) {
		for (unsigned x = 0; x < width; ++x) {
			// Ensure that we always copy with the alpha equal 255 so screenshot has consistent alpha channel.
			targetBuffer[x * height + y] = sourceBuffer[x * height + y] | 0xFF000000;
		}
	}

	if (FAILED(copySurface->UnlockRect())) {
		pixels.reset();
		width = height = 0;
		return false;
	}
	return true;
}
