// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <ue/game/params/I_GameInfoCollector.h>

namespace ue
{

namespace game
{

namespace params
{

/** ue::game::params::C_GameParamsGameModule (VTable=0x01E7A8E0) */
class C_GameParamsGameModule : public C_TickedModule
{
public:
	/** ue::game::params::C_GameParamsGameModule::C_GameInfoCollector (VTable=0x01E7A900) */
	class C_GameInfoCollector : public ue::game::params::I_GameInfoCollector
	{
	public:
		virtual void vfn_0001_CB192C59() = 0;
		virtual void vfn_0002_CB192C59() = 0;
		virtual void vfn_0003_CB192C59() = 0;
	};

	virtual void vfn_0001_9BD3CB0C() = 0;
	virtual void vfn_0002_9BD3CB0C() = 0;
	virtual void vfn_0003_9BD3CB0C() = 0;
	virtual void vfn_0004_9BD3CB0C() = 0;
	virtual void vfn_0005_9BD3CB0C() = 0;
	virtual void vfn_0006_9BD3CB0C() = 0;
};

} // namespace params

} // namespace game

} // namespace ue
