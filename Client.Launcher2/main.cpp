/*************************************************************
*
* Solution   : Mafia 2 Online
* Project    : Launcher
* File       : main.cpp
* Developers : Tyldar <tyldar@mafia2-online.com>
*
***************************************************************/

#include "StdInc.h"

#include "CCore.h"

#ifdef _DEBUG
int main(int ac, char **av)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	CCore::getInstance().run();
	return (0);
}