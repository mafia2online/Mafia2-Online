/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIEdit_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"
#include "CCore.h"

#include "CString.h"
#include "Math/CVector3.h"

#include "CGUI.h"
#include "CGUICallback.h"

#include "CClientScriptingManager.h"
#include "CClientScriptGUIManager.h"

#include "CGUI_Impl.h"
#include "CGUIElement_Impl.h"

#include "CGUIEdit_Impl.h"

CGUIEdit_Impl::CGUIEdit_Impl( CGUI_Impl * pGUI, String strText, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/Editbox", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	m_pWindow->setVisible( true );
	m_pWindow->setUserData( (void *)this );
	m_pWindow->setRect(CEGUI::Absolute, CEGUI::Rect(0.00f, 0.00f, 0.128f, 0.24f));
	SetText(strText);

	m_pWindow->subscribeEvent( CEGUI::Editbox::EventKeyDown, CEGUI::Event::Subscriber( &CGUIEdit_Impl::Event_OnKeyDown, this ) );
	m_pWindow->subscribeEvent( CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber( &CGUIEdit_Impl::Event_OnTextChanged, this ) );
	
	AddEvents();
	if( pParent )
	{
		SetParent( pParent );
	}
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUIEdit_Impl::~CGUIEdit_Impl( void )
{
	DestroyElement();
}

void CGUIEdit_Impl::SetReadOnly( bool bReadOnly )
{
	reinterpret_cast<CEGUI::Editbox*>(m_pWindow)->setReadOnly(bReadOnly);
}

bool CGUIEdit_Impl::IsReadOnly( void )
{
	return reinterpret_cast<CEGUI::Editbox*>(m_pWindow)->isReadOnly();
}

void CGUIEdit_Impl::SetMasked( bool bMasked )
{
	reinterpret_cast<CEGUI::Editbox*>(m_pWindow)->setTextMasked(bMasked);
}

bool CGUIEdit_Impl::IsMasked( void )
{
	return reinterpret_cast<CEGUI::Editbox*>(m_pWindow)->isTextMasked();
}

void CGUIEdit_Impl::SetMaxLength( unsigned int uiMaxLength )
{
	reinterpret_cast<CEGUI::Editbox*>(m_pWindow)->setMaxTextLength(uiMaxLength);
}

unsigned int CGUIEdit_Impl::GetMaxLength( void )
{
	return static_cast<unsigned int>(reinterpret_cast<CEGUI::Editbox*>(m_pWindow)->getMaxTextLength());
}

void CGUIEdit_Impl::SetSelection( unsigned int uiStart, unsigned int uiEnd )
{
	reinterpret_cast<CEGUI::Editbox*>(m_pWindow)->setSelection(uiStart, uiEnd);
}

unsigned int CGUIEdit_Impl::GetSelectionStart(void)
{
	return static_cast<unsigned int>(reinterpret_cast<CEGUI::Editbox*>(m_pWindow)->getSelectionStartIndex());
}

unsigned int CGUIEdit_Impl::GetSelectionEnd(void)
{
	return static_cast<unsigned int>(reinterpret_cast<CEGUI::Editbox*>(m_pWindow)->getSelectionEndIndex());
}

unsigned int CGUIEdit_Impl::GetSelectionLength(void)
{
	return static_cast<unsigned int>(reinterpret_cast<CEGUI::Editbox*>(m_pWindow)->getSelectionLength());
}

bool CGUIEdit_Impl::ActivateOnTab( void )
{
	if( IsVisible() && !IsReadOnly() )
	{
		Activate();
		((CEGUI::Editbox *)GetWindow())->setCaratIndex( GetText().GetLength() );

		return true;
	}
	return false;
}

bool CGUIEdit_Impl::Event_OnTextChanged( const CEGUI::EventArgs &e )
{
	if( m_pfnOnTextChanged )
		m_pfnOnTextChanged(reinterpret_cast<CGUIElement_Impl*>(this));

	if( CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager() )
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementTextChange", this );

	return true;
}

bool CGUIEdit_Impl::Event_OnKeyDown( const CEGUI::EventArgs &e )
{
	const CEGUI::KeyEventArgs& keyEvent = reinterpret_cast<const CEGUI::KeyEventArgs&>(e);

	if( keyEvent.scancode == CEGUI::Key::Return || keyEvent.scancode == CEGUI::Key::NumpadEnter )
	{
		if( m_pfnOnTextAccepted )
			m_pfnOnTextAccepted(reinterpret_cast<CGUIElement_Impl*>(this));

		if( CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager() )
			CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementTextAccept", this );
	}
	return true;
}