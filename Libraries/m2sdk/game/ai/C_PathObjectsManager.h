// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <game/ai/navpoints/S_Selector.h>

namespace game
{

namespace ai
{

/** game::ai::C_PathObjectsManager (VTable=0x01E47A60) */
class C_PathObjectsManager
{
public:
	/** game::ai::C_PathObjectsManager::S_PathObjectSelector (VTable=0x01E47A48) */
	struct S_PathObjectSelector : public game::ai::navpoints::S_Selector
	{
	public:
		virtual void vfn_0001_3E2559FD() = 0;
		virtual void vfn_0002_3E2559FD() = 0;
		virtual void vfn_0003_3E2559FD() = 0;
	};

	virtual void vfn_0001_60D75877() = 0;
	virtual void vfn_0002_60D75877() = 0;
};

} // namespace ai

} // namespace game
