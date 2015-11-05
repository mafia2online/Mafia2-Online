// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/threads/I_LWJobManager.h>
#include <ue/sys/threads/I_ThreadProcedure.h>

namespace ue
{

namespace sys
{

namespace threads
{

/** ue::sys::threads::C_LWJobManagerMT (VTable=0x01E94580) */
class C_LWJobManagerMT : public I_LWJobManager
{
public:
	/** ue::sys::threads::C_LWJobManagerMT::C_Worker (VTable=0x01E94560) */
	class C_Worker : public ue::sys::threads::I_ThreadProcedure
	{
	public:
		virtual void vfn_0001_6DD8790A() = 0;
		virtual void vfn_0002_6DD8790A() = 0;
		virtual void vfn_0003_6DD8790A() = 0;
		virtual void vfn_0004_6DD8790A() = 0;
		virtual void vfn_0005_6DD8790A() = 0;
	};

	virtual void vfn_0001_67A0891C() = 0;
	virtual void vfn_0002_67A0891C() = 0;
	virtual void vfn_0003_67A0891C() = 0;
	virtual void vfn_0004_67A0891C() = 0;
	virtual void vfn_0005_67A0891C() = 0;
	virtual void vfn_0006_67A0891C() = 0;
	virtual void vfn_0007_67A0891C() = 0;
	virtual void vfn_0008_67A0891C() = 0;
	virtual void vfn_0009_67A0891C() = 0;
	virtual void vfn_0010_67A0891C() = 0;
	virtual void vfn_0011_67A0891C() = 0;
	virtual void vfn_0012_67A0891C() = 0;
};

} // namespace threads

} // namespace sys

} // namespace ue
