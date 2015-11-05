// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/threads/I_ThreadProcedure.h>

namespace ue
{

namespace sys
{

namespace threads
{

/** ue::sys::threads::C_DbgDb (VTable=0xFFFFFFFF) */
class C_DbgDb
{
public:
	/** ue::sys::threads::C_DbgDb::C_ControlThreadProc (VTable=0x01E94700) */
	class C_ControlThreadProc : public ue::sys::threads::I_ThreadProcedure
	{
	public:
		virtual void vfn_0001_18C9CC7E() = 0;
		virtual void vfn_0002_18C9CC7E() = 0;
		virtual void vfn_0003_18C9CC7E() = 0;
		virtual void vfn_0004_18C9CC7E() = 0;
		virtual void vfn_0005_18C9CC7E() = 0;
	};
};

} // namespace threads

} // namespace sys

} // namespace ue
