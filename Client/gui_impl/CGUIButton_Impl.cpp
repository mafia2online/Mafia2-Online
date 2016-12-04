/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIButton_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CString.h"

#include "CGUI.h"
#include "CGUICallback.h"

#include "CGUI_Impl.h"
#include "CGUIElement_Impl.h"
#include "CGUIButton_Impl.h"

CGUIButton_Impl::CGUIButton_Impl( CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/Button", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	SetText(strCaption);
	m_pWindow->setSize( CEGUI::Absolute, CEGUI::Size( 128.0f, 24.0f ) );
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

CGUIButton_Impl::~CGUIButton_Impl( void )
{
	DestroyElement();
}