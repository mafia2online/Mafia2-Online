// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>
#include <C_SlotManager.h>
#include <ue/sys/utils/I_Functor_TPL_45E5B885.h>
#include <ue/sys/physics/I_StreamCollisionsCallback.h>
#include <ue/sys/core/I_PriorityRetriever.h>
#include <I_ActorsPack.h>

/** C_SlotManager (VTable=0x01E78E20) */
class C_SlotManager : public C_TickedModule
{
public:
	/** C_SlotManager::C_Slot (VTable=0x01E78E80) */
	class C_Slot : public ue::sys::physics::I_StreamCollisionsCallback, public ue::sys::core::I_PriorityRetriever
	{
	public:
		/** C_SlotManager::C_Slot::C_FrameCreator (VTable=0x01E78EB0) */
		class C_FrameCreator : public I_ActorsPack::I_FrameCreator
		{
		public:
			virtual void vfn_0001_86D0D07C() = 0;
			virtual void vfn_0002_86D0D07C() = 0;
			virtual void vfn_0003_86D0D07C() = 0;
			virtual void vfn_0004_86D0D07C() = 0;
		};

		virtual void vfn_0001_D93DD328() = 0;
		virtual void vfn_0002_D93DD328() = 0;
		virtual void vfn_0003_D93DD328() = 0;
		virtual void vfn_0004_D93DD328() = 0;
	};

	/** C_SlotManager::C_SlotWrapperBase (VTable=0xFFFFFFFF) */
	class C_SlotWrapperBase
	{
	public:
	};

	/** C_SlotManager::C_SlotWrapper (VTable=0x01E78E40) */
	class C_SlotWrapper : public C_SlotWrapperBase
	{
	public:
		virtual void vfn_0001_5CC17624() = 0;
		virtual void vfn_0002_5CC17624() = 0;
		virtual void vfn_0003_5CC17624() = 0;
		virtual void vfn_0004_5CC17624() = 0;
		virtual void vfn_0005_5CC17624() = 0;
		virtual void vfn_0006_5CC17624() = 0;
		virtual void vfn_0007_5CC17624() = 0;
		virtual void vfn_0008_5CC17624() = 0;
		virtual void vfn_0009_5CC17624() = 0;
	};

	/** C_SlotManager::C_PreloadSlot (VTable=0x01E79680) */
	class C_PreloadSlot : public C_SlotWrapper
	{
	public:
		virtual void vfn_0001_501F12C4() = 0;
		virtual void vfn_0002_501F12C4() = 0;
		virtual void vfn_0003_501F12C4() = 0;
		virtual void vfn_0004_501F12C4() = 0;
		virtual void vfn_0005_501F12C4() = 0;
		virtual void vfn_0006_501F12C4() = 0;
		virtual void vfn_0007_501F12C4() = 0;
		virtual void vfn_0008_501F12C4() = 0;
		virtual void vfn_0009_501F12C4() = 0;
	};

	/** C_SlotManager::C_AnonymousSlotWrapper (VTable=0x01E521A0) */
	class C_AnonymousSlotWrapper : public C_SlotWrapper
	{
	public:
		virtual void vfn_0001_7136FBAD() = 0;
		virtual void vfn_0002_7136FBAD() = 0;
		virtual void vfn_0003_7136FBAD() = 0;
		virtual void vfn_0004_7136FBAD() = 0;
		virtual void vfn_0005_7136FBAD() = 0;
		virtual void vfn_0006_7136FBAD() = 0;
		virtual void vfn_0007_7136FBAD() = 0;
		virtual void vfn_0008_7136FBAD() = 0;
		virtual void vfn_0009_7136FBAD() = 0;
	};

	/** C_SlotManager::C_SaveLoadCallback (VTable=0x01E796E8) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_94E1887F() = 0;
		virtual void vfn_0002_94E1887F() = 0;
		virtual void vfn_0003_94E1887F() = 0;
		virtual void vfn_0004_94E1887F() = 0;
	};

	/** C_SlotManager::C_SaveLoadCallback1 (VTable=0x01E796C0) */
	class C_SaveLoadCallback1 : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_94F2887F() = 0;
		virtual void vfn_0002_94F2887F() = 0;
		virtual void vfn_0003_94F2887F() = 0;
		virtual void vfn_0004_94F2887F() = 0;
	};

	/** C_SlotManager::C_TimerFunctor (VTable=0x01E79758) */
	class C_TimerFunctor : public ue::sys::utils::I_Functor_TPL_45E5B885
	{
	public:
		virtual void vfn_0001_E1677B33() = 0;
		virtual void vfn_0002_E1677B33() = 0;
		virtual void vfn_0003_E1677B33() = 0;
	};

	/** C_SlotManager::C_Events (VTable=0x01E49CE0) */
	class C_Events
	{
	public:
		virtual void vfn_0001_CB765648() = 0;
		virtual void vfn_0002_CB765648() = 0;
		virtual void vfn_0003_CB765648() = 0;
	};

	virtual void vfn_0001_5F3FA5A2() = 0;
	virtual void vfn_0002_5F3FA5A2() = 0;
	virtual void vfn_0003_5F3FA5A2() = 0;
	virtual void vfn_0004_5F3FA5A2() = 0;
	virtual void vfn_0005_5F3FA5A2() = 0;
	virtual void vfn_0006_5F3FA5A2() = 0;
};
