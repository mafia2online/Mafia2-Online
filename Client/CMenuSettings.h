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

	CGUI_Impl										*m_pGUI;
	std::shared_ptr<CGUIWindow_Impl>				m_pWindow;
	std::shared_ptr<CGUITabPanel_Impl>				m_pTabs;
	std::shared_ptr<CGUITab_Impl>					m_pMultiplayer;
	std::shared_ptr<CGUITab_Impl>					m_pVideo;
	std::shared_ptr<CGUITab_Impl>					m_pAudio;
	std::shared_ptr<CGUITab_Impl>					m_pAuth;

	std::shared_ptr<CGUILabel_Impl>					m_pNicknameLabel;
	std::shared_ptr<CGUIEdit_Impl>					m_pNickname;
	std::shared_ptr<CGUIButton_Impl>				m_pSave;

	std::shared_ptr<CGUILabel_Impl>					m_pM2OUsernameLabel;
	std::shared_ptr<CGUILabel_Impl>					m_pM2OPasswordLabel;
	std::shared_ptr<CGUIEdit_Impl>					m_pM2OUsername;
	std::shared_ptr<CGUIEdit_Impl>					m_pM2OPassword;
	std::shared_ptr<CGUIButton_Impl>				m_pLogin;
	std::shared_ptr<CGUIButton_Impl>				m_pLogout;
	std::shared_ptr<CGUICheckBox_Impl>				m_pRememberMe;

	std::shared_ptr<CGUIScrollBar_Impl>				m_pConnectionsValue;
	std::shared_ptr<CGUILabel_Impl	>				m_pConnectionsValueLabel;

	std::shared_ptr<CGUILabel_Impl>					m_pResolutionLabel;
	std::shared_ptr<CGUIComboBox_Impl>				m_pResolutionCombo;

	std::shared_ptr<CGUILabel_Impl>					m_pMSAALabel;
	std::shared_ptr<CGUIComboBox_Impl>				m_pMSAACombo;

	std::shared_ptr<CGUILabel_Impl>					m_pASFLabel;
	std::shared_ptr<CGUIComboBox_Impl>				m_pASFCombo;

	std::shared_ptr<CGUICheckBox_Impl>				m_pResolutionFullscreen;
	std::shared_ptr<CGUICheckBox_Impl>				m_pVerticalSync;
	std::shared_ptr<CGUICheckBox_Impl>				m_pAmbientOcclusion;

	std::shared_ptr<CGUILabel_Impl>					m_pAudioQualityLabel;
	std::shared_ptr<CGUIComboBox_Impl>				m_pAudioQuality;

	std::shared_ptr<CGUILabel_Impl>					m_pSFXLabel;
	std::shared_ptr<CGUIScrollBar_Impl>				m_pSFXVolume;
	std::shared_ptr<CGUILabel_Impl>					m_pSFXVolumeValue;

	std::shared_ptr<CGUILabel_Impl>					m_pVoicesLabel;
	std::shared_ptr<CGUIScrollBar_Impl>				m_pVoicesVolume;
	std::shared_ptr<CGUILabel_Impl>					m_pVoicesVolumeValue;

	std::shared_ptr<CGUILabel_Impl>					m_pMusicLabel;
	std::shared_ptr<CGUIScrollBar_Impl>				m_pMusicVolume;
	std::shared_ptr<CGUILabel_Impl>					m_pMusicVolumeValue;

	std::shared_ptr<CGUILabel_Impl>					m_pRadioLabel;
	std::shared_ptr<CGUIScrollBar_Impl>				m_pRadioVolume;
	std::shared_ptr<CGUILabel_Impl>					m_pRadioVolumeValue;

	int												m_iLoadedWidth;
	int												m_iLoadedHeight;

	bool				Event_OnSaveClick			( CGUIElement_Impl * pElement );

	bool				Event_OnSFXScroll			( CGUIElement_Impl * pElement );
	bool				Event_OnVoicesScroll		( CGUIElement_Impl * pElement );
	bool				Event_OnMusicScroll			( CGUIElement_Impl * pElement );
	bool				Event_OnRadioScroll			( CGUIElement_Impl * pElement );

	bool				Event_OnConnectionsScroll	( CGUIElement_Impl * pElement );

	bool				Event_OnClickLogin			( CGUIElement_Impl * pElement );
	bool				Event_OnClickLogout			( CGUIElement_Impl * pElement );

public:

						CMenuSettings				( CGUI_Impl * pGUI );
						~CMenuSettings				( void );

	void				SetVisible					( bool bVisible );
	bool				IsVisilbe					( void );

	void				LoadSettings				( void );
	void				SaveSettings				( void );

	void				OnScreenSizeChange			( float fX, float fY );

};