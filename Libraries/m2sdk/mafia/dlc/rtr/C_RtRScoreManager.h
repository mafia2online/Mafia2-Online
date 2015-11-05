// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/game/params/C_DifficultyManager.h>

namespace mafia
{

namespace dlc
{

namespace rtr
{

/** mafia::dlc::rtr::C_RtRScoreManager (VTable=0xFFFFFFFF) */
class C_RtRScoreManager
{
public:
	/** mafia::dlc::rtr::C_RtRScoreManager::I_RtREventListener (VTable=0xFFFFFFFF) */
	class I_RtREventListener
	{
	public:
	};

	/** mafia::dlc::rtr::C_RtRScoreManager::C_RetryHandler (VTable=0x01E7EBE0) */
	class C_RetryHandler : public ue::game::params::C_DifficultyManager::I_NotifyHandler
	{
	public:
		virtual void vfn_0001_7DECB2D7() = 0;
		virtual void vfn_0002_7DECB2D7() = 0;
		virtual void vfn_0003_7DECB2D7() = 0;
		virtual void vfn_0004_7DECB2D7() = 0;
		virtual void vfn_0005_7DECB2D7() = 0;
		virtual void vfn_0006_7DECB2D7() = 0;
	};
};

} // namespace rtr

} // namespace dlc

} // namespace mafia
