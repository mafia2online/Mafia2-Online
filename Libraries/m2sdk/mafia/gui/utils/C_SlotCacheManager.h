// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_SlotManager.h>

namespace mafia
{

namespace gui
{

namespace utils
{

/** mafia::gui::utils::C_SlotCacheManager (VTable=0x01E6C268) */
class C_SlotCacheManager : public C_SlotManager::C_Events
{
public:
	/** mafia::gui::utils::C_SlotCacheManager::I_SlotWatcher (VTable=0x01E745E0) */
	class I_SlotWatcher
	{
	public:
		virtual void vfn_0001_0A56CD1B() = 0;
		virtual void vfn_0002_0A56CD1B() = 0;
		virtual void vfn_0003_0A56CD1B() = 0;
		virtual void vfn_0004_0A56CD1B() = 0;
		virtual void vfn_0005_0A56CD1B() = 0;
	};

	virtual void vfn_0001_239EDBB0() = 0;
	virtual void vfn_0002_239EDBB0() = 0;
	virtual void vfn_0003_239EDBB0() = 0;
};

} // namespace utils

} // namespace gui

} // namespace mafia
