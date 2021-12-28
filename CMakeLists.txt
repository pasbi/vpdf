cmake_minimum_required(VERSION 3.5)

project(vpdf VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Widgets LinguistTools Pdf PdfWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Pdf PdfWidgets REQUIRED)

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

target_link_libraries(vpdf PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                   Qt${QT_VERSION_MAJOR}::Pdf
                                   Qt${QT_VERSION_MAJOR}::PdfWidgets)

set_target_properties(vpdf PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(vpdf)
endif()
