// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/C_RefPtrCounter.h>
#include <ue/sys/memorymanager/C_AllocOperator_TPL_3FBCEACC.h>

namespace ue
{

/** ue::C_RefPtr_TPL_EC1EC7EB (VTable=0xFFFFFFFF) */
class C_RefPtr_TPL_EC1EC7EB
{
public:
	/** ue::C_RefPtr<ue::sys::memorymanager::I_Allocator,ue::C_NullDeleter<ue::sys::memorymanager::I_Allocator>,ue::sys::memorymanager::C_AllocOperator<ue::sys::memorymanager::C_PlatformAllocator,ue::NullType>>::S_RefCount (VTable=0x01E6A980) */
	struct S_RefCount : public ue::C_RefPtrCounter, public ue::sys::memorymanager::C_AllocOperator_TPL_3FBCEACC
	{
	public:
		virtual void vfn_0001_4E7E8DBC() = 0;
		virtual void vfn_0002_4E7E8DBC() = 0;
	};
};

} // namespace ue
