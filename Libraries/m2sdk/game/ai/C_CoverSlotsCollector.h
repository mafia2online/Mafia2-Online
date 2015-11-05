// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <game/ai/C_PlacesCollector.h>
#include <game/ai/I_CoverSlotsEnumerator.h>

namespace game
{

namespace ai
{

/** game::ai::C_CoverSlotsCollector (VTable=0x01E56800) */
class C_CoverSlotsCollector : public C_PlacesCollector, public game::ai::I_CoverSlotsEnumerator::I_EnumerateCallbacks
{
public:
	virtual void vfn_0001_C044EAEB() = 0;
	virtual void vfn_0002_C044EAEB() = 0;
	virtual void vfn_0003_C044EAEB() = 0;
	virtual void vfn_0004_C044EAEB() = 0;
	virtual void vfn_0005_C044EAEB() = 0;
	virtual void vfn_0006_C044EAEB() = 0;
	virtual void vfn_0007_C044EAEB() = 0;
};

} // namespace ai

} // namespace game
