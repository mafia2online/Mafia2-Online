/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Server
* File       : C3DtextLabel.h
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#pragma once

//class C3DTextLabel : public CMultiplayerBlip - BUGGED
class C3DTextLabel
{

private:

	EntityId						m_textID;
	String							m_text;
	CVector3						m_position;
	int								m_color;
	float							m_distance;

public:

									C3DTextLabel				(EntityId textID, const char *text, CVector3 vecPosition, int color, int drawDistance);
									~C3DTextLabel				(void);
	void							AddForWorld					(void);
	void							AddForPlayer				(EntityId playerId);
	void							RemoveForPlayer				(EntityId playerId);
	void							RemoveForWorld				(void);

};