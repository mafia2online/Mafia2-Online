/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUICheckBox_Impl.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CString.h"
#include "Math/CVector3.h"

#include "CGUI.h"

#include "CGUI_Impl.h"
#include "CGUIElement_Impl.h"
#include "CGUICheckBox_Impl.h"

CGUICheckBox_Impl::CGUICheckBox_Impl( CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/Checkbox", strName.Get() );
	m_pWindow->setDestroyedByParent( false );
	SetText(strCaption);
	m_pWindow->setVisible( true );

	m_pWindow->setUserData( reinterpret_cast<void *>(this) );

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

CGUICheckBox_Impl::~CGUICheckBox_Impl( void )
{
	DestroyElement();
}

void CGUICheckBox_Impl::SetChecked( bool bChecked )
{
	reinterpret_cast<CEGUI::Checkbox *>(m_pWindow)->setSelected(!bChecked);
}

bool CGUICheckBox_Impl::IsChecked( void )
{
	return !(reinterpret_cast<CEGUI::Checkbox *>(m_pWindow)->isSelected());
}