/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2VideoSettings.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;
bool						CM2VideoSettings::m_bChangeResolution = false;
bool						CM2VideoSettings::m_bChangeFullScreen = false;
bool						CM2VideoSettings::m_bChangeVerticalSync = false;
int							CM2VideoSettings::m_iResolutionIndex = -1;
bool						CM2VideoSettings::m_bFullScreenEnabled = false;
bool						CM2VideoSettings::m_bVerticalSyncEnabled = false;
int							CM2VideoSettings::m_iMultisampleAntiAliasing = 0;
int							CM2VideoSettings::m_iAnisotropicFiltering = 0;
bool						CM2VideoSettings::m_bAmbientOcculsionEnabled = false;
DWORD						CM2VideoSettings::m_dwSystemRenderingModule = 0x0;
bool						CM2VideoSettings::m_bWaitingForDeviceReset = false;

// Video
DWORD						FUNC_C_SystemRenderingModule__SetFullScreenEnabled = 0xB65CB0;
DWORD						FUNC_C_SystemRenderingModule__SetVerticalSyncEnabled = 0xB65F80;
DWORD						FUNC_SetScreenResolution = 0x8366A0;
DWORD						FUNC_SetMultisampleAntiAliasing = 0x836820;
DWORD						FUNC_SetAnisotropicFiltering = 0x8368B0;
DWORD						FUNC_EnableAmbientOcclusion = 0x8366F0;

// Audio
DWORD						FUNC_SetAudioQuality = 0x836540;
DWORD						FUNC_SetSFXVolume = 0x8363F0;
DWORD						FUNC_SetVoicesVolume = 0x836450;
DWORD						FUNC_SetMusicVolume = 0x8364B0;
DWORD						FUNC_SetRadioVolume = 0x836510;

void CM2VideoSettings::Load( void )
{
}

void CM2VideoSettings::Save( void )
{
}

void CM2VideoSettings::Pulse ( void )
{
	// Should we change resolution?
	if ( m_bChangeResolution )
	{
		m_bChangeResolution = false;

		CLogFile::Printf ( "Changing res to %d...", m_iResolutionIndex );

		int iResolutionIndex = m_iResolutionIndex;
		_asm push iResolutionIndex;
		_asm call FUNC_SetScreenResolution;

		CLogFile::Printf ( "Done!" );
	}

	// Should we change fullscreen?
	if ( m_bChangeFullScreen )
	{
		m_bChangeFullScreen = false;

		CLogFile::Print ( "Setting fullscreen" );

		DWORD FUNC_C_SystemRenderingModule__Singleton = 0xB2CBA0;
		bool bFullScreenEnabled = m_bFullScreenEnabled;
		_asm push bFullScreenEnabled;
		_asm call FUNC_C_SystemRenderingModule__Singleton;
		_asm mov ecx, eax;
		_asm call FUNC_C_SystemRenderingModule__SetFullScreenEnabled;

		CLogFile::Print ( "Done!" );
	}
	
	// Should we change vertical sync?
	if ( m_bChangeVerticalSync )
	{
		DWORD FUNC_C_SystemRenderingModule__Singleton = 0xB2CBA0;
		bool bVerticalSyncEnabled = m_bVerticalSyncEnabled;
		_asm push bVerticalSyncEnabled;
		_asm call FUNC_C_SystemRenderingModule__Singleton;
		_asm mov ecx, eax;
		_asm call FUNC_C_SystemRenderingModule__SetVerticalSyncEnabled;

		m_bChangeVerticalSync = false;
	}
}

void CM2VideoSettings::SetResolutionIndex ( int iIndex, bool bRefresh )
{
	// Has the resolution index changed?
	if ( iIndex != m_iResolutionIndex )
	{
		// Update the current resolution index
		m_iResolutionIndex = iIndex;

		// Mark as refresh resolution
		m_bChangeResolution = bRefresh;
	}
}

void CM2VideoSettings::SetFullScreenEnabled ( bool bEnabled, bool bRefresh )
{
	// Has the fullscreen flag changed?
	if ( bEnabled != m_bFullScreenEnabled )
	{
		// Update the current fullscreen flag
		m_bFullScreenEnabled = bEnabled;

		// Mark as refresh fullscreen
		m_bChangeFullScreen = bRefresh;
	}
}

void CM2VideoSettings::SetVerticalSyncEnabled ( bool bEnabled, bool bRefresh )
{
	// Has the vertical sync flag changed?
	if ( bEnabled != m_bVerticalSyncEnabled )
	{
		// Update the current vertical sync flag
		m_bVerticalSyncEnabled = bEnabled;

		// Mark as refresh vertical sync
		m_bChangeVerticalSync = bRefresh;
	}
}

void CM2VideoSettings::SetMultisampleAntiAliasing ( int iMultisampleAntiAliasing )
{
	_asm push iMultisampleAntiAliasing;
	_asm call FUNC_SetMultisampleAntiAliasing;

	// Store the value
	m_iMultisampleAntiAliasing = iMultisampleAntiAliasing;
}

void CM2VideoSettings::SetAnisotropicFiltering ( int iAnisotropicFiltering )
{
	_asm push iAnisotropicFiltering;
	_asm call FUNC_SetAnisotropicFiltering;

	// Store the value
	m_iAnisotropicFiltering = iAnisotropicFiltering;
}

void CM2VideoSettings::SetAmbientOcculsionEnabled ( bool bEnabled )
{
	_asm push bEnabled;
	_asm call FUNC_EnableAmbientOcclusion;

	// Store the value
	m_bAmbientOcculsionEnabled = bEnabled;
}

void CM2VideoSettings::SetAudioQuality ( bool bHighQuality )
{
	_asm push bHighQuality;
	_asm call FUNC_SetAudioQuality;
}

void CM2VideoSettings::SetSFXVolume ( int iVolume )
{
	float fVolume = ((float)iVolume / 100.0f);

	_asm push fVolume;
	_asm call FUNC_SetSFXVolume;
}

void CM2VideoSettings::SetVoiceVolume ( int iVolume )
{
	float fVolume = ((float)iVolume / 100.0f);

	_asm push fVolume;
	_asm call FUNC_SetVoicesVolume;
}

void CM2VideoSettings::SetMusicVolume ( int iVolume )
{
	float fVolume = ((float)iVolume / 100.0f);

	_asm push fVolume;
	_asm call FUNC_SetMusicVolume;
}

void CM2VideoSettings::SetRadioVolume ( int iVolume )
{
	float fVolume = ((float)iVolume / 100.0f);

	_asm push fVolume;
	_asm call FUNC_SetRadioVolume;
}