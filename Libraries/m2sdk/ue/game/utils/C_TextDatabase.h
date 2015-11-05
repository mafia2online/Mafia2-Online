// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/game/params/I_SaveLoadCallBack.h>
#include <ue/sys/core/I_MemFileManagerListener.h>
#include <ue/sys/core/I_XMLManagerListener.h>

namespace ue
{

namespace game
{

namespace utils
{

/** ue::game::utils::C_TextDatabase (VTable=0xFFFFFFFF) */
class C_TextDatabase
{
public:
	/** ue::game::utils::C_TextDatabase::C_SaveLoadCallback (VTable=0x01E65100) */
	class C_SaveLoadCallback : public ue::game::params::I_SaveLoadCallBack
	{
	public:
		virtual void vfn_0001_CCBBC3AD() = 0;
		virtual void vfn_0002_CCBBC3AD() = 0;
		virtual void vfn_0003_CCBBC3AD() = 0;
		virtual void vfn_0004_CCBBC3AD() = 0;
	};

	/** ue::game::utils::C_TextDatabase::C_TextDatabaseMemFileListener (VTable=0x01E65118) */
	class C_TextDatabaseMemFileListener : public ue::sys::core::I_MemFileManagerListener
	{
	public:
		virtual void vfn_0001_DE83B92C() = 0;
		virtual void vfn_0002_DE83B92C() = 0;
		virtual void vfn_0003_DE83B92C() = 0;
		virtual void vfn_0004_DE83B92C() = 0;
	};

	/** ue::game::utils::C_TextDatabase::C_TextDatabaseXMLListener (VTable=0x01E65140) */
	class C_TextDatabaseXMLListener : public ue::sys::core::I_XMLManagerListener
	{
	public:
		virtual void vfn_0001_5BCE3571() = 0;
		virtual void vfn_0002_5BCE3571() = 0;
		virtual void vfn_0003_5BCE3571() = 0;
		virtual void vfn_0004_5BCE3571() = 0;
		virtual void vfn_0005_5BCE3571() = 0;
		virtual void vfn_0006_5BCE3571() = 0;
	};
};

} // namespace utils

} // namespace game

} // namespace ue
