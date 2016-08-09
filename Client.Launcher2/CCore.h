#pragma once

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "CSingleton.h"

class CCore : public CSingleton<CCore>
{
	friend class CSingleton<CCore>;

public:
	void		run();
	bool		init();
	bool		inject();

protected:
	CCore();
	~CCore() = default;

private:
	char						m_sInstallDirectory[MAX_PATH];
	std::ostringstream 			m_sLaunchPath;
	std::ostringstream 			m_sApplicationPath;

	std::ostringstream			m_sModulePath;
	std::ostringstream			m_sBassPath;

	bool						m_bFoundCustomDir;

	int							m_injectRetn;

	STARTUPINFO					m_siStartupInfo;
	PROCESS_INFORMATION			m_piProcessInfo;
};