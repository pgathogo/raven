cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")

project(OATS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_PREFIX_PATH "C:/Qt/6.2.4/mingw_64/lib/cmake")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(rave_framework_INCLUDE_DIRS "../framework")
set(rave_security_INCLUDE_DIRS "../framework")
set(rave_audiolib_INCLUDE_DIRS "../audio")

set(postgreSQL_ROOT "C:/Program Files/PostgreSQL/9.5")
find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

set(AUDIO_INCLUDE "../audiolib/headers")
set(AUDIO_LIB "../audiolib/lib_debug")

find_path(AUDIO_INCLUDE_DIR cueeditor.h ${AUDIO_INCLUDE})
find_library(AUDIO_LIBRARY cueeditor ${AUDIO_LIB})

#timeanalyzerwidget.h
#timeanalyzerwidget.cpp

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        datetimewidget.h
        datetimewidget.cpp
        playlistcontrolwidget.h
        playlistcontrolwidget.cpp
        scheduleitem.h
        scheduleitem.cpp
        gridpanel.h
        gridpanel.cpp
        subjectobserver.h
        subjectobserver.cpp
        schedulegriditem.h
        schedulegriditem.cpp
        timegridpanel.h
        timegridpanel.cpp
        trackgridpanel.h
        trackgridpanel.cpp
        statusgridpanel.h
        statusgridpanel.cpp
        transitionmodegridpanel.h
        transitionmodegridpanel.cpp
        outputpanel.h
        outputpanel.cpp
        playmodepanel.h
        playmodepanel.cpp
        commercialviewer.h
        commercialviewer.cpp
        trackinfo.h
        trackinfo.cpp
        jinglegrid.h
        jinglegrid.cpp
        cartpanel.h
        cartpanel.cpp
        oats.qrc

        ../framework/baseentity.cpp
        ../framework/entityfields.cpp
        ../framework/entitydatamodel.cpp
        ../framework/relationmapper.cpp
        ../framework/dataprovider.cpp
        ../framework/ravenexception.cpp
        ../framework/databaseconnector.cpp
        ../framework/databasemanager.cpp
        ../framework/ravenexception.h
        ../framework/ravenexception.cpp
        ../framework/tree.h
        ../framework/tree.cpp
        ../framework/treeviewmodel.h
        ../framework/treeviewmodel.cpp
        ../framework/letterfilterwidget.h
        ../framework/letterfilterwidget.cpp
        ../framework/schedule.h
        ../framework/schedule.cpp

        ../audio/audio.cpp
        ../audio/artist.cpp
        ../audio/audiofile.cpp
        ../audio/audiolibrary.h
        ../audio/audiolibrary.cpp
        ../audio/audiofolder.h
        ../audio/audiofolder.cpp
        ../audio/audiotrackviewer.h
        ../audio/audiotrackviewer.cpp
        ../audio/audiotrackitem.h
        ../audio/audiotrackitem.cpp
        ../audio/audiolibitem.h
        ../audio/audiolibitem.cpp
        ../audio/audiotool.h
        ../audio/audiotool.cpp
        ../audio/audiohistoryform.h
        ../audio/audiohistoryform.cpp
        ../audio/trackbrowser.h
        ../audio/trackbrowser.cpp
        ../audio/trackpickerdialog.h
        ../audio/trackpickerdialog.cpp
        ../audio/editor/audioplayer.h
        ../audio/editor/audioplayer.cpp
        ../audio/editor/audiothread.h
        ../audio/editor/audiothread.cpp
        ../audio/audiocache.h
        ../audio/audiocache.cpp
        ../audio/audiocachemanager.h
        ../audio/audiocachequeuethread.h
        ../audio/cacheupdaterthread.h

        ../security/authentication.cpp
        ../security/user.cpp

        ../utils/tools.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(OATS
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(OATS SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(OATS
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(OATS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
#target_link_libraries(OATS PRIVATE Qt${QT_VERSION_MAJOR}::Charts)

target_include_directories(OATS PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(OATS PRIVATE ${rave_framework_INCLUDE_DIRS})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})
link_directories(${rave_audiolib_INCLUDE_DIRS})

target_link_libraries(OATS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(OATS PRIVATE ${PostgreSQL_LIBRARIES})
target_link_libraries(OATS PRIVATE ${RAVE_AUDIO_LIBRARIES})

set(BASS_INCLUDE_DIR "../lib/BassLib/Include")
set(BASS_LIBRARY_DIR "../lib/BassLib/Lib")
find_library(BASS_LIBRARY bass ${BASS_LIBRARY_DIR})
if (BASS_LIBRARY)
        message(STATUS "Bass library found.")
endif()

target_link_directories(OATS PRIVATE ${BASS_LIBRARY_DIR})
target_link_libraries(OATS PRIVATE ${BASS_LIBRARY})

set(sqlite3_INCLUDE_DIR "../lib/sqlite3/include")
set(sqlite3_LIBRARY_DIR "../lib/sqlite3/lib")

find_library(SQLite3_LIBRARY sqlite3 ${sqlite3_LIBARRY_DIR})
if (SQLite3_LIBRARY)
    message(STATUS "SQLite3 library found!")
endif()

include_directories(${sqlite3_INCLUDE_DIR})
target_link_directories(OATS PRIVATE ${sqlite3_LIBRARY_DIR})
target_link_libraries(OATS PRIVATE ${SQLite3_LIBRARY})


set_target_properties(OATS PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(OATS)
endif()
