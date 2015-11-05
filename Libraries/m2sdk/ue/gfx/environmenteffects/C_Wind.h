// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/gfx/environmenteffects/I_Wind.h>
#include <ue/sys/database/I_Observer.h>

namespace ue
{

namespace gfx
{

namespace environmenteffects
{

/** ue::gfx::environmenteffects::C_Wind (VTable=0x01E6AF40) */
class C_Wind : public I_Wind
{
public:
	/** ue::gfx::environmenteffects::C_Wind::C_TableObserver (VTable=0x01E6AF18) */
	class C_TableObserver : public ue::sys::database::I_Observer
	{
	public:
		virtual void vfn_0001_65B78EDC() = 0;
		virtual void vfn_0002_65B78EDC() = 0;
		virtual void vfn_0003_65B78EDC() = 0;
	};

	virtual void vfn_0001_CA9CCAFB() = 0;
	virtual void vfn_0002_CA9CCAFB() = 0;
	virtual void vfn_0003_CA9CCAFB() = 0;
	virtual void vfn_0004_CA9CCAFB() = 0;
	virtual void vfn_0005_CA9CCAFB() = 0;
	virtual void vfn_0006_CA9CCAFB() = 0;
	virtual void vfn_0007_CA9CCAFB() = 0;
	virtual void vfn_0008_CA9CCAFB() = 0;
	virtual void vfn_0009_CA9CCAFB() = 0;
	virtual void vfn_0010_CA9CCAFB() = 0;
	virtual void vfn_0011_CA9CCAFB() = 0;
	virtual void vfn_0012_CA9CCAFB() = 0;
	virtual void vfn_0013_CA9CCAFB() = 0;
	virtual void vfn_0014_CA9CCAFB() = 0;
};

} // namespace environmenteffects

} // namespace gfx

} // namespace ue
