#include "StdInc.h"

#include "ProjectGlobals.h"
#include "SharedUtility.h"

#include "CCore.h"

CCore::CCore() : CSingleton<CCore>(),
	m_bFoundCustomDir(false)
{
	memset(&m_siStartupInfo, 0, sizeof(m_siStartupInfo));
	memset(&m_piProcessInfo, 0, sizeof(m_piProcessInfo));
	m_siStartupInfo.cb = sizeof(m_siStartupInfo);
}

bool	CCore::init()
{
#ifndef _DEBUG
	if (IsDebuggerPresent())
		return (false);
#endif

	/* If process already running */
	if (SharedUtility::IsProcessRunning(const_cast<char *>(ProjectGlobals::getGameName().c_str())))
		return (false);

	/* If the gameDir of Mafia2 isn't set correctly */
	if (!SharedUtility::ReadRegistryString(HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia2-Online", "GameDir", NULL, m_sInstallDirectory, sizeof(m_sInstallDirectory)))
	{
		if (MessageBox(NULL, "Failed to find Mafia2 install directory. Do you want to select it now ?", ProjectGlobals::getModName().c_str(), (MB_ICONEXCLAMATION | MB_YESNO)) == IDYES)
		{
			BROWSEINFO browseInfo = { 0 };
			browseInfo.lpszTitle = "Select your Mafia II directory";
			ITEMIDLIST * pItemIdList = SHBrowseForFolder(&browseInfo);

			if (pItemIdList != NULL)
			{
				if (SHGetPathFromIDList(pItemIdList, m_sInstallDirectory))
					m_bFoundCustomDir = true;

				IMalloc * pIMalloc = NULL;
				if (SUCCEEDED(SHGetMalloc(&pIMalloc)))
				{
					pIMalloc->Free(pItemIdList);
					pIMalloc->Release();
				}
			}
			if (!m_bFoundCustomDir)
				return (false);
		}
	}

	/* Preparing paths */
	m_sLaunchPath << m_sInstallDirectory << "\\pc";
	m_sApplicationPath << m_sLaunchPath.str() << "\\Mafia2.exe";

	/* Testing things */
	if (!SharedUtility::Exists(m_sApplicationPath.str().c_str()))
		return (false);

	if (m_bFoundCustomDir)
		SharedUtility::WriteRegistryString(HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\Mafia2-Online", "GameDir", m_sInstallDirectory, sizeof(m_sInstallDirectory));

	m_sModulePath << SharedUtility::GetAppPath() << "\\" << ProjectGlobals::getLibraryName();
	if (!SharedUtility::Exists(m_sModulePath.str().c_str()))
		return (false);

	m_sBassPath << SharedUtility::GetAppPath() << "\\" << ProjectGlobals::getBassName();
	if (!SharedUtility::Exists(m_sBassPath.str().c_str()))
		return (false);

	return (true);
}

bool	CCore::inject()
{	
	/* Start Mafia2.exe process */
	if (!CreateProcess(m_sApplicationPath.str().c_str(), NULL, NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, SharedUtility::GetAppPath(), &m_siStartupInfo, &m_piProcessInfo))
		return (false);

	/* Inject m2online.dll */
	m_injectRetn = SharedUtility::InjectLibraryIntoProcess(m_piProcessInfo.hProcess, m_sModulePath.str().c_str());
	if (m_injectRetn > 0)
		return (false);

	/* Inject bass.dll */
	m_injectRetn += SharedUtility::InjectLibraryIntoProcess(m_piProcessInfo.hProcess, m_sBassPath.str().c_str());
	if (m_injectRetn > 0)
		return (false);

	return (true);
}

void	CCore::run()
{
	if (this->init() == false)
	{
		MessageBox(NULL, "Cannot init the launcher", ProjectGlobals::getModName().c_str(), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	if (this->inject() == false)
	{
		MessageBox(NULL, "Cannot start and inject process", ProjectGlobals::getModName().c_str(), MB_ICONEXCLAMATION | MB_OK);
		TerminateProcess(m_piProcessInfo.hProcess, 0);
		if (SharedUtility::IsProcessRunning(const_cast<char *>(ProjectGlobals::getGameName().c_str()))){
			SharedUtility::_TerminateProcess(const_cast<char *>(ProjectGlobals::getGameName().c_str()));
		}
		return;
	}

	ResumeThread(m_piProcessInfo.hThread);
}