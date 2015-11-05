// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/audio/driver/I_DataManager.h>
#include <ue/sys/audio/driver/fmod/C_DataManagerBinary.h>
#include <ue/sys/audio/C_RefCounter.h>

namespace ue
{

namespace sys
{

namespace audio
{

namespace driver
{

namespace fmod
{

/** ue::sys::audio::driver::fmod::C_DataManagerBinary (VTable=0x01E84040) */
class C_DataManagerBinary : public ue::sys::audio::driver::I_DataManager
{
public:
	/** ue::sys::audio::driver::fmod::C_DataManagerBinary::C_Record (VTable=0x01E84020) */
	class C_Record : public ue::sys::audio::C_RefCounter
	{
	public:
		virtual void vfn_0001_CB75C0CC() = 0;
		virtual void vfn_0002_CB75C0CC() = 0;
		virtual void vfn_0003_CB75C0CC() = 0;
		virtual void vfn_0004_CB75C0CC() = 0;
	};

	/** ue::sys::audio::driver::fmod::C_DataManagerBinary::C_StreamRecord (VTable=0x01E83FF0) */
	class C_StreamRecord : public C_Record
	{
	public:
		virtual void vfn_0001_F6F39B93() = 0;
		virtual void vfn_0002_F6F39B93() = 0;
		virtual void vfn_0003_F6F39B93() = 0;
		virtual void vfn_0004_F6F39B93() = 0;
	};

	/** ue::sys::audio::driver::fmod::C_DataManagerBinary::C_StaticRecord (VTable=0x01E84008) */
	class C_StaticRecord : public C_Record
	{
	public:
		virtual void vfn_0001_2C72EA14() = 0;
		virtual void vfn_0002_2C72EA14() = 0;
		virtual void vfn_0003_2C72EA14() = 0;
		virtual void vfn_0004_2C72EA14() = 0;
	};

	/** ue::sys::audio::driver::fmod::C_DataManagerBinary::I_InputInterface (VTable=0xFFFFFFFF) */
	class I_InputInterface
	{
	public:
	};

	/** ue::sys::audio::driver::fmod::C_DataManagerBinary::C_FileInput (VTable=0x01E83F00) */
	class C_FileInput : public I_InputInterface
	{
	public:
		virtual void vfn_0001_AD1FBFA5() = 0;
		virtual void vfn_0002_AD1FBFA5() = 0;
		virtual void vfn_0003_AD1FBFA5() = 0;
		virtual void vfn_0004_AD1FBFA5() = 0;
		virtual void vfn_0005_AD1FBFA5() = 0;
		virtual void vfn_0006_AD1FBFA5() = 0;
		virtual void vfn_0007_AD1FBFA5() = 0;
		virtual void vfn_0008_AD1FBFA5() = 0;
		virtual void vfn_0009_AD1FBFA5() = 0;
	};

	/** ue::sys::audio::driver::fmod::C_DataManagerBinary::C_FileSeekableInput (VTable=0x01E83F40) */
	class C_FileSeekableInput : public I_InputInterface
	{
	public:
		virtual void vfn_0001_143F272A() = 0;
		virtual void vfn_0002_143F272A() = 0;
		virtual void vfn_0003_143F272A() = 0;
		virtual void vfn_0004_143F272A() = 0;
		virtual void vfn_0005_143F272A() = 0;
		virtual void vfn_0006_143F272A() = 0;
		virtual void vfn_0007_143F272A() = 0;
		virtual void vfn_0008_143F272A() = 0;
		virtual void vfn_0009_143F272A() = 0;
	};

	/** ue::sys::audio::driver::fmod::C_DataManagerBinary::C_StreamInput (VTable=0x01E83F80) */
	class C_StreamInput : public I_InputInterface
	{
	public:
		virtual void vfn_0001_326D34A1() = 0;
		virtual void vfn_0002_326D34A1() = 0;
		virtual void vfn_0003_326D34A1() = 0;
		virtual void vfn_0004_326D34A1() = 0;
		virtual void vfn_0005_326D34A1() = 0;
		virtual void vfn_0006_326D34A1() = 0;
		virtual void vfn_0007_326D34A1() = 0;
		virtual void vfn_0008_326D34A1() = 0;
		virtual void vfn_0009_326D34A1() = 0;
	};

	/** ue::sys::audio::driver::fmod::C_DataManagerBinary::C_StreamSeekableInput (VTable=0x01E83FC0) */
	class C_StreamSeekableInput : public I_InputInterface
	{
	public:
		virtual void vfn_0001_22A9657F() = 0;
		virtual void vfn_0002_22A9657F() = 0;
		virtual void vfn_0003_22A9657F() = 0;
		virtual void vfn_0004_22A9657F() = 0;
		virtual void vfn_0005_22A9657F() = 0;
		virtual void vfn_0006_22A9657F() = 0;
		virtual void vfn_0007_22A9657F() = 0;
		virtual void vfn_0008_22A9657F() = 0;
		virtual void vfn_0009_22A9657F() = 0;
	};

	virtual void vfn_0001_BA83B06E() = 0;
	virtual void vfn_0002_BA83B06E() = 0;
	virtual void vfn_0003_BA83B06E() = 0;
	virtual void vfn_0004_BA83B06E() = 0;
	virtual void vfn_0005_BA83B06E() = 0;
	virtual void vfn_0006_BA83B06E() = 0;
	virtual void vfn_0007_BA83B06E() = 0;
	virtual void vfn_0008_BA83B06E() = 0;
	virtual void vfn_0009_BA83B06E() = 0;
	virtual void vfn_0010_BA83B06E() = 0;
	virtual void vfn_0011_BA83B06E() = 0;
	virtual void vfn_0012_BA83B06E() = 0;
	virtual void vfn_0013_BA83B06E() = 0;
	virtual void vfn_0014_BA83B06E() = 0;
	virtual void vfn_0015_BA83B06E() = 0;
	virtual void vfn_0016_BA83B06E() = 0;
	virtual void vfn_0017_BA83B06E() = 0;
	virtual void vfn_0018_BA83B06E() = 0;
	virtual void vfn_0019_BA83B06E() = 0;
};

} // namespace fmod

} // namespace driver

} // namespace audio

} // namespace sys

} // namespace ue
