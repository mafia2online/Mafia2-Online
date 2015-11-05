/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2VideoSettings.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CM2VideoSettings
{

private:

	static	bool											m_bChangeResolution;
	static	bool											m_bChangeFullScreen;
	static	bool											m_bChangeVerticalSync;
	static	int												m_iResolutionIndex;
	static	bool											m_bFullScreenEnabled;
	static	bool											m_bVerticalSyncEnabled;

	static	int												m_iMultisampleAntiAliasing;
	static	int												m_iAnisotropicFiltering;
	static	bool											m_bAmbientOcculsionEnabled;

	static	DWORD											m_dwSystemRenderingModule;
	static	bool											m_bWaitingForDeviceReset;

public:

	static	void				Load						( void );
	static	void				Save						( void );

	static	void				Pulse						( void );

	static	void				SetResolutionIndex			( int iIndex, bool bRefresh );
	static	int					GetResolutionIndex			( void ) { return m_iResolutionIndex; }

	static	void				SetFullScreenEnabled		( bool bEnabled, bool bRefresh );
	static	bool				IsFullScreenEnabled			( void ) { return m_bFullScreenEnabled; }

	static	void				SetVerticalSyncEnabled		( bool bEnabled, bool bRefresh );
	static	bool				IsVerticalSyncEnabled		( void ) { return m_bVerticalSyncEnabled; }



	static	void				SetMultisampleAntiAliasing	( int iMultisampleAntiAliasing );
	static	int					GetMultisampleAntiAliasing	( void ) { return m_iMultisampleAntiAliasing; }

	static	void				SetAnisotropicFiltering		( int iAnisotropicFiltering );
	static	int					GetAnisotropicFiltering		( void ) { return m_iAnisotropicFiltering; }

	static	void				SetAmbientOcculsionEnabled	( bool bEnabled );
	static	bool				IsAmbientOcculsionEnabled	( void ) { return m_bAmbientOcculsionEnabled; }



	static	void				SetWaitingForDeviceReset	( bool bWaiting ) { m_bWaitingForDeviceReset = bWaiting; }

	static	void				SetAudioQuality				( bool bHighQuality );
	static	void				SetSFXVolume				( int iVolume );
	static	void				SetVoiceVolume				( int iVolume );
	static	void				SetMusicVolume				( int iVolume );
	static	void				SetRadioVolume				( int iVolume );

};