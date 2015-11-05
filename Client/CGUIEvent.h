/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIEvent.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CGUITypes.h"

class CGUIElement_Impl;
struct CGUIEventArgs
{
	CGUIElement_Impl			* pWindow;
};

struct CGUIKeyEventArgs
{
	CGUIElement_Impl			* pWindow;
	unsigned int				codepoint;
	CGUIKeys::Scan				scancode;
	unsigned int				sysKeys;
};

struct CGUIMouseEventArgs
{
	CGUIElement_Impl			* pWindow;
	Vector2						position;
	Vector2						moveDelta;
	CGUIMouse::MouseButton		button;
	unsigned int				sysKeys;
	float						wheelChange;
};

struct CGUIFocusEventArgs
{
    CGUIElement_Impl			* pActivatedWindow;
    CGUIElement_Impl			* pDeactivatedWindow;
};