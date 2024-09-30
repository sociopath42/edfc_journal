cmake_minimum_required(VERSION 3.5)

project(edcf_journalSync VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network )

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        journalsequencer.h
        journalsequencer.cpp
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_definitions(__DEBUG)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(edcf_journalSync
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
        add_executable(edcf_journalSync
            ${PROJECT_SOURCES}
        )
endif()

target_link_libraries(edcf_journalSync PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

set_target_properties(edcf_journalSync PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    WIN32_EXECUTABLE TRUE
)

install(TARGETS edcf_journalSync
    BUNDLE DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/install
    LIBRARY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/install
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(edcf_journalSync)
endif()
