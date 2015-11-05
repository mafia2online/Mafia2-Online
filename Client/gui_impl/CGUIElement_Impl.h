/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIElement_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIElement_Impl
{

protected:

	CEGUI::Window									* m_pWindow;
	CGUIElement_Impl								* m_pParent;
	CGUI_Impl										* m_pManager;

	GUI_CALLBACK									m_pfnClickHandler;
	GUI_CALLBACK									m_pfnMoveHandler;
	GUI_CALLBACK									m_pfnResizeHandler;
	GUI_CALLBACK									m_pfnMouseEnterHandler;
	GUI_CALLBACK									m_pfnMouseLeaveHandler;

	bool				Event_OnClick				( const CEGUI::EventArgs &e );
	bool				Event_OnMove				( const CEGUI::EventArgs &e );
	bool				Event_OnResize				( const CEGUI::EventArgs &e );
	bool				Event_OnMouseEnter			( const CEGUI::EventArgs &e );
	bool				Event_OnMouseLeave			( const CEGUI::EventArgs &e );

	void				AddEvents					( void );
	void				DestroyElement				( void );

public:

						CGUIElement_Impl			( void );

	void				SetVisible					( bool bVisible );
	bool				IsVisible					( void );

	void				SetEnabled					( bool bEnabled );
	bool				IsEnabled					( void );

	void				SetZOrderingEnabled			( bool bZOrderingEnabled );
	bool				IsZOrderingEnabled			( void );

	void				BringToFront				( void );
	void				SendToBack					( void );

	void				SetParent					( CGUIElement_Impl * pParent );
	CGUIElement_Impl	* GetParent					( void ) { return m_pParent; }

	bool				HasParent					( void ) { return (m_pParent != NULL); }
	bool				HasParent					( CGUIElement_Impl * pParent ) { return (m_pParent == pParent); }
	
	void				SetText						( String strText );
	String				GetText						( void );

	void				SetProperty					( String strProperty, String strValue );
	String				GetProperty					( String strProperty );

	void				SetPosition					( Vector2 vecPosition, bool bRelative = false );
	Vector2				GetPosition					( bool bRelative = false );

	void				SetSize						( Vector2 vecSize, bool bRelative = false );
	Vector2				GetSize						( bool bRelative = false );

	void				SetMinimumSize				( Vector2 vecSize );
	Vector2				GetMinimumSize				( void );

	void				SetMaximumSize				( Vector2 vecSize );
	Vector2				GetMaximumSize				( void );

	void				SetAlpha					( float fAlpha );
	float				GetAlpha					( void );

	void				SetInheritsAlpha			( bool bInheritsAlpha );
	bool				InheritsAlpha				( void );

	void				SetAlwaysOnTop				( bool bAlwaysOnTop );
	bool				IsAlwaysOnTop				( void );

	void				Activate					( void );
	void				Deactivate					( void );
	bool				IsActive					( void );

	void				ForceRedraw					( void );
	void				CorrectEdges				( void );

	void				SetFont						( const char * szFontName );
	const char			* GetFont					( void );

	void				SetClickHandler				( GUI_CALLBACK pfnClickHandler ) { m_pfnClickHandler = pfnClickHandler; }
	void				SetMoveHandler				( GUI_CALLBACK pfnMoveHandler ) { m_pfnMoveHandler = pfnMoveHandler; }
	void				SetResizeHandler			( GUI_CALLBACK pfnResizeHandler ) { m_pfnResizeHandler = pfnResizeHandler; }
	void				SetMouseEnterHandler		( GUI_CALLBACK pfnMouseEnterHandler ) { m_pfnMouseEnterHandler = pfnMouseEnterHandler; }
	void				SetMouseLeaveHandler		( GUI_CALLBACK pfnMouseLeaveHandler ) { m_pfnMouseLeaveHandler = pfnMouseLeaveHandler; }

	CEGUI::Window		* GetWindow					( void ) { return m_pWindow; }

};