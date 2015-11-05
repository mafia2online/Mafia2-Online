// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_EntityListenerCallback.h>
#include <C_HumanAIState.h>

/** C_HumanAIState (VTable=0x01E75238) */
class C_HumanAIState : public C_EntityListenerCallback
{
public:
	/** C_HumanAIState::S_AimLook (VTable=0x01E75270) */
	struct S_AimLook
	{
	public:
		virtual void vfn_0001_DF1AD158() = 0;
		virtual void vfn_0002_DF1AD158() = 0;
		virtual void vfn_0003_DF1AD158() = 0;
		virtual void vfn_0004_DF1AD158() = 0;
		virtual void vfn_0005_DF1AD158() = 0;
	};

	/** C_HumanAIState::S_Look (VTable=0x01E75250) */
	struct S_Look : public S_AimLook
	{
	public:
		virtual void vfn_0001_DF89F5D2() = 0;
		virtual void vfn_0002_DF89F5D2() = 0;
		virtual void vfn_0003_DF89F5D2() = 0;
		virtual void vfn_0004_DF89F5D2() = 0;
		virtual void vfn_0005_DF89F5D2() = 0;
	};

	/** C_HumanAIState::S_Aim (VTable=0x01E76340) */
	struct S_Aim : public S_AimLook
	{
	public:
		virtual void vfn_0001_6DFFAECD() = 0;
		virtual void vfn_0002_6DFFAECD() = 0;
		virtual void vfn_0003_6DFFAECD() = 0;
		virtual void vfn_0004_6DFFAECD() = 0;
		virtual void vfn_0005_6DFFAECD() = 0;
	};

	virtual void vfn_0001_BDA92D85() = 0;
	virtual void vfn_0002_BDA92D85() = 0;
	virtual void vfn_0003_BDA92D85() = 0;
};
