// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/render/I_SyncObjectTarget.h>
#include <ue/I_Callback_TPL_9AA54AEF.h>

namespace ue
{

namespace sys
{

namespace render
{

/** ue::sys::render::C_RenderCore (VTable=0x01E8F6B0) */
class C_RenderCore : public I_SyncObjectTarget
{
public:
	/** ue::sys::render::C_RenderCore::I_RenderManagersFinCallback (VTable=0xFFFFFFFF) */
	class I_RenderManagersFinCallback : public ue::I_Callback_TPL_9AA54AEF
	{
	public:
	};

	virtual void vfn_0001_58360B62() = 0;
	virtual void vfn_0002_58360B62() = 0;
	virtual void vfn_0003_58360B62() = 0;
	virtual void vfn_0004_58360B62() = 0;
	virtual void vfn_0005_58360B62() = 0;
};

} // namespace render

} // namespace sys

} // namespace ue
