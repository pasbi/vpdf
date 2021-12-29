cmake_minimum_required(VERSION 3.5)

project(vpdf VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})
include(GetGitRevisionDescription)

find_package(QT NAMES Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES vpdf_de_DE.ts)

set(PROJECT_SOURCES
        calibration.cpp
        calibration.h
        calibrationdialog.cpp
        calibrationdialog.h
        calibrationdialog.ui
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        measurerect.cpp
        measurerect.h
        pdfview.cpp
        pdfview.h
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(vpdf MANUAL_FINALIZATION ${PROJECT_SOURCES})
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(vpdf ${PROJECT_SOURCES})
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

get_target_property(qt_qmake_location Qt5::qmake IMPORTED_LOCATION)
execute_process(COMMAND ${qt_qmake_location} -query QT_INSTALL_TRANSLATIONS
                OUTPUT_VARIABLE qt_qm_path
                OUTPUT_STRIP_TRAILING_WHITESPACE)

target_link_libraries(vpdf PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                   poppler-qt5)

set_target_properties(vpdf PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(vpdf)
endif()

install(TARGETS vpdf RUNTIME DESTINATION bin)
include(Packaging)

if (WIN32)
  set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
  include(InstallRequiredSystemLibraries)
  execute_process(COMMAND "${qt_qmake_location}" -query QT_INSTALL_PREFIX
      OUTPUT_VARIABLE qt_install_prefix
      OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  execute_process(COMMAND "${qt_qmake_location}" -query QT_INSTALL_PLUGINS
      OUTPUT_VARIABLE qt_install_plugins
      OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  get_filename_component(MinGW_bin_path ${CMAKE_CXX_COMPILER} PATH)

  # These two dependencies are strange:
  # I don't know why this project depdends on them.
  # On my PC, fixup_bundle finds them and installs and packages them correctly.
  # On github's CI, however, these files are not installed/packaged, which results in a corrupted
  # artifact (the installed application will not be executable).
  install(FILES "${MinGW_bin_path}/libssl-1_1-x64.dll" "${MinGW_bin_path}/libcrypto-1_1-x64.dll"
          TYPE BIN)

  install(CODE "
    include(BundleUtilities)
    execute_process(COMMAND
        \"${qt_install_prefix}/bin/windeployqt.exe\"
        \"\${CMAKE_INSTALL_PREFIX}/bin/vpdf.exe\"
    )
    fixup_bundle(
      \"\${CMAKE_INSTALL_PREFIX}/bin/vpdf.exe\"
      \"\"
      \"\"
    )"
    COMPONENT Runtime
  )
endif()

configure_file(${CMAKE_SOURCE_DIR}/cmake/make-appimage.sh.in ${CMAKE_BINARY_DIR}/make-appimage.sh @ONLY)
add_custom_target(appimage
                  COMMAND "${CMAKE_BINARY_DIR}/make-appimage.sh"
                  COMMENT "Packing AppImage"
)
