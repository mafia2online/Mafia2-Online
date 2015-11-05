// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/threads/I_ThreadProcedure.h>
#include <ue/sys/render/C_RenderCore.h>

namespace ue
{

namespace sys
{

namespace core
{

/** ue::sys::core::C_RenderProcHandler (VTable=0x01E867B0) */
class C_RenderProcHandler : public ue::sys::threads::I_ThreadProcedure
{
public:
	/** ue::sys::core::C_RenderProcHandler::C_RMUFinishedCallback (VTable=0x01E87208) */
	class C_RMUFinishedCallback : public ue::sys::render::C_RenderCore::I_RenderManagersFinCallback
	{
	public:
		virtual void vfn_0001_CADC0779() = 0;
		virtual void vfn_0002_CADC0779() = 0;
		virtual void vfn_0003_CADC0779() = 0;
	};

	virtual void vfn_0001_4266A5E2() = 0;
	virtual void vfn_0002_4266A5E2() = 0;
	virtual void vfn_0003_4266A5E2() = 0;
	virtual void vfn_0004_4266A5E2() = 0;
	virtual void vfn_0005_4266A5E2() = 0;
};

} // namespace core

} // namespace sys

} // namespace ue
