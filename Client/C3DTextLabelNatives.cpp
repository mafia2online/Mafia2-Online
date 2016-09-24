/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabelNatives.cpp
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#include "BaseInc.h"

#include "CCore.h"

#include "Math/CVector3.h"

#include "C3DTextLabel.h"
#include "C3DTextLabelManager.h"

#include "Scripting/CScriptingManager.h"

#include "C3DTextLabelNatives.h"

void C3DTextLabelNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("create3DTextLabel", Create3DTextLabel, 6, "fffsif");
	pScriptingManager->RegisterFunction("remove3DTextLabel", Remove3DTextLabel, 1, "i");
	pScriptingManager->RegisterFunction("count3DTextLabels", GetCount, 0, NULL);
	pScriptingManager->RegisterFunction("set3DTextLabelText", SetText, 2, "is");
	pScriptingManager->RegisterFunction("set3DTextLabelDrawDistance", SetDrawDistance, 2, "if");
	pScriptingManager->RegisterFunction("set3DTextLabelColor", SetColor, 2, "ii");
	pScriptingManager->RegisterFunction("set3DTextLabelPosition", SetPosition, 4, "ifff");
	pScriptingManager->RegisterFunction("set3DTextLabelActive", SetActive, 2, "ib");

	pScriptingManager->RegisterConstant("MAX_3DTEXTLABELS", MAX_3DTEXTS);
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
	sq_pushbool(pVM, CCore::Instance()->Get3DTextLabelManager()->Add(fX, fY, fZ, str, color, distance));
	return (true);
}

SQInteger C3DTextLabelNatives::Remove3DTextLabel(SQVM * pVM)
{
	// Declare variable
	SQInteger textId;

	// Fetch value
	sq_getinteger(pVM, -1, &textId);

	// Remove and return
	sq_pushbool(pVM, CCore::Instance()->Get3DTextLabelManager()->Remove(textId));
	return (true);
}

SQInteger C3DTextLabelNatives::GetCount(SQVM * pVM)
{
	// Return value
	sq_pushinteger(pVM, CCore::Instance()->Get3DTextLabelManager()->GetCount());
	return (true);
}

SQInteger C3DTextLabelNatives::SetText(SQVM * pVM)
{
	// Declare variables
	SQInteger textID;
	const SQChar *text;
	String str;

	// Fetch values
	sq_getinteger(pVM, -2, &textID);
	sq_getstring(pVM, -1, &text);

	str.Set(text);

	// Declare label
	C3DTextLabel * label = CCore::Instance()->Get3DTextLabelManager()->Get(textID);

	// If exist
	if (label != NULL){

		// Set the text and return
		label->SetText(str);
		sq_pushbool(pVM, true);
		SAFE_DELETE(label);
		return (true);
	}
	SAFE_DELETE(label);
	return (false);
}

SQInteger C3DTextLabelNatives::SetDrawDistance(SQVM * pVM)
{
	// Declare variables
	SQInteger textID;
	SQFloat drawDistance;

	// Fetch values
	sq_getinteger(pVM, -2, &textID);
	sq_getfloat(pVM, -1, &drawDistance);

	// Declare label
	C3DTextLabel * label = CCore::Instance()->Get3DTextLabelManager()->Get(textID);

	// If exist
	if (label != NULL){

		// Set the distance and return
		label->SetDrawDistance(drawDistance);
		sq_pushbool(pVM, true);
		SAFE_DELETE(label);
		return (true);
	}
	SAFE_DELETE(label);
	return (false);
}

SQInteger C3DTextLabelNatives::SetColor(SQVM * pVM)
{
	// Declare variables
	SQInteger textID;
	SQInteger color;

	// Fetch values
	sq_getinteger(pVM, -2, &textID);
	sq_getinteger(pVM, -1, &color);

	// Declare label
	C3DTextLabel * label = CCore::Instance()->Get3DTextLabelManager()->Get(textID);

	// If exist
	if (label != NULL){

		// Set the color and return
		label->SetColor(color);
		sq_pushbool(pVM, true);
		SAFE_DELETE(label);
		return (true);
	}
	SAFE_DELETE(label);
	return (false);
}

SQInteger C3DTextLabelNatives::SetPosition(SQVM * pVM)
{
	// Declare variables
	SQInteger textID;
	SQFloat fX;
	SQFloat fY;
	SQFloat fZ;

	// Fetch values
	sq_getinteger(pVM, -4, &textID);
	sq_getfloat(pVM, -3, &fX);
	sq_getfloat(pVM, -2, &fY);
	sq_getfloat(pVM, -1, &fZ);

	CVector3 pos;
	pos.fX = fX;
	pos.fY = fY;
	pos.fZ = fZ;

	// Declare label
	C3DTextLabel * label = CCore::Instance()->Get3DTextLabelManager()->Get(textID);

	// If exist
	if (label != NULL){

		// Set the position and return
		label->SetPosition(pos);
		sq_pushbool(pVM, true);
		SAFE_DELETE(label);
		return (true);
	}
	SAFE_DELETE(label);
	return (false);
}

SQInteger C3DTextLabelNatives::SetActive(SQVM * pVM)
{
	// Declare variables
	SQInteger textID;
	SQBool active;

	// Fetch values
	sq_getinteger(pVM, -2, &textID);
	sq_getbool(pVM, -1, &active);

	// Declare label
	C3DTextLabel * label = CCore::Instance()->Get3DTextLabelManager()->Get(textID);

	// If exist
	if (label != NULL){

		// Set the position and return
		label->SetActive(active);
		sq_pushbool(pVM, true);
		SAFE_DELETE(label);
		return (true);
	}
	SAFE_DELETE(label);
	return (false);
}