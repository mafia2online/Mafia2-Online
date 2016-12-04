/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUITab_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include "CCore.h"

#include "CString.h"
#include "Math/CVector3.h"

#include "CGUITypes.h"

#include "CGUI.h"

#include "CClientScriptGUIManager.h"
#include "CClientScriptingManager.h"

#include "CGUI_Impl.h"
#include "CGUIElement_Impl.h"
#include "CGUIWindow_Impl.h"
#include "CGUITab_Impl.h"
#include "CGUITabPanel_Impl.h"

CGUITab_Impl::CGUITab_Impl( CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	String strName = pGUI->GetUniqueName();
	m_pWindow = pGUI->GetWindowManager()->createWindow( "DefaultWindow", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	SetText(strCaption);
	m_pWindow->setUserData( reinterpret_cast<void *>(this) );

	AddEvents();
	if( pParent )
	{
		SetParent( pParent );
		reinterpret_cast<CEGUI::TabControl *>(reinterpret_cast<CGUITabPanel_Impl *>(pParent)->GetWindow())->setAbsoluteTabHeight( 24.0f );
	}
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUITab_Impl::~CGUITab_Impl( void )
{
	DestroyElement();
}

void CGUITab_Impl::SetCaption( String strText )
{
	m_pWindow->setText( strText.Get() );
}

void CGUITab_Impl::SetVisible( bool bVisible )
{
	CEGUI::TabControl * pTabControl = reinterpret_cast < CEGUI::TabControl* > ( reinterpret_cast<CGUITabPanel_Impl *>(m_pParent)->GetWindow() );
	pTabControl->getButtonForTabContents( m_pWindow )->setVisible ( bVisible );
}

bool CGUITab_Impl::IsVisible( void )
{
	CEGUI::TabControl * pTabControl = reinterpret_cast < CEGUI::TabControl* > ( reinterpret_cast<CGUITabPanel_Impl *>(m_pParent)->GetWindow() );
	return pTabControl->getButtonForTabContents( m_pWindow )->isVisible ();
}

void CGUITab_Impl::SetEnabled( bool bEnabled )
{
	CEGUI::TabControl * pTabControl = reinterpret_cast < CEGUI::TabControl* > ( reinterpret_cast<CGUITabPanel_Impl *>(m_pParent)->GetWindow() );
	pTabControl->getButtonForTabContents( m_pWindow )->setEnabled ( bEnabled );
}

bool CGUITab_Impl::IsEnabled( void )
{
	CEGUI::TabControl * pTabControl = reinterpret_cast < CEGUI::TabControl* > ( reinterpret_cast<CGUITabPanel_Impl *>(m_pParent)->GetWindow() );
	return !pTabControl->getButtonForTabContents( m_pWindow )->isDisabled ();
}