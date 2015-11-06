/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUITexture_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUITexture_Impl::CGUITexture_Impl( CGUI_Impl * pGUI )
{
	// Save the renderer
	m_pRenderer = pGUI->GetRenderer();

	// Create the texture
	m_pTexture = m_pRenderer->createTexture();
}

CGUITexture_Impl::~CGUITexture_Impl( void )
{
	// Destroy the texture
	if( m_pTexture )
		m_pRenderer->destroyTexture( m_pTexture );
}

bool CGUITexture_Impl::LoadFromFile(String strFile)
{
	// Does the file not exist?
	if( !SharedUtility::Exists( strFile.Get() ) )
		return false;

	// Try and load the file
	try
	{
		m_pTexture->loadFromFile(strFile.Get(), "");
	}
	catch( CEGUI::Exception )
	{
		return false;
	}

	return true;
}

void CGUITexture_Impl::LoadFromMemory( const void * pBuffer, unsigned int uiWidth, unsigned int uiHeight )
{
	// Load the texture from memory
	m_pTexture->loadFromMemory( pBuffer, uiWidth, uiHeight );
}

void CGUITexture_Impl::Clear( void )
{
	// Destroy the texture
	m_pRenderer->destroyTexture( m_pTexture );

	// Create the texture
	m_pTexture = m_pRenderer->createTexture();
}

IDirect3DTexture9 * CGUITexture_Impl::GetD3DTexture( void )
{
	return ((CEGUI::DirectX9Texture *)m_pTexture)->getD3DTexture();
}

void CGUITexture_Impl::CreateTexture( unsigned int uiWidth, unsigned int uiHeight )
{
	return ((CEGUI::DirectX9Texture *)m_pTexture)->createRenderTarget( uiWidth, uiHeight );
}