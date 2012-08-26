;NSIS Modern User Interface
;Repeater install script
;Written by Jonathan Naylor

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Configuration

  ;General
  Name "ircDDB Gateway 20120826"
  OutFile "ircDDBGateway-20120826.exe"

  ;Folder selection page
  InstallDir "$PROGRAMFILES\ircDDBGateway"
 
   ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
 
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\COPYING.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Repeater Program Files" SecProgram

  SetOutPath "$INSTDIR"
  
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Release\ircDDBGateway.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Release\RemoteControl.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Release\StarNetServer.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Release\TimerControl.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Release\TimeServer.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Release\XReflector.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Release\wxbase28u_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Release\wxmsw28u_adv_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Release\wxmsw28u_core_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\CHANGES.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\COPYING.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\TODO.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\DCS_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\DExtra_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\DPlus_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\TIME_de_DE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\TIME_de_DE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\TIME_en_GB.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\TIME_en_GB.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\TIME_en_US.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\TIME_en_US.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\TIME_se_SE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\TIME_se_SE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\de_DE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\de_DE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\dk_DK.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\dk_DK.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\en_GB.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\en_GB.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\en_US.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\en_US.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\es_ES.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\es_ES.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\fr_FR.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\fr_FR.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\it_IT.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\it_IT.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\no_NO.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\no_NO.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\pl_PL.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\pl_PL.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\se_SE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2010\Projects\opendv\ircDDBGateway\Data\se_SE.indx"

  ;Create start menu entry
  CreateDirectory "$SMPROGRAMS\ircDDBGateway"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\ircDDB Gateway.lnk"   "$INSTDIR\ircDDBGateway.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Remote Control.lnk"   "$INSTDIR\RemoteControl.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\StarNet Server.lnk"   "$INSTDIR\StarNetServer.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Timer Control.lnk"    "$INSTDIR\TimerControl.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Time Server.lnk"      "$INSTDIR\TimeServer.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\XReflector.lnk"       "$INSTDIR\XReflector.exe"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Changes.lnk"          "$INSTDIR\CHANGES.txt"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Licence.lnk"          "$INSTDIR\COPYING.txt"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\ToDo.lnk"             "$INSTDIR\TODO.txt"
  CreateShortCut "$SMPROGRAMS\ircDDBGateway\Uninstall.lnk"        "$INSTDIR\Uninstall.exe"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\*.*"
  RMDir  "$INSTDIR"

  Delete "$SMPROGRAMS\ircDDBGateway\*.*"
  RMDir  "$SMPROGRAMS\ircDDBGateway"

  DeleteRegKey /ifempty HKCU "Software\G4KLX\IRCDDB Gateway"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Remote Control"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\StarNet Server"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Timer Control"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Time Server"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\XReflector"

SectionEnd
