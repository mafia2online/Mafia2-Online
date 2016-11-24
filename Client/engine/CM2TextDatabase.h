#pragma once

class M2TextDatabase
{
public:

	void Clear();
	void Done();
	void GetString(char **str, int textId);
	void Init();
	void LoadIconsMap(const char *map);
	void ResetHistory();
};

class CM2TextDatabase
{
private:

	M2TextDatabase		*m_pTextDB;

public:

	CM2TextDatabase();
	CM2TextDatabase(M2TextDatabase *pTextDB);
	~CM2TextDatabase();

	void			SetTextDatabase(M2TextDatabase *pTextDB) { m_pTextDB = pTextDB; }
	M2TextDatabase	*GetTextDatabase() { return m_pTextDB; }


	void			Clear();
	void			Done();
	void			GetString(char **str, int textId);
	void			Init();
	void			LoadIconsMap(const char *map);
	void			ResetHistory();
};