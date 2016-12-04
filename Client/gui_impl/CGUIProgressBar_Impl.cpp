/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIProgressBar_Impl.cpp
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
#include "CGUIProgressBar_Impl.h"

CGUIProgressBar_Impl::CGUIProgressBar_Impl( CGUI_Impl * pGUI, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	String strName = pGUI->GetUniqueName();

	m_pWindow = pGUI->GetWindowManager()->createWindow( "CGUI/ProgressBar", strName.Get() );
	m_pWindow->setDestroyedByParent( false );

	m_pWindow->setUserData( reinterpret_cast<void *>(this) );

	AddEvents();

	SetProgress( 0 );

	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUIProgressBar_Impl::~CGUIProgressBar_Impl( void )
{
	DestroyElement();
}

void CGUIProgressBar_Impl::SetProgress( float fProgress )
{
	reinterpret_cast<CEGUI::ProgressBar *>(m_pWindow)->setProgress( fProgress );
}

float CGUIProgressBar_Impl::GetProgress( void )
{
	return reinterpret_cast<CEGUI::ProgressBar *>(m_pWindow)->getProgress();
}