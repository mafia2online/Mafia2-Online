// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <C_SlotManager.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

/** C_GameGfxEnvEffModule (VTable=0x01E79C80) */
class C_GameGfxEnvEffModule : public C_TickedModule
{
public:
	/** C_GameGfxEnvEffModule::C_SDSGameLoadNotifyHandler (VTable=0x01E79C50) */
	class C_SDSGameLoadNotifyHandler : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_72D1470B() = 0;
		virtual void vfn_0002_72D1470B() = 0;
		virtual void vfn_0003_72D1470B() = 0;
	};

	/** C_GameGfxEnvEffModule::C_SDSNotifyHandlerGlass (VTable=0x01E79CA8) */
	class C_SDSNotifyHandlerGlass : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_9943DDCD() = 0;
		virtual void vfn_0002_9943DDCD() = 0;
		virtual void vfn_0003_9943DDCD() = 0;
	};

	/** C_GameGfxEnvEffModule::C_SDSNotifyHandlerWeather (VTable=0x01E79CC0) */
	class C_SDSNotifyHandlerWeather : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_076A4906() = 0;
		virtual void vfn_0002_076A4906() = 0;
		virtual void vfn_0003_076A4906() = 0;
	};

	/** C_GameGfxEnvEffModule::C_SDSNotifyHandler (VTable=0x01E79CD8) */
	class C_SDSNotifyHandler : public C_SlotManager::C_Events
	{
	public:
		virtual void vfn_0001_0FA21557() = 0;
		virtual void vfn_0002_0FA21557() = 0;
		virtual void vfn_0003_0FA21557() = 0;
	};

	/** C_GameGfxEnvEffModule::C_SaveLoadCallback (VTable=0x01E7B918) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_37267E0B() = 0;
		virtual void vfn_0002_37267E0B() = 0;
		virtual void vfn_0003_37267E0B() = 0;
		virtual void vfn_0004_37267E0B() = 0;
	};

	virtual void vfn_0001_E75BC1F7() = 0;
	virtual void vfn_0002_E75BC1F7() = 0;
	virtual void vfn_0003_E75BC1F7() = 0;
	virtual void vfn_0004_E75BC1F7() = 0;
	virtual void vfn_0005_E75BC1F7() = 0;
	virtual void vfn_0006_E75BC1F7() = 0;
	virtual void vfn_0007_E75BC1F7() = 0;
	virtual void vfn_0008_E75BC1F7() = 0;
};
