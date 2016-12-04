/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIRadioButton_Impl.cpp
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
#include "CGUIRadioButton_Impl.h"

CGUIRadioButton_Impl::CGUIRadioButton_Impl( CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{

	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/RadioButton", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	SetText(strCaption);
	m_pWindow->setVisible( true );
	m_pWindow->setUserData( (void *)this );

	SetSize( Vector2( 128, 16 ) );
	SetChecked( false );

	AddEvents();
	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUIRadioButton_Impl::~CGUIRadioButton_Impl( void )
{
	DestroyElement();
}

void CGUIRadioButton_Impl::SetChecked( bool bChecked )
{
	reinterpret_cast<CEGUI::RadioButton *>(m_pWindow)->setSelected( bChecked );
}

bool CGUIRadioButton_Impl::IsChecked( void )
{
	return reinterpret_cast<CEGUI::RadioButton *>(m_pWindow)->isSelected();
}