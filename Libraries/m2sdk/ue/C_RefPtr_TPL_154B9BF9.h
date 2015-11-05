// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/C_RefPtrCounter.h>
#include <ue/sys/memorymanager/C_AllocOperator_TPL_3FBCEACC.h>

namespace ue
{

/** ue::C_RefPtr_TPL_154B9BF9 (VTable=0xFFFFFFFF) */
class C_RefPtr_TPL_154B9BF9
{
public:
	/** ue::C_RefPtr<ue::sys::memorymanager::C_IncrementalAllocator<ue::sys::memorymanager::C_GlobalAllocator,4l,ue::sys::threads::C_SingleThread,ue::sys::memorymanager::I_ResetableAllocator>,ue::C_NullDeleter<ue::sys::memorymanager::C_IncrementalAllocator<ue::sys::memorymanager::C_GlobalAllocator,4l,ue::sys::threads::C_SingleThread,ue::sys::memorymanager::I_ResetableAllocator>>,ue::sys::memorymanager::C_AllocOperator<ue::sys::memorymanager::C_PlatformAllocator,ue::NullType>>::S_RefCount (VTable=0x01E83B80) */
	struct S_RefCount : public ue::C_RefPtrCounter, public ue::sys::memorymanager::C_AllocOperator_TPL_3FBCEACC
	{
	public:
		virtual void vfn_0001_592BB0BE() = 0;
		virtual void vfn_0002_592BB0BE() = 0;
	};
};

} // namespace ue
