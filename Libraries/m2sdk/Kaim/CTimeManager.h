// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <Kaim/CObject.h>

namespace Kaim
{

/** Kaim::CTimeManager (VTable=0x01E9D270) */
class CTimeManager : public CObject
{
public:
	/** Kaim::CTimeManager::CTeamInfo (VTable=0x01E9D280) */
	class CTeamInfo : public Kaim::CObject
	{
	public:
		virtual void vfn_0001_E535E4AE() = 0;
		virtual void vfn_0002_E535E4AE() = 0;
	};

	/** Kaim::CTimeManager::CEntityInfo (VTable=0x01E9D290) */
	class CEntityInfo : public Kaim::CObject
	{
	public:
		virtual void vfn_0001_B00F6B99() = 0;
		virtual void vfn_0002_B00F6B99() = 0;
	};

	virtual void vfn_0001_DD1F65D9() = 0;
	virtual void vfn_0002_DD1F65D9() = 0;
};

} // namespace Kaim
