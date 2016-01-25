/*************************************************************
*
* Solution   : Mafia2-Online
* Project    : Client
* File       : CNameTag.h
* Developers : Tyldar <tyldar@m2-online.com>
*
***************************************************************/

#pragma once

class CNameTag
{
private:
	void			DrawPlayer		(void);
	void			DrawPed			(void);
public:
					CNameTag		();
					~CNameTag		();

	void			Draw			(void);
};
