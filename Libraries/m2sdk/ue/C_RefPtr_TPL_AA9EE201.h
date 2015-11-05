// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/C_RefPtrCounter.h>
#include <ue/sys/memorymanager/C_AllocOperator_TPL_3FBCEACC.h>

namespace ue
{

/** ue::C_RefPtr_TPL_AA9EE201 (VTable=0xFFFFFFFF) */
class C_RefPtr_TPL_AA9EE201
{
public:
	/** ue::C_RefPtr<ue::sys::memorymanager::C_IncrementalAllocator<ue::sys::memorymanager::I_Allocator,4l,ue::sys::threads::C_SingleThread,ue::NullType>,ue::C_NullDeleter<ue::sys::memorymanager::C_IncrementalAllocator<ue::sys::memorymanager::I_Allocator,4l,ue::sys::threads::C_SingleThread,ue::NullType>>,ue::sys::memorymanager::C_AllocOperator<ue::sys::memorymanager::C_PlatformAllocator,ue::NullType>>::S_RefCount (VTable=0x01E8C700) */
	struct S_RefCount : public ue::C_RefPtrCounter, public ue::sys::memorymanager::C_AllocOperator_TPL_3FBCEACC
	{
	public:
		virtual void vfn_0001_C2FE44BD() = 0;
		virtual void vfn_0002_C2FE44BD() = 0;
	};
};

} // namespace ue
