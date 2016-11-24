
#include "BaseInc.h"

#include "../COffsets.h"

#include "CM2TextDatabase.h"

void _declspec(naked) M2TextDatabase::Clear()
{
	_asm jmp COffsets::FUNC_CTextDatabase__Clear;
}

void _declspec(naked) M2TextDatabase::Done()
{
	_asm jmp COffsets::FUNC_CTextDatabase__Done;
}

void _declspec(naked) M2TextDatabase::GetString(char **str, int textId)
{
	_asm jmp COffsets::FUNC_CTextDatabase__GetString;
}

void _declspec(naked) M2TextDatabase::Init()
{
	_asm jmp COffsets::FUNC_CTextDatabase__Init;
}

void _declspec(naked) M2TextDatabase::LoadIconsMap(const char *map)
{
	_asm jmp COffsets::FUNC_CTextDatabase__LoadIconsMap;
}

void _declspec(naked) M2TextDatabase::ResetHistory()
{
	_asm jmp COffsets::FUNC_CTextDatabase__ResetHistory;
}

CM2TextDatabase::CM2TextDatabase() :
	m_pTextDB(nullptr)
{
}

CM2TextDatabase::CM2TextDatabase(M2TextDatabase *pTextDB)
{
	SetTextDatabase(pTextDB);
}

CM2TextDatabase::~CM2TextDatabase()
{
}

void CM2TextDatabase::Clear()
{
	if (!m_pTextDB)
		return;

	m_pTextDB->Clear();
}

void CM2TextDatabase::Done()
{
	if (!m_pTextDB)
		return;

	m_pTextDB->Done();
}

void CM2TextDatabase::GetString(char **str, int textid)
{
	if (!m_pTextDB)
		return;

	m_pTextDB->GetString(str, textid);
}

void CM2TextDatabase::Init()
{
	if (!m_pTextDB)
		return;

	m_pTextDB->Init();
}

void CM2TextDatabase::LoadIconsMap(const char *map)
{
	if (!m_pTextDB)
		return;

	m_pTextDB->LoadIconsMap(map);
}

void CM2TextDatabase::ResetHistory()
{
	if (!m_pTextDB)
		return;

	m_pTextDB->ResetHistory();
}