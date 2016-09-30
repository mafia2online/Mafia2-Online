/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2VideoSettings.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CM2VideoSettings.h"

#include "CLogFile.h"

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
	if ( m_bChangeResolution )
	{
		m_bChangeResolution = false;

		DEBUG_LOG ( "Changing res to %d...", m_iResolutionIndex );

		int iResolutionIndex = m_iResolutionIndex;

		_asm
		{
			push iResolutionIndex;
			call FUNC_SetScreenResolution;
		}

		DEBUG_LOG ( "Done!" );
	}

	if ( m_bChangeFullScreen )
	{
		m_bChangeFullScreen = false;

		DEBUG_LOG ( "Setting fullscreen" );

		DWORD FUNC_C_SystemRenderingModule__Singleton = 0xB2CBA0;
		bool bFullScreenEnabled = m_bFullScreenEnabled;
		_asm
		{
			push bFullScreenEnabled;
			call FUNC_C_SystemRenderingModule__Singleton;
			mov ecx, eax;
			call FUNC_C_SystemRenderingModule__SetFullScreenEnabled;
		}

		DEBUG_LOG ( "Done!" );
	}

	if ( m_bChangeVerticalSync )
	{
		DWORD FUNC_C_SystemRenderingModule__Singleton = 0xB2CBA0;
		bool bVerticalSyncEnabled = m_bVerticalSyncEnabled;
		_asm
		{
			push bVerticalSyncEnabled;
			call FUNC_C_SystemRenderingModule__Singleton;
			mov ecx, eax;
			call FUNC_C_SystemRenderingModule__SetVerticalSyncEnabled;
		}

		m_bChangeVerticalSync = false;
	}
}

void CM2VideoSettings::SetResolutionIndex ( int iIndex, bool bRefresh )
{
	if ( iIndex != m_iResolutionIndex )
	{
		m_iResolutionIndex = iIndex;

		m_bChangeResolution = bRefresh;
	}
}

void CM2VideoSettings::SetFullScreenEnabled ( bool bEnabled, bool bRefresh )
{
	if ( bEnabled != m_bFullScreenEnabled )
	{
		m_bFullScreenEnabled = bEnabled;

		m_bChangeFullScreen = bRefresh;
	}
}

void CM2VideoSettings::SetVerticalSyncEnabled ( bool bEnabled, bool bRefresh )
{
	if ( bEnabled != m_bVerticalSyncEnabled )
	{
		m_bVerticalSyncEnabled = bEnabled;

		m_bChangeVerticalSync = bRefresh;
	}
}

void CM2VideoSettings::SetMultisampleAntiAliasing ( int iMultisampleAntiAliasing )
{
	_asm
	{
		push iMultisampleAntiAliasing;
		call FUNC_SetMultisampleAntiAliasing;
	}

	// Store the value
	m_iMultisampleAntiAliasing = iMultisampleAntiAliasing;
}

void CM2VideoSettings::SetAnisotropicFiltering ( int iAnisotropicFiltering )
{
	_asm
	{
		push iAnisotropicFiltering;
		call FUNC_SetAnisotropicFiltering;
	}

	// Store the value
	m_iAnisotropicFiltering = iAnisotropicFiltering;
}

void CM2VideoSettings::SetAmbientOcculsionEnabled ( bool bEnabled )
{
	_asm
	{
		push bEnabled;
		call FUNC_EnableAmbientOcclusion;
	}

	// Store the value
	m_bAmbientOcculsionEnabled = bEnabled;
}

void CM2VideoSettings::SetAudioQuality ( bool bHighQuality )
{
	_asm
	{
		push bHighQuality;
		call FUNC_SetAudioQuality;
	}
}

void CM2VideoSettings::SetSFXVolume ( int iVolume )
{
	float fVolume = ((float)iVolume / 100.0f);

	_asm
	{
		push fVolume;
		call FUNC_SetSFXVolume;
	}
}

void CM2VideoSettings::SetVoiceVolume ( int iVolume )
{
	float fVolume = ((float)iVolume / 100.0f);

	_asm
	{
		push fVolume;
		call FUNC_SetVoicesVolume;
	}
}

void CM2VideoSettings::SetMusicVolume ( int iVolume )
{
	float fVolume = ((float)iVolume / 100.0f);

	_asm
	{
		push fVolume;
		call FUNC_SetMusicVolume;
	}
}

void CM2VideoSettings::SetRadioVolume ( int iVolume )
{
	float fVolume = ((float)iVolume / 100.0f);

	_asm
	{
		push fVolume;
		call FUNC_SetRadioVolume;
	}
}