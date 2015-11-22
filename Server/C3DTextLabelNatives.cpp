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
	pScriptingManager->RegisterFunction("create3DTextLabel", Create, 6, "siffff");
	pScriptingManager->RegisterFunction("delete3DTextLabel", Delete, 1, "i");
}

SQInteger C3DTextLabelNatives::Create(SQVM *pVM)
{
	const char	*text;
	SQInteger	color;
	SQFloat		posX;
	SQFloat		posY;
	SQFloat		posZ;
	SQFloat		drawDistance;

	sq_getstring(pVM, -6, &text);
	sq_getinteger(pVM, -5, &color);
	sq_getfloat(pVM, -4, &posX);
	sq_getfloat(pVM, -3, &posY);
	sq_getfloat(pVM, -2, &posZ);
	sq_getfloat(pVM, -1, &drawDistance);

	sq_pushinteger(pVM, pCore->Get3DTextLabelManager()->Add(text, color, posX, posY, posZ, drawDistance));
	return (1);
}

SQInteger C3DTextLabelNatives::Delete(SQVM *pVM)
{
	SQInteger	textID;

	sq_getinteger(pVM, -1, &textID);

	pCore->Get3DTextLabelManager()->Remove(textID);
	sq_pushbool(pVM, true);
	return (1);
}