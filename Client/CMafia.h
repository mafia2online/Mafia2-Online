/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMafia.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			   @FRi
*
***************************************************************/

#pragma once

class CM2PhysFS;
class CM2Navigation;
class CMafia
{

private:

	String									m_strWeather;
	bool									m_bFocus;
	bool									m_bMapOpen;
	bool									m_bSummer;

	CM2PhysFS								* m_pFileSystem;
	CM2Navigation							* m_pNavigation;

public:

					CMafia					( void );
					~CMafia					( void );

	void			SetFocus				( bool bFocus ) { m_bFocus = bFocus; }
	bool			Focused					( void ) { return m_bFocus; }

	void			LoadPointers			( void );

	void			FadeSound				( bool bFadeOut, float fTimeInSeconds );
	void			ScoreShow				( bool bToggle );
	void			ActionButtonsShow		( bool bToggle );
	void			InfoAreaShow			( bool bToggle );
	void			SpeedoMeterShow			( bool bToggle );
	void			MoneyShow				( bool bToggle );
	void			PlayMusic				( const char * szMusic );
	void			StopMusic				( void );
	void			ChangeWeather			( String strWeather, int iChangeTime = 0 );
	String			GetWeather				( void ) { return m_strWeather; }
	void			ChangeSeason			( bool bSummer );
	bool			IsSummer				( void ) { return m_bSummer; }
	void			Spawn					( bool bFade = true );

	void			SwitchGenerators		( bool bPeds, bool bFarAmbients );
	void			CreateTimer				( float fTime );
	void			StartTimer				( void );
	void			StopTimer				( void );

	void			SetMouseSensitivityMultiplier		( float fSensitivity );
	float			GetMouseSensitivityMultiplier		( void );

	void			SetRadioContent			( const char * szChannel, const char * szContent );
	void			SetSummerRadio			( bool bSummerRadio = true );

	void			* allocate				( int iSize );
	void			free					( void* pMemory );
	void			* allocateBuffer		( int iLen );

	bool			OpenMap					( bool bOpen );
	bool			IsMapOpen				( void ) { return m_bMapOpen; }

	// Main Menu
	void			OnGameStart				( void );

	CM2PhysFS		* GetFileSystem			( void ) { return m_pFileSystem; }
	CM2Navigation	* GetNavigation			( void ) { return m_pNavigation; }

};