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

	float					m_fX;
	float					m_fY;
	float					m_fZ;
	String					m_text;
	int						m_color;
	float					m_distance;

	bool					m_active;

public:

							C3DTextLabel				(float fX, float fY, float fZ, String text, int color, float distance);
							~C3DTextLabel				(void);

	bool					IsActive					(void) { return m_active; }
	void					SetActive					(bool active) { m_active = active; }

	CVector3				* GetPosition				(CVector3 * pos);
	String					GetText						(void) { return m_text; }
	float					GetDrawDistance				(void) { return m_distance; }
};