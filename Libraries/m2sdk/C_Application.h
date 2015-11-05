// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <mafia/gui/I_GUIDialogCallback.h>
#include <ue/sys/userservices/I_SysNotificationCallback.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

/** C_Application (VTable=0x01E7F6C0) */
class C_Application : public C_TickedModule
{
public:
	/** C_Application::C_GuiLoadFailDialog (VTable=0x01E7F6E0) */
	class C_GuiLoadFailDialog : public mafia::gui::I_GUIDialogCallback
	{
	public:
		virtual void vfn_0001_3E0A372D() = 0;
		virtual void vfn_0002_3E0A372D() = 0;
		virtual void vfn_0003_3E0A372D() = 0;
	};

	/** C_Application::C_SysNotificationCallback (VTable=0x01E7F6A0) */
	class C_SysNotificationCallback : public ue::sys::userservices::I_SysNotificationCallback
	{
	public:
		virtual void vfn_0001_A55078CF() = 0;
		virtual void vfn_0002_A55078CF() = 0;
		virtual void vfn_0003_A55078CF() = 0;
	};

	/** C_Application::C_SaveLoadCallback1 (VTable=0x01E81CC8) */
	class C_SaveLoadCallback1 : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_F7C54A21() = 0;
		virtual void vfn_0002_F7C54A21() = 0;
		virtual void vfn_0003_F7C54A21() = 0;
		virtual void vfn_0004_F7C54A21() = 0;
	};

	/** C_Application::C_SaveLoadCallback (VTable=0x01E81CF0) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_F7D44A21() = 0;
		virtual void vfn_0002_F7D44A21() = 0;
		virtual void vfn_0003_F7D44A21() = 0;
		virtual void vfn_0004_F7D44A21() = 0;
	};

	/** C_Application::C_SaveLoadCallbackFirst (VTable=0x01E81CA0) */
	class C_SaveLoadCallbackFirst : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_5C4031E2() = 0;
		virtual void vfn_0002_5C4031E2() = 0;
		virtual void vfn_0003_5C4031E2() = 0;
		virtual void vfn_0004_5C4031E2() = 0;
	};

	virtual void vfn_0001_768C0D05() = 0;
	virtual void vfn_0002_768C0D05() = 0;
	virtual void vfn_0003_768C0D05() = 0;
	virtual void vfn_0004_768C0D05() = 0;
	virtual void vfn_0005_768C0D05() = 0;
	virtual void vfn_0006_768C0D05() = 0;
};
