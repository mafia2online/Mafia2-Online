// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/C_RefPtrCounter.h>
#include <ue/sys/memorymanager/C_AllocOperator_TPL_3FBCEACC.h>

namespace ue
{

/** ue::C_RefPtr_TPL_A5FD6EB9 (VTable=0xFFFFFFFF) */
class C_RefPtr_TPL_A5FD6EB9
{
public:
	/** ue::C_RefPtr<ue::sys::memorymanager::C_NedAllocator<ue::sys::memorymanager::C_PlatformAllocator,ue::sys::memorymanager::I_Allocator>,ue::C_NullDeleter<ue::sys::memorymanager::C_NedAllocator<ue::sys::memorymanager::C_PlatformAllocator,ue::sys::memorymanager::I_Allocator>>,ue::sys::memorymanager::C_AllocOperator<ue::sys::memorymanager::C_PlatformAllocator,ue::NullType>>::S_RefCount (VTable=0x01E84440) */
	struct S_RefCount : public ue::C_RefPtrCounter, public ue::sys::memorymanager::C_AllocOperator_TPL_3FBCEACC
	{
	public:
		virtual void vfn_0001_62EFB5C0() = 0;
		virtual void vfn_0002_62EFB5C0() = 0;
	};
};

} // namespace ue
