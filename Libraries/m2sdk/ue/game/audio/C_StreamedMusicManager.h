// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/game/audio/C_AudioDataStreaming.h>

namespace ue
{

namespace game
{

namespace audio
{

/** ue::game::audio::C_StreamedMusicManager (VTable=0xFFFFFFFF) */
class C_StreamedMusicManager
{
public:
	/** ue::game::audio::C_StreamedMusicManager::C_StreamedMusicSound (VTable=0x01E4F680) */
	class C_StreamedMusicSound : public ue::game::audio::C_AudioDataStreaming::I_AudioDataStreamingNotify
	{
	public:
		virtual void vfn_0001_18F3717C() = 0;
		virtual void vfn_0002_18F3717C() = 0;
		virtual void vfn_0003_18F3717C() = 0;
		virtual void vfn_0004_18F3717C() = 0;
		virtual void vfn_0005_18F3717C() = 0;
		virtual void vfn_0006_18F3717C() = 0;
	};
};

} // namespace audio

} // namespace game

} // namespace ue
