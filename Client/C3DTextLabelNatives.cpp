/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabelNatives.cpp
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore			* pCore;

void CCameraNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("getCameraPosition", GetPosition, 0, NULL);
	pScriptingManager->RegisterFunction("getCameraRotation", GetRotation, 0, NULL);
}