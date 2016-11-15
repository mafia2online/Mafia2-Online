/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGraphics.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <list>
#include <map>
#include <memory>

#include "d3d9.h"
#include "d3dx9.h"

// Font indexes
#define		FONT_INDEX_TAHOMA		0
#define		FONT_INDEX_TAHOMA_BD	1
#define		FONT_INDEX_VERDANA		2
#define		FONT_INDEX_ARIAL		3
#define		NUM_FONTS				4

// Undefine WinAPI DrawText macro.
#ifdef DrawText
#	undef DrawText
#endif

class CFont;
class CVector3;

class CGraphics
{

private:

	LPD3DXSPRITE					m_pSprite;
	IDirect3DDevice9				* m_pDevice;
	IDirect3DSurface9				* m_pOriginalTarget;
	IDirect3DStateBlock9			* m_pStateBlock;

	//Graphic card detection
	IDirect3D9						*m_pDirect;
	D3DADAPTER_IDENTIFIER9			m_pD3Identifier;

	std::list< CFont* >				m_fonts;
	std::list< CFont* >				m_bigFonts;
	std::list< CFont* >				m_customFonts;

	struct sCachedTextureInfo
	{
		IDirect3DTexture9			* pTexture;
		unsigned long				ulLastTimeUsed;
	};

	std::map< std::string, sCachedTextureInfo > m_cachedTextureMap;

	void							CreateStateBlock(void);
public:

	CGraphics( );
	~CGraphics( );

	void							Initialise( IDirect3DDevice9 * pDevice );
	bool							LoadFonts( void );
	bool							LoadFont( String strName, unsigned int uiSize, bool bBold, std::list< CFont* > * list );
	bool							LoadFont( String strName, unsigned int uiSize, bool bBold );

	// Drawing
	void							DrawText( float uiLeft, float uiTop, float uiRight, float uiBottom, unsigned long ulColor, const char * szText, float fScaleX, float fScaleY, unsigned long ulFormat, String strFont, bool bShadow );
	void							DrawText( float uiLeft, float uiTop, unsigned long ulColor, const char * szText, float fScale, String strFont, bool bShadow );
	void							DrawText( float uiLeft, float uiTop, unsigned long ulColor, float fScale, String strFont, bool bShadow, const char * szText, ... );
	void							DrawText( float uiLeft, float uiTop, unsigned long ulColor, float fScale, String strFont, bool bShadow, unsigned long ulFormat, const char * szText, ... );
	void							DrawBox( float fLeft, float fTop, float fWidth, float fHeight, DWORD dwColorBox );
	void							DrawLine( float fLeft, float fTop, float fRight, float fBottom, DWORD dwColour );

	// Drawing 3d
	void							DrawText( CVector3 vecPosition, float fDistance, unsigned long ulColor, float fScale, String strFont, bool bShadow, String strText );
	void							DrawBox( CVector3 vecPosition, float fWidth, float fHeight, float fDistance, DWORD dwBoxColour );

	// Textures
	IDirect3DTexture9				* CreateTexture( DWORD * dwBitMap, unsigned int uiWidth, unsigned int uiHeight );
	IDirect3DTexture9				* LoadTexture( const char * szFile );
	IDirect3DTexture9				* LoadTexture( const char * szFile, unsigned int uiWidth, unsigned int uiHeight );
	void							DrawTexture( IDirect3DTexture9 * pTexture, float fLeft, float fTop, float fScaleX, float fScaleY, float fRotation, float fCenterX, float fCenterY, unsigned char ucAlpha );

	// Texture caching
	IDirect3DTexture9				* CacheTexture( const char * szFile );
	void							ExpireCachedTextures( bool bErase = false );

	// Device
	void							OnLostDevice( IDirect3DDevice9 * pDevice );
	void							OnRestoreDevice( IDirect3DDevice9 * pDevice );

	ID3DXFont						* GetFont( String strFont );
	ID3DXFont						* GetBigFont( String strFont );

	float							GetTextWidth( const char * szText, float fScale, String strFont );
	float							GetFontHeight( float fScale, String strFont );
	float							GetCharacterWidth( char c, float fScale, String strFont );

	void							WorldToScreen( CVector3 vecWorld, CVector3 * vecScreen );
	void							ScreenToWorld( CVector3 vecScreen, CVector3 * vecWorld );

	void							BeginRender( void );
	void							EndRender ( void );

	unsigned int					GetFontIndex( const char * szFont );

	IDirect3DDevice9				* GetDevice( void ) { return m_pDevice; }

	// AMD Detection
	bool							IsUsingAMD( void );
	bool							IsUsingNVIDIA(void);
	bool							IsUsingChipset( void );

	bool							GetImage(std::unique_ptr<uint8_t[]> &pixels, unsigned &width, unsigned &height);

};
