// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <CryptoPP/Exception.h>
#include <CryptoPP/PK_SignatureScheme.h>

namespace CryptoPP
{

/** CryptoPP::PK_SignatureScheme (VTable=0x01EB1020) */
class PK_SignatureScheme
{
public:
	/** CryptoPP::PK_SignatureScheme::InvalidKeyLength (VTable=0x01EB2E68) */
	class InvalidKeyLength : public CryptoPP::Exception
	{
	public:
		virtual void vfn_0001_2B4E470D() = 0;
		virtual void vfn_0002_2B4E470D() = 0;
		virtual void vfn_0003_2B4E470D() = 0;
	};

	/** CryptoPP::PK_SignatureScheme::KeyTooShort (VTable=0x01EB2E50) */
	class KeyTooShort : public InvalidKeyLength
	{
	public:
		virtual void vfn_0001_FF39AB7D() = 0;
		virtual void vfn_0002_FF39AB7D() = 0;
		virtual void vfn_0003_FF39AB7D() = 0;
	};

	virtual void vfn_0001_185CBFC5() = 0;
	virtual void vfn_0002_185CBFC5() = 0;
	virtual void vfn_0003_185CBFC5() = 0;
	virtual void vfn_0004_185CBFC5() = 0;
	virtual void vfn_0005_185CBFC5() = 0;
	virtual void vfn_0006_185CBFC5() = 0;
	virtual void vfn_0007_185CBFC5() = 0;
	virtual void vfn_0008_185CBFC5() = 0;
	virtual void vfn_0009_185CBFC5() = 0;
	virtual void vfn_0010_185CBFC5() = 0;
};

} // namespace CryptoPP
