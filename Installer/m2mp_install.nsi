!include "MUI2.nsh"
!include "x64.nsh"

!define MUI_ABORTWARNING

!define MOD_NAME	"Mafia2-Online"
!define	MOD_VERS	"v0.1 RC4 "
!define MOD_NAME_S	"M2Online"
!define MOD_OUTPUT	"m2online-01-rc4.exe"
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

Var MODDIR
Var GAMEDIR

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
	ReadRegStr $MODDIR HKLM "${REG_NODE}" "InstallLocation"
	${If} $MODDIR == ""
		StrCpy $INSTDIR "$PROGRAMFILES\${MOD_NAME}"
	${Else}
		StrCpy $INSTDIR "$MODDIR"
	${EndIf}
FunctionEnd

# After they choose the Mafia 2 Multiplayer directory
Function OnChosenM2MPDirectory
	StrCpy $MODDIR "$INSTDIR"
FunctionEnd

# Find the Mafia 2 install directory
Function FindMafia2Directory
	ReadRegStr $GAMEDIR HKLM "${REG_NODE}" "GameDir"
	${If} $GAMEDIR != ""
		StrCpy $INSTDIR $GAMEDIR

		goto exit
	${EndIf}

	ReadRegStr $0 HKLM "SOFTWARE\Wow6432Node\2K Games\Mafia II" "Installfolder"
	${If} $0 == ""
		ReadRegStr $0 HKLM "SOFTWARE\Wow6432Node\Valve\Steam" "InstallPath"
		${If} $0 != ""
			StrCpy $INSTDIR "$0\steamapps\common\mafia ii"
		${EndIf}
	${Else}
		StrCpy $INSTDIR $0
	${EndIf}

	exit:
FunctionEnd

# Confirm the selected Mafia 2 directory is valid
Function ConfirmMafia2Directory
	IfFileExists "$INSTDIR\pc\APEX_Clothing.dll" skip
		MessageBox MB_OK|MB_ICONSTOP "Invalid directory specified. Please select only the /mafia ii/ folder and not any sub-directories."
		Abort
	skip:
	StrCpy $GAMEDIR $INSTDIR
FunctionEnd

Section "Install"
	SetOverwrite on

	CreateDirectory "$MODDIR"
	CreateDirectory "$MODDIR\cache"
	CreateDirectory "$MODDIR\data"
	CreateDirectory "$MODDIR\data\browser"
	CreateDirectory "$MODDIR\data\game"
	CreateDirectory "$MODDIR\data\gui"
	CreateDirectory "$MODDIR\data\gui\fonts"
	CreateDirectory "$MODDIR\data\gui\images"
	CreateDirectory "$MODDIR\data\gui\skins"
	CreateDirectory "$MODDIR\logs"
	CreateDirectory "$MODDIR\screenshots"

	SetOutPath "$MODDIR"
	File ..\Binary\release\m2online.exe
	File ..\Binary\release\m2online.dll
	File ..\Binary\bass.dll

	SetOutPath "$MODDIR\data\game"
	File ..\Binary\gamefiles\0.m2o
	File ..\Binary\gamefiles\1.m2o
	File ..\Binary\gamefiles\2.m2o
	File ..\Binary\gamefiles\3.m2o

	SetOutPath "$MODDIR\data\gui\fonts"
	File ..\Binary\guifiles\tahoma.ttf
	File ..\Binary\guifiles\tahoma-bold.ttf
	File ..\Binary\guifiles\verdana.ttf
	File ..\Binary\guifiles\verdana-bold.ttf
	File ..\Binary\guifiles\aurora-bold-condensed-bt.ttf

	SetOutPath "$MODDIR\data\gui\images"
	File ..\Binary\guifiles\1.png
	File ..\Binary\guifiles\2.png
	File ..\Binary\guifiles\3.png
	File ..\Binary\guifiles\4.png
	File ..\Binary\guifiles\5.png
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

	SetOutPath "$MODDIR\data\gui\skins"
	File ..\Binary\guifiles\default.png
	File ..\Binary\guifiles\default.xml
	File ..\Binary\guifiles\default.looknfeel.xml
	File ..\Binary\guifiles\default.imageset.xml

	SetOutPath "$GAMEDIR\pc\sds\missionscript"
	File ..\Binary\gamefiles\freeraid_m2mp.sds

	SetOutPath "$GAMEDIR\edit"
	File ..\Binary\gamefiles\sdsmpconf.bin

	SetOutPath "$GAMEDIR\edit\tables"
	File ..\Binary\gamefiles\StreamM2MP.bin
	File ..\Binary\gamefiles\tables.sds

	SetOutPath "$MODDIR\data\sounds"
	File ..\Binary\sounds\menu.mp3

	# Write the uninstaller
	WriteUninstaller "$MODDIR\Uninstall.exe"

	# Create the desktop shortcut
	SetOutPath "$MODDIR"
	CreateShortCut "$DESKTOP\${MOD_NAME}.lnk" "$MODDIR\m2online.exe"

	# Create the start menu shortcuts
	CreateDirectory "$SMPROGRAMS\${MOD_NAME}"
	CreateShortCut "$SMPROGRAMS\${MOD_NAME}\${MOD_NAME}.lnk" "$MODDIR\m2online.exe"
	CreateShortCut "$SMPROGRAMS\${MOD_NAME}\Uninstall.lnk" "$MODDIR\Uninstall.exe"

	# Write the registry keys
	WriteRegStr HKLM "${REG_NODE}" "DisplayName" "${MOD_NAME}"
	WriteRegStr HKLM "${REG_NODE}" "InstallLocation" "$MODDIR"
	WriteRegStr HKLM "${REG_NODE}" "GameDir" "$GAMEDIR"
	WriteRegStr HKLM "${REG_NODE}" "Version" "${MOD_VERS}"

	# Write the URI scheme
	WriteRegStr HKCR "m2online" "" "Mafia2-Online Protocol"
	WriteRegStr HKCR "m2online" "URL Protocol" ""
	WriteRegStr HKCR "m2online\DefaultIcon" "" "$\"$MODDIR\m2online.exe$\",1"
	WriteRegStr HKCR "m2online\shell\open\command" "" "$\"$MODDIR\m2online.exe$\" $\"-uri %1$\""

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

	ReadRegStr $GAMEDIR HKLM "${REG_NODE}" "GameDir"
	ReadRegStr $MODDIR HKLM "${REG_NODE}" "InstallLocation"

	Delete "$GAMEDIR\..\sds\missionscript\freeraid_m2o.sds"
	Delete "$GAMEDIR\..\..\edit\tables\StreamM2O.bin"
	RMDir /r "$MODDIR"

	# Delete the desktop shortcut
	Delete "$DESKTOP\${MOD_NAME}.lnk"

	# Delete the start menu items
	RMDir /r "$SMPROGRAMS\${MOD_NAME}"

	# Delete the registry keys
	DeleteRegKey HKLM "${REG_NODE}"

SectionEnd
