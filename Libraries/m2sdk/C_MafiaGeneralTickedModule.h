// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <C_TickedModule.h>
#include <C_PlayerModelManager.h>

/** C_MafiaGeneralTickedModule (VTable=0x01E7AA00) */
class C_MafiaGeneralTickedModule : public C_TickedModule
{
public:
	/** C_MafiaGeneralTickedModule::C_ChangePlayerModel (VTable=0x01E7B1C0) */
	class C_ChangePlayerModel : public C_PlayerModelManager::C_ChangeModelCommand
	{
	public:
		virtual void vfn_0001_B4B0B506() = 0;
		virtual void vfn_0002_B4B0B506() = 0;
		virtual void vfn_0003_B4B0B506() = 0;
		virtual void vfn_0004_B4B0B506() = 0;
		virtual void vfn_0005_B4B0B506() = 0;
		virtual void vfn_0006_B4B0B506() = 0;
		virtual void vfn_0007_B4B0B506() = 0;
	};

	virtual void vfn_0001_36DA534E() = 0;
	virtual void vfn_0002_36DA534E() = 0;
	virtual void vfn_0003_36DA534E() = 0;
	virtual void vfn_0004_36DA534E() = 0;
	virtual void vfn_0005_36DA534E() = 0;
	virtual void vfn_0006_36DA534E() = 0;
};
