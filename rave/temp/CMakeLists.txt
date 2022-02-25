cmake_minimum_required(VERSION 3.5)

project(OATS VERSION 0.1 LANGUAGES CXX)

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

set(rave_framework_INCLUDE_DIRS "../framework")
set(rave_security_INCLUDE_DIRS "../framework")
set(rave_audiolib_INCLUDE_DIRS "../audio")

set(postgreSQL_ROOT "C:/Program Files/PostgreSQL/9.5")
find_package(PostgreSQL REQUIRED)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Charts REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

set(AUDIO_INCLUDE "../audiolib/headers")
set(AUDIO_LIB "../audiolib/lib_debug")

find_path(AUDIO_INCLUDE_DIR cueeditor.h ${AUDIO_INCLUDE})
find_library(AUDIO_LIBRARY cueeditor ${AUDIO_LIB})

if (AUDIO_INCLUDE_DIR)
        message(STATUS "Audio include DIR found.")
endif()

if (AUDIO_LIBRARY)
        message(STATUS "Audio library found.")
endif()

include_directories(${AUDIO_INCLUDE_DIR})
set(RAVE_AUDIO_LIBRARIES ${AUDIO_LIBRARY})

#include_directories(${AUDIO_LIB})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        timeanalyzerwidget.h
        timeanalyzerwidget.cpp
        datetimewidget.h
        datetimewidget.cpp
)
add_executable(OATS
    ${PROJECT_SOURCES}
)

#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#    qt_add_executable(OATS
#        MANUAL_FINALIZATION
#        ${PROJECT_SOURCES}
#    )
#else()
#    if(ANDROID)
#        add_library(OATS SHARED
#            ${PROJECT_SOURCES}
#        )
#    else()
#        add_executable(OATS
#            ${PROJECT_SOURCES}
#        )
#    endif()
#endif()


target_include_directories(OATS PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(OATS PRIVATE ${rave_framework_INCLUDE_DIRS})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})
link_directories(${rave_audiolib_INCLUDE_DIRS})

#target_link_libraries(OATS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts)
target_link_libraries(OATS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(OATS PRIVATE ${PostgreSQL_LIBRARIES})
target_link_libraries(OATS PRIVATE ${RAVE_AUDIO_LIBRARIES})

set_target_properties(OATS PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(OATS)
endif()
