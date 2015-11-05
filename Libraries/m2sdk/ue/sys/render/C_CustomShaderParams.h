// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/render/I_ShaderParamsProvider.h>

namespace ue
{

namespace sys
{

namespace render
{

/** ue::sys::render::C_CustomShaderParams (VTable=0x01E8F810) */
class C_CustomShaderParams : public I_ShaderParamsProvider
{
public:
	/** ue::sys::render::C_CustomShaderParams::S_ViewGroupParams (VTable=0x01E90640) */
	struct S_ViewGroupParams : public ue::sys::render::I_ShaderParamsProvider::S_ParamGroup
	{
	public:
		virtual void vfn_0001_8CAE5907() = 0;
		virtual void vfn_0002_8CAE5907() = 0;
		virtual void vfn_0003_8CAE5907() = 0;
		virtual void vfn_0004_8CAE5907() = 0;
		virtual void vfn_0005_8CAE5907() = 0;
	};

	/** ue::sys::render::C_CustomShaderParams::S_ObjectGroupParams (VTable=0x01E906A0) */
	struct S_ObjectGroupParams : public ue::sys::render::I_ShaderParamsProvider::S_ParamGroup
	{
	public:
		virtual void vfn_0001_12C1C7CC() = 0;
		virtual void vfn_0002_12C1C7CC() = 0;
		virtual void vfn_0003_12C1C7CC() = 0;
		virtual void vfn_0004_12C1C7CC() = 0;
		virtual void vfn_0005_12C1C7CC() = 0;
	};

	/** ue::sys::render::C_CustomShaderParams::S_MaterialGroupParams (VTable=0x01E906C8) */
	struct S_MaterialGroupParams : public ue::sys::render::I_ShaderParamsProvider::S_ParamGroup
	{
	public:
		virtual void vfn_0001_32ECC316() = 0;
		virtual void vfn_0002_32ECC316() = 0;
		virtual void vfn_0003_32ECC316() = 0;
		virtual void vfn_0004_32ECC316() = 0;
		virtual void vfn_0005_32ECC316() = 0;
	};

	virtual void vfn_0001_AAE0493C() = 0;
	virtual void vfn_0002_AAE0493C() = 0;
};

} // namespace render

} // namespace sys

} // namespace ue
