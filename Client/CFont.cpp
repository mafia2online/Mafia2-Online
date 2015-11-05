/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFont.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern CCore		* pCore;

CFont::CFont( String strName, unsigned int uiSize, bool bBold )
{
	// Set variables
	m_pFont = NULL;
	m_strName = strName;
	m_uiSize = uiSize;
	m_bBold = bBold;
}

CFont::~CFont( void )
{
	// Destroy the font
	if( m_pFont )
		SAFE_DELETE( m_pFont );
}

bool CFont::Load( void )
{
	return SUCCEEDED( D3DXCreateFont( pCore->GetGraphics()->GetDevice(), m_uiSize, 0, (m_bBold ? FW_BOLD : FW_NORMAL), 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, (DEFAULT_PITCH | FF_DONTCARE), m_strName.Get(), &m_pFont ) );
}