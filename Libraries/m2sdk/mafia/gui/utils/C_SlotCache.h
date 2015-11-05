// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_SlotManager.h>

namespace mafia
{

namespace gui
{

namespace utils
{

/** mafia::gui::utils::C_SlotCache (VTable=0x01E6C280) */
class C_SlotCache : public C_SlotManager::C_Events
{
public:
	/** mafia::gui::utils::C_SlotCache::I_SlotCacheCallback (VTable=0x01E55160) */
	class I_SlotCacheCallback
	{
	public:
		virtual void vfn_0001_387DC760() = 0;
		virtual void vfn_0002_387DC760() = 0;
		virtual void vfn_0003_387DC760() = 0;
		virtual void vfn_0004_387DC760() = 0;
		virtual void vfn_0005_387DC760() = 0;
		virtual void vfn_0006_387DC760() = 0;
		virtual void vfn_0007_387DC760() = 0;
	};

	virtual void vfn_0001_033EBBDE() = 0;
	virtual void vfn_0002_033EBBDE() = 0;
	virtual void vfn_0003_033EBBDE() = 0;
};

} // namespace utils

} // namespace gui

} // namespace mafia
