/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMenuSettings.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include "StdInc.h"

extern CCore				* pCore;

struct GameResolution
{
	unsigned int uiIndex;
	String strResoltion;
}
resolutions[] =
{
	{ 0, "640 x 480" },
	{ 1, "720 x 480" },
	{ 2, "720 x 576" },
	{ 3, "800 x 600" },
	{ 4, "1024 x 768" },
	{ 5, "1152 x 864" },
	{ 6, "1280 x 720" },
	{ 7, "1280 x 768" },
	{ 8, "1280 x 800" },
	{ 9, "1280 x 960" },
	{ 10, "1280 x 1024" },
	{ 11, "1360 x 768" },
	{ 12, "1360 x 1024" },
	{ 13, "1366 x 768" },
	{ 14, "1400 x 1050" },
	{ 15, "1440 x 900" },
	{ 16, "1600 x 900" },
	{ 17, "1600 x 1050" },
	{ 18, "1920 x 1080" }
};

struct GameAntiAliasing
{
	unsigned int uiIndex;
	String strAA;
}
AntiAliasing[] =
{
	{ 0, "Disabled" },
	{ 1, "1x" },
	{ 2, "2x" },
	{ 3, "3x" },
	{ 4, "4x" }
};

struct GameAnisotropicFiltering
{
	unsigned int uiIndex;
	String strAF;
}
AnisotropicFiltering[] =
{
	{ 0, "Disabled" },
	{ 1, "2x" },
	{ 2, "4x" },
	{ 3, "8x" },
	{ 4, "16x" }
};

struct GameAudioQuality
{
	unsigned int uiIndex;
	String strQuality;
}
AudioQuality[] =
{
	{ 0, "Normal" },
	{ 1, "High" }
};

CMenuSettings::CMenuSettings( CGUI_Impl * pGUI )
{
	// Store the gui pointer
	m_pGUI = pGUI;

	// Get the screen resolution
	float fX = pGUI->GetResolution().fX;
	float fY = pGUI->GetResolution().fY;

	// Create the main window
	m_pWindow = pGUI->CreateWnd( "Settings" );
	m_pWindow->SetSize( Vector2( 300, 275 ) );
	m_pWindow->SetPosition( Vector2( (fX / 2) - 150, (fY / 2) - 137.5 ) );
	m_pWindow->SetVisible( false );
	m_pWindow->SetAlwaysOnTop( true );
	m_pWindow->SetSizingEnabled( false );

	// Create the tab panel
	m_pTabs = pGUI->CreateTabPanel( m_pWindow );
	m_pTabs->SetSize( Vector2( 284, 215 ) );
	m_pTabs->SetPosition( Vector2( 8, 25 ) );

	// Create the save button
	m_pSave = pGUI->CreateButton( "Close", m_pWindow );
	m_pSave->SetSize( Vector2( 75, 20 ) );
	m_pSave->SetPosition( Vector2( 8, 245 ) );
	m_pSave->SetClickHandler( GUI_CALLBACK( &CMenuSettings::Event_OnSaveClick, this ) );

	// Create the tabs
	m_pMultiplayer = m_pTabs->CreateTab ( "Multiplayer" );
	m_pVideo = m_pTabs->CreateTab ( "Video" );
	m_pAudio = m_pTabs->CreateTab ( "Audio" );

	// Create the nickname edit label
	m_pNicknameLabel = pGUI->CreateLabel( "Nickname:", NULL, m_pMultiplayer );
	m_pNicknameLabel->SetSize( Vector2( 50, 25 ) );
	m_pNicknameLabel->SetPosition( Vector2( 15, 15 ) );

	// Create the nickname edit
	m_pNickname = pGUI->CreateEdit( "", m_pMultiplayer );
	m_pNickname->SetSize( Vector2( 100, 25 ) );
	m_pNickname->SetPosition( Vector2( 75, 15 ) );
	m_pNickname->SetMaxLength( MAX_PLAYER_NAME );

	// Create the connections value label
	m_pConnectionsValueLabel = pGUI->CreateLabel ( "Browser Sockets:", NULL, m_pMultiplayer );
	m_pConnectionsValueLabel->SetVisible ( false );
	//m_pConnectionsValueLabel->SetSize( Vector2( 100, 25 ) );
	//m_pConnectionsValueLabel->SetPosition( Vector2( 75, 15 ) );

	// Create the connections value
	m_pConnectionsValue = pGUI->CreateScrollBar ( true, m_pMultiplayer );
	m_pConnectionsValue->SetSize ( Vector2( 130, 20 ) );
	m_pConnectionsValue->SetPosition( Vector2( 90, 50 ) );
	m_pConnectionsValue->SetOnScrollHandler ( GUI_CALLBACK ( &CMenuSettings::Event_OnConnectionsScroll, this ) );
	m_pConnectionsValue->SetProperty ( "StepSize", "0.1" );

	// Create the resolution label
	m_pResolutionLabel = pGUI->CreateLabel( "Resolution:", NULL, m_pVideo );
	m_pResolutionLabel->SetSize( Vector2( 120, 25 ) );
	m_pResolutionLabel->SetPosition( Vector2( 15, 15 ) );

	// Create the resoultion combo box
	m_pResolutionCombo = pGUI->CreateComboBox ( "", m_pVideo );
	m_pResolutionCombo->SetSize ( Vector2 ( 128, 120 ) );
	m_pResolutionCombo->SetPosition( Vector2( 120, 15 ) );
	m_pResolutionCombo->SetReadOnly ( true );

	// Add the current resolution options to the combo box items
	for( int i = 0; i < ARRAY_LENGTH( resolutions ); i++ )
		m_pResolutionCombo->AddItem( resolutions[i].strResoltion.Get () )->SetData ( (void *)resolutions[i].uiIndex );

	// Create the MSAA label
	m_pMSAALabel = pGUI->CreateLabel( "Anti Aliasing:", NULL, m_pVideo );
	m_pMSAALabel->SetSize( Vector2( 120, 25 ) );
	m_pMSAALabel->SetPosition( Vector2( 15, 45 ) );

	// Create the MSAA combo box
	m_pMSAACombo = pGUI->CreateComboBox ( "", m_pVideo );
	m_pMSAACombo->SetSize ( Vector2 ( 128, 100 ) );
	m_pMSAACombo->SetPosition( Vector2( 120, 45 ) );
	m_pMSAACombo->SetReadOnly ( true );

	// Add the current MSAA options the combo box items
	for( int i = 0; i < ARRAY_LENGTH( AntiAliasing ); i++ )
		m_pMSAACombo->AddItem( AntiAliasing[i].strAA.Get () )->SetData ( (void *)AntiAliasing[i].uiIndex );

	// Create the Anisotropic Filtering label
	m_pASFLabel = pGUI->CreateLabel( "Anisotropic Filtering:", NULL, m_pVideo );
	m_pASFLabel->SetSize( Vector2( 120, 25 ) );
	m_pASFLabel->SetPosition( Vector2( 15, 75 ) );

	// Create the Anisotropic Filtering combo box
	m_pASFCombo = pGUI->CreateComboBox ( "", m_pVideo );
	m_pASFCombo->SetSize ( Vector2 ( 128, 100 ) );
	m_pASFCombo->SetPosition( Vector2( 120, 75 ) );
	m_pASFCombo->SetReadOnly ( true );

	// Add the current AF options the combo box items
	for( int i = 0; i < ARRAY_LENGTH( AnisotropicFiltering ); i++ )
		m_pASFCombo->AddItem( AnisotropicFiltering[i].strAF.Get () )->SetData ( (void *)AnisotropicFiltering[i].uiIndex );

	// Create the resolution fullscreen checkbox
	m_pResolutionFullscreen = pGUI->CreateCheckBox( "Fullscreen Mode", m_pVideo );
	m_pResolutionFullscreen->SetPosition( Vector2( 15, 110 ) );

	// Create the vertical sync checkbox
	m_pVerticalSync = pGUI->CreateCheckBox ( "Enable Vertical Sync", m_pVideo );
	m_pVerticalSync->SetPosition ( Vector2( 15, 135 ) );

	// Create the ambient occlusion checkbox
	m_pAmbientOcclusion = pGUI->CreateCheckBox ( "Enable Ambient Occlusion", m_pVideo );
	m_pAmbientOcclusion->SetSize ( Vector2( 150, 16 ) );
	m_pAmbientOcclusion->SetPosition ( Vector2( 15, 160 ) );

	// Create the audio quality label
	m_pAudioQualityLabel = pGUI->CreateLabel ( "Audio Quality:", NULL, m_pAudio );
	m_pAudioQualityLabel->SetSize( Vector2( 120, 25 ) );
	m_pAudioQualityLabel->SetPosition( Vector2( 15, 15 ) );

	// Create the audio quality combobox
	m_pAudioQuality = pGUI->CreateComboBox ( "", m_pAudio );
	m_pAudioQuality->SetSize ( Vector2 ( 128, 60 ) );
	m_pAudioQuality->SetPosition( Vector2( 90, 15 ) );
	m_pAudioQuality->SetReadOnly ( true );

	// Add the current audio quality options the combo box items
	for( int i = 0; i < ARRAY_LENGTH( AudioQuality ); i++ )
		m_pAudioQuality->AddItem( AudioQuality[i].strQuality.Get () )->SetData ( (void *)AudioQuality[i].uiIndex );

	// Create the audio SFX label
	m_pSFXLabel = pGUI->CreateLabel ( "SFX Volume:", NULL, m_pAudio );
	m_pSFXLabel->SetSize( Vector2( 120, 25 ) );
	m_pSFXLabel->SetPosition( Vector2( 15, 50 ) );

	// Create the audio SFX volume scrollbar
	m_pSFXVolume = pGUI->CreateScrollBar ( true, m_pAudio );
	m_pSFXVolume->SetSize ( Vector2( 130, 20 ) );
	m_pSFXVolume->SetPosition( Vector2( 90, 50 ) );
	m_pSFXVolume->SetOnScrollHandler ( GUI_CALLBACK ( &CMenuSettings::Event_OnSFXScroll, this ) );
	m_pSFXVolume->SetProperty ( "StepSize", "0.01" );

	// Create the audio SFX volume value label
	m_pSFXVolumeValue = pGUI->CreateLabel ( "", NULL, m_pAudio );
	m_pSFXVolumeValue->SetSize( Vector2( 120, 25 ) );
	m_pSFXVolumeValue->SetPosition( Vector2( 230, 50 ) );

	// Create the audio voices label
	m_pVoicesLabel = pGUI->CreateLabel ( "Voices Volume:", NULL, m_pAudio );
	m_pVoicesLabel->SetSize( Vector2( 120, 25 ) );
	m_pVoicesLabel->SetPosition( Vector2( 15, 80 ) );

	// Create the audio voices volume scrollbar
	m_pVoicesVolume = pGUI->CreateScrollBar ( true, m_pAudio );
	m_pVoicesVolume->SetSize ( Vector2( 130, 20 ) );
	m_pVoicesVolume->SetPosition( Vector2( 90, 80 ) );
	m_pVoicesVolume->SetOnScrollHandler ( GUI_CALLBACK ( &CMenuSettings::Event_OnVoicesScroll, this ) );
	m_pVoicesVolume->SetProperty ( "StepSize", "0.01" );

	// Create the audio voices volume value label
	m_pVoicesVolumeValue = pGUI->CreateLabel ( "", NULL, m_pAudio );
	m_pVoicesVolumeValue->SetSize( Vector2( 120, 25 ) );
	m_pVoicesVolumeValue->SetPosition( Vector2( 230, 80 ) );

	// Create the audio music label
	m_pMusicLabel = pGUI->CreateLabel ( "Music Volume:", NULL, m_pAudio );
	m_pMusicLabel->SetSize( Vector2( 120, 25 ) );
	m_pMusicLabel->SetPosition( Vector2( 15, 110 ) );

	// Create the audio music volume scrollbar
	m_pMusicVolume = pGUI->CreateScrollBar ( true, m_pAudio );
	m_pMusicVolume->SetSize ( Vector2( 130, 20 ) );
	m_pMusicVolume->SetPosition( Vector2( 90, 110 ) );
	m_pMusicVolume->SetOnScrollHandler ( GUI_CALLBACK ( &CMenuSettings::Event_OnMusicScroll, this ) );
	m_pMusicVolume->SetProperty ( "StepSize", "0.01" );

	// Create the audio music volume value label
	m_pMusicVolumeValue = pGUI->CreateLabel ( "", NULL, m_pAudio );
	m_pMusicVolumeValue->SetSize( Vector2( 120, 25 ) );
	m_pMusicVolumeValue->SetPosition( Vector2( 230, 110 ) );

	// Create the audio radio label
	m_pRadioLabel = pGUI->CreateLabel ( "Radio Volume:", NULL, m_pAudio );
	m_pRadioLabel->SetSize( Vector2( 120, 25 ) );
	m_pRadioLabel->SetPosition( Vector2( 15, 140 ) );

	// Create the audio music volume scrollbar
	m_pRadioVolume = pGUI->CreateScrollBar ( true, m_pAudio );
	m_pRadioVolume->SetSize ( Vector2( 130, 20 ) );
	m_pRadioVolume->SetPosition( Vector2( 90, 140 ) );
	m_pRadioVolume->SetOnScrollHandler ( GUI_CALLBACK ( &CMenuSettings::Event_OnRadioScroll, this ) );
	m_pRadioVolume->SetProperty ( "StepSize", "0.01" );

	// Create the audio music volume value label
	m_pRadioVolumeValue = pGUI->CreateLabel ( "", NULL, m_pAudio );
	m_pRadioVolumeValue->SetSize( Vector2( 120, 25 ) );
	m_pRadioVolumeValue->SetPosition( Vector2( 230, 140 ) );
}

CMenuSettings::~CMenuSettings( void )
{
	// Delete gui
	SAFE_DELETE( m_pConnectionsValueLabel );
	SAFE_DELETE( m_pConnectionsValue );
	SAFE_DELETE( m_pNickname );
	SAFE_DELETE( m_pNicknameLabel );
	SAFE_DELETE( m_pResolutionLabel );
	SAFE_DELETE( m_pResolutionCombo );
	SAFE_DELETE( m_pMSAALabel );
	SAFE_DELETE( m_pMSAACombo );
	SAFE_DELETE( m_pASFCombo );
	SAFE_DELETE( m_pASFCombo );
	SAFE_DELETE( m_pResolutionFullscreen );
	SAFE_DELETE( m_pVerticalSync );
	SAFE_DELETE( m_pAmbientOcclusion );
	SAFE_DELETE( m_pAudioQualityLabel );
	SAFE_DELETE( m_pAudioQuality );
	SAFE_DELETE( m_pSFXLabel );
	SAFE_DELETE( m_pSFXVolume );
	SAFE_DELETE( m_pSFXVolumeValue );
	SAFE_DELETE( m_pVoicesLabel );
	SAFE_DELETE( m_pVoicesVolume );
	SAFE_DELETE( m_pVoicesVolumeValue );
	SAFE_DELETE( m_pMusicLabel );
	SAFE_DELETE( m_pMusicVolume );
	SAFE_DELETE( m_pMusicVolumeValue );
	SAFE_DELETE( m_pRadioLabel );
	SAFE_DELETE( m_pRadioVolume );
	SAFE_DELETE( m_pRadioVolumeValue );

	// Delete the tabs
	SAFE_DELETE( m_pMultiplayer );
	SAFE_DELETE( m_pVideo );
	SAFE_DELETE( m_pAudio );

	// Delete the tab panel
	SAFE_DELETE( m_pTabs );

	// Delete the main window
	SAFE_DELETE( m_pWindow );
}

bool CMenuSettings::Event_OnSaveClick( CGUIElement_Impl * pElement )
{
	// Save the settings
	pCore->GetGUI()->GetSettings()->SaveSettings();

	// Hide the settings window
	SetVisible( false );

	return true;
}

void CMenuSettings::SetVisible( bool bVisible )
{
	// Toggle the main window visibility
	m_pWindow->SetVisible( bVisible );
	
	// Bring the window to the front
	m_pWindow->BringToFront();
}

bool CMenuSettings::IsVisilbe( void )
{
	return m_pWindow->IsVisible();
}

void CMenuSettings::LoadSettings( void )
{
	// Get the player nick
	const char * szNickname;
	CVAR_GET( "nick", &szNickname );

	// Set the player nickname
	pCore->SetNick( szNickname );

	// Update the nickname edit
	m_pNickname->SetText( szNickname );

	// Load the window dimensions
	m_iLoadedWidth = *(int *)0x1BAE8D8;
	m_iLoadedHeight = *(int *)0x1BAE8DC;

	// Load the fullscreen flag
	bool bFullScreenEnabled = *(bool *)0x1BAE8E0;

	// Load the vertical sync flag
	bool bVerticalSyncEnabled = *(bool *)0x1BAE8E4;

	// Update the resolution combo text
	m_pResolutionCombo->SetText ( String ( "%d x %d", m_iLoadedWidth, m_iLoadedHeight ) );

	// Loop over all available resolutions
	for( int i = 0; i < ARRAY_LENGTH( resolutions ); i++ )
	{
		// Is this the resolution we have?
		if ( !resolutions[i].strResoltion.Compare ( m_pResolutionCombo->GetText () ) )
		{
			//
			CM2VideoSettings::SetResolutionIndex ( resolutions[i].uiIndex, false );
			break;
		}
	}

	// Update the fullscreen checkbox
	m_pResolutionFullscreen->SetChecked( bFullScreenEnabled );

	// Update the vertical sync checkbox
	m_pVerticalSync->SetChecked ( bVerticalSyncEnabled );

	// Load settings that we can't save
	bool bAmbientOcclusion = CVAR_GET_BOOL ( "ambient-occlusion" );
	int iAntiAliasing = Math::Clamp< int > ( 0, CVAR_GET_INTEGER ( "multisample-anti-aliasing" ), 4 );
	int iAntisotropic = Math::Clamp< int > ( 0, CVAR_GET_INTEGER ( "anisotropic-filtering" ), 4 );

	// Update the ambient occlusion checkbox
	m_pAmbientOcclusion->SetChecked ( bAmbientOcclusion );

	// Loop over all available anti-aliasing's
	for( int i = 0; i < ARRAY_LENGTH( AntiAliasing ); i++ )
	{
		// Is this the AA we have?
		if ( AntiAliasing[i].uiIndex == iAntiAliasing )
		{
			//
			m_pMSAACombo->SetText ( AntiAliasing[i].strAA );
			break;
		}
	}

	// Loop over all available ASF's
	for( int i = 0; i < ARRAY_LENGTH( AnisotropicFiltering ); i++ )
	{
		// Is this the AA we have?
		if ( AnisotropicFiltering[i].uiIndex == iAntisotropic )
		{
			//
			m_pASFCombo->SetText ( AnisotropicFiltering[i].strAF );
			break;
		}
	}

	// Load audio settings
	int iAudioQuality = Math::Clamp< int > ( 0, CVAR_GET_INTEGER ( "audio-quality" ), 1 );
	int iSFXVolume = Math::Clamp< int > ( 0, CVAR_GET_INTEGER ( "sfx-volume" ), 100 );
	int iVoicesVolume = Math::Clamp< int > ( 0, CVAR_GET_INTEGER ( "voice-volume" ), 100 );
	int iMusicVolume = Math::Clamp< int > ( 0, CVAR_GET_INTEGER ( "music-volume" ), 100 );
	int iRadioVolume = Math::Clamp< int > ( 0, CVAR_GET_INTEGER ( "radio-volume" ), 100 );

	// Loop over all available ASF's
	for( int i = 0; i < ARRAY_LENGTH( AudioQuality ); i++ )
	{
		// Is this the AA we have?
		if ( AudioQuality[i].uiIndex == iAudioQuality )
		{
			//
			m_pAudioQuality->SetText ( AudioQuality[i].strQuality );
			break;
		}
	}

	// Update the sfx volume
	m_pSFXVolume->SetScrollPosition ( ((float)iSFXVolume / 100.0f) );
	m_pSFXVolumeValue->SetText ( String ( "%d%%", iSFXVolume ) );

	// Update the voices volume
	m_pVoicesVolume->SetScrollPosition ( ((float)iVoicesVolume / 100.0f) );
	m_pVoicesVolumeValue->SetText ( String ( "%d%%", iVoicesVolume ) );

	// Update the music volume
	m_pMusicVolume->SetScrollPosition ( ((float)iMusicVolume / 100.0f) );
	m_pMusicVolumeValue->SetText ( String ( "%d%%", iMusicVolume ) );

	// Update the radio volume
	m_pRadioVolume->SetScrollPosition ( ((float)iRadioVolume / 100.0f) );
	m_pRadioVolumeValue->SetText ( String ( "%d%%", iRadioVolume ) );

	// Update the video settings
	CM2VideoSettings::SetFullScreenEnabled ( bFullScreenEnabled, false );
	CM2VideoSettings::SetVerticalSyncEnabled  ( bVerticalSyncEnabled, false );
	CM2VideoSettings::SetMultisampleAntiAliasing ( iAntiAliasing );
	CM2VideoSettings::SetAnisotropicFiltering ( iAntisotropic );
	CM2VideoSettings::SetAmbientOcculsionEnabled ( bAmbientOcclusion );

	// Update the audio settings
	CM2VideoSettings::SetAudioQuality ( iAudioQuality );
	CM2VideoSettings::SetSFXVolume ( iSFXVolume );
	CM2VideoSettings::SetVoiceVolume ( iVoicesVolume );
	CM2VideoSettings::SetMusicVolume ( iMusicVolume );
	CM2VideoSettings::SetRadioVolume ( iRadioVolume );
}

void CMenuSettings::SaveSettings( void )
{
	// Get the player nick
	const char * szNickname;
	CVAR_GET( "nick", &szNickname );

	// Get the new player nick
	String strNewNick = m_pNickname->GetText();

	// Has the player nick changed?
	if( !strNewNick.IsEmpty() && strNewNick.Compare( szNickname ) )
	{
		// Write the new nick
		CVAR_SET( "nick", strNewNick.Get() );

		// Change the player nickname
		pCore->SetNick( strNewNick );

		// Set the localplayer nick
		if ( pCore->GetPlayerManager() && pCore->GetPlayerManager()->GetLocalPlayer() )
			pCore->GetPlayerManager()->GetLocalPlayer()->SetNick( strNewNick );

		// Are we connected to a server?
		if( pCore->GetNetworkModule()->IsConnected() )
		{
			// Construct a new bitstream
			RakNet::BitStream bsSend;

			// Write the new nickname
			bsSend.Write( RakNet::RakString( strNewNick.Get() ) );
			
			// Send it to the server
			pCore->GetNetworkModule()->Call( RPC_PLAYERCHANGENICK, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, true );

			// Is the client scripting manager active?
			if( pCore->GetClientScriptingManager() )
			{
				// Call the client event
				CSquirrelArguments args;
				args.push( strNewNick.Get() );
				args.push( szNickname );
				pCore->GetClientScriptingManager()->GetEvents()->Call( "onClientChangeNick", &args );
			}
		}
	}

	// Update the ambient occlusion
	CM2VideoSettings::SetAmbientOcculsionEnabled ( m_pAmbientOcclusion->IsChecked () );

	// Save video settings
	CVAR_SET ( "ambient-occlusion", m_pAmbientOcclusion->IsChecked () );

	// Did we select a resolution?
	if ( m_pResolutionCombo->GetSelectedItem() )
	{
		CLogFile::Printf ( "Updating res: %d", (int)m_pResolutionCombo->GetSelectedItem()->GetData () );

		// Update the current resolution index
		CM2VideoSettings::SetResolutionIndex ( (int)m_pResolutionCombo->GetSelectedItem()->GetData (), true );
	}

	// Did we select an MSAA?
	if ( m_pMSAACombo->GetSelectedItem() )
	{
		// Update the current MSAA value
		CM2VideoSettings::SetMultisampleAntiAliasing ( (int)m_pMSAACombo->GetSelectedItem()->GetData () );

		// Save the anti-aliasing value
		CVAR_SET ( "multisample-anti-aliasing", (int)m_pMSAACombo->GetSelectedItem()->GetData () );
	}

	// Did we select an AF?
	if ( m_pASFCombo->GetSelectedItem() )
	{
		// Update the current AF value
		CM2VideoSettings::SetAnisotropicFiltering ( (int)m_pASFCombo->GetSelectedItem()->GetData () );

		// Save the anisotropic filtering value
		CVAR_SET ( "anisotropic-filtering", (int)m_pASFCombo->GetSelectedItem()->GetData () );
	}

	// Update the fullscreen
	CM2VideoSettings::SetFullScreenEnabled ( m_pResolutionFullscreen->IsChecked (), true );

	// Update the vertical sync
	CM2VideoSettings::SetVerticalSyncEnabled ( m_pVerticalSync->IsChecked (), true );

	// Did we select an audio quality?
	if ( m_pAudioQuality->GetSelectedItem() )
	{
		// Update the audio quality
		CM2VideoSettings::SetAudioQuality ( (int)m_pAudioQuality->GetSelectedItem()->GetData () );

		// Save the audio quality
		CVAR_SET ( "audio-quality", (int)m_pAudioQuality->GetSelectedItem()->GetData () );
	}

	// Save audio settings
	CVAR_SET ( "sfx-volume", (int)(m_pSFXVolume->GetScrollPosition() * 100.0f) );
	CVAR_SET ( "voice-volume", (int)(m_pVoicesVolume->GetScrollPosition() * 100.0f) );
	CVAR_SET ( "music-volume", (int)(m_pMusicVolume->GetScrollPosition() * 100.0f) );
	CVAR_SET ( "radio-volume", (int)(m_pRadioVolume->GetScrollPosition() * 100.0f) );
}

bool CMenuSettings::Event_OnSFXScroll ( CGUIElement_Impl * pElement )
{
	// Get the scroll position
	float fVolume = (m_pSFXVolume->GetScrollPosition() * 100.0f);

	// Update the volume value label
	m_pSFXVolumeValue->SetText ( String ( "%d%%", (int)fVolume ) );

	// Update the SFX volume
	CM2VideoSettings::SetSFXVolume ( (int)fVolume );
	return true;
}

bool CMenuSettings::Event_OnVoicesScroll ( CGUIElement_Impl * pElement )
{
	// Get the scroll position
	float fVolume = (m_pVoicesVolume->GetScrollPosition() * 100.0f);

	// Update the volume value label
	m_pVoicesVolumeValue->SetText ( String ( "%d%%", (int)fVolume ) );

	// Update the voices volume
	CM2VideoSettings::SetVoiceVolume ( (int)fVolume );
	return true;
}

bool CMenuSettings::Event_OnMusicScroll ( CGUIElement_Impl * pElement )
{
	// Get the scroll position
	float fVolume = (m_pMusicVolume->GetScrollPosition() * 100.0f);

	// Update the volume value label
	m_pMusicVolumeValue->SetText ( String ( "%d%%", (int)fVolume ) );

	// Update the music volume
	CM2VideoSettings::SetMusicVolume ( (int)fVolume );
	return true;
}

bool CMenuSettings::Event_OnRadioScroll ( CGUIElement_Impl * pElement )
{
	// Get the scroll position
	float fVolume = (m_pRadioVolume->GetScrollPosition() * 100.0f);

	// Update the volume value label
	m_pRadioVolumeValue->SetText ( String ( "%d%%", (int)fVolume ) );

	// Update the radio volume
	CM2VideoSettings::SetRadioVolume ( (int)fVolume );
	return true;
}

bool CMenuSettings::Event_OnConnectionsScroll ( CGUIElement_Impl * pElement )
{
	// Get the scroll position
	//float fVolume = (m_pConnectionsValue->GetScrollPosition() * 100.0f);

	// Update the volume value label
	//m_pRadioVolumeValue->SetText ( String ( "%d%%", (int)fVolume ) );

	// Update the radio volume
	//CM2VideoSettings::SetRadioVolume ( (int)fVolume );
	return true;
}

void CMenuSettings::OnScreenSizeChange ( float fX, float fY )
{
	// Adjust the window position
	m_pWindow->SetPosition( Vector2( (fX / 2) - 150, (fY / 2) - 137.5 ) );
}