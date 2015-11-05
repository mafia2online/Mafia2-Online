// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/game/anim/C_AnimNode.h>
#include <ue/game/anim/C_AnimNodeParamTable.h>

namespace ue
{

namespace game
{

namespace anim
{

/** ue::game::anim::C_AnimNodeAutomat (VTable=0x01E380C0) */
class C_AnimNodeAutomat : public C_AnimNode
{
public:
	/** ue::game::anim::C_AnimNodeAutomat::S_AnimNodeAutomatEnumProvider (VTable=0x01E380E8) */
	struct S_AnimNodeAutomatEnumProvider : public ue::game::anim::C_AnimNodeParamTable::S_EnumProvider
	{
	public:
		virtual void vfn_0001_82CEC47C() = 0;
		virtual void vfn_0002_82CEC47C() = 0;
		virtual void vfn_0003_82CEC47C() = 0;
	};

	virtual void vfn_0001_B112991E() = 0;
	virtual void vfn_0002_B112991E() = 0;
	virtual void vfn_0003_B112991E() = 0;
	virtual void vfn_0004_B112991E() = 0;
	virtual void vfn_0005_B112991E() = 0;
	virtual void vfn_0006_B112991E() = 0;
	virtual void vfn_0007_B112991E() = 0;
};

} // namespace anim

} // namespace game

} // namespace ue
