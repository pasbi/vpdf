set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${GIT_DESCRIBE}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PDF viewer with measuring tools.")
set(CPACK_PACKAGE_CONTACT "https://github.com/pasbi/vpdf/")
set(CPACK_PACKAGE_VENDOR "The vpdf Team")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)

set(CPACK_SOURCE_IGNORE_FILES
   "/.gitignore"
   "${CMAKE_BINARY_DIR}/"
   "/.git/"
   "/.deps/"
   "/.build/"
   "/.clangd"
)

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

# Set package peoperties for Windows
if(WIN32)
  set(CPACK_GENERATOR "NSIS")
  set(CPACK_PACKAGE_EXECUTABLES "vpdf;${CMAKE_PROJECT_NAME}")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_PROJECT_NAME}")
  # SET(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/data/icons/${CMAKE_PROJECT_NAME}.ico")
  # SET(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/data/icons/${CMAKE_PROJECT_NAME}.ico")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_INSTALL_BINDIR}\\\\vpdf.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CMAKE_PROJECT_NAME}")
  set(CPACK_NSIS_HELP_LINK "https://github.com/pasbi/vpdf")
  set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/pasbi/vpdf")
  set(CPACK_NSIS_MODIFY_PATH OFF)
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

  # set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

  # register kbrowser in the Windows registry. this is needed for open-with.
  set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    WriteRegStr HKLM 'SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\App Paths\\\\vpdf.exe' '' '$INSTDIR\\\\bin\\\\vpdf.exe'
    WriteRegStr HKLM 'SOFTWARE\\\\Classes\\\\Applications\\\\vpdf.exe\\\\shell\\\\open\\\\command' '' '\\\"$INSTDIR\\\\bin\\\\vpdf.exe\\\" \\\"%L\\\"'
    WriteRegStr HKLM 'SOFTWARE\\\\Classes\\\\Directory\\\\shell\\\\${CMAKE_PROJECT_NAME}' '' 'Open with ${CMAKE_PROJECT_NAME}'
    WriteRegStr HKLM 'SOFTWARE\\\\Classes\\\\Directory\\\\shell\\\\${CMAKE_PROJECT_NAME}\\\\command' '' '\\\"$INSTDIR\\\\bin\\\\vpdf.exe\\\" \\\"%L\\\"'
   ")
  set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    DeleteRegKey HKLM 'SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\App Paths\\\\vpdf.exe'
    DeleteRegKey HKLM 'SOFTWARE\\\\Classes\\\\Applications\\\\vpdf.exe'
    DeleteRegKey HKLM 'SOFTWARE\\\\Classes\\\\Directory\\\\shell\\\\${CMAKE_PROJECT_NAME}'
  ")

  set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}
    WriteRegStr HKLM 'SOFTWARE\\\\Classes\\\\.pdf\\\\OpenWithList\\\\vpdf.exe' '' ''
  ")
  set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}
    DeleteRegKey HKLM 'SOFTWARE\\\\Classes\\\\.pdf\\\\OpenWithList\\\\vpdf.exe'
  ")
endif()

include(CPack)
