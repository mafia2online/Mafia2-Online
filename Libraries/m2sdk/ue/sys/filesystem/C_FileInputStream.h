// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/I_InputStream.h>
#include <ue/sys/filesystem/C_StreamCache.h>

namespace ue
{

namespace sys
{

namespace filesystem
{

/** ue::sys::filesystem::C_FileInputStream (VTable=0x01E966C0) */
class C_FileInputStream : public ue::I_InputStream
{
public:
	/** ue::sys::filesystem::C_FileInputStream::C_CacheReadInterface (VTable=0x01E96A00) */
	class C_CacheReadInterface : public ue::sys::filesystem::C_StreamCache::I_ReadInterface
	{
	public:
		virtual void vfn_0001_1E7EE7FF() = 0;
		virtual void vfn_0002_1E7EE7FF() = 0;
		virtual void vfn_0003_1E7EE7FF() = 0;
		virtual void vfn_0004_1E7EE7FF() = 0;
		virtual void vfn_0005_1E7EE7FF() = 0;
		virtual void vfn_0006_1E7EE7FF() = 0;
	};

	virtual void vfn_0001_3E261BB0() = 0;
	virtual void vfn_0002_3E261BB0() = 0;
	virtual void vfn_0003_3E261BB0() = 0;
	virtual void vfn_0004_3E261BB0() = 0;
	virtual void vfn_0005_3E261BB0() = 0;
	virtual void vfn_0006_3E261BB0() = 0;
	virtual void vfn_0007_3E261BB0() = 0;
	virtual void vfn_0008_3E261BB0() = 0;
	virtual void vfn_0009_3E261BB0() = 0;
};

} // namespace filesystem

} // namespace sys

} // namespace ue
