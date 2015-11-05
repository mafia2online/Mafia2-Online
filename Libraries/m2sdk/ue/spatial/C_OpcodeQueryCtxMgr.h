// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/spatial/I_OpcodeQueryCtxMgr.h>
#include <ue/sys/threads/C_ThreadSpecificData.h>

namespace ue
{

namespace spatial
{

/** ue::spatial::C_OpcodeQueryCtxMgr (VTable=0x01E8CC18) */
class C_OpcodeQueryCtxMgr : public I_OpcodeQueryCtxMgr
{
public:
	/** ue::spatial::C_OpcodeQueryCtxMgr::CCtxDestructor (VTable=0x01E8CD18) */
	class CCtxDestructor : public ue::sys::threads::C_ThreadSpecificData::I_CleanUp
	{
	public:
		virtual void vfn_0001_9E9C0B7B() = 0;
		virtual void vfn_0002_9E9C0B7B() = 0;
		virtual void vfn_0003_9E9C0B7B() = 0;
	};

	virtual void vfn_0001_6B0324F3() = 0;
	virtual void vfn_0002_6B0324F3() = 0;
	virtual void vfn_0003_6B0324F3() = 0;
};

} // namespace spatial

} // namespace ue
