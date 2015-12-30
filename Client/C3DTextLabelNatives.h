/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabelNatives.h
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#pragma once

class C3DTextLabelNatives
{

private:

	static	SQInteger		Create3DTextLabel(SQVM * pVM);
	static	SQInteger		Remove3DTextLabel(SQVM * pVM);
	static	SQInteger		GetCount(SQVM * pVM);
	static	SQInteger		SetText(SQVM * pVM);
	static	SQInteger		SetDrawDistance(SQVM * pVM);
	static	SQInteger		SetColor(SQVM * pVM);
	static	SQInteger		SetPosition(SQVM * pVM);
	static	SQInteger		SetActive(SQVM * pVM);

public:

	static	void			Register(CScriptingManager * pScriptingManager);

};