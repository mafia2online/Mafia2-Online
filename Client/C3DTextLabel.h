/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : C3DTextLabel.h
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#pragma once

class C3DTextLabel
{
private:

	EntityId				m_serverID;
	float					m_fX;
	float					m_fY;
	float					m_fZ;
	String					m_text;
	int						m_color;
	float					m_distance;

	bool					m_active;

public:

							C3DTextLabel				(EntityId serverID, float fX, float fY, float fZ, String text, int color, float distance);
							~C3DTextLabel				(void);

	bool					IsActive					(void) { return m_active; }
	void					SetActive					(bool active) { m_active = active; }

	void					SetPosition					(CVector3 pos);
	CVector3				* GetPosition				(CVector3 * pos);

	void					SetText						(String text) { m_text = text; }
	String					GetText						(void) { return m_text; }

	int						GetColor					(void) { return m_color; }
	void					SetColor					(int color) { m_color = color; }

	void					SetDrawDistance				(float distance) { m_distance = distance; }
	float					GetDrawDistance				(void) { return m_distance; }
};