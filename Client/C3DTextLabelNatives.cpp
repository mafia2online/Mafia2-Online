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

void C3DTextLabelNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("create3DTextLabel", Create3DTextLabel, 6, "fffsif");
	pScriptingManager->RegisterFunction("remove3DTextLabel", Remove3DTextLabel, 1, "i");
	pScriptingManager->RegisterFunction("count3DTextLabels", GetCount, 0, NULL);

	pScriptingManager->RegisterConstant("MAX_3DTEXTLABELS", MAX_3DTEXTS);
}

SQInteger C3DTextLabelNatives::GetCount(SQVM * pVM)
{
	// Return value
	sq_pushinteger(pVM, pCore->Get3DTextLabelManager()->GetCount());
	return (true);
}

SQInteger C3DTextLabelNatives::Create3DTextLabel(SQVM * pVM)
{
	// Declare variables
	SQFloat fX;
	SQFloat fY;
	SQFloat fZ;
	const SQChar *text;
	String str;
	SQInteger color;
	SQFloat distance;

	// Fetch values
	sq_getfloat(pVM, -6, &fX);
	sq_getfloat(pVM, -5, &fY);
	sq_getfloat(pVM, -4, &fZ);

	sq_getstring(pVM, -3, &text);

	sq_getinteger(pVM, -2, &color);
	sq_getfloat(pVM, -1, &distance);


	// Transform string
	str.Set(text);

	// Create and return
	sq_pushbool(pVM, pCore->Get3DTextLabelManager()->Add(fX, fY, fZ, str, color, distance));
	return (true);
}

SQInteger C3DTextLabelNatives::Remove3DTextLabel(SQVM * pVM)
{
	// Declare variable
	SQInteger textId;

	// Fetch value
	sq_getinteger(pVM, -1, &textId);

	// Remove and return
	sq_pushbool(pVM, pCore->Get3DTextLabelManager()->Remove(textId));
	return (true);
}