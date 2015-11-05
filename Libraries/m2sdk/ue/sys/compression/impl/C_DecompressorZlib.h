// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/compression/I_Decompressor.h>
#include <ue/sys/threads/I_ThreadProcedure.h>

namespace ue
{

namespace sys
{

namespace compression
{

namespace impl
{

/** ue::sys::compression::impl::C_DecompressorZlib (VTable=0x01E84AC0) */
class C_DecompressorZlib : public ue::sys::compression::I_Decompressor
{
public:
	/** ue::sys::compression::impl::C_DecompressorZlib::C_ThreadProc (VTable=0x01E84AE8) */
	class C_ThreadProc : public ue::sys::threads::I_ThreadProcedure
	{
	public:
		virtual void vfn_0001_465056C3() = 0;
		virtual void vfn_0002_465056C3() = 0;
		virtual void vfn_0003_465056C3() = 0;
		virtual void vfn_0004_465056C3() = 0;
		virtual void vfn_0005_465056C3() = 0;
	};

	virtual void vfn_0001_69AB6028() = 0;
	virtual void vfn_0002_69AB6028() = 0;
	virtual void vfn_0003_69AB6028() = 0;
	virtual void vfn_0004_69AB6028() = 0;
	virtual void vfn_0005_69AB6028() = 0;
	virtual void vfn_0006_69AB6028() = 0;
	virtual void vfn_0007_69AB6028() = 0;
	virtual void vfn_0008_69AB6028() = 0;
};

} // namespace impl

} // namespace compression

} // namespace sys

} // namespace ue
