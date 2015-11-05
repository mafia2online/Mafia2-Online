// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/threads/I_ThreadProcedure.h>

namespace ue
{

namespace sys
{

namespace utils
{

/** ue::sys::utils::C_PriorityStreamManager (VTable=0x01E95A00) */
class C_PriorityStreamManager
{
public:
	/** ue::sys::utils::C_PriorityStreamManager::C_PriorityStreamManagerThreadProc (VTable=0x01E959E0) */
	class C_PriorityStreamManagerThreadProc : public ue::sys::threads::I_ThreadProcedure
	{
	public:
		virtual void vfn_0001_E46F1081() = 0;
		virtual void vfn_0002_E46F1081() = 0;
		virtual void vfn_0003_E46F1081() = 0;
		virtual void vfn_0004_E46F1081() = 0;
		virtual void vfn_0005_E46F1081() = 0;
	};

	virtual void vfn_0001_ECB60631() = 0;
	virtual void vfn_0002_ECB60631() = 0;
};

} // namespace utils

} // namespace sys

} // namespace ue
