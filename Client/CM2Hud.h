/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CM2Hud.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

class M2HudSpeedo
{
public:
	PAD(M2HudSpeedo, pad0, 0x74);			// 0000 - 0074
	bool m_bIsShowing;						// 0074 - 0075
};

class M2HudTimer
{
public:
	PAD(M2HudTimer, pad0, 0x40);			// 0000 - 0040
	void * m_pHudComponent;					// 0040 - 0044
	PAD(M2HudTimer, pad1, 0x4);				// 0044 - 0048
	float m_fTime;							// 0048 - 004C
	float m_fRemainingTime;					// 004C - 0050
	PAD(M2HudTimer, pad2, 0x4);				// 0050 - 0054
	bool m_bIsRunning;						// 0054 - 0055
	bool m_bIsStopped;						// 0055 - 0056
};

class M2HudFader
{
public:
	PAD(M2HudFader, pad0, 0x175C);			// 0000 - 175C
	void * m_pScreen;						// 175C - 1760
};

class M2HudMessageArea
{
public:
	PAD(M2HudMessageArea, pad0, 0x3DC);		// 0000 - 03DC
	void* m_pActionButtons;					// 03DC - 03E0
	PAD(M2HudMessageArea, pad1, 0x4);		// 03E0 - 03E4
	void* m_pMessageArea;					// 03E4 - 03E8
	void* m_pMessageArea2;					// 03E8 - 03EC
	PAD(M2HudMessageArea, pad2, 0x4);		// 03EC - 03F0
	void* m_pInfoArea;						// 03F0 - 03F4
};

class M2HudHealthFX
{
public:
	PAD(M2HudLowHealthFX, pad0, 0x5D);		// 0000 - 005D
	bool m_bShow;							// 005D - 005E
};

// 0x1BA71F8 = VAR_Hud
class M2Hud
{
public:
	// 0x50 = (M2HudSpeedo *)m_pSpeedo;
	//	+ 0x30 = Unknown
	//		+ 0x156 = Unknown
	//	+ 0x74 = (bool)m_bShowing;
	// 0x54 = (M2HudMessageArea *)m_pMessageArea;
	//	+ 0x3DC = action buttons
	//	+ 0x3E4 = message area
	//	+ 0x3E8 = message area
	//	+ 0x3EC = ???
	//	+ 0x3F0 = info area
	// 0x58 = (M2HudRadar *)m_pRadar;
	// 0x5C = Money
	//	+ 0x30 = Current money shown
	//	+ 0x38 = Old money shown
	// 0x6C = (M2HudTimer *)m_pTimer;
	// 0x78 = ???
	// 0x84 = (void *)m_pWantedLevel; sub_883860( m_pWantedLevel, iWantedLevel, 1.0, 1.0 )
	// 0x8C = (void *)m_pLowHealthFX;
	//	+ 0x5D = (bool)m_bShow;
	// 0x94 = Inventory
	// 0x9C = (M2HudScore *)m_pScore;


	PAD(M2Hud, pad0, 0x50);					// 0000 - 0050
	M2HudSpeedo* m_pSpeedo;					// 0050 - 0054
	M2HudMessageArea* m_pMessageArea;		// 0054 - 0058
	void* m_pRadar;							// 0058 - 005C
	void* m_pMoneyArea;						// 005C - 0060
	PAD(M2Hud, pad1, 0xC);					// 0060 - 006C
	M2HudTimer* m_pTimer;					// 006C - 0070
	PAD(M2Hud, pad2, 0x8);					// 0070 - 0078
	void* m_pSubtitles;						// 0078 - 007C
	PAD(M2Hud, pad3, 0x8);					// 007C - 0084
	void* m_pWantedLevel;					// 0084 - 008C (883860(m_pWantedLevel, int iLevel, float 1.0, float 1.0);)
	M2HudHealthFX* m_pHealthFX;				// 008C - 0090
	PAD(M2Hud, pad4, 0x4);					// 0090 - 0094
	void* m_pWeaponInventory;				// 0094 - 0098
	PAD(M2Hud, pad5, 0x4);					// 0098 - 009C
	void* m_pScore;							// 009C - 00A0
};

class CM2HudTimer
{

private:

	M2HudTimer			* m_pHudTimer;

public:

	CM2HudTimer( M2HudTimer * pHudTimer );
	~CM2HudTimer( void );

	void				SetHudTimer( M2HudTimer * pHudTimer ) { m_pHudTimer = pHudTimer; }
	M2HudTimer			* GetHudTimer( void ) { return m_pHudTimer; }

	void				Toggle( bool bToggle );
	void				SetTime( float fTime );
	float				GetTime( void );
	float				GetRemainingTime( void );
	void				Start( void );
	void				Stop( void );
	bool				IsRunning( void );
	bool				IsStopped( void );

};

class CM2Hud
{

private:

	M2Hud				* m_pHud;
	CM2HudTimer			* m_pHudTimer;
	M2HudFader			* m_pHudFader;

	bool				m_bShowing;

public:

	CM2Hud( M2Hud * pHud );
	~CM2Hud( void );

	void				SetHud( M2Hud * pHud ) { m_pHud = pHud; }
	M2Hud				* GetHud( void ) { return m_pHud; }

	void				SetHudTimer( CM2HudTimer * pHudTimer ) { m_pHudTimer = pHudTimer; }
	CM2HudTimer			* GetHudTimer( void ) { return m_pHudTimer; }

	void				SetHudFader( M2HudFader * pHudFader ) { m_pHudFader = pHudFader; }
	M2HudFader			* GetHudFader( void ) { return m_pHudFader; }

	void				FadeIn( float fTime );
	void				FadeOut( float fTime );

	void				Show( bool bShow );
	bool				IsShowing ( void ) { return m_bShowing; }

	void				RadarShow( bool bShow );
	void				ScoreShow( bool bShow );
	void				SubtitlesShow( bool bShow );
	void				ActionButtonsShow( bool bShow );
	void				MessageAreaShow( bool bShow );
	void				InfoAreaShow( bool bShow );
	void				ShowLowHealthFX( bool bShow );
	void				SpeedoShow( bool bShow );

};