// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/I_SeekableInputStream.h>
#include <ue/sys/filesystem/C_StreamCache.h>

namespace ue
{

namespace sys
{

namespace filesystem
{

/** ue::sys::filesystem::C_FileSeekableInputStream (VTable=0x01E96680) */
class C_FileSeekableInputStream : public ue::I_SeekableInputStream
{
public:
	/** ue::sys::filesystem::C_FileSeekableInputStream::C_CacheReadInterface (VTable=0x01E969A0) */
	class C_CacheReadInterface : public ue::sys::filesystem::C_StreamCache::I_ReadInterface
	{
	public:
		virtual void vfn_0001_A6A1B922() = 0;
		virtual void vfn_0002_A6A1B922() = 0;
		virtual void vfn_0003_A6A1B922() = 0;
		virtual void vfn_0004_A6A1B922() = 0;
		virtual void vfn_0005_A6A1B922() = 0;
		virtual void vfn_0006_A6A1B922() = 0;
	};

	virtual void vfn_0001_0C42703D() = 0;
	virtual void vfn_0002_0C42703D() = 0;
	virtual void vfn_0003_0C42703D() = 0;
	virtual void vfn_0004_0C42703D() = 0;
	virtual void vfn_0005_0C42703D() = 0;
	virtual void vfn_0006_0C42703D() = 0;
	virtual void vfn_0007_0C42703D() = 0;
	virtual void vfn_0008_0C42703D() = 0;
	virtual void vfn_0009_0C42703D() = 0;
	virtual void vfn_0010_0C42703D() = 0;
	virtual void vfn_0011_0C42703D() = 0;
};

} // namespace filesystem

} // namespace sys

} // namespace ue
