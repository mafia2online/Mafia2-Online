// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/threads/I_ThreadProcedure.h>

namespace ue
{

namespace game
{

namespace audio
{

/** ue::game::audio::C_AudioDataStreaming (VTable=0xFFFFFFFF) */
class C_AudioDataStreaming
{
public:
	/** ue::game::audio::C_AudioDataStreaming::C_StreamingProc (VTable=0x01E5A0E0) */
	class C_StreamingProc : public ue::sys::threads::I_ThreadProcedure
	{
	public:
		virtual void vfn_0001_19750075() = 0;
		virtual void vfn_0002_19750075() = 0;
		virtual void vfn_0003_19750075() = 0;
		virtual void vfn_0004_19750075() = 0;
		virtual void vfn_0005_19750075() = 0;
	};

	/** ue::game::audio::C_AudioDataStreaming::I_StreamFunctor (VTable=0xFFFFFFFF) */
	class I_StreamFunctor
	{
	public:
	};

	/** ue::game::audio::C_AudioDataStreaming::I_AudioDataStreamingNotify (VTable=0x01E4F8E0) */
	class I_AudioDataStreamingNotify
	{
	public:
		virtual void vfn_0001_D920B69E() = 0;
		virtual void vfn_0002_D920B69E() = 0;
		virtual void vfn_0003_D920B69E() = 0;
		virtual void vfn_0004_D920B69E() = 0;
		virtual void vfn_0005_D920B69E() = 0;
		virtual void vfn_0006_D920B69E() = 0;
	};
};

} // namespace audio

} // namespace game

} // namespace ue
