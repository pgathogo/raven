cmake_minimum_required(VERSION 3.5)

project(Tester VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ../audio/audiofile.h
        ../audio/audiofile.cpp
        ../audio/audiotool.h
        ../audio/audiotool.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Tester
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Tester SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Tester
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Tester PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


set(AUDIO_INCLUDE_DIR "../audiolib/headers")
set(AUDIO_LIBRARY_DIR "../audiolib/lib_debug")
find_library(AUDIO_LIBRARY cueeditor ${AUDIO_LIBRARY_DIR})

if (AUDIO_LIBRARY)
        message(STATUS "CueEditor library is found.")
endif()

target_link_directories(Tester PRIVATE ${AUDIO_LIBRARY_DIR})
target_link_libraries(Tester PRIVATE ${AUDIO_LIBRARY})

set_target_properties(Tester PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Tester)
endif()
