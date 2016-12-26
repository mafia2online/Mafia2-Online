#pragma once

class CUpdater
{
private:
	String			m_modulePath;
public:
	CUpdater(String);
	~CUpdater();

	bool			IsNewVersionAvailable();

	String			GetPathForNewVersion();
	bool			StartUpdating();
};