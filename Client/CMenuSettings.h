/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CMenuSettings.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class CMenuSettings
{

private:

	CGUI_Impl										* m_pGUI;
	CGUIWindow_Impl									* m_pWindow;
	CGUITabPanel_Impl								* m_pTabs;
	CGUITab_Impl									* m_pMultiplayer;
	CGUITab_Impl									* m_pVideo;
	CGUITab_Impl									* m_pAudio;

	CGUILabel_Impl									* m_pNicknameLabel;
	CGUIEdit_Impl									* m_pNickname;
	CGUIButton_Impl									* m_pSave;

	CGUIScrollBar_Impl								* m_pConnectionsValue;
	CGUILabel_Impl									* m_pConnectionsValueLabel;

	CGUILabel_Impl									* m_pResolutionLabel;
	CGUIComboBox_Impl								* m_pResolutionCombo;

	CGUILabel_Impl									* m_pMSAALabel;
	CGUIComboBox_Impl								* m_pMSAACombo;

	CGUILabel_Impl									* m_pASFLabel;
	CGUIComboBox_Impl								* m_pASFCombo;

	CGUICheckBox_Impl								* m_pResolutionFullscreen;
	CGUICheckBox_Impl								* m_pVerticalSync;
	CGUICheckBox_Impl								* m_pAmbientOcclusion;

	CGUILabel_Impl									* m_pAudioQualityLabel;
	CGUIComboBox_Impl								* m_pAudioQuality;

	CGUILabel_Impl									* m_pSFXLabel;
	CGUIScrollBar_Impl								* m_pSFXVolume;
	CGUILabel_Impl									* m_pSFXVolumeValue;

	CGUILabel_Impl									* m_pVoicesLabel;
	CGUIScrollBar_Impl								* m_pVoicesVolume;
	CGUILabel_Impl									* m_pVoicesVolumeValue;

	CGUILabel_Impl									* m_pMusicLabel;
	CGUIScrollBar_Impl								* m_pMusicVolume;
	CGUILabel_Impl									* m_pMusicVolumeValue;

	CGUILabel_Impl									* m_pRadioLabel;
	CGUIScrollBar_Impl								* m_pRadioVolume;
	CGUILabel_Impl									* m_pRadioVolumeValue;

	int												m_iLoadedWidth;
	int												m_iLoadedHeight;

	bool				Event_OnSaveClick			( CGUIElement_Impl * pElement );

	bool				Event_OnSFXScroll			( CGUIElement_Impl * pElement );
	bool				Event_OnVoicesScroll		( CGUIElement_Impl * pElement );
	bool				Event_OnMusicScroll			( CGUIElement_Impl * pElement );
	bool				Event_OnRadioScroll			( CGUIElement_Impl * pElement );

	bool				Event_OnConnectionsScroll	( CGUIElement_Impl * pElement );

public:

						CMenuSettings				( CGUI_Impl * pGUI );
						~CMenuSettings				( void );

	void				SetVisible					( bool bVisible );
	bool				IsVisilbe					( void );

	void				LoadSettings				( void );
	void				SaveSettings				( void );

	void				OnScreenSizeChange			( float fX, float fY );

};