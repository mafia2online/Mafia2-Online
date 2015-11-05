/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CFont.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

class CFont
{

private:

	ID3DXFont				* m_pFont;
	String					m_strName;
	unsigned int			m_uiSize;
	bool					m_bBold;

public:

	CFont( String strName, unsigned int uiSize, bool bBold );
	~CFont( void );

	bool					Load( void );

	ID3DXFont				* GetFont( void ) { return m_pFont; }
	String					GetName( void ) { return m_strName; }
	unsigned int			GetSize( void ) { return m_uiSize; }
	bool					IsBold( void ) { return m_bBold; }

};