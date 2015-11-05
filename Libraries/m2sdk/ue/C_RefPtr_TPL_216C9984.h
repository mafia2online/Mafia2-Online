// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/C_RefPtrCounter.h>
#include <ue/sys/memorymanager/C_AllocOperator_TPL_3FBCEACC.h>

namespace ue
{

/** ue::C_RefPtr_TPL_216C9984 (VTable=0xFFFFFFFF) */
class C_RefPtr_TPL_216C9984
{
public:
	/** ue::C_RefPtr<ue::sys::memorymanager::C_PlatformAllocator,ue::C_NullDeleter<ue::sys::memorymanager::C_PlatformAllocator>,ue::sys::memorymanager::C_AllocOperator<ue::sys::memorymanager::C_PlatformAllocator,ue::NullType>>::S_RefCount (VTable=0x01E8C580) */
	struct S_RefCount : public ue::C_RefPtrCounter, public ue::sys::memorymanager::C_AllocOperator_TPL_3FBCEACC
	{
	public:
		virtual void vfn_0001_21F1660C() = 0;
		virtual void vfn_0002_21F1660C() = 0;
	};
};

} // namespace ue
