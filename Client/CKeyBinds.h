/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CKeyBinds.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CKeyBind
{

private:

	String											m_strKey;
	String											m_strState;
	SQVM											* m_pVM;
	SQObjectPtr										m_pFunction;
	bool											m_bActive;

public:

							CKeyBind				( const char * szKey, const char * szState, SQVM * pVM, SQObjectPtr pFunction );
							~CKeyBind				( void ) {};

	String					GetKey					( void ) { return m_strKey; }
	String					GetState				( void ) { return m_strState; }

	void					SetActive				( bool bActive ) { m_bActive = bActive; }
	bool					IsActive				( void ) { return m_bActive; }

	void					Trigger					( void );

};

class CKeyBinds
{

private:

	std::list< CKeyBind* >							m_keyBinds;
	std::list< String >								m_pressedKeys;

public:

							CKeyBinds				( void ) {};
							~CKeyBinds				( void );

	bool					BindKey					( const char * szKey, const char * szState, SQVM * pVM, SQObjectPtr callback );
	bool					UnbindKey				( const char * szKey, const char * szState );
	void					UnbindAll				( void );

	CKeyBind				* GetKeyBind			( const char * szKey, const char * szState );
	bool					IsKeyBound				( const char * szKey, const char * szState );

	void					ProcessInput			( UINT uMsg, WPARAM wParam, LPARAM lParam );

};