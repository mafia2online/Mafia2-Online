// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/C_RefPtrCounter.h>

namespace ue
{

namespace spatial
{

/** ue::spatial::C_HSpatialHash_TPL_7925F87C (VTable=0xFFFFFFFF) */
class C_HSpatialHash_TPL_7925F87C
{
public:
	/** ue::spatial::C_HSpatialHash<ue::spatial::SpatialTypesCD,ue::spatial::C_SectorSpatialSubdiv2::C_Obj>::I_Visitor (VTable=0x01E8CD80) */
	class I_Visitor
	{
	public:
		virtual void vfn_0001_0E8496EE() = 0;
		virtual void vfn_0002_0E8496EE() = 0;
		virtual void vfn_0003_0E8496EE() = 0;
	};

	/** ue::spatial::C_HSpatialHash<ue::spatial::SpatialTypesCD,ue::spatial::C_SectorSpatialSubdiv2::C_Obj>::S_Level (VTable=0x01E8CF40) */
	struct S_Level : public ue::C_RefPtrCounter
	{
	public:
		virtual void vfn_0001_01F94370() = 0;
		virtual void vfn_0002_01F94370() = 0;
	};
};

} // namespace spatial

} // namespace ue
