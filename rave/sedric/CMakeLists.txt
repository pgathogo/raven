cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPILER "C:/mingw64/bin/x86_64-w64-mingw32-g++.exe")

project(sedric LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(rave_framework_INCLUDE_DIRS "../framework")
set(rave_security_INCLUDE_DIRS "../framework")
set(rave_audio_INCLUDE_DIRS "../audio")

#set(rave_audiolib_INCLUDE_DIRS "../audiolib")

set(postgreSQL_ROOT "C:/Program Files/PostgreSQL/14")
find_package(PostgreSQL REQUIRED)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

include_directories(${AUDIO_INCLUDE_DIR})
set(RAVE_AUDIO_LIBRARIES ${AUDIO_LIBRARY})


if(ANDROID)
  add_library(sedric SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    scheduleitem.h
    scheduleitem.cpp
  )
else()
  add_executable(sedric
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    scheduleitem.h
    scheduleitem.cpp
    sedricscheduleitem.h
    sedricscheduleitem.cpp
    saveas.h
    saveas.cpp
    sedric.qrc
    ../framework/scheduledatamanager.h
    ../framework/scheduledatamanager.cpp
    ../framework/entitydatamodel.cpp
    ../framework/databaseconnector.cpp
    ../framework/databasemanager.cpp
    ../framework/ravenexception.cpp
    ../framework/dataprovider.cpp
    ../framework/baseentity.cpp
    ../framework/entityfields.cpp
    ../framework/manytomany.cpp
    ../framework/typeexclusion.cpp
    ../framework/queryset.h
    ../framework/valuelist.cpp
    ../framework/treeviewmodel.h
    ../framework/treeviewmodel.cpp
    ../framework/tree.h
    ../framework/tree.cpp
    ../framework/schedule.cpp
    ../framework/relationmapper.h
    ../framework/relationmapper.cpp
    ../framework/datetimeselector.h
    ../framework/datetimeselector.cpp
    ../security/authentication.cpp
    ../security/user.cpp

    ../audio/audiolibrary.cpp
    ../audio/audiofolder.cpp
    ../audio/audio.cpp
    ../audio/artist.cpp
    ../audio/audiofile.cpp
    ../audio/audiotrackitem.cpp
    ../audio/audiotool.cpp
    ../audio/audiolibitem.cpp
    ../audio/audiohistoryform.h
    ../audio/audiohistoryform.cpp

    ../audio/editor/audiographic.cpp
    ../audio/editor/audioplayer.cpp
    ../audio/editor/audiothread.cpp
    ../audio/editor/audiowaveform.cpp
    ../audio/editor/subjectobserver.cpp
    ../audio/editor/vumeter.cpp
    ../audio/editor/audiographic.h
    ../audio/editor/audioplayer.h
    ../audio/editor/audiothread.h
    ../audio/editor/audiowaveform.h
    ../audio/editor/markindicator.h
    ../audio/editor/subjectobserver.h
    ../audio/editor/vumeter.h

    ../traffik/spotaudio.h
    ../traffik/spotaudio.cpp
    ../traffik/spot.cpp
    ../traffik/client.cpp
    ../traffik/clientgroup.cpp
    ../traffik/agent.cpp
    ../traffik/spottypeexclusion.cpp
    ../traffik/spotvoiceover.cpp
    ../traffik/voiceover.cpp
    ../traffik/brand.cpp
    ../traffik/salesperson.cpp
)
endif()

target_include_directories(sedric PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(sedric PRIVATE ${rave_framework_INCLUDE_DIRS})
#target_include_directories(sedric PRIVATE ${rave_audiolib_INCLUDE_DIRS})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})

link_directories(${rave_audio_INCLUDE_DIRS})
#link_directories(${rave_audiolib_INCLUDE_DIRS})

target_link_libraries(sedric PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(sedric PRIVATE ${PostgreSQL_LIBRARIES})

target_link_libraries(sedric PRIVATE ${RAVE_AUDIO_LIBRARIES})

#set(AUDIO_INCLUDE_DIR "../audiolib/headers")
#set(AUDIO_LIBRARY_DIR "../audiolib/lib_debug")
#find_library(AUDIO_LIBRARY cueeditord ${AUDIO_LIBRARY_DIR})

#if (AUDIO_INCLUDE_DIR)
#        message(STATUS "AUDIO_INCLUDE_DIR found")
#endif()

#if (AUDIO_LIBRARY)
#        message(STATUS "CueEditor library found.")
#endif()

#target_link_directories(sedric PRIVATE ${AUDIO_LIBRARY_DIR})
#target_link_libraries(sedric PRIVATE ${AUDIO_LIBRARY})

set(BASS_INCLUDE_DIR "../lib/BassLib/Include")
set(BASS_LIBRARY_DIR "../lib/BassLib/Lib")
find_library(BASS_LIBRARY bass ${BASS_LIBRARY_DIR})
if (BASS_LIBRARY)
        message(STATUS "Bass library found.")
endif()

target_link_directories(sedric PRIVATE ${BASS_LIBRARY_DIR})
target_link_libraries(sedric PRIVATE ${BASS_LIBRARY})

#set_target_properties(sedric PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(sedric)
endif()
