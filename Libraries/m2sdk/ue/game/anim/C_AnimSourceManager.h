// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/game/anim/C_AnimBase.h>
#include <ue/game/anim/I_AnimBase.h>

namespace ue
{

namespace game
{

namespace anim
{

/** ue::game::anim::C_AnimSourceManager (VTable=0x01E379E0) */
class C_AnimSourceManager : public C_AnimBase
{
public:
	/** ue::game::anim::C_AnimSourceManager::S_LodDef (VTable=0x01E379C8) */
	struct S_LodDef : public ue::game::anim::I_AnimBase::I_LodDef
	{
	public:
		virtual void vfn_0001_A5B09C06() = 0;
		virtual void vfn_0002_A5B09C06() = 0;
		virtual void vfn_0003_A5B09C06() = 0;
		virtual void vfn_0004_A5B09C06() = 0;
	};

	virtual void vfn_0001_E7901B52() = 0;
	virtual void vfn_0002_E7901B52() = 0;
};

} // namespace anim

} // namespace game

} // namespace ue
