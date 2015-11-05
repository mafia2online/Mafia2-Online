/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CGUITexture_Impl.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CGUITexture_Impl
{

protected:

	CEGUI::Renderer									* m_pRenderer;
	CEGUI::Texture									* m_pTexture;

public:

						CGUITexture_Impl			( class CGUI_Impl * pGUI );
						~CGUITexture_Impl			( void );

	bool				LoadFromFile				( String strFile );
	void				LoadFromMemory				( const void * pBuffer, unsigned int uiWidth, unsigned int uiHeight );
	void				Clear						( void );

	void				SetTexture					( CEGUI::Texture * pTexture ) { m_pTexture = pTexture; }
	CEGUI::Texture		* GetTexture				( void ) { return m_pTexture; }

	IDirect3DTexture9	* GetD3DTexture				( void );
	void				CreateTexture				( unsigned int uiWidth, unsigned int uiHeight );

	eGUIType			GetType						( void ) { return GUI_TEXTURE; }

};