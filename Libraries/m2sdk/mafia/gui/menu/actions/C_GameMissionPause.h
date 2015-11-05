// auto-generated file (rttidump-exporter by h0pk1nz)

#pragma once

#include <mafia/gui/menu/actions/C_Action.h>
#include <ue/game/cutscenes/I_CutsceneNotifyHandler.h>

namespace mafia
{

namespace gui
{

namespace menu
{

namespace actions
{

/** mafia::gui::menu::actions::C_GameMissionPause (VTable=0x01E71A90) */
class C_GameMissionPause : public C_Action
{
public:
	/** mafia::gui::menu::actions::C_GameMissionPause::C_GameMenuNotifyHandler (VTable=0x01E71AB0) */
	class C_GameMenuNotifyHandler : public ue::game::cutscenes::I_CutsceneNotifyHandler
	{
	public:
		virtual void vfn_0001_4A4CDB3A() = 0;
		virtual void vfn_0002_4A4CDB3A() = 0;
		virtual void vfn_0003_4A4CDB3A() = 0;
	};

	virtual void vfn_0001_AB765439() = 0;
	virtual void vfn_0002_AB765439() = 0;
	virtual void vfn_0003_AB765439() = 0;
};

} // namespace actions

} // namespace menu

} // namespace gui

} // namespace mafia
