/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIStaticImage_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"../StdInc.h"

CGUIStaticImage_Impl::CGUIStaticImage_Impl( CGUI_Impl * pGUI, CGUIElement_Impl * pParent )
{
	// Initialise
	m_pManager = pGUI;
	m_pImagesetManager = pGUI->GetImageSetManager();
	m_pImageset = NULL;
	m_pImage = NULL;
	m_pTexture = NULL;
	m_bCreatedTexture = false;

	// Get a unique name for cegui
	String strName = pGUI->GetUniqueName();

	// Create the static image control and set default settings
	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/StaticImage", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setRect( CEGUI::Relative, CEGUI::Rect( 0.0f, 0.0f, 1.0f, 1.0f ) );
	((CEGUI::StaticImage *)m_pWindow)->setBackgroundEnabled( false );
	m_pWindow->setVisible( true );

	// Store the pointer to this element
	m_pWindow->setUserData( (void *)this );

	// Register our events
	AddEvents();

	// If a parent is set, add it to it's childs list, if not add it as a child to the gui manager
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
	// Clear the image
	Clear();
	
	// Destroy the element
	DestroyElement();
}

bool CGUIStaticImage_Impl::LoadFromFile( String strFile, String strDirectory )
{
	// Modifiy the file name
	strFile = String( "%s\\%s", strDirectory.Get(), strFile.Get() );

	// Is the texture not already created?
	if( !m_pTexture )
	{
		// Create the texture
		m_pTexture = new CGUITexture_Impl( m_pManager );

		// Mark as texture loaded
		m_bCreatedTexture = true;
	}

	// Try and load the texture
	if( !m_pTexture->LoadFromFile( strFile ) )
		return false;

	// Load the image
	return LoadFromTexture( m_pTexture );
}

bool CGUIStaticImage_Impl::LoadFromTexture( CGUITexture_Impl * pTexture )
{
	// Undefine all images if needed
	if( m_pImageset && m_pImage )
		m_pImageset->undefineAllImages();

	// If the texture already exists, delete it
	if( m_pTexture && pTexture != m_pTexture )
	{
		// Delete the texture
		SAFE_DELETE( m_pTexture );

		// Mark as texture not loaded
		m_bCreatedTexture = false;
	}

	// Set the texture
	m_pTexture = pTexture;

	// Get the CEGUI texture
	CEGUI::Texture * pCEGUITexture = m_pTexture->GetTexture();

	// Get a unique name for cegui
	String strName = m_pManager->GetUniqueName();

	// Is the image set invalid?
	if( !m_pImageset )
	{
		// Find a new name if the current name is present
		while( m_pImagesetManager->isImagesetPresent( strName.Get() ) )
			strName = m_pManager->GetUniqueName();

		// Create the image set
		m_pImageset = m_pImagesetManager->createImageset( strName.Get(), pCEGUITexture );
	}

	// Get another unique name for cegui
	strName = m_pManager->GetUniqueName();

	// Define an image and get its pointer
	m_pImageset->defineImage( strName.Get(), CEGUI::Point( 0, 0 ), CEGUI::Size( pCEGUITexture->getWidth(), pCEGUITexture->getHeight() ), CEGUI::Point( 0, 0 ) );
	m_pImage = &m_pImageset->getImage( strName.Get() );

	// Set the image just loaded as the image to be drawn
	((CEGUI::StaticImage *)m_pWindow)->setImage( m_pImage );

	return true;
}

void CGUIStaticImage_Impl::Clear( void )
{
	// Stop the control using the image
	((CEGUI::StaticImage *)m_pWindow)->setImage( NULL );

	// Is the imageset valid?
	if( m_pImageset )
	{
		// Destroy the imageset
		m_pImageset->undefineAllImages();
		m_pImagesetManager->destroyImageset( m_pImageset );

		// Do we have a loaded texture?
		if( m_bCreatedTexture )
		{
			// Destroy the texture
			SAFE_DELETE( m_pTexture );

			// Mark as texture not loaded
			m_bCreatedTexture = false;
		}

		// Reset the imageset pointer
		m_pImageset = NULL;

		// Reset the image pointer
		m_pImage = NULL;
	}
}

CEGUI::Image * CGUIStaticImage_Impl::GetImage( void )
{
	return const_cast< CEGUI::Image* >( ((CEGUI::StaticImage *)m_pWindow)->getImage() );
}

void CGUIStaticImage_Impl::Render( void )
{
	return ((CEGUI::StaticImage *)m_pWindow)->render();
}