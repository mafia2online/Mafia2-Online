// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/game/params/I_StateMachine.h>
#include <ue/game/params/I_SequenceManager.h>
#include <ue/sys/userservices/I_SysNotificationCallback.h>
#include <ue/game/params/C_StateMachine.h>
#include <mafia/gui/I_GUITextDialogCallback.h>

namespace ue
{

namespace game
{

namespace params
{

/** ue::game::params::C_StateMachine (VTable=0x01E7A7E0) */
class C_StateMachine : public I_StateMachine, public I_SequenceManager, public ue::sys::userservices::I_SysNotificationCallback
{
public:
	/** ue::game::params::C_StateMachine::I_State (VTable=0x01E7B988) */
	class I_State
	{
	public:
		virtual void vfn_0001_BDC96852() = 0;
		virtual void vfn_0002_BDC96852() = 0;
		virtual void vfn_0003_BDC96852() = 0;
	};

	/** ue::game::params::C_StateMachine::I_InteractiveState (VTable=0x01E7B200) */
	class I_InteractiveState : public I_State, public mafia::gui::I_GUITextDialogCallback
	{
	public:
		virtual void vfn_0001_5BBAA4E4() = 0;
		virtual void vfn_0002_5BBAA4E4() = 0;
		virtual void vfn_0003_5BBAA4E4() = 0;
		virtual void vfn_0004_5BBAA4E4() = 0;
		virtual void vfn_0005_5BBAA4E4() = 0;
		virtual void vfn_0006_5BBAA4E4() = 0;
		virtual void vfn_0007_5BBAA4E4() = 0;
	};

	/** ue::game::params::C_StateMachine::C_SignInResetWarning (VTable=0x01E7A760) */
	class C_SignInResetWarning : public I_InteractiveState
	{
	public:
		virtual void vfn_0001_AE9ACE5F() = 0;
		virtual void vfn_0002_AE9ACE5F() = 0;
		virtual void vfn_0003_AE9ACE5F() = 0;
		virtual void vfn_0004_AE9ACE5F() = 0;
		virtual void vfn_0005_AE9ACE5F() = 0;
		virtual void vfn_0006_AE9ACE5F() = 0;
		virtual void vfn_0007_AE9ACE5F() = 0;
	};

	/** ue::game::params::C_StateMachine::C_SignOutResetInfo (VTable=0x01E7A7A0) */
	class C_SignOutResetInfo : public I_InteractiveState
	{
	public:
		virtual void vfn_0001_6122FEA3() = 0;
		virtual void vfn_0002_6122FEA3() = 0;
		virtual void vfn_0003_6122FEA3() = 0;
		virtual void vfn_0004_6122FEA3() = 0;
		virtual void vfn_0005_6122FEA3() = 0;
		virtual void vfn_0006_6122FEA3() = 0;
		virtual void vfn_0007_6122FEA3() = 0;
	};

	virtual void vfn_0001_EAB839F2() = 0;
	virtual void vfn_0002_EAB839F2() = 0;
	virtual void vfn_0003_EAB839F2() = 0;
	virtual void vfn_0004_EAB839F2() = 0;
	virtual void vfn_0005_EAB839F2() = 0;
	virtual void vfn_0006_EAB839F2() = 0;
	virtual void vfn_0007_EAB839F2() = 0;
	virtual void vfn_0008_EAB839F2() = 0;
	virtual void vfn_0009_EAB839F2() = 0;
	virtual void vfn_0010_EAB839F2() = 0;
	virtual void vfn_0011_EAB839F2() = 0;
	virtual void vfn_0012_EAB839F2() = 0;
	virtual void vfn_0013_EAB839F2() = 0;
	virtual void vfn_0014_EAB839F2() = 0;
	virtual void vfn_0015_EAB839F2() = 0;
	virtual void vfn_0016_EAB839F2() = 0;
	virtual void vfn_0017_EAB839F2() = 0;
	virtual void vfn_0018_EAB839F2() = 0;
	virtual void vfn_0019_EAB839F2() = 0;
	virtual void vfn_0020_EAB839F2() = 0;
	virtual void vfn_0021_EAB839F2() = 0;
	virtual void vfn_0022_EAB839F2() = 0;
	virtual void vfn_0023_EAB839F2() = 0;
	virtual void vfn_0024_EAB839F2() = 0;
	virtual void vfn_0025_EAB839F2() = 0;
	virtual void vfn_0026_EAB839F2() = 0;
	virtual void vfn_0027_EAB839F2() = 0;
	virtual void vfn_0028_EAB839F2() = 0;
};

} // namespace params

} // namespace game

} // namespace ue
