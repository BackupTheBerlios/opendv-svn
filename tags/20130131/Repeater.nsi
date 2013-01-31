;NSIS Modern User Interface
;Repeater install script
;Written by Jonathan Naylor

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;Configuration

  ;General
  Name "Repeater 20130131"
  OutFile "Repeater-20130131.exe"

  ;Folder selection page
  InstallDir "$PROGRAMFILES\Repeater"
 
   ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
 
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\COPYING.txt"
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

  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\AnalogueRepeater.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\DCSGateway.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\DExtraGateway.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\DummyRepeater.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\DVAPNode.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\DVRPTRRepeater.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\GMSKRepeater.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\ParrotController.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\SoundCardRepeater.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\SplitRepeater.exe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\portaudio_x86.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\wxbase28u_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\wxmsw28u_adv_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Release\wxmsw28u_core_vc_custom.dll"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\WindowsUSB\xDVRPTR-32-64-2.inf"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\WindowsUSB\dvrptr_cdc.inf"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\WindowsUSB\gmsk.inf"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\WindowsUSB\gmsk.cat"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\DCS_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\DExtra_Hosts.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\de_DE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\de_DE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\dk_DK.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\dk_DK.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\en_GB.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\en_GB.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\en_US.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\en_US.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\es_ES.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\es_ES.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\fr_FR.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\fr_FR.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\it_IT.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\it_IT.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\no_NO.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\no_NO.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\pl_PL.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\pl_PL.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\se_SE.ambe"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\Data\se_SE.indx"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\COPYING.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\CHANGES.txt"
  File "C:\Users\Jonathan\Documents\Visual Studio 2008\Projects\opendv\trunk\Repeater\TODO.txt"

  ;Create start menu entry
  CreateDirectory "$SMPROGRAMS\Repeater"
  CreateShortCut "$SMPROGRAMS\Repeater\Analogue Repeater.lnk"        "$INSTDIR\AnalogueRepeater.exe"
  CreateShortCut "$SMPROGRAMS\Repeater\DCS Gateway.lnk"              "$INSTDIR\DCSGateway.exe"
  CreateShortCut "$SMPROGRAMS\Repeater\DExtra Gateway.lnk"           "$INSTDIR\DExtraGateway.exe"
  CreateShortCut "$SMPROGRAMS\Repeater\Dummy Repeater.lnk"           "$INSTDIR\DummyRepeater.exe"
  CreateShortCut "$SMPROGRAMS\Repeater\DVAP Node.lnk"                "$INSTDIR\DVAPNode.exe"
  CreateShortCut "$SMPROGRAMS\Repeater\DV-RPTR Repeater.lnk"         "$INSTDIR\DVRPTRRepeater.exe"
  CreateShortCut "$SMPROGRAMS\Repeater\GMSK Repeater.lnk"            "$INSTDIR\GMSKRepeater.exe"
  CreateShortCut "$SMPROGRAMS\Repeater\Parrot Controller.lnk"        "$INSTDIR\ParrotController.exe"
  CreateShortCut "$SMPROGRAMS\Repeater\Sound Card Repeater.lnk"      "$INSTDIR\SoundCardRepeater.exe"
  CreateShortCut "$SMPROGRAMS\Repeater\Split Repeater.lnk"           "$INSTDIR\SplitRepeater.exe"
  CreateShortCut "$SMPROGRAMS\Repeater\Licence.lnk"                  "$INSTDIR\COPYING.txt"
  CreateShortCut "$SMPROGRAMS\Repeater\Changes.lnk"                  "$INSTDIR\CHANGES.txt"
  CreateShortCut "$SMPROGRAMS\Repeater\ToDo.lnk"                     "$INSTDIR\TODO.txt"
  CreateShortCut "$SMPROGRAMS\Repeater\Uninstall.lnk"                "$INSTDIR\Uninstall.exe"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\*.*"
  RMDir  "$INSTDIR"

  Delete "$SMPROGRAMS\Repeater\*.*"
  RMDir  "$SMPROGRAMS\Repeater"

  DeleteRegKey /ifempty HKCU "Software\G4KLX\Analogue Repeater"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\DCS Gateway"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\DExtra Gateway"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Dummy Repeater"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\DVAP Node"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\DV-RPTR Repeater"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\GMSK Repeater"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Parrot Controller"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Sound Card Repeater"
  DeleteRegKey /ifempty HKCU "Software\G4KLX\Split Repeater"

SectionEnd
