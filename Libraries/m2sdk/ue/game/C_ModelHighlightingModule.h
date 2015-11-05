// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_SaveLoadCallBack.h>

namespace ue
{

namespace game
{

/** ue::game::C_ModelHighlightingModule (VTable=0x01E79A80) */
class C_ModelHighlightingModule : public C_TickedModule
{
public:
	/** ue::game::C_ModelHighlightingModule::C_SaveLoadCallback (VTable=0x01E7B818) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_86B08E51() = 0;
		virtual void vfn_0002_86B08E51() = 0;
		virtual void vfn_0003_86B08E51() = 0;
		virtual void vfn_0004_86B08E51() = 0;
	};

	virtual void vfn_0001_D6591FC9() = 0;
	virtual void vfn_0002_D6591FC9() = 0;
	virtual void vfn_0003_D6591FC9() = 0;
	virtual void vfn_0004_D6591FC9() = 0;
	virtual void vfn_0005_D6591FC9() = 0;
	virtual void vfn_0006_D6591FC9() = 0;
};

} // namespace game

} // namespace ue
