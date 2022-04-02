cmake_minimum_required(VERSION 3.5)

project(AudioExplorer VERSION 0.1 LANGUAGES CXX)

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
        audioexplorer.qrc
        artistmanager.h
        artistmanager.cpp
        trashcanform.h
        trashcanform.cpp
        trashcanform.ui
        bookedaudiocheckerform.cpp
        bookedaudiocheckerform.h
        bookedaudiocheckerform.ui
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
        ../audio/audioimporter.h
        ../audio/audioimporter.cpp
        ../audio/mp3oggconverter.h
        ../audio/mp3oggconverter.cpp
        ../audio/audiowaveformgenerator.h
        ../audio/audiowaveformgenerator.cpp
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
        ../framework/treeviewmodel.h
        ../framework/treeviewmodel.cpp
        ../framework/tree.cpp
        ../framework/letterfilterwidget.cpp
        ../framework/picklistbrowser.cpp
        ../framework/selectclosewidget.cpp
        ../framework/ravensetup.h
        ../framework/ravensetup.cpp
        ../security/authentication.cpp
        ../security/user.cpp
        ../utils/notificationbar.cpp
        ../utils/notificationitem.cpp
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

target_link_libraries(AudioExplorer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(AudioExplorer PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(AudioExplorer PRIVATE ${rave_framework_INCLUDE_DIRS})
target_include_directories(AudioExplorer PRIVATE ${rave_audiolib_INCLUDE_DIRS})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})
link_directories(${rave_audio_INCLUDE_DIRS})
link_directories(${rave_audiolib_INCLUDE_DIRS})

target_link_libraries(AudioExplorer PRIVATE ${PostgreSQL_LIBRARIES})
target_link_libraries(AudioExplorer PRIVATE ${RAVE_AUDIO_LIBRARIES})

set(AUDIO_INCLUDE_DIR "../audiolib/headers")
set(AUDIO_LIBRARY_DIR "../audiolib/lib_debug")
#find_path(AUDIO_INCLUDE_DIR cueeditor.h ${AUDIO_INCLUDE_DIR})
find_library(AUDIO_LIBRARY cueeditord ${AUDIO_LIBRARY_DIR})

if (AUDIO_INCLUDE_DIR)
        message(STATUS "AUDIO_INCLUDE_DIR found")
endif()

if (AUDIO_LIBRARY)
        message(STATUS "CueEditor library found.")
endif()

target_link_directories(AudioExplorer PRIVATE ${AUDIO_LIBRARY_DIR})
target_link_libraries(AudioExplorer PRIVATE ${AUDIO_LIBRARY})

#set_target_properties(AudioExplorer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#)

#if(QT_VERSION_MAJOR EQUAL 6)
#    qt_finalize_executable(AudioExplorer)
#endif()
