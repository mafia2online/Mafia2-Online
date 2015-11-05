/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUINatives.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;


void CGUINatives::Register( CScriptingManager * pScriptingManager )
{
	pScriptingManager->RegisterFunction( "dxCreateFont", NewFont, 3, "sib" );
	pScriptingManager->RegisterFunction( "dxDrawText", DrawText, -1, NULL );
	pScriptingManager->RegisterFunction( "dxDrawRectangle", DrawRectangle, 5, "ffffi" );
	pScriptingManager->RegisterFunction( "dxDrawLine", DrawLine, 6, "fffffi" );
	pScriptingManager->RegisterFunction( "dxGetTextDimensions", GetTextDimensions, 3, "sfs" );
	pScriptingManager->RegisterFunction( "isTransferBoxShowing", IsTransferBoxShowing, 0, NULL );
	pScriptingManager->RegisterFunction( "showCursor", ShowCursor, 1, "b" );
	pScriptingManager->RegisterFunction( "isCursorShowing", IsCursorShowing, 0, NULL );
	pScriptingManager->RegisterFunction( "isMainMenuShowing", IsMainMenuShowing, 0, NULL );

	// Register gui functions
	pScriptingManager->RegisterFunction( "guiCreateElement", CreateElement, -1, NULL );
	pScriptingManager->RegisterFunction( "guiDestroyElement", DestroyElement, 1, "p" );
	pScriptingManager->RegisterFunction( "guiSetPosition", GuiSetPosition, -1, NULL );
	pScriptingManager->RegisterFunction( "guiGetPosition", GuiGetPosition, -1, NULL );
	pScriptingManager->RegisterFunction( "guiSetSize", GuiSetSize, -1, NULL );
	pScriptingManager->RegisterFunction( "guiGetSize", GuiGetPosition, -1, NULL );
	pScriptingManager->RegisterFunction( "guiSetVisible", GuiSetVisible, 2, "pb" );
	pScriptingManager->RegisterFunction( "guiIsVisible", GuiIsVisible, 1, "p" );
	pScriptingManager->RegisterFunction( "guiSetText", GuiSetText, 2, "ps" );
	pScriptingManager->RegisterFunction( "guiGetText", GuiGetText, 1, "p" );
	pScriptingManager->RegisterFunction( "guiBringToFront", GuiBringToFront, 1, "p" );
	pScriptingManager->RegisterFunction( "guiSendToBack", GuiSendToBack, 1, "p" );
	pScriptingManager->RegisterFunction( "guiSetAlpha", GuiSetAlpha, 2, "pf" );
	pScriptingManager->RegisterFunction( "guiGetAlpha", GuiGetAlpha, 1, "p" );
	pScriptingManager->RegisterFunction( "guiSetAlwaysOnTop", GuiSetAlwaysOnTop, 2, "pb" );
	pScriptingManager->RegisterFunction( "guiIsAlwaysOnTop", GuiIsAlwaysOnTop, 1, "p" );
	pScriptingManager->RegisterFunction( "guiSetInputMasked", GuiSetInputMasked, 2, "pb" );
	pScriptingManager->RegisterFunction( "guiIsInputMasked", GuiIsInputMasked, 1, "p" );

	// Register GUI constact
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_WINDOW", GUI_WINDOW );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_EDIT", GUI_EDIT );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_CHECKBOX", GUI_CHECKBOX );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_COMBOBOX", GUI_COMBOBOX );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_GRIDLIST", GUI_GRIDLIST );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_LABEL", GUI_LABEL );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_PROGRESSBAR", GUI_PROGRESSBAR );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_RADIOBUTTON", GUI_RADIOBUTTON );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_SCROLLBAR", GUI_SCROLLBAR );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_SCROLLPANE", GUI_SCROLLPANE );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_TABPANEL", GUI_TABPANEL );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_TAB", GUI_TAB );
	pScriptingManager->RegisterConstant ( "ELEMENT_TYPE_IMAGE", GUI_IMAGE );
}

// dxCreateFont( font, size, bold )
SQInteger CGUINatives::NewFont( SQVM * pVM )
{
	const SQChar * szFont;
	SQInteger iSize;
	SQBool bBold;

	sq_getstring( pVM, -3, &szFont );
	sq_getinteger( pVM, -2, &iSize );
	sq_getbool( pVM, -1, &bBold );

	sq_pushbool( pVM, pCore->GetGraphics()->LoadFont( szFont, iSize, bBold ) );
	return 1;
}

// dxDrawText( text, x, y, colour, shadow, font[, scale = 1.0f] );
SQInteger CGUINatives::DrawText( SQVM * pVM )
{
	// Get the stack top
	int iTop = sq_gettop( pVM ) - 1;

	//
	if( iTop < 6 || iTop > 7 )
		CHECK_PARAMS( "dxDrawText", 6 );

	const SQChar * szText;
	float fX, fY;
	SQInteger iColour;
	SQBool bShadow;
	const SQChar * szFont;
	float fScale = 1.0f;

	if( iTop == 6 )
	{
		CHECK_TYPE( "dxDrawText", 1, -6, OT_STRING );
		CHECK_TYPE( "dxDrawText", 2, -5, OT_FLOAT );
		CHECK_TYPE( "dxDrawText", 3, -4, OT_FLOAT );
		CHECK_TYPE( "dxDrawText", 4, -3, OT_INTEGER );
		CHECK_TYPE( "dxDrawText", 5, -2, OT_BOOL );
		CHECK_TYPE( "dxDrawText", 6, -1, OT_STRING );

		sq_getstring( pVM, -6, &szText );
		sq_getfloat( pVM, -5, &fX );
		sq_getfloat( pVM, -4, &fY );
		sq_getinteger( pVM, -3, &iColour );
		sq_getbool( pVM, -2, &bShadow );
		sq_getstring( pVM, -1, &szFont );
	}
	else
	{
		CHECK_TYPE( "dxDrawText", 1, -7, OT_STRING );
		CHECK_TYPE( "dxDrawText", 2, -6, OT_FLOAT );
		CHECK_TYPE( "dxDrawText", 3, -5, OT_FLOAT );
		CHECK_TYPE( "dxDrawText", 4, -4, OT_INTEGER );
		CHECK_TYPE( "dxDrawText", 5, -3, OT_BOOL );
		CHECK_TYPE( "dxDrawText", 6, -2, OT_STRING );
		CHECK_TYPE( "dxDrawText", 7, -1, OT_FLOAT );

		sq_getstring( pVM, -7, &szText );
		sq_getfloat( pVM, -6, &fX );
		sq_getfloat( pVM, -5, &fY );
		sq_getinteger( pVM, -4, &iColour );
		sq_getbool( pVM, -3, &bShadow );
		sq_getstring( pVM, -2, &szFont );
		sq_getfloat( pVM, -1, &fScale );
	}

	// Draw the text
	pCore->GetGraphics()->DrawText( fX, fY, (DWORD)iColour, fScale, szFont, (bool)bShadow, szText );

	sq_pushbool( pVM, true );
	return 1;
}

// dxDrawRectangle( x, y, width, height, colour )
SQInteger CGUINatives::DrawRectangle( SQVM * pVM )
{
	float fX, fY, fWidth, fHeight;
	SQInteger iColour;

	sq_getfloat( pVM, -5, &fX );
	sq_getfloat( pVM, -4, &fY );
	sq_getfloat( pVM, -3, &fWidth );
	sq_getfloat( pVM, -2, &fHeight );
	sq_getinteger( pVM, -1, &iColour );

	// Draw the text
	pCore->GetGraphics()->DrawBox( fX, fY, fWidth, fHeight, (DWORD)iColour );

	sq_pushbool( pVM, true );
	return 1;
}

// dxDrawLine( float x, float y, float end_x, float end_y, float width, int colour );
SQInteger CGUINatives::DrawLine( SQVM * pVM )
{
	float fX, fY, fEndX, fEndY, fWidth;
	SQInteger iColour;

	sq_getfloat( pVM, -6, &fX );
	sq_getfloat( pVM, -5, &fY );
	sq_getfloat( pVM, -4, &fEndX );
	sq_getfloat( pVM, -3, &fEndY );
	sq_getfloat( pVM, -2, &fWidth );
	sq_getinteger( pVM, -1, &iColour );

	// Draw the line
	pCore->GetGraphics()->DrawLine( fX, fY, fEndX, fEndY, fWidth, (DWORD)iColour );

	sq_pushbool( pVM, true );
	return 1;
}

// dxGetTextDimensions( text, scale, font );
SQInteger CGUINatives::GetTextDimensions( SQVM * pVM )
{
	const SQChar * szText, * szFont;
	float fScale;

	sq_getstring( pVM, -3, &szText );
	sq_getfloat( pVM, -2, &fScale );
	sq_getstring( pVM, -1, &szFont );

	CSquirrelArguments args;
	args.push( pCore->GetGraphics()->GetTextWidth( szText, fScale, szFont ) );
	args.push( pCore->GetGraphics()->GetFontHeight( fScale, szFont ) );

	CSquirrelArgument * pArgument = new CSquirrelArgument( args, true );
	pArgument->push( pVM );

	delete pArgument;
	return 1;
}

// isTransferBoxShowing( );
SQInteger CGUINatives::IsTransferBoxShowing( SQVM * pVM )
{
	sq_pushbool( pVM, pCore->GetGUI()->GetDownloadProgress()->IsVisible() );
	return 1;
}

// showCursor( bool toggle );
SQInteger CGUINatives::ShowCursor( SQVM * pVM )
{
	SQBool bToggle;
	sq_getbool( pVM, -1, &bToggle );

	// Toggle the gui cursor
	pCore->GetGUI()->SetCursorVisible( bToggle );

	sq_pushbool( pVM, true );
	return 1;
}

// isCursorShowing();
SQInteger CGUINatives::IsCursorShowing( SQVM * pVM )
{
	sq_pushbool( pVM, pCore->GetGUI()->IsCursorVisible() );
	return 1;
}

// isMainMenuShowing();
SQInteger CGUINatives::IsMainMenuShowing( SQVM * pVM )
{
	sq_pushbool( pVM, pCore->GetGUI()->GetMainMenu()->IsVisible() );
	return 1;
}

// guiCreateElement( eElementType type, string caption, float x, float y, float width, float height[, bool bRelative, CGUIElement * pParent] );
SQInteger CGUINatives::CreateElement( SQVM * pVM )
{
	// Get the stack top
	int iTop = (sq_gettop( pVM ) - 1);

	// 
	if( iTop < 6 || iTop > 8 )
		CHECK_PARAMS_MIN( "guiCreateElement", 6 );

	eGUIType type = eGUIType::GUI_MAX;
	CGUIElement_Impl * pElement = NULL;
	CGUIElement_Impl * pParent = NULL;
	const SQChar * szCaption;
	Vector2 vecPosition;
	Vector2 vecSize;
	SQBool bRelative = false;

	// Do we have 6 params?
	if( iTop == 6 )
	{
		CHECK_TYPE ( "guiCreateElement", 1, -6, OT_INTEGER );
		CHECK_TYPE ( "guiCreateElement", 2, -5, OT_STRING );
		CHECK_TYPE ( "guiCreateElement", 3, -4, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 4, -3, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 5, -2, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 6, -1, OT_FLOAT );

		sq_getinteger( pVM, -6, (SQInteger *)&type );
		sq_getstring( pVM, -5, &szCaption );
		sq_getfloat( pVM, -4, &vecPosition.fX );
		sq_getfloat( pVM, -3, &vecPosition.fY );
		sq_getfloat( pVM, -2, &vecSize.fX );
		sq_getfloat( pVM, -1, &vecSize.fY );
	}
	else if( iTop == 7 )
	{
		CHECK_TYPE ( "guiCreateElement", 1, -7, OT_INTEGER );
		CHECK_TYPE ( "guiCreateElement", 2, -6, OT_STRING );
		CHECK_TYPE ( "guiCreateElement", 3, -5, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 4, -4, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 5, -3, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 6, -2, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 7, -1, OT_BOOL );

		sq_getinteger( pVM, -7, (SQInteger *)&type );
		sq_getstring( pVM, -6, &szCaption );
		sq_getfloat( pVM, -5, &vecPosition.fX );
		sq_getfloat( pVM, -4, &vecPosition.fY );
		sq_getfloat( pVM, -3, &vecSize.fX );
		sq_getfloat( pVM, -2, &vecSize.fY );
		sq_getbool( pVM, -1, &bRelative );
	}
	else if( iTop == 8 )
	{
		CHECK_TYPE ( "guiCreateElement", 1, -8, OT_INTEGER );
		CHECK_TYPE ( "guiCreateElement", 2, -7, OT_STRING );
		CHECK_TYPE ( "guiCreateElement", 3, -6, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 4, -5, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 5, -4, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 6, -3, OT_FLOAT );
		CHECK_TYPE ( "guiCreateElement", 7, -2, OT_BOOL );
		CHECK_TYPE ( "guiCreateElement", 8, -1, OT_USERPOINTER );

		sq_getinteger( pVM, -8, (SQInteger *)&type );
		sq_getstring( pVM, -7, &szCaption );
		sq_getfloat( pVM, -6, &vecPosition.fX );
		sq_getfloat( pVM, -5, &vecPosition.fY );
		sq_getfloat( pVM, -4, &vecSize.fX );
		sq_getfloat( pVM, -3, &vecSize.fY );
		sq_getbool( pVM, -2, &bRelative );
		pParent = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );
	}

	// Get the gui pointer
	CGUI_Impl * pGUI = pCore->GetGUI()->GetCEGUI();

	// Create the gui element
	switch( type )
	{
	case GUI_WINDOW: pElement = pGUI->CreateWnd( szCaption, pParent ); break;
	case GUI_EDIT: pElement = pGUI->CreateEdit( szCaption, pParent ); break;
	case GUI_BUTTON: pElement = pGUI->CreateButton( szCaption, pParent ); break;
	case GUI_CHECKBOX: pElement = pGUI->CreateCheckBox( szCaption, pParent ); break;
	case GUI_COMBOBOX: pElement = pGUI->CreateComboBox( szCaption, pParent ); break;		// todo: change const char into String for CreateComboBox
	case GUI_GRIDLIST: pElement = pGUI->CreateGridList( pParent ); break;							// this
	case GUI_LABEL: pElement = pGUI->CreateLabel( szCaption, NULL, pParent ); break;
	case GUI_PROGRESSBAR: pElement = pGUI->CreateProgressBar( pParent ); break;					// this
	case GUI_RADIOBUTTON: pElement = pGUI->CreateRadioButton( szCaption, pParent ); break;
	case GUI_TABPANEL: pElement = pGUI->CreateTabPanel( pParent ); break;							// this
	case GUI_IMAGE: pElement = pGUI->CreateStaticImage ( pParent ); break;
	default: sq_pushbool( pVM, false ); return 1;
	}

	// Is the element valid?
	if( pElement )
	{
		// Set the element position
		pElement->SetPosition ( vecPosition, bRelative );

		// Set the element size
		pElement->SetSize ( vecSize, bRelative );

		// Activate the element
		pElement->Activate ();

		// Set the element visible
		pElement->SetVisible ( true );

		// Is this element an image?
		if ( type == GUI_IMAGE )
		{
			// Load the image file
			((CGUIStaticImage_Impl *)pElement)->LoadFromFile ( szCaption, SharedUtility::GetFileNameForScriptFile ( "", "", pCore->GetHost (), pCore->GetPort () ) );
		}

		// Get the script pointer from the script vm
		CSquirrel * pScript = pCore->GetClientScriptingManager()->GetScriptingManager()->Get( pVM );

		// Add the element to the client script gui manager
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->Add( pElement, pScript );

		sq_pushpointer< CGUIElement_Impl* >( pVM, pElement );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiDestroyElement( element );
SQInteger CGUINatives::DestroyElement ( SQVM * pVM )
{
	// Get the element pointer
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );

	// Is the element valid?
	if ( pElement )
	{
		// Remove the element from the client script gui manager
		pCore->GetClientScriptingManager()->GetScriptGUIManager()->Delete ( pElement );

		// Delete the element
		SAFE_DELETE ( pElement );

		// Reset the pointer instance
		sq_pushpointer ( pVM, NULL );

		sq_pushbool ( pVM, true );
		return 1;
	}

	sq_pushbool ( pVM, false );
	return 1;
}

// guiSetPosition( element, float x, float y[, bool relative = false] );
SQInteger CGUINatives::GuiSetPosition( SQVM * pVM )
{
	int iTop = (sq_gettop( pVM ) - 1);

	if( iTop < 3 || iTop > 4 )
		CHECK_PARAMS_MIN( "guiSetPosition", 3 );

	CGUIElement_Impl * pElement = NULL;
	Vector2 vecPosition;
	SQBool bRelative = false;

	if( iTop == 3 )
	{
		CHECK_TYPE( "guiSetPosition", 1, -1, OT_FLOAT );
		CHECK_TYPE( "guiSetPosition", 2, -2, OT_FLOAT );
		CHECK_TYPE( "guiSetPosition", 3, -3, OT_USERPOINTER );

		pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -3 );
		sq_getfloat( pVM, -2, &vecPosition.fX );
		sq_getfloat( pVM, -1, &vecPosition.fY );
	}
	else if( iTop == 4 )
	{
		CHECK_TYPE( "guiSetPosition", 1, -1, OT_BOOL );
		CHECK_TYPE( "guiSetPosition", 2, -2, OT_FLOAT );
		CHECK_TYPE( "guiSetPosition", 3, -3, OT_FLOAT );
		CHECK_TYPE( "guiSetPosition", 4, -4, OT_USERPOINTER );

		pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -4 );
		sq_getfloat( pVM, -3, &vecPosition.fX );
		sq_getfloat( pVM, -2, &vecPosition.fY );
		sq_getbool( pVM, -1, &bRelative );
	}

	CLogFile::Printf( "guiSetPosition - element: 0x%p (%f, %f, %s)", pElement, vecPosition.fX, vecPosition.fY, bRelative );

	// Is the element valid?
	if( pElement )
	{
		// Set the element position
		pElement->SetPosition( vecPosition, bRelative );
		
		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiGetPosition( element[, bool bRelative = false] );
SQInteger CGUINatives::GuiGetPosition( SQVM * pVM )
{
	// Get the stack top
	int iTop = (sq_gettop( pVM ) - 1);

	// Do we have too many params?
	if( iTop < 1 || iTop > 2 )
		CHECK_PARAMS_MIN( "guiGetPosition", 1 );

	//
	CGUIElement_Impl * pElement = NULL;
	SQBool bRelative = false;

	//
	if( iTop == 2 )
	{
		CHECK_TYPE( "guiGetPosition", 1, -2, OT_USERPOINTER );
		CHECK_TYPE( "guiGetPosition", 2, -1, OT_BOOL );

		sq_getbool( pVM, -1, &bRelative );
		pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -2 );
	}
	else if( iTop == 1 )
	{
		CHECK_TYPE( "guiGetPosition", 1, -1, OT_USERPOINTER );
		pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );
	}

	// Is the element valid?
	if( pElement )
	{
		// Get the element position
		Vector2 vecPosition = pElement->GetPosition( bRelative );

		//
		sq_newarray( pVM, 0 );

		sq_pushfloat( pVM, vecPosition.fX );
		sq_arrayappend( pVM, -2 );

		sq_pushfloat( pVM, vecPosition.fY );
		sq_arrayappend( pVM, -2 );

		sq_push( pVM, -1 );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiSetSize( element, float width, float height[, bool relative = false] );
SQInteger CGUINatives::GuiSetSize( SQVM * pVM )
{
	int iTop = (sq_gettop( pVM ) - 1);

	if( iTop < 3 || iTop > 4 )
		CHECK_PARAMS( "guiSetPosition", 3 );

	CGUIElement_Impl * pElement = NULL;
	Vector2 vecSize;
	SQBool bRelative = false;

	if( iTop == 3 )
	{
		CHECK_TYPE( "guiSetPosition", 1, -1, OT_FLOAT );
		CHECK_TYPE( "guiSetPosition", 2, -2, OT_FLOAT );
		CHECK_TYPE( "guiSetPosition", 3, -3, OT_USERPOINTER );

		pElement = sq_getpointer< CGUIElement_Impl* >( pVM );
		sq_getfloat( pVM, 2, &vecSize.fX );
		sq_getfloat( pVM, 3, &vecSize.fY );

	}
	else if( iTop == 4 )
	{
		CHECK_TYPE( "guiSetPosition", 1, -1, OT_BOOL );
		CHECK_TYPE( "guiSetPosition", 2, -2, OT_FLOAT );
		CHECK_TYPE( "guiSetPosition", 3, -3, OT_FLOAT );
		CHECK_TYPE( "guiSetPosition", 4, -4, OT_USERPOINTER );

		pElement = sq_getpointer< CGUIElement_Impl* >( pVM );
		sq_getfloat( pVM, 2, &vecSize.fX );
		sq_getfloat( pVM, 3, &vecSize.fY );
		sq_getbool( pVM, 4, &bRelative );
	}

	// Is the element valid?
	if( pElement )
	{
		// Set the element size
		pElement->SetSize( vecSize, bRelative );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiGetSize( element[, bool bRelative = false] );
SQInteger CGUINatives::GuiGetSize( SQVM * pVM )
{
	// Get the stack top
	int iTop = (sq_gettop( pVM ) - 1);

	// Do we have too many params?
	if( iTop < 1 || iTop > 2 )
		CHECK_PARAMS_MIN( "guiGetSize", 1 );

	//
	CGUIElement_Impl * pElement = NULL;
	SQBool bRelative = false;

	//
	if( iTop == 2 )
	{
		CHECK_TYPE( "guiGetSize", 1, -2, OT_USERPOINTER );
		CHECK_TYPE( "guiGetSize", 2, -1, OT_BOOL );

		sq_getbool( pVM, -1, &bRelative );
		pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -2 );
	}
	else if( iTop == 1 )
	{
		CHECK_TYPE( "guiGetSize", 1, -1, OT_USERPOINTER );
		pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );
	}

	// Is the element valid?
	if( pElement )
	{
		// Get the element size
		Vector2 vecSize = pElement->GetSize( bRelative );

		//
		sq_newarray( pVM, 0 );

		sq_pushfloat( pVM, vecSize.fX );
		sq_arrayappend( pVM, -2 );

		sq_pushfloat( pVM, vecSize.fY );
		sq_arrayappend( pVM, -2 );

		sq_push( pVM, -1 );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiSetVisible( element, bool visible );
SQInteger CGUINatives::GuiSetVisible( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -2 );
	SQBool bVisible;
	sq_getbool( pVM, -1, &bVisible );

	// Is the element valid?
	if( pElement )
	{
		// Set the element visibility
		pElement->SetVisible( bVisible );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiIsVisible( element );
SQInteger CGUINatives::GuiIsVisible( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );

	// Is the element valid?
	if( pElement )
	{
		// Push the element visibility
		sq_pushbool( pVM, pElement->IsVisible() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiSetText( element, const char * text );
SQInteger CGUINatives::GuiSetText( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -2 );
	const SQChar * szText;
	sq_getstring( pVM, -1, &szText );

	// Is the element valid?
	if( pElement )
	{
		// Set the element text
		pElement->SetText( szText );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiGetText( element );
SQInteger CGUINatives::GuiGetText( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );

	// Is the element valid?
	if( pElement )
	{
		// Push the element text
		sq_pushstring( pVM, pElement->GetText().Get(), pElement->GetText().GetLength() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiBringToFront( element );
SQInteger CGUINatives::GuiBringToFront( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );

	// Is the element valid?
	if( pElement )
	{
		// Bring the element to the foreground
		pElement->BringToFront();

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiSendToBack( element );
SQInteger CGUINatives::GuiSendToBack( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );

	// Is the element valid?
	if( pElement )
	{
		// Send the element to the background
		pElement->SendToBack();

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiSetAlpha( element, float alpha );
SQInteger CGUINatives::GuiSetAlpha( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -2 );
	float fAlpha;
	sq_getfloat( pVM, -1, &fAlpha );

	// Is the element valid?
	if( pElement )
	{
		// Set the element alpha
		pElement->SetAlpha( fAlpha );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiGetAlpha( element );
SQInteger CGUINatives::GuiGetAlpha( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );

	// Is the element valid?
	if( pElement )
	{
		// Push the element alpha
		sq_pushfloat( pVM, pElement->GetAlpha() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiSetAlwaysOnTop( element, bool always_on_top );
SQInteger CGUINatives::GuiSetAlwaysOnTop( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -2 );
	SQBool bAlwaysOnTop;
	sq_getbool( pVM, -1, &bAlwaysOnTop );

	// Is the element valid?
	if( pElement )
	{
		// Set the element always on top
		pElement->SetAlwaysOnTop( bAlwaysOnTop );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiIsAlwaysOnTop( element );
SQInteger CGUINatives::GuiIsAlwaysOnTop( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );

	// Is the element valid?
	if( pElement )
	{
		// Push the element always on top flag
		sq_pushfloat( pVM, pElement->IsAlwaysOnTop() );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiSetInputMasked( element, bool bMasked );
SQInteger CGUINatives::GuiSetInputMasked( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -2 );
	SQBool bMasked;
	sq_getbool( pVM, -1, &bMasked );

	// Is the element valid?
	if( pElement )
	{
		// Set the element masking
		((CGUIEdit_Impl *)pElement)->SetMasked ( bMasked );

		sq_pushbool( pVM, true );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}

// guiIsInputMasked( element );
SQInteger CGUINatives::GuiIsInputMasked( SQVM * pVM )
{
	CGUIElement_Impl * pElement = sq_getpointer< CGUIElement_Impl* >( pVM, -1 );

	// Is the element valid?
	if( pElement )
	{
		// Push the element masking
		sq_pushfloat( pVM, ((CGUIEdit_Impl *)pElement)->IsMasked () );
		return 1;
	}

	sq_pushbool( pVM, false );
	return 1;
}