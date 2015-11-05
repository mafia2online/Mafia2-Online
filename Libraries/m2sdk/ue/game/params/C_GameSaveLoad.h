// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/game/params/I_Serializer.h>
#include <ue/sys/userservices/I_SysNotificationCallback.h>

namespace ue
{

namespace game
{

namespace params
{

/** ue::game::params::C_GameSaveLoad (VTable=0x01E64440) */
class C_GameSaveLoad : public I_Serializer
{
public:
	/** ue::game::params::C_GameSaveLoad::C_SysNotificationCallback (VTable=0x01E64460) */
	class C_SysNotificationCallback : public ue::sys::userservices::I_SysNotificationCallback
	{
	public:
		virtual void vfn_0001_D8A792DA() = 0;
		virtual void vfn_0002_D8A792DA() = 0;
		virtual void vfn_0003_D8A792DA() = 0;
	};

	virtual void vfn_0001_EDFCAFD0() = 0;
	virtual void vfn_0002_EDFCAFD0() = 0;
	virtual void vfn_0003_EDFCAFD0() = 0;
	virtual void vfn_0004_EDFCAFD0() = 0;
	virtual void vfn_0005_EDFCAFD0() = 0;
	virtual void vfn_0006_EDFCAFD0() = 0;
};

} // namespace params

} // namespace game

} // namespace ue
