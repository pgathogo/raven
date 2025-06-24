cmake_minimum_required(VERSION 3.15)

string(REGEX REPLACE "/W[3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(MSVS_BUILD_NUM "14.42.34433")

set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/${MSVS_BUILD_NUM}/bin/Hostx64/x64/cl.exe")

project(sedric LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
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

set(CMAKE_EXE_LINKER_FLAGS "-static")

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
    bookedcommercialdlg.h
    bookedcommercialdlg.cpp

    ../data/commercialviewer.h
    ../data/commercialviewer.cpp

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

    ../framework/tree.h
    ../framework/tree.cpp
    ../framework/treeviewmodel.h
    ../framework/treeviewmodel.cpp

    ../framework/schedule.cpp
    ../framework/relationmapper.h
    ../framework/relationmapper.cpp
    ../framework/datetimeselector.h
    ../framework/datetimeselector.cpp
    ../framework/Logger.h
    ../framework/Logger.cpp

    ../security/authentication.cpp
    ../security/baserole.h
    ../security/baserole.cpp
    ../security/user.cpp
    ../security/loginform.h
    ../security/loginform.cpp
    ../security/selectstationform.h
    ../security/selectstationform.cpp
    ../security/userconfig.h
    ../security/userconfig.cpp
    ../security/structs.h

    ../utils/notificationbar.h
    ../utils/notificationbar.cpp
    ../utils/notificationitem.h
    ../utils/notificationitem.cpp

    ../audio/trackbrowser.h
    ../audio/trackbrowser.cpp
    ../audio/trackpickerdialog.h
    ../audio/trackpickerdialog.cpp
    ../audio/audiodatamodel.h
    ../audio/audiodatamodel.cpp
    ../audio/audiofolderwidget.h
    ../audio/audiofolderwidget.cpp
    ../audio/audiosearchwidget.h
    ../audio/audiosearchwidget.cpp
    ../audio/audiolibwidget.h
    ../audio/audiolibwidget.cpp
    ../audio/audiotrackwidgettoolbar.h
    ../audio/audiotrackwidgettoolbar.cpp
    ../audio/audiotrackwidget.h
    ../audio/audiotrackwidget.cpp
    ../audio/audiotrackviewer.h
    ../audio/audiotrackviewer.cpp
    ../audio/audiolibrary.h
    ../audio/audiolibrary.cpp
    ../audio/audiometer.h
    ../audio/audiometer.cpp

    ../framework/letterfilterwidget.h
    ../framework/letterfilterwidget.cpp

    ../audio/audiofolder.cpp
    ../audio/audio.cpp
    ../audio/artist.cpp
    ../audio/audiofile.cpp
    ../audio/audiotrackitem.cpp
    ../audio/audiotool.cpp
    ../audio/audiolibitem.cpp
    ../audio/audiohistoryform.h
    ../audio/audiohistoryform.cpp
    ../audio/audiowaveformgenerator.h
    ../audio/audiowaveformgenerator.cpp
    ../audio/mp3oggconverter.h
    ../audio/mp3oggconverter.cpp
    ../audio/audioconverter.h
    ../audio/audioconverter.cpp
    ../audio/mtstomp3converter.h
    ../audio/mtstomp3converter.cpp
    ../audio/oggtooggconverter.h
    ../audio/oggtooggconverter.cpp

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

    ../../ravecluster/rave/clusterman/clustercontroller.h
    ../../ravecluster/rave/clusterman/clustercontroller.cpp
    ../../ravecluster/rave/clusterman/server.h
    ../../ravecluster/rave/clusterman/server.cpp
    ../../ravecluster/rave/clusterman/station.h
    ../../ravecluster/rave/clusterman/station.cpp
    ../../ravecluster/rave/clusterman/cluster.h
    ../../ravecluster/rave/clusterman/cluster.cpp
    ../rave.qrc

)
endif()


if (MSVC)
    target_compile_options(sedric PRIVATE "/W0")
    target_compile_options(sedric PUBLIC "/wd4716")
    target_compile_options(sedric PUBLIC "/Zc:__cplusplus")
    target_compile_options(sedric PUBLIC "/EHsc")
 else()
     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type")
 endif()

target_compile_options(sedric PRIVATE /MT)
target_link_options(sedric PRIVATE /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)

target_include_directories(sedric PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(sedric PRIVATE ${rave_framework_INCLUDE_DIRS})
#target_include_directories(sedric PRIVATE ${rave_audiolib_INCLUDE_DIRS})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})

link_directories(${rave_audio_INCLUDE_DIRS})

target_link_libraries(sedric PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(sedric PRIVATE ${PostgreSQL_LIBRARIES})

target_link_libraries(sedric PRIVATE ${RAVE_AUDIO_LIBRARIES})


set(BASS_INCLUDE_DIR "../lib/BassLib/Include")
set(BASS_LIBRARY_DIR "../lib/BassLib/Lib")
find_library(BASS_LIBRARY bass ${BASS_LIBRARY_DIR})
if (BASS_LIBRARY)
        message(STATUS "Bass library found.")
endif()

target_link_directories(sedric PRIVATE ${BASS_LIBRARY_DIR})
target_link_libraries(sedric PRIVATE ${BASS_LIBRARY})

set(sqlite3_INCLUDE_DIR "../lib/sqlite3/include")
set(sqlite3_LIBRARY_DIR "../lib/sqlite3/lib")

find_library(SQLite3_LIBRARY sqlite3 ${sqlite3_LIBRARY_DIR})
if (SQLite3_LIBRARY)
    message(STATUS "SQLite3 library found.")
endif()

include_directories(${sqlite3_INCLUDE_DIR})
target_link_directories(sedric PRIVATE ${sqlite3_LIBRARY_DIR})
target_link_libraries(sedric PRIVATE ${SQLite3_LIBRARY})

#set_target_properties(sedric PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(sedric)
endif()
