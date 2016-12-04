/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUITexture_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include "CCore.h"

#include "CString.h"
#include "Math/CVector3.h"

#include "CGUI.h"

#include "CClientScriptGUIManager.h"
#include "CClientScriptingManager.h"

#include "SharedUtility.h"

#include "CGUI_Impl.h"
#include "CGUIWindow_Impl.h"
#include "CGUITexture_Impl.h"

CGUITexture_Impl::CGUITexture_Impl( CGUI_Impl * pGUI )
{
	m_pRenderer = pGUI->GetRenderer();
	m_pTexture = m_pRenderer->createTexture();
}

CGUITexture_Impl::~CGUITexture_Impl( void )
{
	if( m_pTexture )
		m_pRenderer->destroyTexture( m_pTexture );
}

bool CGUITexture_Impl::LoadFromFile(String strFile)
{
	if( !SharedUtility::Exists( strFile.Get() ) )
		return false;

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
	m_pTexture->loadFromMemory( pBuffer, uiWidth, uiHeight );
}

void CGUITexture_Impl::Clear( void )
{
	m_pRenderer->destroyTexture( m_pTexture );
	m_pTexture = m_pRenderer->createTexture();
}

IDirect3DTexture9 * CGUITexture_Impl::GetD3DTexture( void )
{
	return reinterpret_cast<CEGUI::DirectX9Texture *>(m_pTexture)->getD3DTexture();
}

void CGUITexture_Impl::CreateTexture( unsigned int uiWidth, unsigned int uiHeight )
{
	return reinterpret_cast<CEGUI::DirectX9Texture *>(m_pTexture)->createRenderTarget( uiWidth, uiHeight );
}