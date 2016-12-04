/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIStaticImage_Impl.cpp
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

#include "CGUI_Impl.h"
#include "CGUIWindow_Impl.h"
#include "CGUIStaticImage_Impl.h"

CGUIStaticImage_Impl::CGUIStaticImage_Impl( CGUI_Impl * pGUI, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	m_pImagesetManager = pGUI->GetImageSetManager();
	m_pImageset = NULL;
	m_pImage = NULL;
	m_pTexture = NULL;
	m_bCreatedTexture = false;

	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/StaticImage", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setRect( CEGUI::Relative, CEGUI::Rect( 0.0f, 0.0f, 1.0f, 1.0f ) );
	reinterpret_cast<CEGUI::StaticImage *>(m_pWindow)->setBackgroundEnabled( false );
	m_pWindow->setVisible( true );
	m_pWindow->setUserData( reinterpret_cast<void *>(this) );

	AddEvents();
	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUIStaticImage_Impl::~CGUIStaticImage_Impl( void )
{
	Clear();
	DestroyElement();
}

bool CGUIStaticImage_Impl::LoadFromFile( String strFile, String strDirectory )
{
	strFile.Format( "%s\\%s", strDirectory.Get(), strFile.Get() );

	if( !m_pTexture )
	{
		m_pTexture = new CGUITexture_Impl( m_pManager );
		m_bCreatedTexture = true;
	}
	if( !m_pTexture->LoadFromFile( strFile ) )
		return false;

	return LoadFromTexture( m_pTexture );
}

bool CGUIStaticImage_Impl::LoadFromTexture( CGUITexture_Impl * pTexture )
{
	if( m_pImageset && m_pImage )
		m_pImageset->undefineAllImages();

	if( m_pTexture && pTexture != m_pTexture )
	{
		SAFE_DELETE( m_pTexture );
		m_bCreatedTexture = false;
	}

	m_pTexture = reinterpret_cast<CGUITexture_Impl *>(pTexture);

	CEGUI::Texture * pCEGUITexture = m_pTexture->GetTexture();

	String strName = m_pManager->GetUniqueName();
	if( !m_pImageset )
	{
		while( m_pImagesetManager->isImagesetPresent( strName.Get() ) )
			strName = m_pManager->GetUniqueName();

		m_pImageset = m_pImagesetManager->createImageset( strName.Get(), pCEGUITexture );
	}
	strName = m_pManager->GetUniqueName();

	m_pImageset->defineImage( strName.Get(), CEGUI::Point( 0, 0 ), CEGUI::Size( pCEGUITexture->getWidth(), pCEGUITexture->getHeight() ), CEGUI::Point( 0, 0 ) );
	m_pImage = &m_pImageset->getImage( strName.Get() );

	reinterpret_cast<CEGUI::StaticImage *>(m_pWindow)->setImage( m_pImage );

	return true;
}

void CGUIStaticImage_Impl::Clear( void )
{
	reinterpret_cast<CEGUI::StaticImage *>(m_pWindow)->setImage( NULL );

	if( m_pImageset )
	{
		m_pImageset->undefineAllImages();
		m_pImagesetManager->destroyImageset( m_pImageset );
		if( m_bCreatedTexture )
		{
			SAFE_DELETE( m_pTexture );
			m_bCreatedTexture = false;
		}
		m_pImageset = NULL;
		m_pImage = NULL;
	}
}

CEGUI::Image * CGUIStaticImage_Impl::GetImage( void )
{
	return const_cast<CEGUI::Image*>( reinterpret_cast<CEGUI::StaticImage *>(m_pWindow)->getImage() );
}

void CGUIStaticImage_Impl::Render( void )
{
	return reinterpret_cast<CEGUI::StaticImage *>(m_pWindow)->render();
}