// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/compression/I_Compressor.h>
#include <ue/sys/threads/I_ThreadProcedure.h>

namespace ue
{

namespace sys
{

namespace compression
{

namespace impl
{

/** ue::sys::compression::impl::C_CompressorZlib (VTable=0x01E84B08) */
class C_CompressorZlib : public ue::sys::compression::I_Compressor
{
public:
	/** ue::sys::compression::impl::C_CompressorZlib::C_ThreadProc (VTable=0x01E84B28) */
	class C_ThreadProc : public ue::sys::threads::I_ThreadProcedure
	{
	public:
		virtual void vfn_0001_74EFC96B() = 0;
		virtual void vfn_0002_74EFC96B() = 0;
		virtual void vfn_0003_74EFC96B() = 0;
		virtual void vfn_0004_74EFC96B() = 0;
		virtual void vfn_0005_74EFC96B() = 0;
	};

	virtual void vfn_0001_F835C81B() = 0;
	virtual void vfn_0002_F835C81B() = 0;
	virtual void vfn_0003_F835C81B() = 0;
	virtual void vfn_0004_F835C81B() = 0;
	virtual void vfn_0005_F835C81B() = 0;
};

} // namespace impl

} // namespace compression

} // namespace sys

} // namespace ue
