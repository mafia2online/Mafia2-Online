// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <ue/C_RefPtrCounter.h>

namespace ue
{

namespace sys
{

namespace compression
{

namespace huffman
{

/** ue::sys::compression::huffman::C_DataPack (VTable=0x01E90490) */
class C_DataPack : public ue::C_RefPtrCounter
{
public:
	/** ue::sys::compression::huffman::C_DataPack::C_Block (VTable=0x01E904B0) */
	class C_Block : public ue::C_RefPtrCounter
	{
	public:
		virtual void vfn_0001_0DE06085() = 0;
		virtual void vfn_0002_0DE06085() = 0;
	};

	virtual void vfn_0001_05E0D6B9() = 0;
	virtual void vfn_0002_05E0D6B9() = 0;
};

} // namespace huffman

} // namespace compression

} // namespace sys

} // namespace ue
