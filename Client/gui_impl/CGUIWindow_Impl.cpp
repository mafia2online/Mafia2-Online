/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIWindow_Impl.cpp
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

CGUIWindow_Impl::CGUIWindow_Impl( CGUI_Impl * pGUI, String strCaption, CGUIElement_Impl * pParent )
	: CGUIElement_Impl(pGUI)
{
	String strName = pGUI->GetUniqueName();

	if (!m_pWindow)
	{
		m_pWindow = pGUI->GetWindowManager()->createWindow("CGUI/FrameWindow", strName.Get());
		m_pWindow->setRect(CEGUI::Relative, CEGUI::Rect(0.10f, 0.10f, 0.60f, 0.90f));
		m_pWindow->setAlpha(0.8f);
		reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->setRollupEnabled(false);

		SetText(strCaption);
	}
	m_pWindow->setDestroyedByParent(false);
	m_pWindow->setUserData(reinterpret_cast<void *>(this));
	m_pWindow->setMetricsMode(CEGUI::Absolute);
	m_pWindow->setMinimumSize(CEGUI::Size(96.0f, 48.0f));

	AddEvents();
	if( pParent )
		SetParent( pParent );
	else
	{
		pGUI->AddChild( this );
		SetParent( NULL );
	}
}

CGUIWindow_Impl::~CGUIWindow_Impl( void )
{
	DestroyElement();
}

void CGUIWindow_Impl::SetMovable( bool bMovable )
{
	reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->setDragMovingEnabled( bMovable );
}

bool CGUIWindow_Impl::IsMovable( void )
{
	return reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->isDragMovingEnabled();
}

void CGUIWindow_Impl::SetSizingEnabled( bool bSizingEnabled )
{
	reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->setSizingEnabled( bSizingEnabled );
}

bool CGUIWindow_Impl::IsSizingEnabled( void )
{
	return reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->isSizingEnabled();
}

void CGUIWindow_Impl::SetFrameEnabled( bool bFrameEnabled )
{
	reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->setFrameEnabled( bFrameEnabled );
}

bool CGUIWindow_Impl::IsFrameEnabled( void )
{
	return reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->isFrameEnabled();
}

void CGUIWindow_Impl::SetCloseButtonEnabled( bool bCloseButtonEnabled )
{
	reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->setCloseButtonEnabled( bCloseButtonEnabled );
}

bool CGUIWindow_Impl::IsCloseButtonEnabled( void )
{
	return reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->isCloseButtonEnabled();
}

void CGUIWindow_Impl::SetTitlebarEnabled( bool bTitlebarEnabled )
{
	reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->setTitleBarEnabled( bTitlebarEnabled );
}

bool CGUIWindow_Impl::IsTitlebarEnabled( void )
{
	return reinterpret_cast<CEGUI::FrameWindow *>(m_pWindow)->isTitleBarEnabled();
}