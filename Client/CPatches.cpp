/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CPatches.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*			 : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CPatches.h"

#include "CMafia.h"

#include "engine/CM2Entity.h"
#include "engine/CM2Ped.h"

#include "engine/CM2Door.h"

#include "CLocalPlayer.h"

#include "CCore.h"

#include "CIE.h"

#include "CPatcher.h"

#include "COffsets.h"

#include "CM2EntityMessage.h"

#include "CLogFile.h"

#include "ExceptionHandler.h"

#include "CWindowSubclass.h"

sub_410440				CPatches::onGameInit = NULL;
onGameEvent_t			CPatches::onGameEvent = NULL;
ProcessEntities_t		CPatches::processEntities = NULL;
OnGameProcessStart_t	CPatches::onGameProcessStart = NULL;
bool					bProcessRespawn = false;
bool					bFirstLoad = true;

typedef void			( __thiscall * C_Player__ProcessKeyboard_t )	( M2Entity * pEntity, int a2, int a3, float a4 );
C_Player__ProcessKeyboard_t C_Player__ProcessKeyboard = NULL;

typedef int (__cdecl * CreateObjectByType_tp)(int);
CreateObjectByType_tp CreateObjectByTypePatch = NULL;

M2Ped * pPed = NULL;
DWORD sub_98B630_jmp = 0x98B63C;
DWORD sub_98B630_edx = 0x0;
//triggered with:
//	0xCC8F80 = Garage enter
//  0x4347B0 = Water enter
//  0x98BF90 = Passenger exit
void __declspec( naked ) sub_98B630( void )
{
	_asm
	{
		mov eax, [ecx+0A0h]
		mov pPed, eax
		push ebp
		mov ebp, esp
		mov eax, [ebp+4]
		mov sub_98B630_edx, eax
		pop ebp
		pushad
	}

	// Is the playermanager and localplayer active?
	if( CLocalPlayer::Instance() )
	{
		// hacky
		if( sub_98B630_edx == 0x4348F7 && CLocalPlayer::Instance()->IsSpawned() && pPed == CLocalPlayer::Instance()->GetPlayerPed()->GetPed() )
		{
			// Flag the localplayer for fast exit vehicle
			CLocalPlayer::Instance()->FlagForFastExitVehicle( true );

			// Handle the water enter with the localplayer
			CLocalPlayer::Instance()->OnEnterWater();
		}
	}

	_asm
	{
		popad
		sub esp, 18h
		push esi
		mov esi, ecx
		mov ecx, [esi+88h]
		jmp sub_98B630_jmp
	}
}

// crashy!
DWORD HOOK_CDoor__OnUse__JMP = 0x4DB43B;
DWORD HOOK_CDoor__OnUse__ECX = 0x0;
void __declspec ( naked ) HOOK_CDoor__OnUse ( void )
{
	_asm mov HOOK_CDoor__OnUse__ECX, ecx;
	_asm pushad;

	// Is the playermanager and localplayer active?
	if( CLocalPlayer::Instance() && CLocalPlayer::Instance()->IsSpawned() )
	{

		DEBUG_LOG ( "HOOK_CDoor__OnUse ( 0x%p )", HOOK_CDoor__OnUse__ECX );
		CM2Door * pDoor = *(CM2Door **) HOOK_CDoor__OnUse__ECX;

		if ( pDoor )
		{
			DEBUG_LOG ( "Is door closed? %s", (pDoor->IsClosed () ? "Yes" : "No") );
			pDoor->GetHash ();
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

	DEBUG_LOG ( "HOOK_C_SDSManager__ActivateStreamMapLine < 0x%p > () - Requested to load '%s'", _this_ebx, szStreamMapLine );

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
}

int iiTargetSeat;
DWORD C_Vehicle__PlayerStartEnter__JMP = 0x953B26; // Steam: 0x0964EF6
void __declspec(naked) C_Vehicle__PlayerStartEnter ( void )
{
	_asm push ebp;
	_asm mov ebp, esp;
	_asm mov eax, [ebp+12];
	_asm mov iiTargetSeat, eax;
	_asm pop ebp;
	_asm pushad;


	_asm popad;
	_asm sub esp, 1Ch;
	_asm push esi;
	_asm mov esi, ecx;
	_asm jmp C_Vehicle__PlayerStartEnter__JMP;
}

DWORD C_Human__TakeDamage_JMP = 0x97EE6F; // Steam: 0x09907DF
DWORD C_Human__TakeDamage_End = 0x97F392; // Steam: 0x0990D02
M2Ped *pVehicleCrashPlayer, *pLocalPedddddd;
CLocalPlayer * ppLocalPlayer;
void __declspec(naked) C_Human__TakeDamage ( void )
{
	_asm mov pVehicleCrashPlayer, ecx;
	_asm pushad;

	// Get the localplayer ped
	ppLocalPlayer = CLocalPlayer::Instance();
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

DWORD HOOK_C_Game__OnGameLoad__JMP = 0x587A25; // Steam: 0x0597C15
void __declspec ( naked ) HOOK_C_Game__OnGameLoad ( void )
{
	_asm pushad;

	// Call the game load handler
	CCore::Instance()->OnGameLoad ();

	_asm
	{
		popad;
		push ebx;
		mov ebx, ecx;
		mov eax, [ebx];
		jmp HOOK_C_Game__OnGameLoad__JMP;
	}
}

void __fastcall HOOK_C_Player__ProcessKeyboard ( M2Entity * pEntity, void * _EDX, int a2, int a3, float a4 )
{
	// If this ped is the localplayer, process keyboard controls
	if ( pEntity == IE::GetGame()->m_pLocalPed )
		C_Player__ProcessKeyboard ( pEntity, a2, a3, a4 );
}

void __fastcall Hook_CreateObject(int type)
{
		CLogFile::Printf("Request create object of type : %d | %04x", type, type);
		CreateObjectByTypePatch(type);
}

HWND hwGameWindow = 0;
unsigned OnMainWindowCreateHookRetn = 0;
void _declspec(naked) OnMainWindowCreateHook()
{
	_asm
	{
		pushad

		mov hwGameWindow, esi
	}

	// Subclass the games window
	CWindowSubclass::Subclass( hwGameWindow );

	_asm
	{
		popad
		pop     ebx
		pop     edi
		pop     esi
		mov     al, 1
		pop     ebp
		add     esp, 108h
		retn
	}
}

void CPatches::Initialise( void )
{
	DEBUG_LOG( "Installing patches..." );

	const unsigned uBaseAddress = CCore::Instance()->GetBaseAddress();

	// Unprotect the .text segment
	CPatcher::Unprotect( (uBaseAddress + 0x400000 + 0x1000), 0x94C000 );

	// Hook inside C_InitDone::Init_MainWindow called right after creating the window.
	CPatcher::InstallJmpPatch(uBaseAddress + 0x0049F8A1, (DWORD)OnMainWindowCreateHook);

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
	CPatcher::InstallJmpPatch ( 0x587A20, (DWORD)HOOK_C_Game__OnGameLoad ); // Steam: 0x0597C10
	//CPatcher::InstallJmpPatch ( 0x5B1C90, (DWORD)C_Game__OnGameUnload );

	// Hook to prevent remote player peds following the localplayer keyboard controls
	C_Player__ProcessKeyboard = (C_Player__ProcessKeyboard_t) CPatcher::InstallJmpPatch ( 0x42A9E0, (DWORD)HOOK_C_Player__ProcessKeyboard );
	CPatcher::InstallNopPatch ( 0x4387A3, 0x86 ); // Fix crash - NEED TO LOOK AT THIS MORE (CAUSES CRASH WITH LOCALPLAYER WHEN JUMPING) // Steam: 0x0438A43

	// Hook CreateObject
	//CreateObjectByTypePatch = (CreateObjectByType_tp)CPatcher::InstallCallPatch(0x0115F840, (DWORD)Hook_CreateObject);

	// Hook C_HumanInventory__DoShot
	CPatcher::InstallJmpPatch ( COffsets::FUNC_CHumanInventory__ProcessShot, (DWORD)HOOK_CHumanInventory__DoShot );

	// Hook C_HumanInventory__DoReload
	CPatcher::InstallJmpPatch( 0x958140, (DWORD)HOOK_CHumanInventory__DoReload ); // Steam: 0x0969510

	// Hook C_Human__TakeDamage
	CPatcher::InstallJmpPatch ( 0x97EE60, (DWORD)C_Human__TakeDamage ); // Steam: 0x09907D0

	// Vehicle enter stuff
	CPatcher::InstallJmpPatch ( 0x953B20, (DWORD)C_Vehicle__PlayerStartEnter ); // Steam: 0x0964EF0

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
	CPatcher::PatchAddress( 0x119D8E0, 0xC300B0 ); // mov al, 0; retn // Steam: 0x11A62C0

	// Disable loading screens
	CPatcher::PatchAddress( 0x8B9140, 0xC300B0 ); // mov al, 0; retn // Steam: 0x08CA820

	// Disable CityShops script loading
	CPatcher::PatchAddress(0x46EE90, 0x0004C2);

	// Disable save games
	CPatcher::InstallNopPatch ( 0xB3A850, 0x27C ); // Steam: 0x0B40570

	// Crash fix #1 (Still another crash in-vehicle around this area - PhysX APEX stuff)
	CPatcher::InstallNopPatch ( 0x567B3D, 0x1E );
	CPatcher::InstallNopPatch ( 0x567C67, 0x1E );

	// Disable remote players turning into zombies if they die in car explosion
	CPatcher::InstallNopPatch ( 0xA033A1, 0x7 );

	// Disable the game having control over vehicle sirens
	CPatcher::InstallNopPatch ( 0x4CE573, 0x2 ); // enter vehicle 1
	CPatcher::InstallNopPatch ( 0x4CE577, 0x8 ); // enter vehicle 2
	CPatcher::InstallNopPatch ( 0x94504F, 0x12 ); // exit vehicle

	// Hook for when vehicle fuel tank is shot - bugged
	//CPatcher::InstallCallPatch ( 0x4E48F1, (DWORD)C_Vehicle__OnFuelTankShot );

	// Disable vehicle exit
	//CPatcher::InstallNopPatch ( 0x43B85B, 0x13 );

	//
	//CPatcher::InstallNopPatch ( 0x956345, 0x26 );

	// Test - try move player into passenger seat?
	//CPatcher::PatchAddress ( 0x9563C7, 0x1 );

	// Stop game from changing vehicle light state
	CPatcher::InstallNopPatch ( 0xCF6EC0, 0x65 ); // Steam: 0x0CFDDA0
	CPatcher::InstallNopPatch ( 0x4473E3, 0x16 ); // Steam: 0x0447B13
	CPatcher::InstallNopPatch ( 0x447418, 0x16 ); // Steam: 00447B48
	CPatcher::InstallNopPatch ( 0x468C4D, 0xB );
	CPatcher::InstallNopPatch ( 0x468C7F, 0xB );

	// Stop game from changing vehicle indicator light state
	CPatcher::InstallNopPatch( 0xCF6F7D, 0x1D);

	// Prevent vehicle starting animation and engine starting when entering
	// CPatcher::InstallNopPatch(0x98A771, 0x8F); // This disable the whole system (you enter in vehicle but no door opening an you can't leave, no auto engine starting and no auto radio triggering)

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
	const char guiMain[] = "/sds/mp/gui-main";
	CPatcher::PatchAddress( COffsets::VAR_GuiMainSdsFolder, (unsigned char *)guiMain, strlen(guiMain) + 1 );

	// Patch the main sds address
	const char guiMainSds[21] = "/sds/mp/gui-main.sds";
	CPatcher::PatchAddress( COffsets::VAR_GuiMainSds, (unsigned char *)guiMainSds, strlen(guiMainSds) + 1 );

	// Patch the ingame gui address
	const char guiSds[] = "/sds/mp/gui.sds";
	CPatcher::PatchAddress( COffsets::VAR_GuiSds, (unsigned char *)guiSds, strlen(guiSds) + 1 );

	// Patch the main tables address
	const char tables[] = "/sds/mp/tables.sds";
	CPatcher::PatchAddress( COffsets::VAR_TablesSds, (unsigned char *)tables, strlen(tables) + 1);

	// Patch the stream map binary file
	const char streamMapa[] = "/tables/StreamM2MP.bin";
	CPatcher::PatchAddress( COffsets::VAR_StreamBin, (unsigned char *)streamMapa, strlen(streamMapa) + 1);

	// Patch the SDS config file
	const char sdsConf[] = "/sdsmpconf.bin";
	CPatcher::PatchAddress( 0x18F76A4, (unsigned char *)sdsConf, strlen(sdsConf) + 1 );

	DEBUG_LOG( "Patches installed." );
}

int CPatches::HOOK_CGame__OnGameInit( lua_State * a1, signed int a2, const PCHAR a3 )
{
	// Was this the game start?
	if( !strcmp( a3, "onGameInit" ) )
	{
		CCore *pCore = CCore::Instance();

		// Is the game not yet loaded?
		if( !pCore->IsGameLoaded() )
		{
			// Call the game preload handler
			pCore->OnGamePreLoad();

			// Loaded
			bFirstLoad = false;

			// Return to init
			return onGameInit( a1, a2, a3 );
		}

		if( !bFirstLoad )
		{
			// Get the localplayer
			CLocalPlayer * pLocalPlayer = CLocalPlayer::Instance();

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
	CCore *pCore = CCore::Instance();

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
	ExceptionHandler::ReapplyExceptionFilter();

	return onGameProcessStart( hInstance, a2, a3, a4 );
}

/**
 * Checks if the weapon is local player weapon.
 *
 * @param[in] pWeaponData The weapon data to be checked.
 * @return @c true in case weapon data is local player current weapon data
 *         @c false otherwise.
 */
static bool IsLocalPlayerWeapon ( M2WeaponData * pWeaponData )
{
	CLocalPlayer *pLocalPlayer = CLocalPlayer::Instance();
	if ( !pLocalPlayer )
		return false;

	CM2Ped *pPed = pLocalPlayer->GetPlayerPed();
	if ( !pPed )
		return false;

	M2Ped *pGamePed = pPed->GetPed();
	assert ( pGamePed );

	return pGamePed->m_pWeaponData == pWeaponData;
}

M2WeaponData * pDoShotHumanInventory = NULL;
DWORD FUNC_CHumanInventory__ProcessShot_END = 0x978016; // Steam: 0x09898E6
void __declspec( naked ) CPatches::HOOK_CHumanInventory__DoShot( void )
{
	_asm mov		pDoShotHumanInventory, ecx;
	_asm pushad;

	// Was this the localplayer?
	if( IsLocalPlayerWeapon(pDoShotHumanInventory) )
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
DWORD CHumanInventory__DoReload_CALL = 0x958149; // Steam : 0x:0958149
void __declspec( naked ) CPatches::HOOK_CHumanInventory__DoReload( void )
{
	_asm mov		pDoReloadHumanInventory, ecx;
	_asm pushad;

	// Was this the localplayer?
	if( IsLocalPlayerWeapon(pDoReloadHumanInventory) )
	{
		// Handle the reload
		CLocalPlayer::Instance()->OnReloadWeapon();
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
