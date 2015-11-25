/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CAudioNatives.h
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#pragma once

class CAudioNatives
{

private:
	static SQInteger		CAudioNatives::Create(SQVM *pVM);
	static SQInteger		CAudioNatives::Delete(SQVM *pVM);
	static SQInteger		CAudioNatives::Play(SQVM *pVM);
	static SQInteger		CAudioNatives::Stop(SQVM *pVM);
	static SQInteger		CAudioNatives::Pause(SQVM *pVM);
	static SQInteger		CAudioNatives::SetVolume(SQVM *pVM);
	static SQInteger		CAudioNatives::GetVolume(SQVM *pVM);
	static SQInteger		CAudioNatives::ResetVolume(SQVM *pVM);

public:

	static	void			Register(CScriptingManager * pScriptingManager);

};

