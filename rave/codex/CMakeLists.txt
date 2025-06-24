cmake_minimum_required(VERSION 3.15)

string(REGEX REPLACE "/W[3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(MSVS_BUILD_NUM "14.39.33519")

set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/${MSVS_BUILD_NUM}/bin/Hostx64/x64/cl.exe")

project(sedric LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(codex VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        codexrc.qrc
        ../Network/clientsocket.cpp
        ../Network/clientsocket.h
        ../Network/request_response_handler.cpp
        ../Network/request_response_handler.h
        ../Network/requestresponsemanager.cpp
        ../Network/requestresponsemanager.h
        ../Network/serversocket.cpp
        ../Network/serversocket.h
        ../Network/message.h
        ../Network/message.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(codex
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET codex APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(codex SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(codex
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if (MSVC)
    target_compile_options(codex PRIVATE "/W0")
    target_compile_options(codex PUBLIC "/wd4716")
    target_compile_options(codex PUBLIC "/Zc:__cplusplus")
    target_compile_options(codex PUBLIC "/EHsc")
 else()
     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type")
 endif()

target_link_libraries(codex PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(codex PRIVATE Qt${QT_VERSION_MAJOR}::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.codex)
endif()
set_target_properties(codex PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if (WIN32)
    target_link_libraries(codex PRIVATE Kernel32 User32)
endif()

include(GNUInstallDirs)
install(TARGETS codex
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(codex)
endif()
