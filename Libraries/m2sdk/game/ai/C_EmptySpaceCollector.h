// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <game/ai/C_PlacesCollector.h>
#include <game/ai/I_EmptySpaceEnumerator.h>

namespace game
{

namespace ai
{

/** game::ai::C_EmptySpaceCollector (VTable=0x01E567C0) */
class C_EmptySpaceCollector : public C_PlacesCollector, public game::ai::I_EmptySpaceEnumerator::I_EnumerateCallbacks
{
public:
	virtual void vfn_0001_2D86F6F0() = 0;
	virtual void vfn_0002_2D86F6F0() = 0;
	virtual void vfn_0003_2D86F6F0() = 0;
	virtual void vfn_0004_2D86F6F0() = 0;
	virtual void vfn_0005_2D86F6F0() = 0;
	virtual void vfn_0006_2D86F6F0() = 0;
	virtual void vfn_0007_2D86F6F0() = 0;
};

} // namespace ai

} // namespace game
