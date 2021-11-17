cmake_minimum_required(VERSION 3.5)

project(TraffikRave VERSION 0.1 LANGUAGES CXX)

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

set(AUDIO_INCLUDE "../audiolib/headers")
set(AUDIO_LIB "../audiolib/lib_debug")
set(LIMEREPORT_INCLUDE_DIR "../lib/limerport/include")
set(LIMEREPORT_LIB "../lib/limereport")

find_path(AUDIO_INCLUDE_DIR cueeditor.h ${AUDIO_INCLUDE})
find_library(AUDIO_LIBRARY cueeditor ${AUDIO_LIB})
find_library(LIMEREPORT_LIBRARY limereport ${LIMEREPORT_LIB})

if (AUDIO_INCLUDE_DIR)
        message(STATUS "Audio Include dir found.")
endif()

if (AUDIO_LIBRARY)
        message(STATUS "Audio library found.")
endif()

if (LIMEREPORT_LIB)
        message(STATUS "Limereport library found.")
endif()

include_directories(${AUDIO_INCLUDE_DIR})
include_directories(${AUDIO_LIB})
include_directories(${LIMEREPORT_INCLUDE_DIR})

#set(RAVE_AUDIO_LIBRARIES ${AUDIO_LIBRARY})
#set(LIMEREPORT_LIBRARIES ${LIMEREPORT_LIBRARY})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        approverform.h
        bookingsegment.h
        bookingwizard.h
        tree.h
        tree.cpp
        treeviewmodel.h
        treeviewmodel.cpp
        schedulemantreeviewmodel.h
        schedulemantreeviewmodel.cpp
        breakcreateform.h
        breaklayout.h
        breaklayoutbrowser.h
        breaklayoutform.h
        breaklayoutline.h
        commlogform.h
        mergedbrowser.h
        order.h
        orderapprover.h
        orderbooking.h
        orderbrowser.h
        orderform.h
        orderpackage.h
        scheduleform.h
        scheduleform.cpp
        scheduleform.ui
        setupbrowser.h
        setupform.h
        spotaudio.h
        spotaudiobrowser.h
        spotaudioform.h
        traffiksetup.h
        approverform.cpp
        bookingsegment.cpp
        bookingwizard.cpp
        breakcreateform.cpp
        breaklayout.cpp
        breaklayoutbrowser.cpp
        breaklayoutform.cpp
        breaklayoutline.cpp
        commlogform.cpp
        mergedbrowser.cpp
        order.cpp
        orderapprover.cpp
        orderbooking.cpp
        orderbrowser.cpp
        orderform.cpp
        orderpackage.cpp
        setupbrowser.cpp
        setupform.cpp
        spotaudio.cpp
        spotaudiobrowser.cpp
        spotaudioform.cpp
        traffiksetup.cpp
        approverform.ui
        bookingwizard.ui
        breakcreateform.ui
        breaklayoutbrowser.ui
        breaklayoutform.ui
        commlogform.ui
        mergedbrowser.ui
        orderbrowser.ui
        orderform.ui
        setupbrowser.ui
        setupform.ui
        spotaudiobrowser.ui
        spotaudioform.ui
        commlogtreeviewmodel.cpp
        traffikrules.cpp
        agent.cpp
        agentbrowser.cpp
        agentform.cpp
        brand.cpp
        brandbrowser.cpp
        brandform.cpp
        client.cpp
        clientbrowser.cpp
        clientform.cpp
        clientgroup.cpp
        clientgroupdetaildlg.cpp
        clientgroupdlg.cpp
        salesperson.cpp
        salespersonbrowser.cpp
        salespersonform.cpp
        spot.cpp
        spotbrowser.cpp
        spotform.cpp
        spottypeexclusion.cpp
        spotvoiceover.cpp
        timeband.cpp
        timebandbrowser.cpp
        timebandform.cpp
        typeexclusionbrowser.cpp
        typeexclusiondetails.cpp
        voiceover.cpp
        voiceoverbrowser.cpp
        voiceoverform.cpp
        bookingorderbrowser.cpp
        commlogtreeviewmodel.h
        traffikrules.h
        agent.h
        agentbrowser.h
        agentform.h
        brand.h
        brandbrowser.h
        brandform.h
        client.h
        clientbrowser.h
        clientform.h
        clientgroup.h
        clientgroupdetaildlg.h
        clientgroupdlg.h
        bookingorderbrowser.h
        salesperson.h
        salespersonbrowser.h
        salespersonform.h
        spot.h
        spotbrowser.h
        spotform.h
        spottypeexclusion.h
        spotvoiceover.h
        timeband.h
        timebandbrowser.h
        timebandform.h
        typeexclusionbrowser.h
        typeexclusiondetails.h
        voiceover.h
        voiceoverbrowser.h
        voiceoverform.h
        agentbrowser.ui
        agentform.ui
        brandbrowser.ui
        brandform.ui
        clientbrowser.ui
        clientform.ui
        clientgroupdetaildlg.ui
        clientgroupdlg.ui
        salespersonbrowser.ui
        salespersonform.ui
        spotbrowser.ui
        spotform.ui
        timebandbrowser.ui
        timebandform.ui
        typeexclusionbrowser.ui
        typeexclusiondetails.ui
        bookingorderbrowser.ui
        voiceoverbrowser.ui
        voiceoverform.ui
        ../framework/entityregister.h
        ../framework/relationmapper.h
        ../framework/schedule.h
        ../framework/schedule.cpp
        ../framework/relationmapper.cpp
        ../framework/datetimeselector.cpp
        ../framework/letterfilterwidget.cpp
        ../framework/typeexclusion.cpp
        ../framework/baseentity.cpp
        ../framework/baseentitybrowserdlg.cpp
        ../framework/baseentitydetaildlg.cpp
        ../framework/databaseconnector.cpp
        ../framework/databasemanager.cpp
        ../framework/dataprovider.cpp
        ../framework/entitydatamodel.cpp
        ../framework/entityfields.cpp
        ../framework/manytomany.cpp
        ../framework/manytomanybrowser.cpp
        ../framework/picklistbrowser.cpp
        ../framework/ravenexception.cpp
        ../framework/selectclosewidget.cpp
        ../framework/valuelist.cpp
        ../framework/valuelistbrowser.cpp
        ../framework/valuelistdetaildlg.cpp
        ../framework/datetimeselector.h
        ../framework/letterfilterwidget.h
        ../framework/typeexclusion.h
        ../framework/baseentity.h
        ../framework/baseentitybrowserdlg.h
        ../framework/baseentitydetaildlg.h
        ../framework/choicefield.h
        ../framework/databaseconnector.h
        ../framework/databasemanager.h
        ../framework/dataprovider.h
        ../framework/entitydatamodel.h
        ../framework/entityfields.h
        ../framework/manytomany.h
        ../framework/manytomanybrowser.h
        ../framework/picklistbrowser.h
        ../framework/queryset.h
        ../framework/ravenexception.h
        ../framework/selectclosewidget.h
        ../framework/valuelist.h
        ../framework/valuelistbrowser.h
        ../framework/valuelistdetaildlg.h
        ../framework/datetimeselector.ui
        ../framework/letterfilterwidget.ui
        ../framework/baseentitybrowserdlg.ui
        ../framework/baseentitydetaildlg.ui
        ../framework/manytomanybrowser.ui
        ../framework/picklistbrowser.ui
        ../framework/selectclosewidget.ui
        ../framework/valuelistbrowser.ui
        ../framework/valuelistdetaildlg.ui
        ../audio/artist.h
        ../audio/audio.h
        ../audio/audiofile.h
        ../audio/audiotool.h
        ../audio/audiofile.cpp
        ../audio/audiotool.cpp
        ../audio/artist.cpp
        ../audio/audio.cpp
        ../utils/qchecklist.h
        ../utils/daypartgrid.h
        ../utils/notificationbar.h
        ../utils/notificationitem.h
        ../utils/plainform.h
        ../utils/tools.h
        ../utils/types.h
        ../utils/daypartgrid.ui
        ../utils/notificationitem.ui
        ../utils/plainform.ui
        ../utils/daypartgrid.cpp
        ../utils/notificationbar.cpp
        ../utils/notificationitem.cpp
        ../utils/plainform.cpp
        ../security/accesscontroller.h
        ../security/authentication.h
        ../security/accesscontroller.cpp
        ../security/authentication.cpp
        ../security/loginform.cpp
        ../security/role.cpp
        ../security/user.cpp
        ../security/userbrowser.cpp
        ../security/userform.cpp
        ../security/rolemember.cpp
        ../security/content.cpp
        ../security/contentauth.cpp
        ../security/contentauthbrowser.cpp
        ../security/contentauthform.cpp
        ../security/contentbrowser.cpp
        ../security/contentdetailform.cpp
        ../security/contentform.cpp
        ../security/rolebrowser.cpp
        ../security/roleform.cpp
        ../security/loginform.h
        ../security/role.h
        ../security/user.h
        ../security/userbrowser.h
        ../security/userform.h
        ../security/rolemember.h
        ../security/content.h
        ../security/contentauth.h
        ../security/contentauthbrowser.h
        ../security/contentauthform.h
        ../security/contentbrowser.h
        ../security/contentdetailform.h
        ../security/contentform.h
        ../security/rolebrowser.h
        ../security/roleform.h
        ../security/userbrowser.ui
        ../security/userform.ui
        ../security/contentauthbrowser.ui
        ../security/contentauthform.ui
        ../security/contentbrowser.ui
        ../security/contentdetailform.ui
        ../security/contentform.ui
        ../security/rolebrowser.ui
        ../security/roleform.ui
        ../security/loginform.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TraffikRave
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(TraffikRave SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(TraffikRave
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TraffikRave PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(TraffikRave PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(TraffikRave PRIVATE ${rave_framework_INCLUDE_DIRS})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})
link_directories(${rave_audiolib_INCLUDE_DIRS})

target_link_libraries(TraffikRave PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(TraffikRave PRIVATE ${PostgreSQL_LIBRARIES})
target_link_libraries(TraffikRave PRIVATE ${RAVE_AUDIO_LIBRARIES})
target_link_libraries(TraffikRave PRIVATE ${LIMEREPORT_LIBRARIES})

set_target_properties(TraffikRave PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TraffikRave)
endif()
