cmake_minimum_required(VERSION 3.5)

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
set(rave_audiolib_INCLUDE_DIRS "../audiolib")

set(postgreSQL_ROOT "C:/Program Files/PostgreSQL/9.5")
find_package(PostgreSQL REQUIRED)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

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

include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

#set(AUDIO_INCLUDE "../audiolib/headers")
#set(AUDIO_LIB "../audiolib/lib_debug")
#find_path(AUDIO_INCLUDE_DIR cueeditor.h ${AUDIO_INCLUDE})
#find_library(AUDIO_LIBRARY cueeditor ${AUDIO_LIB})

#if (AUDIO_INCLUDE_DIR)
#        message(STATUS "Audio Include dir found.")
#endif()

#if (AUDIO_LIBRARY)
#        message(STATUS "Audio library found.")
#endif()


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
target_include_directories(sedric PRIVATE ${rave_audiolib_INCLUDE_DIRS})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})
link_directories(${rave_audio_INCLUDE_DIRS})
link_directories(${rave_audiolib_INCLUDE_DIRS})

target_link_libraries(sedric PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(sedric PRIVATE ${PostgreSQL_LIBRARIES})
target_link_libraries(sedric PRIVATE ${RAVE_AUDIO_LIBRARIES})

set(AUDIO_INCLUDE_DIR "../audiolib/headers")
set(AUDIO_LIBRARY_DIR "../audiolib/lib_debug")
find_library(AUDIO_LIBRARY cueeditord ${AUDIO_LIBRARY_DIR})

if (AUDIO_INCLUDE_DIR)
        message(STATUS "AUDIO_INCLUDE_DIR found")
endif()

if (AUDIO_LIBRARY)
        message(STATUS "CueEditor library found.")
endif()

target_link_directories(sedric PRIVATE ${AUDIO_LIBRARY_DIR})
target_link_libraries(sedric PRIVATE ${AUDIO_LIBRARY})
