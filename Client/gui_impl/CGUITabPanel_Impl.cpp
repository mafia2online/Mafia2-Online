/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUITabPanel_Impl.cpp
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
#include "CGUITabPanel_Impl.h"

CGUITabPanel_Impl::CGUITabPanel_Impl( CGUI_Impl * pGUI, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/TabControl", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	reinterpret_cast<CEGUI::TabControl *>(m_pWindow)->setAbsoluteTabTextPadding( 10.0f );
	m_pWindow->setVisible( true );
	m_pWindow->setUserData( reinterpret_cast<void *>(this) );
	m_pWindow->subscribeEvent( CEGUI::TabControl::EventSelectionChanged, CEGUI::Event::Subscriber( &CGUITabPanel_Impl::Event_OnSelectionChange, this ) );

	AddEvents();
	SetSize( Vector2( 300, 150 ) );
	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUITabPanel_Impl::~CGUITabPanel_Impl( void )
{
	DestroyElement();
}

std::shared_ptr<CGUITab_Impl> CGUITabPanel_Impl::CreateTab( String strCaption )
{
	return std::make_shared<CGUITab_Impl>( m_pManager, strCaption, this );
}

void CGUITabPanel_Impl::DeleteTab( CGUITab_Impl * pTab )
{
	reinterpret_cast<CEGUI::TabControl *>(m_pWindow)->removeTab( pTab->GetWindow()->getName() );
}

void CGUITabPanel_Impl::SetSelectedTab( CGUITab_Impl * pTab )
{
	try
	{
		reinterpret_cast<CEGUI::TabControl *>(m_pWindow)->setSelectedTab( pTab->GetWindow()->getName() );
	}
	catch ( CEGUI::Exception ) {}
}

CGUITab_Impl * CGUITabPanel_Impl::GetSelectedTab( void )
{
	CEGUI::TabControl * pTabControl = (CEGUI::TabControl *)m_pWindow;

	if ( pTabControl->getTabCount() > 0 )
	{
		try
		{
			CEGUI::Window * pTabWindow = pTabControl->getTabContentsAtIndex( pTabControl->getSelectedTabIndex() );
			return reinterpret_cast<CGUITab_Impl *>(pTabWindow->getUserData());
		}
		catch ( CEGUI::Exception )
		{
			return NULL;
		}
	}

	return NULL;
}

bool CGUITabPanel_Impl::IsTabSelected( CGUITab_Impl * pTab )
{
	return reinterpret_cast<CEGUI::TabControl *>(m_pWindow)->isTabContentsSelected( pTab->GetWindow() );
}

bool CGUITabPanel_Impl::Event_OnSelectionChange( const CEGUI::EventArgs &e )
{
	if( m_pfnHandler )
		m_pfnHandler( reinterpret_cast<CGUIElement_Impl *>(GetSelectedTab()) );

	if( CCore::Instance()->GetClientScriptingManager() && CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager() )
		CCore::Instance()->GetClientScriptingManager()->GetScriptGUIManager()->HandleEvent( "onGuiElementTabChange", GetSelectedTab() );

	return true;
}