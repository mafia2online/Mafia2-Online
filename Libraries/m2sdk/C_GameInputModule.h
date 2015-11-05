// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/sys/userservices/I_SysNotificationCallback.h>

/** C_GameInputModule (VTable=0x01E79E40) */
class C_GameInputModule : public C_TickedModule
{
public:
	/** C_GameInputModule::C_GameInputSysNotifyCallback (VTable=0x01E79E10) */
	class C_GameInputSysNotifyCallback : public ue::sys::userservices::I_SysNotificationCallback
	{
	public:
		virtual void vfn_0001_7A6EBF4B() = 0;
		virtual void vfn_0002_7A6EBF4B() = 0;
		virtual void vfn_0003_7A6EBF4B() = 0;
	};

	virtual void vfn_0001_015C58C4() = 0;
	virtual void vfn_0002_015C58C4() = 0;
	virtual void vfn_0003_015C58C4() = 0;
	virtual void vfn_0004_015C58C4() = 0;
	virtual void vfn_0005_015C58C4() = 0;
	virtual void vfn_0006_015C58C4() = 0;
};
