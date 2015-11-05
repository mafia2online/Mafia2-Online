// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/sys/compression/huffman/C_Decompressor.h>
#include <ue/C_RefPtrCounter.h>

namespace ue
{

namespace sys
{

namespace compression
{

namespace huffman
{

/** ue::sys::compression::huffman::C_Decompressor (VTable=0xFFFFFFFF) */
class C_Decompressor
{
public:
	/** ue::sys::compression::huffman::C_Decompressor::C_MemHolder (VTable=0xFFFFFFFF) */
	class C_MemHolder : public ue::C_RefPtrCounter
	{
	public:
	};

	/** ue::sys::compression::huffman::C_Decompressor::C_NoDeleteMemHolder (VTable=0x01E7B678) */
	class C_NoDeleteMemHolder : public C_MemHolder
	{
	public:
		virtual void vfn_0001_EC9A1B7A() = 0;
		virtual void vfn_0002_EC9A1B7A() = 0;
		virtual void vfn_0003_EC9A1B7A() = 0;
	};
};

} // namespace huffman

} // namespace compression

} // namespace sys

} // namespace ue
