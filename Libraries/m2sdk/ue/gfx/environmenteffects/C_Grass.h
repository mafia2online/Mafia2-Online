// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/gfx/environmenteffects/I_Grass.h>
#include <ue/I_Callback_TPL_9AA54AEF.h>
#include <ue/sys/database/I_Observer.h>

namespace ue
{

namespace gfx
{

namespace environmenteffects
{

/** ue::gfx::environmenteffects::C_Grass (VTable=0x01E6B120) */
class C_Grass : public I_Grass, public ue::I_Callback_TPL_9AA54AEF
{
public:
	/** ue::gfx::environmenteffects::C_Grass::C_MixtureTableObserver (VTable=0x01E6B0F0) */
	class C_MixtureTableObserver : public ue::sys::database::I_Observer
	{
	public:
		virtual void vfn_0001_5ECA9FEE() = 0;
		virtual void vfn_0002_5ECA9FEE() = 0;
		virtual void vfn_0003_5ECA9FEE() = 0;
	};

	virtual void vfn_0001_F3A95BCB() = 0;
	virtual void vfn_0002_F3A95BCB() = 0;
	virtual void vfn_0003_F3A95BCB() = 0;
	virtual void vfn_0004_F3A95BCB() = 0;
	virtual void vfn_0005_F3A95BCB() = 0;
	virtual void vfn_0006_F3A95BCB() = 0;
	virtual void vfn_0007_F3A95BCB() = 0;
	virtual void vfn_0008_F3A95BCB() = 0;
	virtual void vfn_0009_F3A95BCB() = 0;
	virtual void vfn_0010_F3A95BCB() = 0;
	virtual void vfn_0011_F3A95BCB() = 0;
	virtual void vfn_0012_F3A95BCB() = 0;
	virtual void vfn_0013_F3A95BCB() = 0;
	virtual void vfn_0014_F3A95BCB() = 0;
	virtual void vfn_0015_F3A95BCB() = 0;
	virtual void vfn_0016_F3A95BCB() = 0;
	virtual void vfn_0017_F3A95BCB() = 0;
	virtual void vfn_0018_F3A95BCB() = 0;
	virtual void vfn_0019_F3A95BCB() = 0;
};

} // namespace environmenteffects

} // namespace gfx

} // namespace ue
