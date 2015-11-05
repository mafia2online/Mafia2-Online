/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUIStaticImage_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUIStaticImage_Impl : public CGUIElement_Impl
{

protected:

	CEGUI::ImagesetManager							* m_pImagesetManager;
	CEGUI::Imageset									* m_pImageset;
	const CEGUI::Image								* m_pImage;
	CGUITexture_Impl								* m_pTexture;
	bool											m_bCreatedTexture;

public:

						CGUIStaticImage_Impl		( class CGUI_Impl * pGUI, CGUIElement_Impl * pParent = NULL );
						~CGUIStaticImage_Impl		( void );

	bool				LoadFromFile				( String strFile, String strDirectory );
	bool				LoadFromTexture				( CGUITexture_Impl * pTexture );
	void				Clear						( void );

	CEGUI::Image		* GetImage					( void );
	void				Render						( void );

	eGUIType			GetType						( void ) { return GUI_IMAGE; }

};