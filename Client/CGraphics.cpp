/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGraphics.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"
#include	<iostream>
#include	<algorithm>

extern	CCore			* pCore;

const unsigned char g_szPixel [] = { 0x42, 0x4D, 0x3A, 0, 0, 0, 0, 0, 0, 0, 0x36, 0, 0, 0, 0x28, 0, 0,
                                    0, 0x1, 0, 0, 0, 0x1, 0, 0, 0, 0x1, 0, 0x18, 0, 0, 0, 0, 0,
                                    0x4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0xFF, 0xFF, 0xFF, 0 };

CGraphics::CGraphics( void )
{
	// Invalidate the device
	m_pDevice = NULL;

	// Invalidate the sprite
	m_pSprite = NULL;

	// Invalidate stuff
	m_pOriginalTarget = NULL;
	m_pPixelTexture = NULL;

	// Install the direct x hook
	CDirect3D9Hook::Install ();

	// Install the direct input 8 hook
	CDirectInput8Hook::Install ();
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
			CLogFile::Printf( "CGraphics::LoadFonts() - Failed to load default fonts." );
			TerminateProcess( GetCurrentProcess(), 0 );
		}

		// Get the original render target
		m_pDevice->GetRenderTarget( 0, &m_pOriginalTarget );

		// Create drawing devices
		D3DXCreateTextureFromFileInMemory( pDevice,  g_szPixel, sizeof( g_szPixel ), &m_pPixelTexture );
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
		m_pFont->DrawText( m_pSprite, szText, -1, &shadow_rect, ulFormat, D3DCOLOR_ARGB( 255, 0, 0, 0 ) );
	}

	// Draw the text
	m_pFont->DrawText( m_pSprite, szText, -1, &rect, ulFormat, ulColor );

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
	pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecCurrentPosition );

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
	pCore->GetPlayerManager()->GetLocalPlayer()->GetPosition( &vecCurrentPosition );

	// Are we too far away?
	if( (vecCurrentPosition - vecPosition).Length() > fDistance )
		return;

	// Draw the box
	DrawBox( (vecScreen.fX - (fWidth / 2)), (vecScreen.fY - (fHeight / 2)), fWidth, fHeight, dwBoxColour );
}

void CGraphics::DrawLine( float fLeft, float fTop, float fRight, float fBottom, float fWidth, DWORD dwColour )
{
	// TODO
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

	// Release the pixel texture
	m_pPixelTexture->Release ();

	// Release the original surface
	m_pOriginalTarget->Release ();
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

	// Create drawing devices
	D3DXCreateTextureFromFileInMemory ( pDevice, g_szPixel, sizeof( g_szPixel ), &m_pPixelTexture );
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
	pCore->GetCamera()->GetWorldViewProjection( &worldViewProjection );

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
	pCore->GetCamera()->GetWorldViewProjection( &worldViewProjection );

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

void CGraphics::DrawBox( float fLeft, float fTop, float fWidth, float fHeight, DWORD dwColorBox )
{
	// Begin the sprite
	m_pSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	// Generate the matrix
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D( &matrix, NULL, 0.0f, &D3DXVECTOR2( fWidth, fHeight ), NULL, 0.0f, &D3DXVECTOR2( fLeft, fTop ) );

	// Set the sprite transform
	m_pSprite->SetTransform( &matrix );

	// Draw the box
	m_pSprite->Draw( m_pPixelTexture, NULL, NULL, NULL, dwColorBox );

	// End the sprite
	m_pSprite->End();
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
	if ( FAILED ( D3DXCreateTextureFromFile( m_pDevice, szFile, &pTexture ) ) )
		CLogFile::Printf ( "ERROR - Graphics::LoadTexture ( %s ) - Unable to create texture! (Error: %d)", szFile, GetLastError() );

	return pTexture;
}

IDirect3DTexture9 * CGraphics::LoadTexture( const char * szFile, unsigned int uiWidth, unsigned int uiHeight )
{
	IDirect3DTexture9 * pTexture = NULL;

	// Attempt to create the texture
	if ( FAILED ( D3DXCreateTextureFromFileEx( m_pDevice, szFile, uiWidth, uiHeight, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &pTexture ) ) )
		CLogFile::Printf ( "ERROR - Graphics::LoadTexture ( %s ) - Unable to create texture! (Error: %d)", szFile, GetLastError() );

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

void CGraphics::OnSceneBegin( void )
{
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

unsigned int CGraphics::GetFontIndex( const char * szFont )
{
	if( !strcmp( szFont, "tahoma" ) ) return FONT_INDEX_TAHOMA;
	else if( !strcmp( szFont, "tahoma-bold" ) ) return FONT_INDEX_TAHOMA_BD;
	else if( !strcmp( szFont, "verdana" ) ) return FONT_INDEX_VERDANA;
	else if( !strcmp( szFont, "arial" ) ) return FONT_INDEX_ARIAL;

	return FONT_INDEX_TAHOMA;
}

bool CGraphics::GetFrontBufferPixels( unsigned char ** ucData )
{
	//
	if( !pCore->GetCamera() )
		return false;

	// Don't we have a valid data pointer?
	if( !*ucData )
		return false;

	// Get the screen display mode
	D3DDISPLAYMODE displayMode;
	if( FAILED( m_pDevice->GetDisplayMode( 0, &displayMode ) ) )
		return false;

	// Create an offscreen plain surface
	IDirect3DSurface9 * pReadSurface = NULL;
	if( FAILED( m_pDevice->CreateOffscreenPlainSurface( displayMode.Width, displayMode.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pReadSurface, NULL ) ) )
	{
		return false;
	}

	// Copy the front buffer data into the read surface
	if( FAILED( m_pDevice->GetFrontBufferData( 0, pReadSurface ) ) )
	{
		// Release the read surface
		SAFE_RELEASE( pReadSurface );

		return false;
	}

	//
	unsigned int uiWindowWidth = pCore->GetCamera()->GetWindowWidth();
	unsigned int uiWindowHeight = pCore->GetCamera()->GetWindowHeight();

	// Build the client rect
	RECT clientRect;
	{
		POINT clientPoint;
		clientPoint.x = 0;
		clientPoint.y = 0;
		ClientToScreen( IE::GetWindow()->m_hWnd, &clientPoint );
		clientRect.left = clientPoint.x;
		clientRect.top = clientPoint.y;
		clientRect.right = (clientRect.left + uiWindowWidth);
		clientRect.bottom = (clientRect.top + uiWindowHeight);
	}

	// Get pixels from the read surface
	D3DLOCKED_RECT lockedRect;
	if( FAILED( pReadSurface->LockRect( &lockedRect, &clientRect, D3DLOCK_READONLY | D3DLOCK_NOSYSLOCK ) ) )
	{
		// Release the read surface
		SAFE_RELEASE( pReadSurface );

		return false;
	}

	//
	unsigned int * dP = (unsigned int *)*ucData;
	unsigned char * sP = (unsigned char *)lockedRect.pBits;

	// Is the monitor running on 16 bit colours?
	if( (displayMode.Format == D3DFMT_X8R8G8B8) && (0xFF000000 != (*dP & 0xFF000000)) )
	{
		// Copy the data
		for( unsigned int y = 0; y < uiWindowHeight; y++ )
		{
			for( unsigned int x = 0; x < uiWindowWidth; x++ )
			{
				*dP = *((unsigned int *)sP) | 0xFF000000;
				dP++;
				sP += 4;
			}

			sP += (lockedRect.Pitch - (4 *  uiWindowWidth));
		}
	}
	else
	{
		// Copy the data
		for( unsigned int y = 0; y < uiWindowHeight; y++ )
		{
			memcpy( dP, sP, ( uiWindowWidth * 4) );
			sP += lockedRect.Pitch;
			dP += uiWindowWidth;
		}
	}

	// Unlock the rect
	pReadSurface->UnlockRect();

	// Release the read surface
	SAFE_RELEASE( pReadSurface );

	return true;
}