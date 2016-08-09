// Client.Launcher2.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef _DEBUG
#define CORE_MODULE	"m2mp_d.dll"
#else
#define	CORE_MODULE	"m2mp.dll"
#endif

CUpdate * pUpdater = NULL;

int main(array<System::String ^> ^args)
{
#ifndef _DEBUG
	// Is there a debugger present?
	if ( IsDebuggerPresent () )
	{
		// Exit
		ExitProcess ( -1 );
	}
#endif

	// Create the updater instance
	//pUpdater = new CUpdate ();
	
	// Check for updates
	//pUpdater->CheckForUpdates ();

	// Enabling Windows XP visual effects before any controls are created
	ClientLauncher2::Application::EnableVisualStyles();
	ClientLauncher2::Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	ClientLauncher2::Form1 ^ form = gcnew ClientLauncher2::Form1();
	form->label1->Text = L"LOL";

	ClientLauncher2::Application::Run(form);

	return 0;
}