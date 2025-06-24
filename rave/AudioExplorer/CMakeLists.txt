cmake_minimum_required(VERSION 3.15)

#set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")
string(REGEX REPLACE "/W[3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(MSVS_BUILD_NUM "14.42.34433")

set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/${MSVS_BUILD_NUM}/bin/Hostx64/x64/cl.exe")

project(AudioExplorer VERSION 0.1 LANGUAGES CXX)

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

message(STATUS "****************")
message(STATUS "QT_VERSION_MAJOR: ${QT_VERSION_MAJOR}")
message(STATUS "****************")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

include_directories(${AUDIO_INCLUDE_DIR})
set(RAVE_AUDIO_LIBRARIES ${AUDIO_LIBRARY})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        datatoolbar.h
        datatoolbar.cpp
        artistform.h
        artistform.cpp
        artistform.ui
        audioform.h
        audioform.cpp
        audioform.ui
        foldernameform.h
        foldernameform.cpp
        foldernameform.ui
        artistmanager.h
        artistmanager.cpp
        trashcanform.h
        trashcanform.cpp
        trashcanform.ui
        bookedaudiocheckerform.cpp
        bookedaudiocheckerform.h
        bookedaudiocheckerform.ui
        setupform.ui
        setupform.h
        setupform.cpp
        audioexplorer.qrc
        ../rave.qrc

        ../audio/audiolibrary.h
        ../audio/audiolibrary.cpp
        ../audio/audiofolder.cpp
        ../audio/audio.cpp
        ../audio/artist.cpp
        ../audio/audiofile.cpp
        ../audio/audiolibitem.cpp
        ../audio/audiotool.cpp
        ../audio/artisttypeitem.cpp
        ../audio/genretypeitem.h
        ../audio/genretypeitem.cpp
        ../audio/genreform.cpp
        ../audio/audioconverter.h
        ../audio/audioconverter.cpp
        ../audio/mp3oggconverter.h
        ../audio/mp3oggconverter.cpp
        ../audio/oggtooggconverter.h
        ../audio/oggtooggconverter.cpp
        ../audio/mtstomp3converter.h
        ../audio/mtstomp3converter.cpp
        ../audio/audiowaveformgenerator.h
        ../audio/audiowaveformgenerator.cpp
        ../audio/audiofileinfo.h
        ../audio/audiofileinfo.cpp
        ../audio/audiometer.h
        ../audio/audiometer.cpp

        ../audio/editor/audiographic.cpp
        ../audio/editor/audioplayer.cpp
        ../audio/editor/audiothread.cpp
        ../audio/editor/audiowaveform.h
        ../audio/editor/audiowaveform.cpp
        ../audio/editor/audiowaveform.ui
        ../audio/editor/subjectobserver.cpp
        ../audio/editor/vumeter.cpp
        ../audio/editor/audiographic.h
        ../audio/editor/audioplayer.h
        ../audio/editor/audiothread.h
        ../audio/editor/markindicator.h
        ../audio/editor/subjectobserver.h
        ../audio/editor/vumeter.h

        ../framework/baseentity.cpp
        ../framework/baseentitybrowserdlg.cpp
        ../framework/baseentitybrowserdlg.ui
        ../framework/baseentitydetaildlg.cpp
        ../framework/baseentitydetaildlg.ui
        ../framework/entitydatamodel.cpp
        ../framework/relationmapper.cpp
        ../framework/entityfields.cpp
        ../framework/databaseconnector.cpp
        ../framework/databasemanager.cpp
        ../framework/ravenexception.cpp
        ../framework/dataprovider.cpp
        ../framework/manytomany.cpp
        ../framework/typeexclusion.cpp
        ../framework/queryset.h
        ../framework/valuelist.cpp
        ../framework/tree.h
        ../framework/tree.cpp
        ../framework/treeviewmodel.h
        ../framework/treeviewmodel.cpp
        ../framework/letterfilterwidget.cpp
        ../framework/picklistbrowser.cpp
        ../framework/selectclosewidget.cpp
        ../framework/ravensetup.h
        ../framework/ravensetup.cpp
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

        ../utils/notificationbar.cpp
        ../utils/notificationitem.cpp

        ../../ravecluster/rave/clusterman/clustercontroller.h
        ../../ravecluster/rave/clusterman/clustercontroller.cpp
        ../../ravecluster/rave/clusterman/server.h
        ../../ravecluster/rave/clusterman/server.cpp
        ../../ravecluster/rave/clusterman/station.h
        ../../ravecluster/rave/clusterman/station.cpp
        ../../ravecluster/rave/clusterman/cluster.h
        ../../ravecluster/rave/clusterman/cluster.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AudioExplorer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(AudioExplorer SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(AudioExplorer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set(CMAKE_EXE_LINKER_FLAGS "-static")


if (MSVC)
    #target_compile_definitions(AudioExplorer PRIVATE $<$<CONFIG:Debug>:MDd_DynamicDebug>)

    target_compile_options(AudioExplorer PRIVATE "/W0")
    target_compile_options(AudioExplorer PUBLIC "/wd4716")
    target_compile_options(AudioExplorer PUBLIC "/Zc:__cplusplus")
    target_compile_options(AudioExplorer PUBLIC "/EHsc")
 else()
     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type")
 endif()

target_compile_options(AudioExplorer PRIVATE /MT)

#set(CMAKE_CXX_FLAGS_RELEASE "/MT")
#target_link_options(AudioExplorer PRIVATE /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
#target_link_options(AudioExplorer PRIVATE /INCREMENTAL:NO /NODEFAULTLIB:MSVCRTD)

target_link_libraries(AudioExplorer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(AudioExplorer PRIVATE ${postgreSQL_INCLUDE_DIRS})

target_include_directories(AudioExplorer PRIVATE ${rave_framework_INCLUDE_DIRS})
#target_include_directories(AudioExplorer PRIVATE ${rave_audiolib_INCLUDE_DIRS})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})

#link_directories(${rave_audio_INCLUDE_DIRS})
#link_directories(${rave_audiolib_INCLUDE_DIRS})

target_link_libraries(AudioExplorer PRIVATE ${PostgreSQL_LIBRARIES})

#target_link_libraries(AudioExplorer PRIVATE ${RAVE_AUDIO_LIBRARIES})

#set(AUDIO_INCLUDE_DIR "../audiolib/headers")
#set(AUDIO_LIBRARY_DIR "../audiolib/lib_debug")
#find_library(AUDIO_LIBRARY cueeditord ${AUDIO_LIBRARY_DIR})

#if (AUDIO_INCLUDE_DIR)
#        message(STATUS "AUDIO_INCLUDE_DIR found")
#endif()

#if (AUDIO_LIBRARY)
#        message(STATUS "CueEditor library found.")
#endif()

#target_link_directories(AudioExplorer PRIVATE ${AUDIO_LIBRARY_DIR})
#target_link_libraries(AudioExplorer PRIVATE ${AUDIO_LIBRARY})

set(BASS_INCLUDE_DIR "../lib/BassLib/Include")
set(BASS_LIBRARY_DIR "../lib/BassLib/Lib")
find_library(BASS_LIBRARY bass ${BASS_LIBRARY_DIR})
if (BASS_LIBRARY)
        message(STATUS "Bass library found.")
endif()

target_link_directories(AudioExplorer PRIVATE ${BASS_LIBRARY_DIR})
target_link_libraries(AudioExplorer PRIVATE ${BASS_LIBRARY})

set(sqlite3_INCLUDE_DIR "../lib/sqlite3/include")
set(sqlite3_LIBRARY_DIR "../lib/sqlite3/lib")

find_library(SQLite3_LIBRARY sqlite3 ${sqlite3_LIBRARY_DIR})
if (SQLite3_LIBRARY)
    message(STATUS "SQLite3 library found.")
endif()

include_directories(${sqlite3_INCLUDE_DIR})
target_link_directories(AudioExplorer PRIVATE ${sqlite3_LIBRARY_DIR})
target_link_libraries(AudioExplorer PRIVATE ${SQLite3_LIBRARY})

# QWT Library
#set(Qwt_INCLUDE_DIR "../lib/qwt5/include")
#set(Qwt_LIBRARY_DIR "../lib/qwt5/lib")
#find_library(Qwt_LIBRARY qwt5-qt6 ${Qwt_LIBRARY_DIR})
#if(Qwt_LIBRARY)
#    message(STATUS "Qwt5 Lib found.")
#endif()



include_directories(${Qwt_INCLUDE_DIR})
target_link_directories(AudioExplorer PRIVATE ${Qwt_LIBRARY_DIR})
target_link_libraries(AudioExplorer PRIVATE ${Qwt_LIBRARY})


set_target_properties(AudioExplorer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AudioExplorer)
endif()
