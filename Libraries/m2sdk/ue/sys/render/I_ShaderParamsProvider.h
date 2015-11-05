// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/render/I_ShaderParamsProvider.h>

namespace ue
{

namespace sys
{

namespace render
{

/** ue::sys::render::I_ShaderParamsProvider (VTable=0x01E906F0) */
class I_ShaderParamsProvider
{
public:
	/** ue::sys::render::I_ShaderParamsProvider::I_ParamsGroup (VTable=0x01E90700) */
	class I_ParamsGroup
	{
	public:
		virtual void vfn_0001_90E2B9CB() = 0;
		virtual void vfn_0002_90E2B9CB() = 0;
		virtual void vfn_0003_90E2B9CB() = 0;
		virtual void vfn_0004_90E2B9CB() = 0;
		virtual void vfn_0005_90E2B9CB() = 0;
	};

	/** ue::sys::render::I_ShaderParamsProvider::S_ParamGroup (VTable=0x01E90680) */
	struct S_ParamGroup : public I_ParamsGroup
	{
	public:
		virtual void vfn_0001_A887E9F6() = 0;
		virtual void vfn_0002_A887E9F6() = 0;
		virtual void vfn_0003_A887E9F6() = 0;
		virtual void vfn_0004_A887E9F6() = 0;
		virtual void vfn_0005_A887E9F6() = 0;
	};

	virtual void vfn_0001_60CCC460() = 0;
	virtual void vfn_0002_60CCC460() = 0;
};

} // namespace render

} // namespace sys

} // namespace ue
