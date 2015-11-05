/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUINatives.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

// dxDrawText( string text, float x, float y, int colour, bool shadow, string font[, float scale = 1.0] );
// dxDrawRectangle( float x, float y, float width, float height, int colour );
// dxDrawLine( float x, float y, float end_x, float end_y, float width, int colour );
// dxDrawImage(  );
// dxGetTextDimensions( string text, float scale, string font );
// isTransferBoxShowing();
// showCursor( bool toggle );
// isCursorShowing();
// isMainMenuShowing();

// createGuiElement( int type, string text, float x, float y, float width, float height[, bool bRelative, GUIElement * pParent] );
// guiSetPosition( element, float x, float y[, bool relative = false] );
// guiGetPosition( element[, bool relative = false] );
// guiSetSize( element, float width, float height[, bool relative = false] );
// guiGetSize( element[, bool relative = false] );
// guiSetVisible( element, bool visible );
// guiIsVisible( element );
// guiSetText( element, char * text );
// guiGetText( element );
// guiBringToFront( element );
// guiSendToBack( element );
// guiSetAlpha( element, float alpha );
// guiGetAlpha( element );
// guiSetAlwaysOnTop( element, bool always_on_top );
// guiIsAlwaysOnTop( element );
// guiSetInputMasked( element, bool );
// guiIsInputMasked( element );

class CGUINatives
{

private:

	static	SQInteger		NewFont								( SQVM * pVM );
	static	SQInteger		DrawText							( SQVM * pVM );
	static	SQInteger		DrawRectangle						( SQVM * pVM );
	static	SQInteger		DrawLine							( SQVM * pVM );
	static	SQInteger		DrawImage							( SQVM * pVM );
	static	SQInteger		GetTextDimensions					( SQVM * pVM );
	static	SQInteger		IsTransferBoxShowing				( SQVM * pVM );
	static	SQInteger		ShowCursor							( SQVM * pVM );
	static	SQInteger		IsCursorShowing						( SQVM * pVM );
	static	SQInteger		IsMainMenuShowing					( SQVM * pVM );

	static	SQInteger		CreateElement						( SQVM * pVM );
	static	SQInteger		DestroyElement						( SQVM * pVM );
	static	SQInteger		GuiSetPosition						( SQVM * pVM );
	static	SQInteger		GuiGetPosition						( SQVM * pVM );
	static	SQInteger		GuiSetSize							( SQVM * pVM );
	static	SQInteger		GuiGetSize							( SQVM * pVM );
	static	SQInteger		GuiSetVisible						( SQVM * pVM );
	static	SQInteger		GuiIsVisible						( SQVM * pVM );
	static	SQInteger		GuiSetText							( SQVM * pVM );
	static	SQInteger		GuiGetText							( SQVM * pVM );
	static	SQInteger		GuiBringToFront						( SQVM * pVM );
	static	SQInteger		GuiSendToBack						( SQVM * pVM );
	static	SQInteger		GuiSetAlpha							( SQVM * pVM );
	static	SQInteger		GuiGetAlpha							( SQVM * pVM );
	static	SQInteger		GuiSetAlwaysOnTop					( SQVM * pVM );
	static	SQInteger		GuiIsAlwaysOnTop					( SQVM * pVM );
	static	SQInteger		GuiSetInputMasked					( SQVM * pVM );
	static	SQInteger		GuiIsInputMasked					( SQVM * pVM );

public:

	static	void			Register( CScriptingManager * pScriptingManager );

};