cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPLIER "C:/mingw64/bin/g++.exe")

project(clusterman VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(rave_framework_INCLUDE_DIRS "../../../rave/framework")
set(rave_security_INCLUDE_DIRS "../../../rave/security")

set(postgreSQL_ROOT "C:/Program Files/PostgreSQL/9.5")
find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(UTILS_DIR "../../../rave/utils")


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        cluster.h
        cluster.cpp
        clusterbrowser.h
        clusterbrowser.cpp
        clusterbrowser.ui
        server.h
        server.cpp
        database.h
        database.cpp
        station.h
        station.cpp
        audiostore.h
        audiostore.cpp
        storagedisk.h
        storagedisk.cpp
        audiofolder.h
        audiofolder.cpp
        stationsetup.h
        stationsetup.cpp
        clustermanagerdlg.h
        clustermanagerdlg.cpp
        clusterform.h
        clusterform.cpp
        stationform.h
        stationform.cpp
        confignode.h
        server.h
        server.cpp
        serverform.h
        serverform.cpp
        storagediskform.h
        storagediskform.cpp
        audiofolderform.h
        audiofolderform.cpp
        audiofolderform.ui
        clusterman.qrc
        ../../../rave/security/authentication.h
        ../../../rave/security/authentication.cpp
        ../../../rave/security/rolemember.h
        ../../../rave/security/rolemember.cpp
        ../../../rave/security/role.h
        ../../../rave/security/role.cpp
        ../../../rave/security/user.h
        ../../../rave/security/user.cpp
        ../../../rave/security/userbrowser.h
        ../../../rave/security/userbrowser.cpp
        ${rave_security_INCLUDE_DIRS}/roleform.h
        ${rave_security_INCLUDE_DIRS}/roleform.cpp
        ${rave_security_INCLUDE_DIRS}/userform.h
        ${rave_security_INCLUDE_DIRS}/userform.cpp
        ${rave_security_INCLUDE_DIRS}/content.h
        ${rave_security_INCLUDE_DIRS}/content.cpp
        ${rave_security_INCLUDE_DIRS}/contentform.h
        ${rave_security_INCLUDE_DIRS}/contentform.cpp
        ${rave_security_INCLUDE_DIRS}/contentform.ui
        ${rave_security_INCLUDE_DIRS}/contentdetailform.h
        ${rave_security_INCLUDE_DIRS}/contentdetailform.cpp

        ${rave_framework_INCLUDE_DIRS}/baseentity.cpp
        ${rave_framework_INCLUDE_DIRS}/baseentitydetaildlg.h
        ${rave_framework_INCLUDE_DIRS}/baseentitydetaildlg.cpp
        ${rave_framework_INCLUDE_DIRS}/baseentitybrowserdlg.h
        ${rave_framework_INCLUDE_DIRS}/baseentitybrowserdlg.cpp
        ${rave_framework_INCLUDE_DIRS}/letterfilterwidget.h
        ${rave_framework_INCLUDE_DIRS}/letterfilterwidget.cpp
        ${rave_framework_INCLUDE_DIRS}/baseentitydetaildlg.cpp
        ${rave_framework_INCLUDE_DIRS}/manytomanybrowser.h
        ${rave_framework_INCLUDE_DIRS}/manytomanybrowser.cpp
        ${rave_framework_INCLUDE_DIRS}/selectclosewidget.h
        ${rave_framework_INCLUDE_DIRS}/selectclosewidget.cpp
        ${rave_framework_INCLUDE_DIRS}/picklistbrowser.h
        ${rave_framework_INCLUDE_DIRS}/picklistbrowser.cpp
        ${rave_framework_INCLUDE_DIRS}/manytomany.h
        ${rave_framework_INCLUDE_DIRS}/manytomany.cpp
        ${rave_framework_INCLUDE_DIRS}/typeexclusion.h
        ${rave_framework_INCLUDE_DIRS}/typeexclusion.cpp
        ../../../rave/framework/entityfields.h
        ../../../rave/framework/entityfields.cpp
        ../../../rave/framework/dataprovider.h
        ../../../rave/framework/dataprovider.cpp
        ../../../rave/framework/databaseconnector.cpp
        ../../../rave/framework/databasemanager.cpp
        ../../../rave/framework/ravenexception.h
        ../../../rave/framework/ravenexception.cpp
        ../../../rave/framework/entitydatamodel.h
        ../../../rave/framework/entitydatamodel.cpp
        ../../../rave/framework/relationmapper.h
        ../../../rave/framework/relationmapper.cpp
        ${UTILS_DIR}/notificationbar.h
        ${UTILS_DIR}/notificationbar.cpp
        ${UTILS_DIR}/notificationitem.h
        ${UTILS_DIR}/notificationitem.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(clusterman
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(clusterman SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(clusterman
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(clusterman PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(clusterman PRIVATE ${PostgreSQL_LIBRARIES})

target_include_directories(clusterman PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(clusterman PRIVATE ${rave_framework_INCLUDE_DIRS})

#target_link_libraries(Traffik PRIVATE ${LIMEREPORT_LIBRARIES})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})

set(sqlite3_INCLUDE_DIR "../../../rave/lib/sqlite3/include")
set(sqlite3_LIBRARY_DIR "../../../rave/lib/sqlite3/lib")

find_library(SQLite3_LIBRARY sqlite3 ${sqlite3_LIBARRY_DIR})
if (SQLite3_LIBRARY)
    message(STATUS "SQLite3 library found!")
endif()

include_directories(${sqlite3_INCLUDE_DIR})
target_link_directories(clusterman PRIVATE ${sqlite3_LIBRARY_DIR})
target_link_libraries(clusterman PRIVATE ${SQLite3_LIBRARY})


set_target_properties(clusterman PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(clusterman)
endif()
