// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/audio/implementation/C_ContextSoundObserver.h>

namespace ue
{

namespace sys
{

namespace audio
{

namespace implementation
{

/** ue::sys::audio::implementation::C_ContextSoundObserverActions (VTable=0xFFFFFFFF) */
class C_ContextSoundObserverActions
{
public:
	/** ue::sys::audio::implementation::C_ContextSoundObserverActions::C_StopAction (VTable=0x01E839A8) */
	class C_StopAction : public ue::sys::audio::implementation::C_ContextSoundObserver::I_Action
	{
	public:
		virtual void vfn_0001_DC4BF19F() = 0;
		virtual void vfn_0002_DC4BF19F() = 0;
		virtual void vfn_0003_DC4BF19F() = 0;
	};

	/** ue::sys::audio::implementation::C_ContextSoundObserverActions::C_PlayAction (VTable=0x01E83968) */
	class C_PlayAction : public ue::sys::audio::implementation::C_ContextSoundObserver::I_Action
	{
	public:
		virtual void vfn_0001_81740D12() = 0;
		virtual void vfn_0002_81740D12() = 0;
		virtual void vfn_0003_81740D12() = 0;
	};

	/** ue::sys::audio::implementation::C_ContextSoundObserverActions::C_DeleteAction (VTable=0x01E839C8) */
	class C_DeleteAction : public ue::sys::audio::implementation::C_ContextSoundObserver::I_Action
	{
	public:
		virtual void vfn_0001_10B99752() = 0;
		virtual void vfn_0002_10B99752() = 0;
		virtual void vfn_0003_10B99752() = 0;
	};
};

} // namespace implementation

} // namespace audio

} // namespace sys

} // namespace ue
