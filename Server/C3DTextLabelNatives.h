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

	static	SQInteger		Create(SQVM * pVM);
	static	SQInteger		Delete(SQVM * pVM);

public:

	static	void			Register(CScriptingManager * pScriptingManager);

};