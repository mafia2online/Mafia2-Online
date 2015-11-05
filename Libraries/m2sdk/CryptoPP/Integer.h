// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <CryptoPP/InitializeInteger.h>
#include <CryptoPP/ASN1Object.h>
#include <CryptoPP/Exception.h>

namespace CryptoPP
{

/** CryptoPP::Integer (VTable=0x01EB2538) */
class Integer : public InitializeInteger, public ASN1Object
{
public:
	/** CryptoPP::Integer::RandomNumberNotFound (VTable=0x01EB41F0) */
	class RandomNumberNotFound : public CryptoPP::Exception
	{
	public:
		virtual void vfn_0001_2E4E28A2() = 0;
		virtual void vfn_0002_2E4E28A2() = 0;
		virtual void vfn_0003_2E4E28A2() = 0;
	};

	/** CryptoPP::Integer::DivideByZero (VTable=0x01EB2650) */
	class DivideByZero : public CryptoPP::Exception
	{
	public:
		virtual void vfn_0001_F110066B() = 0;
		virtual void vfn_0002_F110066B() = 0;
		virtual void vfn_0003_F110066B() = 0;
	};

	virtual void vfn_0001_B26AB9D4() = 0;
	virtual void vfn_0002_B26AB9D4() = 0;
	virtual void vfn_0003_B26AB9D4() = 0;
	virtual void vfn_0004_B26AB9D4() = 0;
	virtual void vfn_0005_B26AB9D4() = 0;
};

} // namespace CryptoPP
