!include "MUI2.nsh"
!include "x64.nsh"

!define MUI_ABORTWARNING

!define MOD_NAME	"Mafia2-Online"
!define	MOD_VERS	"v0.1-rc1"
!define MOD_NAME_S	"M2Online"
!define MOD_OUTPUT	"m2online-01-rc1.exe"
!define REG_NODE	"SOFTWARE\Wow6432Node\${MOD_NAME}"
!define MOD_DIR 	"$PROGRAMFILES\${MOD_NAME}"

!define MUI_ICON 	"../Binary/icons/install.ico"
!define MUI_UNICON 	"../Binary/icons/install.ico"

Name "${MOD_NAME}"
Caption "${MOD_NAME} Installer"

OutFile "..\Binary\${MOD_OUTPUT}"
SetCompressor /SOLID lzma
BrandingText /TRIMCENTER "${MOD_NAME} Setup"
ShowInstDetails show
ShowUninstDetails show
RequestExecutionLevel admin

# Pages
!define MUI_DIRECTORYPAGE_TEXT_TOP "Please choose the location where you want to install ${MOD_NAME}."
!define MUI_PAGE_CUSTOMFUNCTION_PRE ChooseM2MPDirectory
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE OnChosenM2MPDirectory
!insertmacro MUI_PAGE_DIRECTORY

!define MUI_DIRECTORYPAGE_TEXT_TOP "Please location your Mafia II (PC) directory."
!define MUI_PAGE_CUSTOMFUNCTION_PRE FindMafia2Directory
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE ConfirmMafia2Directory
!insertmacro MUI_PAGE_DIRECTORY

!insertmacro MUI_PAGE_LICENSE "License.txt"

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Var vcredist2010set

Var installDir

!macro VerifyUserIsAdmin
	UserInfo::GetAccountType
	pop $0
	${If} $0 != "admin"
		MessageBox MB_ICONSTOP "Administrator rights required!"
		SetErrorLevel 740
		Quit
	${EndIf}
!macroend

Function .onInit
	SetShellVarContext all
	!insertmacro VerifyUserIsAdmin
FunctionEnd

# Choose the Mafia 2 Multiplayer install location
Function ChooseM2MPDirectory
	StrCpy $INSTDIR "$PROGRAMFILES\"
FunctionEnd

# After they choose the Mafia 2 Multiplayer directory
Function OnChosenM2MPDirectory
	StrCpy $installDir "$INSTDIR\${MOD_NAME}"
FunctionEnd

# Find the Mafia 2 install directory
Function FindMafia2Directory
	ReadRegStr $0 HKLM "SOFTWARE\Wow6432Node\2K Games\Mafia II" "Installfolder"
	${If} $0 == ""
		ReadRegStr $0 HKLM "SOFTWARE\Wow6432Node\Valve\Steam" "InstallPath"
		${If} $0 != ""
			StrCpy $INSTDIR "$0\steamapps\common\mafia ii"
		${EndIf}
	${Else}
		StrCpy $INSTDIR $0
	${EndIf}
FunctionEnd

# Confirm the selected Mafia 2 directory is valid
Function ConfirmMafia2Directory
	IfFileExists "$INSTDIR\pc\APEX_Clothing.dll" skip
		MessageBox MB_OK|MB_ICONSTOP "Invalid directory specified. Please select only the /mafia ii/ folder and not any sub-directories."
		Abort
	skip:
FunctionEnd

Section "Install"
	
	SetOverwrite on
	
	CreateDirectory "$installDir"
	CreateDirectory "$installDir\cache"
	CreateDirectory "$installDir\data"
	CreateDirectory "$installDir\data\browser"
	CreateDirectory "$installDir\data\game"
	CreateDirectory "$installDir\data\gui"
	CreateDirectory "$installDir\data\gui\fonts"
	CreateDirectory "$installDir\data\gui\images"
	CreateDirectory "$installDir\data\gui\skins"
	CreateDirectory "$installDir\logs"
	CreateDirectory "$installDir\screenshots"
	
	SetOutPath "$installDir"
	${If} ${RunningX64}
		File ..\Binary\dist\vcredist_x64.exe
	${Else}
		File ..\Binary\dist\vcredist_x86.exe
	${EndIf}
	Call vcredist2010installer
	
	File ..\Binary\release\m2online.exe
	File ..\Binary\release\m2online.dll
	File ..\Binary\bass.dll
	File ..\Binary\crashprt\crashrpt_lang.ini
	File ..\Binary\crashprt\CrashRpt1401.dll
	File ..\Binary\crashprt\CrashSender1401.exe
	File ..\Binary\crashprt\dbghelp.dll
	
	SetOutPath "$installDir\data\game"
	File ..\Binary\gamefiles\0.m2o
	File ..\Binary\gamefiles\1.m2o
	File ..\Binary\gamefiles\2.m2o
	File ..\Binary\gamefiles\3.m2o
	
	SetOutPath "$installDir\data\gui\fonts"
	File ..\Binary\guifiles\tahoma.ttf
	File ..\Binary\guifiles\tahoma-bold.ttf
	File ..\Binary\guifiles\verdana.ttf
	File ..\Binary\guifiles\verdana-bold.ttf
	File ..\Binary\guifiles\aurora-bold-condensed-bt.ttf
	
	SetOutPath "$installDir\data\gui\images"
	File ..\Binary\guifiles\1.jpg
	File ..\Binary\guifiles\2.jpg
	File ..\Binary\guifiles\3.jpg
	File ..\Binary\guifiles\4.jpg
	File ..\Binary\guifiles\loadingscreen.png
	File ..\Binary\guifiles\loadingscreen_logos.png
	File ..\Binary\guifiles\logo.png
	File ..\Binary\guifiles\logo_m2mp.png
	File ..\Binary\guifiles\logo_raknet.jpg
	File ..\Binary\guifiles\quick_connect.png
	File ..\Binary\guifiles\connect.png
	File ..\Binary\guifiles\disconnect.png
	File ..\Binary\guifiles\refresh.png
	File ..\Binary\guifiles\settings.png
	File ..\Binary\guifiles\quit.png
	File ..\Binary\guifiles\locked.png
	
	SetOutPath "$installDir\data\gui\skins"
	File ..\Binary\guifiles\default.png
	File ..\Binary\guifiles\default.xml
	File ..\Binary\guifiles\default.looknfeel.xml
	File ..\Binary\guifiles\default.imageset.xml
	
	SetOutPath "$INSTDIR\pc\sds\missionscript"
	File ..\Binary\gamefiles\freeraid_m2mp.sds
	
	SetOutPath "$INSTDIR\edit"
	File ..\Binary\gamefiles\sdsconfig_m2mp.bin
	
	SetOutPath "$INSTDIR\edit\tables"
	File ..\Binary\gamefiles\StreamM2MP.bin
	File ..\Binary\gamefiles\tables.sds

	SetOutPath "$installDir\data\sounds"
	File ..\Binary\sounds\menu.mp3
	
	# Write the uninstaller
	WriteUninstaller "$installDir\Uninstall.exe"
	
	# Create the desktop shortcut
	SetOutPath "$installDir"
	CreateShortCut "$DESKTOP\${MOD_NAME}.lnk" "$installDir\m2online.exe"
	
	# Create the start menu shortcuts
	CreateDirectory "$SMPROGRAMS\${MOD_NAME}"
	CreateShortCut "$SMPROGRAMS\${MOD_NAME}\${MOD_NAME}.lnk" "$installDir\m2online.exe"
	CreateShortCut "$SMPROGRAMS\${MOD_NAME}\Uninstall.lnk" "$installDir\Uninstall.exe"
	
	# Write the registry keys
	WriteRegStr HKLM "${REG_NODE}" "DisplayName" "${MOD_NAME}"
	WriteRegStr HKLM "${REG_NODE}" "InstallLocation" "$installDir"
	WriteRegStr HKLM "${REG_NODE}" "GameDir" "$INSTDIR"
	WriteRegStr HKLM "${REG_NODE}" "Version" "${MOD_VERS}"
	
	# Write the URI scheme
	WriteRegStr HKCR "m2online" "" "Mafia2-Online Protocol"
	WriteRegStr HKCR "m2online" "URL Protocol" ""
	WriteRegStr HKCR "m2online\DefaultIcon" "" "$\"$installDir\m2online.exe$\",1"
	WriteRegStr HKCR "m2online\shell\open\command" "" "$\"$installDir\m2online.exe$\" $\"-uri %1$\""
	
SectionEnd

Function un.onInit

	SetShellVarContext all

	# Verify they want to uninstall
	MessageBox MB_OKCANCEL "Are you sure you want to uninstall ${MOD_NAME}?" IDOK next
		Abort
		
	next:
		!insertmacro VerifyUserIsAdmin

FunctionEnd

Section "Uninstall"

	Delete "$installDir\m2online.exe"
	Delete "$installDir\m2online.dll"
	Delete "$INSTDIR\..\sds\missionscript\freeraid_m2o.sds"
	Delete "$INSTDIR\..\..\edit\tables\StreamM2O.bin"
	RMDir /r "$installDir"
	
	# Delete the desktop shortcut
	Delete "$DESKTOP\${MOD_NAME}.lnk"
	
	# Delete the start menu items
	RMDir /r "$SMPROGRAMS\${MOD_NAME}"
	
	# Delete the registry keys
	DeleteRegKey HKLM "${REG_NODE}"

SectionEnd

Function vcredist2010installer

	StrCmp $vcredist2010set "" 0 vcredist_done
	StrCpy $vcredist2010set "true"
	
	ReadRegStr $0 HKLM "SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}" "DisplayName"
	StrCmp $0 "Microsoft Visual C++ 2010  x86 Redistributable - 10.0.40219" vcredist_done vcredist_silent_install
	
	vcredist_silent_install:
		DetailPrint "Installing Microsoft Visual C++ 2010 Redistributable..."
		${If} ${RunningX64}
			ExecWait '"$installDir\vcredist_x64.exe" /q' $0
		${Else}
			ExecWait '"$installDir\vcredist_x86.exe" /q' $0
		${EndIf}
		
		ReadRegStr $0 HKLM "SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}" "DisplayName"
		StrCmp $0 "Microsoft Visual C++ 2010  x86 Redistributable - 10.0.40219" vcredist_success vcredist_not_present
		
		vcredist_not_present:
			DetailPrint "Failed to install Microsoft Visual C++ 2010 Redistributable."
			IfSilent vcredist_done vcredist_messagebox
			
      vcredist_messagebox:
      	${If} ${RunningX64}
			MessageBox MB_OK "Failed to install Microsoft Visual C++ 2010 Redistributable ($installDir\vcredist_x64.exe). Please ensure your system meets the minimum requirements before running the installer again."
		${Else}
			MessageBox MB_OK "Failed to install Microsoft Visual C++ 2010 Redistributable ($installDir\vcredist_x86.exe). Please ensure your system meets the minimum requirements before running the installer again."
		${EndIf}
        Goto vcredist_done
		
    vcredist_success:
    	${If} ${RunningX64}
			Delete "$installDir\vcredist_x64.exe"
		${Else}
			Delete "$installDir\vcredist_x86.exe"
		${EndIf}
      	DetailPrint "Microsoft Visual C++ 2010 Redistributable was successfully installed"
	  
	vcredist_done:
		${If} ${RunningX64}
			Delete "$installDir\vcredist_x64.exe"
		${Else}
			Delete "$installDir\vcredist_x86.exe"
		${EndIf}

FunctionEnd

Function savegamebackup
	# Verify they want to backup their savegames
	MessageBox MB_OKCANCEL "We hightly recommend that you backup your save game files. Do you want todo that now?" IDOK next
		Abort
		
next:
FunctionEnd