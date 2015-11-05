// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/C_RefPtrCounter.h>

namespace ue
{

namespace spatial
{

/** ue::spatial::C_HSpatialHash_TPL_71DFE143 (VTable=0xFFFFFFFF) */
class C_HSpatialHash_TPL_71DFE143
{
public:
	/** ue::spatial::C_HSpatialHash<ue::spatial::SpatialTypesCD,ue::spatial::I_BasicSubdivObj>::I_Visitor (VTable=0x01E8CF88) */
	class I_Visitor
	{
	public:
		virtual void vfn_0001_ABAA29E8() = 0;
		virtual void vfn_0002_ABAA29E8() = 0;
		virtual void vfn_0003_ABAA29E8() = 0;
	};

	/** ue::spatial::C_HSpatialHash<ue::spatial::SpatialTypesCD,ue::spatial::I_BasicSubdivObj>::S_Level (VTable=0x01E8D008) */
	struct S_Level : public ue::C_RefPtrCounter
	{
	public:
		virtual void vfn_0001_B504F472() = 0;
		virtual void vfn_0002_B504F472() = 0;
	};
};

} // namespace spatial

} // namespace ue
