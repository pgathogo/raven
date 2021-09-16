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
set(rave_audiolib_INCLUDE_DIRS "../audio")

set(postgreSQL_ROOT "C:/Program Files/PostgreSQL/9.5")
find_package(postgreSQL REQUIRED)

#set(postgreSQL_INCLUDE_DIRS "C:/Program Files/PostgreSQL/9.5/include")
#set(postgreSQL_LINK_DIRS "C:/Program Files/PostgreSQL/9.5/lib/")

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
    ../framework/treeviewmodel.cpp
    ../framework/tableviewmodel.cpp
    ../framework/tree.cpp
    ../framework/schedule.cpp
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
)
endif()


target_include_directories(sedric PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(sedric PRIVATE ${rave_framework_INCLUDE_DIRS})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})
link_directories(${rave_audiolib_INCLUDE_DIRS})

target_link_libraries(sedric PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(sedric PRIVATE ${PostgreSQL_LIBRARIES})
