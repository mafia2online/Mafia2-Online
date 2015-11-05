// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/spatial/I_SpatialSubdivVisitor.h>
#include <ue/sys/render/I_SceneQuery.h>

namespace ue
{

namespace sys
{

namespace render
{

/** ue::sys::render::I_SceneVisitor (VTable=0x01E922C0) */
class I_SceneVisitor : public ue::spatial::I_SpatialSubdivVisitor, public I_SceneQuery
{
public:
	virtual void vfn_0001_CE8F5FB4() = 0;
	virtual void vfn_0002_CE8F5FB4() = 0;
	virtual void vfn_0003_CE8F5FB4() = 0;
	virtual void vfn_0004_CE8F5FB4() = 0;
	virtual void vfn_0005_CE8F5FB4() = 0;
	virtual void vfn_0006_CE8F5FB4() = 0;
};

} // namespace render

} // namespace sys

} // namespace ue
