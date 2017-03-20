!include "StrFunc.nsh"
!include "LogicLib.nsh"
!include "FileFunc.nsh"
!include "x64.nsh"
!include "MUI2.nsh"

#Unicode true
RequestExecutionLevel admin
SetCompress off

!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "English"

#!define APP_NAME "XShow"

!define APP_EXE	 "${APP_NAME}.exe"
!define KSHOW_EXE "kshow.exe"
!define APP_EXE_PATH	"..\release\${APP_EXE}"
!getdllversion ${APP_EXE_PATH} version_
!define APP_VERSION	${version_1}.${version_2}.${version_3}.${version_4}

!define APP_7Z 	   		"app.7z"
!define SKIN_7Z       	"skin.zip"

!define UNINSTALLER_EXE "uninstall.exe"

LangString InstallerTitle ${LANG_ENGLISH} "${APP_NAME} Installer"
LangString InstallerTitle ${LANG_SIMPCHINESE} "${APP_NAME} 安装程序"

LangString UnInstallerTitle ${LANG_ENGLISH} "Uninstall ${APP_NAME}"
LangString UnInstallerTitle ${LANG_SIMPCHINESE} "卸载 ${APP_NAME}"

LangString lblDirText ${LANG_ENGLISH} "Destination:"
LangString lblDirText ${LANG_SIMPCHINESE} "安装路径："

LangString InstDirInvalid ${LANG_ENGLISH} "Invalid install directory"
LangString InstDirInvalid ${LANG_SIMPCHINESE} "安装路径无效"

LangString lblSpaceNeededText ${LANG_ENGLISH} "Space Needed: 30M"
LangString lblSpaceNeededText ${LANG_SIMPCHINESE} "所需空间：30M"

LangString lblSpaceLeftText ${LANG_ENGLISH} "Space Left: "
LangString lblSpaceLeftText ${LANG_SIMPCHINESE} "剩余空间："

LangString msgSpaceLimit ${LANG_ENGLISH} "No enough disk space, please select another disk."
LangString msgSpaceLimit ${LANG_SIMPCHINESE} "目标磁盘空间不足，请使用其他的磁盘安装！"

LangString chkLicenseText ${LANG_ENGLISH} 		"I Accept the"
LangString chkLicenseText ${LANG_SIMPCHINESE} 	"我已阅读并认可"

LangString lnkLicenseText ${LANG_ENGLISH} "End-User License Agreement"
LangString lnkLicenseText ${LANG_SIMPCHINESE} "用户许可协议"

LangString btnLicenseAgreeText ${LANG_ENGLISH} "Accept"
LangString btnLicenseAgreeText ${LANG_SIMPCHINESE} "同意"

LangString btnInstallText ${LANG_ENGLISH} "Install"
LangString btnInstallText ${LANG_SIMPCHINESE} "安装"

LangString lblProgressTipText ${LANG_ENGLISH} "Installing"
LangString lblProgressTipText ${LANG_SIMPCHINESE} "正在安装"

LangString InfoTitle ${LANG_ENGLISH} "Information"
LangString InfoTitle ${LANG_SIMPCHINESE} "提示"

LangString RunninText ${LANG_ENGLISH} "${APP_NAME} is running, please exit and try again."
LangString RunninText ${LANG_SIMPCHINESE} "${APP_NAME} 正在运行，请退出后重试！"

LangString AbortText ${LANG_ENGLISH} "Setup is not completed yet, are you sure you want to quit? "
LangString AbortText ${LANG_SIMPCHINESE} "安装尚未完成，您确定要退出么？"

LangString btnRunText ${LANG_ENGLISH} "Start"
LangString btnRunText ${LANG_SIMPCHINESE} "开始使用"

LangString lblUnPromptText ${LANG_ENGLISH} "Are you sure you want to uninstall ${APP_NAME} ?"
LangString lblUnPromptText ${LANG_SIMPCHINESE} "确定要卸载 ${APP_NAME}？"

LangString chkReserveDataText ${LANG_ENGLISH} "Reserve user data"
LangString chkReserveDataText ${LANG_SIMPCHINESE} "保留我的数据"

LangString btnUnConfirmText ${LANG_ENGLISH} "Confirm"
LangString btnUnConfirmText ${LANG_SIMPCHINESE} "确定"

LangString btnUnCancelText ${LANG_ENGLISH} "Cancel"
LangString btnUnCancelText ${LANG_SIMPCHINESE} "取消"

LangString lblUnProgressTipText ${LANG_ENGLISH} "Uninstalling"
LangString lblUnProgressTipText ${LANG_SIMPCHINESE} "正在卸载"

LangString lblUnEndText ${LANG_ENGLISH} "Uninstall completed."
LangString lblUnEndText ${LANG_SIMPCHINESE} "卸载成功"

LangString btnUnEndText ${LANG_ENGLISH} "Finish"
LangString btnUnEndText ${LANG_SIMPCHINESE} "完成"

Name ${APP_NAME}

OutFile "${OUT_DIR}\${APP_NAME}_${APP_VERSION}_setup.exe"

InstallDir "$APPDATA\${APP_NAME}\"

!define ICON_NAME "${APP_NAME}.ico"
!define ICON_PATH "..\release\image\icon\${ICON_NAME}"
Icon              ${ICON_PATH}
UninstallIcon     ${ICON_PATH}

!define BANNER "file='banner_${APP_NAME}.png'"

VIAddVersionKey "ProductName"       ${APP_NAME}
VIAddVersionKey "InternalName"      ${APP_NAME}
VIAddVersionKey "ProductVersion"    ${APP_VERSION}
VIAddVersionKey "FileVersion"       ${APP_VERSION}
VIAddVersionKey "FileDescription"   ${APP_NAME}
VIAddVersionKey "Comments"    		${APP_NAME}
VIAddVersionKey "LegalCopyright"    "Copyright © 2016"
VIProductVersion             		${APP_VERSION}
VIFileVersion						${APP_VERSION}

!define PAGE_TAB			"pageTab"
!define PAGE_CONFIG 		0
!define PAGE_PROGRESS 		1
!define PAGE_END 			2
!define UN_PAGE_CONFIG 		3
!define UN_PAGE_PROGRESS 	4
!define UN_PAGE_END 		5

Section "dummy"
SectionEnd

Page custom DuiPage

UninstPage custom un.DuiPage

Var hDlg
Var reserveData
Var complete
Var unProgress

!macro rmFixed
	RMDIR /r "$INSTDIR\bin"
	RMDIR /r "$INSTDIR\doc"
	RMDIR /r "$INSTDIR\image"
	RMDIR /r "$INSTDIR\language"
	RMDIR /r "$INSTDIR\module"
	RMDIR /r "$INSTDIR\theme"
	DELETE "$INSTDIR\*"
!macroend

Function DuiPage
    StrCpy $complete "0"

	InitPluginsDir   	
	SetOutPath "$PLUGINSDIR"	
	File "..\release\doc\license\*"
    File ${SKIN_7Z}
	
	nsNiuniuSkin::InitSkinPage "$PLUGINSDIR\" "$LANGUAGE.txt"
    Pop $hDlg

    nsNiuniuSkin::SetDirValue "$INSTDIR\"
	Call OnInstDir
	
	nsNiuniuSkin::SetWindowTile $(InstallerTitle)
	nsNiuniuSkin::ShowPageItem ${PAGE_TAB} ${PAGE_CONFIG}
	nsNiuniuSkin::SetControlAttribute "lblDir" "text" $(lblDirText)
	nsNiuniuSkin::SetControlAttribute "lblSpaceNeed" "text" $(lblSpaceNeededText)
	nsNiuniuSkin::SetControlAttribute "chkLicense" "text" $(chkLicenseText)
	nsNiuniuSkin::SetControlAttribute "lnkLicense" "text" $(lnkLicenseText)
	nsNiuniuSkin::SetControlAttribute "btnInstall" "text" $(btnInstallText)
	nsNiuniuSkin::SetControlAttribute "lblLicenseTitle" "text" "${APP_NAME} $(lnkLicenseText)"
	nsNiuniuSkin::SetControlAttribute "bannerConfig" "bkimage" ${BANNER}
	nsNiuniuSkin::SetControlAttribute "bannerProgress" "bkimage" ${BANNER}
	nsNiuniuSkin::SetControlAttribute "bannerEnd" "bkimage" ${BANNER}
	
    Call BindCallBack
    nsNiuniuSkin::ShowPage
FunctionEnd

Function un.DuiPage
	StrCpy $complete "0"

    InitPluginsDir
	SetOutPath "$PLUGINSDIR"
    File "${SKIN_7Z}"

	nsNiuniuSkin::InitSkinPage "$PLUGINSDIR\" "" 
    Pop $hDlg

	nsNiuniuSkin::ShowPageItem ${PAGE_TAB} ${UN_PAGE_CONFIG}
	nsNiuniuSkin::SetWindowTile $(UnInstallerTitle)
	nsNiuniuSkin::SetWindowSize $hDlg 508 418
	Call un.BindCallBack
	nsNiuniuSkin::SetControlAttribute "lblUnPrompt" "text" "$(lblUnPromptText)"
	nsNiuniuSkin::SetControlAttribute "chkReserveData" "text" "$(chkReserveDataText)"
	nsNiuniuSkin::SetControlAttribute "btnUnConfirm" "text" "$(btnUnConfirmText)"
	nsNiuniuSkin::SetControlAttribute "btnUnCancel" "text" "$(btnUnCancelText)"
	nsNiuniuSkin::SetControlAttribute "bannerConfigUn" "bkimage" ${BANNER}
	nsNiuniuSkin::SetControlAttribute "bannerProgressUn" "bkimage" ${BANNER}
	nsNiuniuSkin::SetControlAttribute "bannerEndUn" "bkimage" ${BANNER}
	
    nsNiuniuSkin::ShowPage	
FunctionEnd

Function un.BindCallBack	
	GetFunctionAddress $0 un.ExitDuiSetup
    nsNiuniuSkin::BindCallBack "btnClose" $0

	GetFunctionAddress $0 un.onUninstall
    nsNiuniuSkin::BindCallBack "btnUnConfirm" $0
	
	GetFunctionAddress $0 un.ExitDuiSetup
    nsNiuniuSkin::BindCallBack "btnUnCancel" $0

	GetFunctionAddress $0 un.ExitDuiSetup
    nsNiuniuSkin::BindCallBack "btnUnEnd" $0	
FunctionEnd

Function BindCallBack
    GetFunctionAddress $0 OnExit
    nsNiuniuSkin::BindCallBack "btnLicenseClose" $0
    
    GetFunctionAddress $0 OnBtnMin
    nsNiuniuSkin::BindCallBack "btnLicenseMin" $0
    
	
	GetFunctionAddress $0 OnLicenseLink
    nsNiuniuSkin::BindCallBack "lnkLicense" $0
	
    GetFunctionAddress $0 OnExit
    nsNiuniuSkin::BindCallBack "btnDirClose" $0
	
	GetFunctionAddress $0 OnExit
    nsNiuniuSkin::BindCallBack "btnLicenseCancel" $0
    
    GetFunctionAddress $0 OnBtnMin
    nsNiuniuSkin::BindCallBack "btnDirMin" $0
    
    GetFunctionAddress $0 OnBtnSelectDir
    nsNiuniuSkin::BindCallBack "btnSelectDir" $0
    
    GetFunctionAddress $0 OnBtnDirPre
    nsNiuniuSkin::BindCallBack "btnDirPre" $0
    
	GetFunctionAddress $0 OnLicenseAgree
    nsNiuniuSkin::BindCallBack "btnLicenseAgree" $0
	
    GetFunctionAddress $0 OnBtnCancel
    nsNiuniuSkin::BindCallBack "btnDirCancel" $0
        
    GetFunctionAddress $0 OnBtnInstall
    nsNiuniuSkin::BindCallBack "btnInstall" $0
    
    GetFunctionAddress $0 OnExit
    nsNiuniuSkin::BindCallBack "btnDetailClose" $0
    
    GetFunctionAddress $0 OnBtnMin
    nsNiuniuSkin::BindCallBack "btnDetailMin" $0

    GetFunctionAddress $0 OnFinished
    nsNiuniuSkin::BindCallBack "btnRun" $0
    
    GetFunctionAddress $0 OnBtnMin
    nsNiuniuSkin::BindCallBack "btnFinishedMin" $0
    
    GetFunctionAddress $0 OnExit
    nsNiuniuSkin::BindCallBack "btnClose" $0
	
	GetFunctionAddress $0 OnLicenseCheck
    nsNiuniuSkin::BindCallBack "chkLicense" $0
	
	GetFunctionAddress $0 OnBtnShowMore
    nsNiuniuSkin::BindCallBack "btnShowMore" $0
	
	GetFunctionAddress $0 OnBtnHideMore
    nsNiuniuSkin::BindCallBack "btnHideMore" $0
	
	GetFunctionAddress $0 OnSysCommandCloseEvent
    nsNiuniuSkin::BindCallBack "syscommandclose" $0
	
	GetFunctionAddress $0 OnInstDir
    nsNiuniuSkin::BindCallBack "editDir" $0
FunctionEnd

Function OnInstDir
	nsNiuniuSkin::GetDirValue
    Pop $0	
	StrCpy $INSTDIR "$0"
	
	Call IsInstDirValid
	${If} $R5 == "0"
		nsNiuniuSkin::SetControlAttribute "lblSpaceLeft" "text" $(InstDirInvalid)
		nsNiuniuSkin::SetControlAttribute "lblSpaceLeft" "textcolor" "#ffff0000"
		nsNiuniuSkin::SetControlAttribute "btnInstall" "enabled" "false"
		goto TextChangeAbort
    ${EndIf}
	
	nsNiuniuSkin::SetControlAttribute "lblSpaceLeft" "textcolor" "#FF999999"
	${If} $R0 > 1024  
	    IntOp $R0  $R0 / 1024;
		IntOp $R1  $R0 % 1024		
		nsNiuniuSkin::SetControlAttribute "lblSpaceLeft" "text" "$(lblSpaceLeftText)$R0.$R1GB"
	${Else}
		nsNiuniuSkin::SetControlAttribute "lblSpaceLeft" "text" "$(lblSpaceLeftText)$R0.$R1MB"
     ${endif}
	
	nsNiuniuSkin::GetCheckboxStatus "chkLicense"
    Pop $0
	${If} $0 == "1"        
		nsNiuniuSkin::SetControlAttribute "btnInstall" "enabled" "true"
	${Else}
		nsNiuniuSkin::SetControlAttribute "btnInstall" "enabled" "false"
    ${EndIf}	
TextChangeAbort:
FunctionEnd

Function HddDetection
	${If} "$R0" == "$9"
		StrCpy $R1 "HDD"
		goto funend
	${Endif}
		Push $0
	funend:
FunctionEnd


Function IsInstDirValid
	${GetRoot} "$INSTDIR" $R3

	StrCpy $R0 "$R3\"  
	StrCpy $R1 "invalid"  
	${GetDrives} "HDD" "HddDetection"

	${If} $R1 == "HDD"    
		StrCpy $R5 "1"	 
		${DriveSpace} "$R3\" "/D=F /S=M" $R0 
		${If} $R0 < 100 
			StrCpy $R5 "-1"
		${endif}
	${Else}  
		StrCpy $R5 "0"
	${endif}
FunctionEnd

Function OnLicenseCheck
	nsNiuniuSkin::GetCheckboxStatus "chkLicense"
    Pop $0
	${If} $0 == "0"
		nsNiuniuSkin::SetControlAttribute "btnInstall" "enabled" "true"
	${Else}
		nsNiuniuSkin::SetControlAttribute "btnInstall" "enabled" "false"
    ${EndIf}
FunctionEnd

Function OnLicenseLink
	nsNiuniuSkin::SetControlAttribute "licenseshow" "visible" "true"
	nsNiuniuSkin::IsControlVisible "moreconfiginfo"
	Pop $0
	${If} $0 = 0        
		nsNiuniuSkin::SetControlAttribute "licenseshow" "pos" "5,35,475,385"
		nsNiuniuSkin::SetControlAttribute "editLicense" "height" "270"
	${Else}
		nsNiuniuSkin::SetControlAttribute "licenseshow" "pos" "5,35,475,495"
		nsNiuniuSkin::SetControlAttribute "editLicense" "height" "375"
    ${EndIf}
	
FunctionEnd

Function CreateShortcut
  SetShellVarContext all
  CreateDirectory "$SMPROGRAMS\${APP_NAME}"
  CreateShortCut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\${APP_EXE}" "" "$INSTDIR\image\icon\${APP_NAME}.ico" 0
  CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${APP_EXE}" "" "$INSTDIR\image\icon\${APP_NAME}.ico" 0
  CreateShortCut "$SMPROGRAMS\${APP_NAME}\$(UnInstallerTitle).lnk" "$INSTDIR\${UNINSTALLER_EXE}"  
  
  SetShellVarContext current
FunctionEnd

Function CreateUninstall	
	WriteUninstaller "$INSTDIR\${UNINSTALLER_EXE}"
	
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayName" "${APP_NAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "UninstallString" "$INSTDIR\${UNINSTALLER_EXE}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayIcon" "$INSTDIR\${UNINSTALLER_EXE}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "DisplayVersion" "${APP_VERSION}"
FunctionEnd

Function OnBtnInstall
    nsNiuniuSkin::GetCheckboxStatus "chkLicense"
    Pop $0
	StrCpy $0 "1"
		
	StrCmp $0 "0" InstallAbort 0
	
	nsProcess::_FindProcess "${KSHOW_EXE}"
	Pop $R0
	
	${If} $R0 == 0
        nsNiuniuSkin::ShowMsgBox $(InfoTitle) $(RunninText)  0
		goto InstallAbort
    ${EndIf}

	nsNiuniuSkin::GetDirValue
    Pop $0
    StrCmp $0 "" InstallAbort 0
    StrCpy $INSTDIR "$0\"
	
	Call IsInstDirValid
	${If} $R5 == "0"
		nsNiuniuSkin::ShowMsgBox $(InfoTitle) $(InstDirInvalid) 0
		goto InstallAbort
    ${EndIf}	
	${If} $R5 == "-1"
		nsNiuniuSkin::ShowMsgBox $(InfoTitle) $(msgSpaceLimit) 0
		goto InstallAbort
    ${EndIf}
	
	SetRegView 32
	#WriteRegStr HKLM "Software\${APP_NAME}" "InstallPath" "$INSTDIR"
	nsNiuniuSkin::SetWindowSize $hDlg 508 418
	nsNiuniuSkin::SetControlAttribute "btnClose" "enabled" "false"
	nsNiuniuSkin::ShowPageItem ${PAGE_TAB} ${PAGE_PROGRESS}
    nsNiuniuSkin::SetSliderRange "slrProgress" 0 100
	nsNiuniuSkin::SetControlAttribute "lblProgressTip" "text" "$(lblProgressTipText)"
	
    GetFunctionAddress $0 ExtractApp
    BgWorker::CallAndWait
    
	Call CreateShortcut
	Call CreateUninstall    
			
	nsNiuniuSkin::SetControlAttribute "btnClose" "enabled" "true"		
	StrCpy $complete "1"
	#Call OnFinished
	nsNiuniuSkin::ShowPageItem ${PAGE_TAB} ${PAGE_END}
	nsNiuniuSkin::SetControlAttribute "btnRun" "text" "$(btnRunText)"
InstallAbort:
FunctionEnd


Function ExtractApp
	SetOutPath $INSTDIR
	!insertmacro rmFixed
    File "${APP_7Z}"
    GetFunctionAddress $R9 ExtractAppCallBack
    nsis7z::ExtractWithCallback "$INSTDIR\${APP_7Z}" $R9
	Delete "$INSTDIR\${APP_7Z}"	
	ExecWait "$INSTDIR\bin\vc2015u3x86.exe /install /quiet /norestart"
FunctionEnd

Function ExtractAppCallBack
    Pop $1
    Pop $2
    System::Int64Op $1 * 100
    Pop $3
    System::Int64Op $3 / $2
    Pop $0
	
    nsNiuniuSkin::SetSliderValue "slrProgress" $0
	nsNiuniuSkin::SetControlAttribute "lblProgressPos" "text" "$0%"
	
    ${If} $1 == $2
        nsNiuniuSkin::SetSliderValue "slrProgress" 100    
		nsNiuniuSkin::SetControlAttribute "lblProgressPos" "text" "100%"
    ${EndIf}
FunctionEnd

Function OnSysCommandCloseEvent
	Call OnExit
FunctionEnd

Function OnExit
	${If} $complete == "0"		
		nsNiuniuSkin::ShowMsgBox $(InfoTitle) $(AbortText)  1
		pop $0
		${If} $0 == 0
			goto endfun
		${EndIf}
	${EndIf}
	nsNiuniuSkin::ExitDuiSetup
endfun:    
FunctionEnd

Function OnBtnMin
    SendMessage $hDlg ${WM_SYSCOMMAND} 0xF020 0
FunctionEnd

Function OnBtnCancel
	nsNiuniuSkin::ExitDuiSetup
FunctionEnd

Function OnFinished			    
    Exec "$INSTDIR\${APP_EXE}"
    Call OnExit
FunctionEnd

Function OnBtnSelectDir
    nsNiuniuSkin::SelectInstallDir
    Pop $0
FunctionEnd

Function StepHeightSizeAsc
${ForEach} $R0 418 528 + 10
  nsNiuniuSkin::SetWindowSize $hDlg 508 $R0
  Sleep 5
${Next}
FunctionEnd

Function StepHeightSizeDsc
${ForEach} $R0 528 418 - 10
  nsNiuniuSkin::SetWindowSize $hDlg 508 $R0
  Sleep 5
${Next}
FunctionEnd

Function OnBtnShowMore	
	nsNiuniuSkin::SetControlAttribute "btnShowMore" "enabled" "false"
	nsNiuniuSkin::SetControlAttribute "btnHideMore" "enabled" "false"
	nsNiuniuSkin::SetControlAttribute "moreconfiginfo" "visible" "true"
	nsNiuniuSkin::SetControlAttribute "btnHideMore" "visible" "true"
	nsNiuniuSkin::SetControlAttribute "btnShowMore" "visible" "false"
	
	GetFunctionAddress $0 StepHeightSizeAsc
    BgWorker::CallAndWait
	
	nsNiuniuSkin::SetWindowSize $hDlg 508 528
	nsNiuniuSkin::SetControlAttribute "btnShowMore" "enabled" "true"
	nsNiuniuSkin::SetControlAttribute "btnHideMore" "enabled" "true"
FunctionEnd

Function OnBtnHideMore
	nsNiuniuSkin::SetControlAttribute "btnShowMore" "enabled" "false"
	nsNiuniuSkin::SetControlAttribute "btnHideMore" "enabled" "false"
	nsNiuniuSkin::SetControlAttribute "moreconfiginfo" "visible" "false"
	nsNiuniuSkin::SetControlAttribute "btnHideMore" "visible" "false"
	nsNiuniuSkin::SetControlAttribute "btnShowMore" "visible" "true"
	
	 GetFunctionAddress $0 StepHeightSizeDsc
    BgWorker::CallAndWait

	nsNiuniuSkin::SetWindowSize $hDlg 508 418
	nsNiuniuSkin::SetControlAttribute "btnShowMore" "enabled" "true"
	nsNiuniuSkin::SetControlAttribute "btnHideMore" "enabled" "true"
FunctionEnd


Function OnLicenseAgree
	nsNiuniuSkin::SetControlAttribute "licenseshow" "visible" "false"
FunctionEnd

Function OnBtnDirPre
    nsNiuniuSkin::PrePage ${PAGE_TAB}
FunctionEnd


Function .onInit	
FunctionEnd

Function .onInstSuccess
FunctionEnd

Function .onInstFailed
FunctionEnd

Function .onVerifyInstDir
FunctionEnd

Function un.onInit
FunctionEnd

Function un.onUninstSuccess
FunctionEnd

Function un.ExitDuiSetup
	nsNiuniuSkin::ExitDuiSetup
FunctionEnd

Function un.onUninstall
	nsNiuniuSkin::GetCheckboxStatus "chkReserveData"
    Pop $0
	StrCpy $reserveData $0
		
	nsProcess::_FindProcess "${KSHOW_EXE}"
	Pop $R0
	
	${If} $R0 == 0
		nsNiuniuSkin::ShowMsgBox $(InfoTitle) $(RunninText) 0
		goto InstallAbort
    ${EndIf}

	nsNiuniuSkin::SetControlAttribute "btnClose" "enabled" "false"
	nsNiuniuSkin::ShowPageItem ${PAGE_TAB} ${UN_PAGE_PROGRESS}
	nsNiuniuSkin::SetControlAttribute "lblUnProgressTip" "text" "$(lblUnProgressTipText)"
	nsNiuniuSkin::SetSliderRange "slrProgress" 0 100
	IntOp $unProgress 0 + 1

	SetRegView 32
	#DeleteRegKey HKLM "Software\${APP_NAME}"	
	
	SetShellVarContext all

	Delete "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk"
	Delete "$DESKTOP\${APP_NAME}.lnk"
	Delete "$SMPROGRAMS\${APP_NAME}\$(UnInstallerTitle).lnk"
	RMDir "$SMPROGRAMS\${APP_NAME}"
	
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
	
	SetShellVarContext current
	
	IntOp $unProgress $unProgress + 8
    
	GetFunctionAddress $0 un.RemoveFiles
    BgWorker::CallAndWait
	InstallAbort:
FunctionEnd

Function un.RemoveFiles
	${If} $reserveData == "1"
		!insertmacro rmFixed
	${Else}
		RMDIR /r "$INSTDIR"
	${EndIf}
	StrCpy $complete "1"
	nsNiuniuSkin::SetControlAttribute "btnClose" "enabled" "true"
	nsNiuniuSkin::SetSliderValue "slrProgress" 100
	nsNiuniuSkin::ShowPageItem ${PAGE_TAB} ${UN_PAGE_END}
	nsNiuniuSkin::SetControlAttribute "lblUnEnd" "text" "$(lblUnEndText)"
	nsNiuniuSkin::SetControlAttribute "btnUnEnd" "text" "$(btnUnEndText)"
FunctionEnd
