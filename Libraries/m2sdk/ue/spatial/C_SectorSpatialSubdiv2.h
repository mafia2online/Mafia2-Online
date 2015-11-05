// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/spatial/I_SectorSpatialSubdiv.h>
#include <ue/spatial/C_HSpatialHash_TPL_7925F87C.h>
#include <ue/spatial/C_SectorSpatialSubdiv2.h>
#include <ue/C_RefPtrCounter.h>
#include <ue/spatial/I_VSDRendererAdaptor.h>
#include <ue/I_VSDGPUBusyWorker.h>

namespace ue
{

namespace spatial
{

/** ue::spatial::C_SectorSpatialSubdiv2 (VTable=0x01E8CAA0) */
class C_SectorSpatialSubdiv2 : public I_SectorSpatialSubdiv
{
public:
	/** ue::spatial::C_SectorSpatialSubdiv2::C_VisitorWrapper<true> (VTable=0x01E8CD58) */
	class C_VisitorWrapper_TPL_7788CC9D : public ue::spatial::C_HSpatialHash_TPL_7925F87C::I_Visitor
	{
	public:
		virtual void vfn_0001_A8C5A8D6() = 0;
		virtual void vfn_0002_A8C5A8D6() = 0;
		virtual void vfn_0003_A8C5A8D6() = 0;
	};

	/** ue::spatial::C_SectorSpatialSubdiv2::C_ContainerVisitorWrapper<ue::C_StaticStdVector<ue::spatial::I_SpatialSector *,512u>> (VTable=0x01E8CD98) */
	class C_ContainerVisitorWrapper_TPL_F15C0255 : public ue::spatial::C_HSpatialHash_TPL_7925F87C::I_Visitor
	{
	public:
		virtual void vfn_0001_9BDA684C() = 0;
		virtual void vfn_0002_9BDA684C() = 0;
		virtual void vfn_0003_9BDA684C() = 0;
	};

	/** ue::spatial::C_SectorSpatialSubdiv2::C_ContainerVisitorWrapper<ue::C_StaticStdVector<ue::spatial::I_SpatialSector *,256u>> (VTable=0x01E8CDB8) */
	class C_ContainerVisitorWrapper_TPL_890DE9B2 : public ue::spatial::C_HSpatialHash_TPL_7925F87C::I_Visitor
	{
	public:
		virtual void vfn_0001_25FC3A9D() = 0;
		virtual void vfn_0002_25FC3A9D() = 0;
		virtual void vfn_0003_25FC3A9D() = 0;
	};

	/** ue::spatial::C_SectorSpatialSubdiv2::C_SectorLimitedVisitorWrapper (VTable=0x01E8CDD8) */
	class C_SectorLimitedVisitorWrapper : public C_VisitorWrapper_TPL_7788CC9D
	{
	public:
		virtual void vfn_0001_A858A00A() = 0;
		virtual void vfn_0002_A858A00A() = 0;
		virtual void vfn_0003_A858A00A() = 0;
	};

	/** ue::spatial::C_SectorSpatialSubdiv2::C_VisitorWrapper<false> (VTable=0x01E8CDF8) */
	class C_VisitorWrapper_TPL_4560BE8C : public ue::spatial::C_HSpatialHash_TPL_7925F87C::I_Visitor
	{
	public:
		virtual void vfn_0001_AF79336C() = 0;
		virtual void vfn_0002_AF79336C() = 0;
		virtual void vfn_0003_AF79336C() = 0;
	};

	/** ue::spatial::C_SectorSpatialSubdiv2::S_VisQueryCamInfo (VTable=0x01E8CE18) */
	struct S_VisQueryCamInfo : public ue::C_RefPtrCounter
	{
	public:
		virtual void vfn_0001_D679991A() = 0;
		virtual void vfn_0002_D679991A() = 0;
	};

	/** ue::spatial::C_SectorSpatialSubdiv2::C_VisQueryCtrl (VTable=0x01E8CE48) */
	class C_VisQueryCtrl : public ue::spatial::I_VSDRendererAdaptor::I_VisQueryCtrl
	{
	public:
		virtual void vfn_0001_0B6ECD5D() = 0;
		virtual void vfn_0002_0B6ECD5D() = 0;
		virtual void vfn_0003_0B6ECD5D() = 0;
	};

	/** ue::spatial::C_SectorSpatialSubdiv2::C_GPUBusyWorker (VTable=0x01E8CE88) */
	class C_GPUBusyWorker : public ue::I_VSDGPUBusyWorker
	{
	public:
		virtual void vfn_0001_D3E0CEFA() = 0;
		virtual void vfn_0002_D3E0CEFA() = 0;
		virtual void vfn_0003_D3E0CEFA() = 0;
	};

	virtual void vfn_0001_C13407FC() = 0;
	virtual void vfn_0002_C13407FC() = 0;
	virtual void vfn_0003_C13407FC() = 0;
	virtual void vfn_0004_C13407FC() = 0;
	virtual void vfn_0005_C13407FC() = 0;
	virtual void vfn_0006_C13407FC() = 0;
	virtual void vfn_0007_C13407FC() = 0;
	virtual void vfn_0008_C13407FC() = 0;
	virtual void vfn_0009_C13407FC() = 0;
	virtual void vfn_0010_C13407FC() = 0;
	virtual void vfn_0011_C13407FC() = 0;
	virtual void vfn_0012_C13407FC() = 0;
	virtual void vfn_0013_C13407FC() = 0;
	virtual void vfn_0014_C13407FC() = 0;
	virtual void vfn_0015_C13407FC() = 0;
	virtual void vfn_0016_C13407FC() = 0;
	virtual void vfn_0017_C13407FC() = 0;
	virtual void vfn_0018_C13407FC() = 0;
	virtual void vfn_0019_C13407FC() = 0;
	virtual void vfn_0020_C13407FC() = 0;
	virtual void vfn_0021_C13407FC() = 0;
	virtual void vfn_0022_C13407FC() = 0;
	virtual void vfn_0023_C13407FC() = 0;
	virtual void vfn_0024_C13407FC() = 0;
	virtual void vfn_0025_C13407FC() = 0;
	virtual void vfn_0026_C13407FC() = 0;
	virtual void vfn_0027_C13407FC() = 0;
	virtual void vfn_0028_C13407FC() = 0;
	virtual void vfn_0029_C13407FC() = 0;
};

} // namespace spatial

} // namespace ue
