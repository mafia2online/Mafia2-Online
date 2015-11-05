/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPatches.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			 : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#include		"StdInc.h"

extern	CCore			* pCore;
sub_410440				CPatches::onGameInit = NULL;
onGameEvent_t			CPatches::onGameEvent = NULL;
ProcessEntities_t		CPatches::processEntities = NULL;
OnGameProcessStart_t	CPatches::onGameProcessStart = NULL;
bool					bProcessRespawn = false;
bool					bFirstLoad = true;

typedef void			( __thiscall * C_Player__ProcessKeyboard_t )	( M2Entity * pEntity, int a2, int a3, float a4 );
C_Player__ProcessKeyboard_t C_Player__ProcessKeyboard = NULL;

M2Ped * pPed = NULL;
DWORD sub_98B630_jmp = 0x98B63C;
DWORD sub_98B630_edx = 0x0;
//triggered with:
//	0xCC8F80 = Garage enter
//  0x4347B0 = Water enter
//  0x98BF90 = Passenger exit
void __declspec( naked ) sub_98B630( void )
{
	_asm mov eax, [ecx+0A0h];
	_asm mov pPed, eax;
	_asm push ebp;
	_asm mov ebp, esp;
	_asm mov eax, [ebp+4];
	_asm mov sub_98B630_edx, eax;
	_asm pop ebp;
	_asm pushad;

	// Is the playermanager and localplayer active?
	if( pCore->GetPlayerManager () && pCore->GetPlayerManager()->GetLocalPlayer () )
	{
		// hacky
		if( sub_98B630_edx == 0x4348F7 && pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned() && pPed == pCore->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed()->GetPed() )
		{
			// Flag the localplayer for fast exit vehicle
			pCore->GetPlayerManager()->GetLocalPlayer()->FlagForFastExitVehicle( true );

			// Handle the water enter with the localplayer
			pCore->GetPlayerManager()->GetLocalPlayer()->OnEnterWater();
		}
	}

	_asm popad;
	_asm sub esp, 18h;
	_asm push esi;
	_asm mov esi, ecx;
	_asm mov ecx, [esi+88h];
	_asm jmp sub_98B630_jmp;
}

// crashy!
DWORD HOOK_CDoor__OnUse__JMP = 0x4DB43B;
DWORD HOOK_CDoor__OnUse__ECX = 0x0;
void __declspec ( naked ) HOOK_CDoor__OnUse ( void )
{
	_asm mov HOOK_CDoor__OnUse__ECX, ecx;
	_asm pushad;

	// Is the playermanager and localplayer active?
	if( pCore->GetPlayerManager () && pCore->GetPlayerManager()->GetLocalPlayer () && pCore->GetPlayerManager()->GetLocalPlayer()->IsSpawned () )
	{

		CLogFile::Printf ( "HOOK_CDoor__OnUse ( 0x%p )", HOOK_CDoor__OnUse__ECX );
		pCore->GetChat()->AddDebugMessage ( "HOOK_CDoor__OnUse ( 0x%p )", HOOK_CDoor__OnUse__ECX );
		CM2Door * pDoor = *(CM2Door **) HOOK_CDoor__OnUse__ECX;

		if ( pDoor )
		{
			CLogFile::Printf ( "Is door closed? %s", (pDoor->IsClosed () ? "Yes" : "No") );
			pCore->GetChat()->AddDebugMessage ( "Is door closed? %s", (pDoor->IsClosed () ? "Yes" : "No") );
			//pDoor->GetHash ();
		}

	}

	_asm popad;
	_asm push ebx;
	_asm push ebp;
	_asm push esi;
	_asm mov esi, ecx;
	_asm mov eax, [esi+320h];
	_asm jmp HOOK_CDoor__OnUse__JMP;
}

const char * szStreamMapLine = "";
DWORD C_SDSManager__ActivateStreamMapLine_JMP = 0x5BF637;
DWORD C_SDSManager__ActivateStreamMapLine_END = 0x05BF921;
DWORD _this_ebx = 0x0;
bool end = false;
void __declspec ( naked ) HOOK_C_SDSManager__ActivateStreamMapLine ( void )
{
	_asm push ebp;
	_asm mov ebp, esp;
	_asm mov eax, [ebp+4];
	_asm mov _this_ebx, eax;
	_asm mov eax, [ebp+12];
	_asm mov szStreamMapLine, eax;
	_asm pop ebp;
	_asm pushad;

	CLogFile::Printf ( "HOOK_C_SDSManager__ActivateStreamMapLine < 0x%p > () - Requested to load '%s'", _this_ebx, szStreamMapLine );

	/*if(!end)
	{
		end=true;*/
		_asm popad;
		_asm sub esp, 2Ch;
		_asm push ebx;
		_asm push ebp;
		_asm push esi;
		_asm push edi;
		_asm jmp C_SDSManager__ActivateStreamMapLine_JMP;
	/*}
	else
	{
		_asm popad;
		_asm jmp C_SDSManager__ActivateStreamMapLine_END;
	}*/

	// Allowed stream map lines
	//if ( !strcmp ( szStreamMapLine, "game_main_load" ) || !strcmp ( szStreamMapLine, "free_area_state" ) || !strcmp ( szStreamMapLine, "freeraid_main" ) || !strcmp ( szStreamMapLine, "free_joe_load" ) || !strcmp ( szStreamMapLine, "free_summer_load" ) )
}

void C_Vehicle__OnFuelTankShot ( void )
{
	pCore->GetChat()->AddDebugMessage ( "BRO U SHOT THE FUEL TANK!" );
}

int iiTargetSeat;
DWORD C_Vehicle__PlayerStartEnter__JMP = 0x953B26;
void __declspec(naked) C_Vehicle__PlayerStartEnter ( void )
{
	_asm push ebp;
	_asm mov ebp, esp;
	_asm mov eax, [ebp+12];
	_asm mov iiTargetSeat, eax;
	_asm pop ebp;
	_asm pushad;

#ifdef DEBUG
	pCore->GetChat()->AddDebugMessage ( "C_Vehicle__PlayerStartEnter - Seat: %d", iiTargetSeat );
#endif

	_asm popad;
	_asm sub esp, 1Ch;
	_asm push esi;
	_asm mov esi, ecx;
	_asm jmp C_Vehicle__PlayerStartEnter__JMP;
}

DWORD C_Human__TakeDamage_JMP = 0x97EE6F;
DWORD C_Human__TakeDamage_End = 0x97F392;
M2Ped *pVehicleCrashPlayer, *pLocalPedddddd;
CLocalPlayer * ppLocalPlayer;
void __declspec(naked) C_Human__TakeDamage ( void )
{
	_asm mov pVehicleCrashPlayer, ecx;
	_asm pushad;

	// Get the localplayer ped
	ppLocalPlayer = pCore->GetPlayerManager()->GetLocalPlayer();
	pLocalPedddddd = ppLocalPlayer->GetPlayerPed()->GetPed();

	if ( pVehicleCrashPlayer == pLocalPedddddd )
	{
		// Is not driver of vehicle don't process health changes
		if (  ppLocalPlayer && ppLocalPlayer->IsInVehicle () && ppLocalPlayer->GetSeat () != 0 )
		{
			// TODO: Check that this is actually a vehicle impact causing the damage

			_asm popad;
			_asm sub     esp, 34h;
			_asm push	 ebp;
			_asm jmp	 C_Human__TakeDamage_End;
		}
		else
			goto end;
	}
	else
		goto end;

end:
	// Call the localplayer damage function
	if ( ppLocalPlayer->OnTakeDamage () )
	{
		_asm popad;
		_asm sub     esp, 34h;
		_asm push    ebp;
		_asm mov     ebp, ecx;
		_asm mov     eax, [ebp+0];
		_asm mov     edx, [eax+0BCh];
		_asm jmp	 C_Human__TakeDamage_JMP;
	}
	else
	{
		_asm popad;
		_asm sub     esp, 34h;
		_asm push	 ebp;
		_asm jmp	 C_Human__TakeDamage_End;
	}
}

DWORD HOOK_C_Game__OnGameLoad__JMP = 0x587A25;
void __declspec ( naked ) HOOK_C_Game__OnGameLoad ( void )
{
	_asm pushad;

	// Call the game load handler
	pCore->OnGameLoad ();

	_asm popad;
	_asm push ebx;
	_asm mov ebx, ecx;
	_asm mov eax, [ebx];
	_asm jmp HOOK_C_Game__OnGameLoad__JMP;
}

void __fastcall HOOK_C_Player__ProcessKeyboard ( M2Entity * pEntity, void * _EDX, int a2, int a3, float a4 )
{
	// If this ped is the localplayer, process keyboard controls
	if ( pEntity == IE::GetGame()->m_pLocalPed )
		C_Player__ProcessKeyboard ( pEntity, a2, a3, a4 );
}

void CPatches::Initialise( void )
{
	CLogFile::Printf( "Installing patches..." );

	// Unprotect the .text segment
	CPatcher::Unprotect( (pCore->GetBaseAddress() + 0x400000 + 0x1000), 0x94C000 );

	// Hook C_Game__OnGameInit
	onGameInit = (sub_410440) CPatcher::InstallJmpPatch( COffsets::FUNC_CGame__OnGameInit, (DWORD)HOOK_CGame__OnGameInit );

	// Hook C_Game__OnGameEvent
	onGameEvent = (onGameEvent_t) CPatcher::InstallJmpPatch( COffsets::FUNC_CGame__OnGameEvent, (DWORD)HOOK_CGame__OnGameEvent );

	// Hook C_EntMgr__ProcessEntities
	processEntities = (ProcessEntities_t) CPatcher::InstallJmpPatch( COffsets::FUNC_CEntMgr__ProcessEntities, (DWORD)HOOK_CEntMgr__ProcessEntities, 0x6 );

	// Hook OnGameProcessStart
	onGameProcessStart = ( OnGameProcessStart_t ) CPatcher::InstallJmpPatch( COffsets::FUNC_OnGameProcessStart, (DWORD)HOOK_OnGameProcessStart, 0x6 );

	//
	//CPatcher::InstallJmpPatch ( 0x5BD0C0, (DWORD)InitialiseGame );
	CPatcher::InstallJmpPatch ( 0x587A20, (DWORD)HOOK_C_Game__OnGameLoad );
	//CPatcher::InstallJmpPatch ( 0x5B1C90, (DWORD)C_Game__OnGameUnload );

	// Hook to prevent remote player peds following the localplayer keyboard controls
	C_Player__ProcessKeyboard = (C_Player__ProcessKeyboard_t) CPatcher::InstallJmpPatch ( 0x42A9E0, (DWORD)HOOK_C_Player__ProcessKeyboard );
	CPatcher::InstallNopPatch ( 0x4387A3, 0x86 ); // Fix crash - NEED TO LOOK AT THIS MORE (CAUSES CRASH WITH LOCALPLAYER WHEN JUMPING)

	// Hook C_HumanInventory__DoShot
	CPatcher::InstallJmpPatch ( COffsets::FUNC_CHumanInventory__ProcessShot, (DWORD)HOOK_CHumanInventory__DoShot );

	// Hook C_HumanInventory__DoReload
	CPatcher::InstallJmpPatch( 0x958140, (DWORD)HOOK_CHumanInventory__DoReload );

	// Hook C_Human__TakeDamage
	CPatcher::InstallJmpPatch ( 0x97EE60, (DWORD)C_Human__TakeDamage );

	// Vehicle enter stuff
	CPatcher::InstallJmpPatch ( 0x953B20, (DWORD)C_Vehicle__PlayerStartEnter );

	// Try disable player moving to driver seat after entering from passenger side
	CPatcher::InstallNopPatch ( 0x4CEBCF, 0xC );
	CPatcher::InstallNopPatch ( 0x4CEBEA, 0xA );
	CPatcher::InstallNopPatch ( 0x4CEC00, 0xA );

	// Hook for when ped enters water (It's function that finds X, Y, Z coord for some things)
	CPatcher::InstallJmpPatch( 0x98B630, (DWORD)sub_98B630 );

	// Hook CDoor__OnUse
	//CPatcher::InstallJmpPatch ( 0x4DB430, (DWORD)HOOK_CDoor__OnUse );

	// Hook C_SDSManager__ActivateStreamMapLine (Game loading missions isn't triggered by this!)
	//CPatcher::InstallJmpPatch ( 0x5BF630, (DWORD)HOOK_C_SDSManager__ActivateStreamMapLine );
	//CPatcher::InstallNopPatch ( 0xAC88F7, 0xF );

	// Always use CVector3 over M2Entity in C_AICommandAimAt::constructor
	*(BYTE *)0x93FFEB = 0x75; // jz -> jnz

	// Always use CVector3 over M2Entity in C_AICommandLookAt::constructor
	*(BYTE *)0x93F961 = 0x75; // jz -> jnz

	// Disable DLC from loading
	CPatcher::PatchAddress( 0x119D8E0, 0xC300B0 ); // mov al, 0; retn

	// Disable loading screens
	CPatcher::PatchAddress( 0x8B9140, 0xC300B0 ); // mov al, 0; retn

	// Disable save games
	CPatcher::InstallNopPatch ( 0xB3A850, 0x27C );

	// Crash fix #1 (Still another crash in-vehicle around this area - PhysX APEX stuff)
	CPatcher::InstallNopPatch ( 0x567B3D, 0x1E );
	CPatcher::InstallNopPatch ( 0x567C67, 0x1E );

	// Disable remote players turning into zombies if they die in car explosion
	CPatcher::InstallNopPatch ( 0xA033A1, 0x7 );

	// Disable the game having control over vehicle sirens
	CPatcher::InstallNopPatch ( 0x4CE573, 0x2 ); // enter vehicle 1
	CPatcher::InstallNopPatch ( 0x4CE577, 0x8 ); // enter vehicle 2
	CPatcher::InstallNopPatch ( 0x94504F, 0x12 ); // exit vehicle

	// Hook for when vehicle fuel tank is shot
	//CPatcher::InstallCallPatch ( 0x4E48F1, (DWORD)C_Vehicle__OnFuelTankShot );

	// Disable vehicle exit
	//CPatcher::InstallNopPatch ( 0x43B85B, 0x13 );

	//
	//CPatcher::InstallNopPatch ( 0x956345, 0x26 );

	// Test - try move player into passenger seat?
	//CPatcher::PatchAddress ( 0x9563C7, 0x1 );

	// Stop game from changing vehicle light state
	CPatcher::InstallNopPatch ( 0xCF6EC0, 0x65 );
	CPatcher::InstallNopPatch ( 0x4473E3, 0x16 );
	CPatcher::InstallNopPatch ( 0x447418, 0x16 );

#ifndef _DEBUG
	// Disable pause when game is minimized or sent to background
	CPatcher::InstallNopPatch( 0xAB6051, 7 );
	CPatcher::InstallNopPatch( 0xAB6037, 7 );
	CPatcher::InstallNopPatch( 0xAB6172, 7 );
	CPatcher::InstallNopPatch( 0xAB61A2, 7 );
#endif

	// Disable garages being able to create vehicles
	CPatcher::PatchAddress ( 0xCD00A0, 0xC300B0 ); // mov al, 0; retn

	// Patch the sds main folder address
	unsigned char guiMain[26] = "/sds/multiplayer/gui-main";
	CPatcher::PatchAddress( COffsets::VAR_GuiMainSdsFolder, guiMain, sizeof(guiMain) );

	// Patch the main sds address
	unsigned char guiMainSds[30] = "/sds/multiplayer/gui-main.sds";
	CPatcher::PatchAddress( COffsets::VAR_GuiMainSds, guiMainSds, sizeof(guiMainSds) );

	// Patch the ingame gui address
	unsigned char guiSds[25] = "/sds/multiplayer/gui.sds";
	CPatcher::PatchAddress( COffsets::VAR_GuiSds, guiSds, sizeof(guiSds) );

	// Patch the main tables address
	unsigned char tables[28] = "/sds/multiplayer/tables.sds";
	CPatcher::PatchAddress( COffsets::VAR_TablesSds, tables, sizeof(tables) );

	// Patch the stream map binary file
	unsigned char streamMapa[23] = "/tables/StreamM2MP.bin";
	CPatcher::PatchAddress( COffsets::VAR_StreamBin, streamMapa, sizeof(streamMapa) );

	// Patch the SDS config file
	unsigned char sdsConf[20] = "/sdsconfig_m2mp.bin";
	CPatcher::PatchAddress( 0x18F76A4, sdsConf, sizeof(sdsConf) );

	CLogFile::Printf( "Patches installed." );
}

int CPatches::HOOK_CGame__OnGameInit( lua_State * a1, signed int a2, const PCHAR a3 )
{
	// Was this the game start?
	if( !strcmp( a3, "onGameInit" ) )
	{
		// Is the game not yet loaded?
		if( !pCore->IsGameLoaded() )
		{
			// Call the game preload handler
			pCore->OnGamePreLoad();

			// Loaded
			bFirstLoad = false;

			//
			return onGameInit( a1, a2, a3 );
		}

		if( !bFirstLoad )
		{
			// Get the localplayer
			CLocalPlayer * pLocalPlayer = pCore->GetPlayerManager()->GetLocalPlayer();

			// Is the localplayer instance valid?
			if( pLocalPlayer )
			{
				// Is the localplayer dead?
				if( pLocalPlayer->IsDead() )
				{
					// Mark as process respawn
					bProcessRespawn = true;
				}
			}

			// Don't let the game run this script again
			return 1;
		}
	}

	return onGameInit( a1, a2, a3 );
}

int CPatches::HOOK_CGame__OnGameEvent( lua_State * a1, void * a2, const char * szFunction, M2EntityMessage * pMessage )
{
	// Send the event message to the handler
	CM2EntityMessage::HandleEntityEvent( pMessage );

	return onGameEvent( a1, a2, szFunction, pMessage );
}

int __fastcall CPatches::HOOK_CEntMgr__ProcessEntities( void * This, void * _EDX, void * pUnk1 )
{
	// Call the game process
	pCore->OnGameProcess ();

	// Is the game loaded?
	if( pCore->IsGameLoaded () )
	{
		// Should we process the respawn?
		if( bProcessRespawn )
		{
			// Handle spawn with game
			pCore->GetGame()->Spawn ( true );

			// Mark as not process respawn
			bProcessRespawn = false;
		}
	}

	return processEntities( This, pUnk1 );
}

int CPatches::HOOK_OnGameProcessStart( HINSTANCE hInstance, int a2, int a3, int a4 )
{
#ifndef _DEBUG
	// Install the crash reporter
	CrashRpt::InstallMain( MOD_NAME, MOD_VERS_STR );
#endif

	CLogFile::Printf ( "HOOK_OnGameProcessStart" );

	return onGameProcessStart( hInstance, a2, a3, a4 );
}

M2WeaponData * pDoShotHumanInventory = NULL;
DWORD FUNC_CHumanInventory__ProcessShot_END = 0x978016;
void __declspec( naked ) CPatches::HOOK_CHumanInventory__DoShot( void )
{
	_asm mov		pDoShotHumanInventory, ecx;
	_asm pushad;

	// Was this the localplayer?
	if( pCore->GetPlayerManager() && pCore->GetPlayerManager()->GetLocalPlayer() && pCore->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed() && pCore->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed()->GetPed()->m_pWeaponData == pDoShotHumanInventory )
	{
		_asm popad;
		_asm sub		esp, 8;
		_asm push		ebx;
		_asm push		esi;
		_asm mov		esi, ecx;
		_asm mov		edx, COffsets::FUNC_CHumanInventory__ProcessShot;
		_asm add		edx, 7;
		_asm jmp		edx;
	}
	// Don't process any ammo changes if this is a remote ped (Stop weapon desync by ped reloading weapon)
	else
	{
		_asm popad;
		_asm jmp FUNC_CHumanInventory__ProcessShot_END;
	}
}

M2WeaponData * pDoReloadHumanInventory = NULL;
DWORD CHumanInventory__DoReload_CALL = 0x958149;
void __declspec( naked ) CPatches::HOOK_CHumanInventory__DoReload( void )
{
	_asm mov		pDoReloadHumanInventory, ecx;
	_asm pushad;

	// Was this the localplayer?
	if( pCore->GetPlayerManager() && pCore->GetPlayerManager()->GetLocalPlayer() && pCore->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed() && pCore->GetPlayerManager()->GetLocalPlayer()->GetPlayerPed()->GetPed()->m_pWeaponData == pDoReloadHumanInventory )
	{
		// Handle the reload
		pCore->GetPlayerManager()->GetLocalPlayer()->OnReloadWeapon();

#ifdef _DEBUG
		pCore->GetChat()->AddDebugMessage( "HOOK_CHumanInventory__DoReload( LocalPlayer )" );
#endif
	}

	_asm popad;
	_asm sub		esp, 8;
	_asm push		ebx;
	_asm push		ebp;
	_asm push		esi;
	_asm push		edi;
	_asm mov		ebx, pDoReloadHumanInventory;
	_asm jmp		CHumanInventory__DoReload_CALL;
}