#pragma once

#include <string>

class ProjectGlobals
{
public:
	ProjectGlobals() = delete;
	~ProjectGlobals() = delete;

	static std::string getModName();
	static std::string getLauncherName();
	static std::string getLibraryName();
	static std::string getBassName();
	static std::string getGameName();
};