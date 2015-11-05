// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/C_RefPtrCounter.h>
#include <game/ai/navpoints/C_CoverUserData.h>

namespace game
{

namespace ai
{

/** game::ai::C_CoverSlots (VTable=0x01E56EB0) */
class C_CoverSlots
{
public:
	/** game::ai::C_CoverSlots::C_Slot (VTable=0x01E56EF8) */
	class C_Slot
	{
	public:
		virtual void vfn_0001_4EEA0D35() = 0;
		virtual void vfn_0002_4EEA0D35() = 0;
		virtual void vfn_0003_4EEA0D35() = 0;
	};

	/** game::ai::C_CoverSlots::C_SlotsRoot (VTable=0x01E56EC0) */
	class C_SlotsRoot : public ue::C_RefPtrCounter, public game::ai::navpoints::C_CoverUserData
	{
	public:
		virtual void vfn_0001_90209476() = 0;
		virtual void vfn_0002_90209476() = 0;
		virtual void vfn_0003_90209476() = 0;
		virtual void vfn_0004_90209476() = 0;
	};

	virtual void vfn_0001_A9371306() = 0;
	virtual void vfn_0002_A9371306() = 0;
};

} // namespace ai

} // namespace game
