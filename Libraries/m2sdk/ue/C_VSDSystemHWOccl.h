// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/I_VSDSystem.h>
#include <ue/C_RefPtrCounter.h>
#include <ue/sys/render/device/I_GPUBusyWorker.h>

namespace ue
{

/** ue::C_VSDSystemHWOccl (VTable=0x01E92540) */
class C_VSDSystemHWOccl : public I_VSDSystem
{
public:
	/** ue::C_VSDSystemHWOccl::S_CameraCtx (VTable=0x01E93710) */
	struct S_CameraCtx : public ue::C_RefPtrCounter
	{
	public:
		virtual void vfn_0001_EAA6D922() = 0;
		virtual void vfn_0002_EAA6D922() = 0;
	};

	/** ue::C_VSDSystemHWOccl::C_GPUBusyWorker (VTable=0x01E936D0) */
	class C_GPUBusyWorker : public ue::sys::render::device::I_GPUBusyWorker
	{
	public:
		virtual void vfn_0001_BF8DFCC1() = 0;
		virtual void vfn_0002_BF8DFCC1() = 0;
		virtual void vfn_0003_BF8DFCC1() = 0;
	};

	virtual void vfn_0001_651FF7D5() = 0;
	virtual void vfn_0002_651FF7D5() = 0;
	virtual void vfn_0003_651FF7D5() = 0;
	virtual void vfn_0004_651FF7D5() = 0;
	virtual void vfn_0005_651FF7D5() = 0;
	virtual void vfn_0006_651FF7D5() = 0;
	virtual void vfn_0007_651FF7D5() = 0;
	virtual void vfn_0008_651FF7D5() = 0;
	virtual void vfn_0009_651FF7D5() = 0;
	virtual void vfn_0010_651FF7D5() = 0;
	virtual void vfn_0011_651FF7D5() = 0;
	virtual void vfn_0012_651FF7D5() = 0;
	virtual void vfn_0013_651FF7D5() = 0;
	virtual void vfn_0014_651FF7D5() = 0;
	virtual void vfn_0015_651FF7D5() = 0;
	virtual void vfn_0016_651FF7D5() = 0;
	virtual void vfn_0017_651FF7D5() = 0;
	virtual void vfn_0018_651FF7D5() = 0;
	virtual void vfn_0019_651FF7D5() = 0;
	virtual void vfn_0020_651FF7D5() = 0;
	virtual void vfn_0021_651FF7D5() = 0;
	virtual void vfn_0022_651FF7D5() = 0;
	virtual void vfn_0023_651FF7D5() = 0;
	virtual void vfn_0024_651FF7D5() = 0;
	virtual void vfn_0025_651FF7D5() = 0;
	virtual void vfn_0026_651FF7D5() = 0;
	virtual void vfn_0027_651FF7D5() = 0;
};

} // namespace ue
