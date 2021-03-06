; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "FreeOrion"
!define PRODUCT_VERSION "0.4.3+"
!define PRODUCT_PUBLISHER "FreeOrion Community"
!define PRODUCT_WEB_SITE "http://www.freeorion.org"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${PRODUCT_NAME}"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor bzip2

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "..\default\COPYING"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\freeorion.exe"
;!define MUI_FINISHPAGE_RUN_PARAMETERS "--fullscreen 1"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\..\FreeOrion-5195-Test-Win32-Setup.exe"
InstallDir "$PROGRAMFILES\FreeOrion"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetShellVarContext all

  SetOutPath "$INSTDIR"
  SetOverwrite try
  File "..\..\vcredist_x86.exe"
  ExecWait "$INSTDIR\vcredist_x86.exe /q"
  Delete "$INSTDIR\vcredist_x86.exe"

  File "..\boost_chrono-vc100-mt-1_51.dll"
  File "..\boost_date_time-vc100-mt-1_51.dll"
  File "..\boost_filesystem-vc100-mt-1_51.dll"
  File "..\boost_python-vc100-mt-1_51.dll"
  File "..\boost_regex-vc100-mt-1_51.dll"
  File "..\boost_serialization-vc100-mt-1_51.dll"
  File "..\boost_signals-vc100-mt-1_51.dll"
  File "..\boost_system-vc100-mt-1_51.dll"
  File "..\boost_thread-vc100-mt-1_51.dll"
  File "..\FreeImage.dll"
  File "..\GiGi.dll"
  File "..\GiGiOgre.dll"
  File "..\GiGiOgrePlugin_OIS.dll"
  File "..\glew32.dll"
  File "..\jpeg.dll"
  File "..\libogg.dll"
  File "..\libpng14.dll"
  File "..\libvorbis.dll"
  File "..\libvorbisfile.dll"
  File "..\log4cpp.dll"
  File "..\OgreMain.dll"
  File "..\OIS.dll"
  File "..\OpenAL32.dll"
  File "..\Plugin_OctreeSceneManager.dll"
  File "..\Plugin_ParticleFX.dll"
  File "..\python27.dll"
  File "..\RenderSystem_GL.dll"
  File "..\wrap_oal.dll"
  File "..\zlib1.dll"
  File "..\Python27.zip"
  File "..\FreeorionCA.exe"
  File "..\FreeorionD.exe"
  File "..\Freeorion.exe"
  File "..\FreeOrion.ico"
  File "..\OISInput.cfg"
  File "..\ogre_plugins.cfg"
  File "..\changelog.txt"

  File /r /x .svn "..\default"

  CreateDirectory "$SMPROGRAMS\FreeOrion"
  CreateShortCut "$SMPROGRAMS\FreeOrion\FreeOrion Fullscreen.lnk" "$INSTDIR\freeorion.exe" "--fullscreen"
  CreateShortCut "$SMPROGRAMS\FreeOrion\FreeOrion Windowed.lnk" "$INSTDIR\freeorion.exe"

  CreateShortCut "$DESKTOP\FreeOrion.lnk" "$INSTDIR\freeorion.exe"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\FreeOrion.org.url" "InternetShortcut" "URL" "http://www.freeorion.org"
  CreateShortCut "$SMPROGRAMS\FreeOrion\FreeOrion.org.lnk" "$INSTDIR\FreeOrion.org.url"
  CreateShortCut "$SMPROGRAMS\FreeOrion\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\FreeOrion.ico"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  SetShellVarContext all

  Delete "$INSTDIR\boost_chrono-vc100-mt-1_51.dll"
  Delete "$INSTDIR\boost_date_time-vc100-mt-1_51.dll"
  Delete "$INSTDIR\boost_filesystem-vc100-mt-1_51.dll"
  Delete "$INSTDIR\boost_python-vc100-mt-1_51.dll"
  Delete "$INSTDIR\boost_regex-vc100-mt-1_51.dll"
  Delete "$INSTDIR\boost_serialization-vc100-mt-1_51.dll"
  Delete "$INSTDIR\boost_signals-vc100-mt-1_51.dll"
  Delete "$INSTDIR\boost_system-vc100-mt-1_51.dll"
  Delete "$INSTDIR\boost_thread-vc100-mt-1_51.dll"
  Delete "$INSTDIR\FreeImage.dll"
  Delete "$INSTDIR\GiGi.dll"
  Delete "$INSTDIR\GiGiOgre.dll"
  Delete "$INSTDIR\GiGiOgrePlugin_OIS.dll"
  Delete "$INSTDIR\glew32.dll"
  Delete "$INSTDIR\jpeg.dll"
  Delete "$INSTDIR\libogg.dll"
  Delete "$INSTDIR\libpng14.dll"
  Delete "$INSTDIR\libvorbis.dll"
  Delete "$INSTDIR\libvorbisfile.dll"
  Delete "$INSTDIR\log4cpp.dll"
  Delete "$INSTDIR\OgreMain.dll"
  Delete "$INSTDIR\OIS.dll"
  Delete "$INSTDIR\OpenAL32.dll"
  Delete "$INSTDIR\Plugin_OctreeSceneManager.dll"
  Delete "$INSTDIR\Plugin_ParticleFX.dll"
  Delete "$INSTDIR\python27.dll"
  Delete "$INSTDIR\RenderSystem_GL.dll"
  Delete "$INSTDIR\wrap_oal.dll"
  Delete "$INSTDIR\zlib1.dll"
  Delete "$INSTDIR\Python27.zip"
  Delete "$INSTDIR\FreeorionCA.exe"
  Delete "$INSTDIR\FreeorionD.exe"
  Delete "$INSTDIR\Freeorion.exe"
  Delete "$INSTDIR\FreeOrion.ico"
  Delete "$INSTDIR\OISInput.cfg"
  Delete "$INSTDIR\ogre_plugins.cfg"
  Delete "$INSTDIR\changelog.txt"

  RMDir /r "$INSTDIR\default"

  Delete "$INSTDIR\FreeOrion.org.url"

  Delete "$DESKTOP\FreeOrion.lnk"

  Delete "$SMPROGRAMS\FreeOrion\FreeOrion Fullscreen.lnk"
  Delete "$SMPROGRAMS\FreeOrion\FreeOrion Windowed.lnk"
  Delete "$SMPROGRAMS\FreeOrion\FreeOrion.org.lnk"
  Delete "$SMPROGRAMS\FreeOrion\Uninstall.lnk"

  RMDir "$SMPROGRAMS\FreeOrion"

  Delete "$INSTDIR\Uninstall.exe"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd