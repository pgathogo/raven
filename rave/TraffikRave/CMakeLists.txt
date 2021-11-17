cmake_minimum_required(VERSION 3.5)

project(Traffik VERSION 0.1 LANGUAGES CXX)

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

set(RAVE_AUDIO_LIBRARIES ${AUDIO_LIBRARY})
set(LIMEREPORT_LIBRARIES ${LIMEREPORT_LIBRARY})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        clientbrowser.h
        clientbrowser.cpp
        clientbrowser.ui
        client.h
        client.cpp
        order.h
        order.cpp
        clientform.h
        clientform.cpp
        clientform.ui
        brandbrowser.h
        brandbrowser.cpp
        brandbrowser.ui
        spotbrowser.h
        spotbrowser.cpp
        spotbrowser.ui
        orderbrowser.h
        orderbrowser.cpp
        orderbrowser.ui
        agent.h
        agent.cpp
        clientgroup.h
        clientgroup.cpp
        salesperson.h
        salesperson.cpp
        orderform.h
        orderform.cpp
        orderform.ui
        traffiksetup.h
        traffiksetup.cpp
        bookingsegment.h
        bookingsegment.cpp
        orderbooking.h
        orderbooking.cpp
        spotaudio.h
        spotaudio.cpp
        spottypeexclusion.h
        spottypeexclusion.cpp
        spot.h
        spot.cpp
        spotvoiceover.h
        spotvoiceover.cpp
        voiceover.h
        voiceover.cpp
        spotform.h
        spotform.cpp
        spotform.ui
        spotaudiobrowser.h
        spotaudiobrowser.cpp
        spotaudiobrowser.ui
        spotaudioform.h
        spotaudioform.cpp
        spotaudioform.ui
        brand.h
        brand.cpp
        brandform.h
        brandform.cpp
        brandform.ui
        orderpackage.h
        orderpackage.cpp
        setupform.h
        setupform.cpp
        setupform.ui
        orderapprover.h
        orderapprover.cpp
        approverform.h
        approverform.cpp
        approverform.ui
        scheduleform.h
        scheduleform.cpp
        scheduleform.ui
        breakcreateform.h
        breakcreateform.cpp
        breakcreateform.ui
        breaklayoutline.h
        breaklayoutline.cpp
        breaklayout.h
        breaklayout.cpp
        breaklayoutform.h
        breaklayoutform.cpp
        breaklayoutform.ui
        breaklayoutbrowser.h
        breaklayoutbrowser.cpp
        breaklayoutbrowser.ui
        schedulemantreeviewmodel.h
        schedulemantreeviewmodel.cpp
        mergedbrowser.h
        mergedbrowser.cpp
        mergedbrowser.ui
        bookingorderbrowser.h
        bookingorderbrowser.cpp
        bookingorderbrowser.ui
        bookingwizard.h
        bookingwizard.cpp
        bookingwizard.ui
        traffikrules.h
        traffikrules.cpp
        timeband.h
        timeband.cpp
        setupbrowser.h
        setupbrowser.cpp
        setupbrowser.ui
        timebandbrowser.h
        timebandbrowser.cpp
        timebandbrowser.ui
        timebandform.h
        timebandform.cpp
        timebandform.ui
        agentbrowser.h
        agentbrowser.cpp
        agentbrowser.ui
        agentform.h
        agentform.cpp
        agentform.ui
        salespersonbrowser.h
        salespersonbrowser.cpp
        salespersonbrowser.ui
        salespersonform.h
        salespersonform.cpp
        salespersonform.ui
        voiceoverbrowser.h
        voiceoverbrowser.cpp
        voiceoverbrowser.ui
        voiceoverform.h
        voiceoverform.cpp
        voiceoverform.ui
        typeexclusionbrowser.h
        typeexclusionbrowser.cpp
        typeexclusionbrowser.ui
        typeexclusiondetails.h
        typeexclusiondetails.cpp
        typeexclusiondetails.ui
        clientgroupdetaildlg.h
        clientgroupdetaildlg.cpp
        clientgroupdetaildlg.ui
        clientgroupdlg.h
        clientgroupdlg.cpp
        clientgroupdlg.ui
        commlogform.h
        commlogform.cpp
        commlogform.ui
        commlogtreeviewmodel.h
        commlogtreeviewmodel.cpp
        tree.h
        tree.cpp
        treeviewmodel.h
        treeviewmodel.cpp

        ../audio/artist.h
        ../audio/audio.h
        ../audio/audiofile.h
        ../audio/audiofile.cpp
        ../audio/audiotool.h
        ../audio/audiotool.cpp
        ../audio/artist.cpp
        ../audio/audio.cpp

        ../framework/entityregister.h
        ../framework/relationmapper.h
        ../framework/schedule.h
        ../framework/schedule.cpp
        ../framework/relationmapper.cpp
        ../framework/entitydatamodel.h
        ../framework/entitydatamodel.cpp
        ../framework/baseentity.cpp
        ../framework/baseentitybrowserdlg.cpp
        ../framework/baseentitydetaildlg.cpp
        ../framework/databaseconnector.cpp
        ../framework/databasemanager.cpp
        ../framework/entityfields.h
        ../framework/entityfields.cpp
        ../framework/queryset.h
        ../framework/dataprovider.h
        ../framework/dataprovider.cpp
        ../framework/ravenexception.h
        ../framework/ravenexception.cpp
        ../framework/letterfilterwidget.cpp
        ../framework/letterfilterwidget.h
        ../framework/letterfilterwidget.ui
        ../framework/choicefield.h
        ../framework/valuelist.h
        ../framework/valuelist.cpp
        ../framework/manytomany.h
        ../framework/manytomany.cpp
        ../framework/typeexclusion.h
        ../framework/typeexclusion.cpp
        ../framework/manytomanybrowser.h
        ../framework/manytomanybrowser.cpp
        ../framework/manytomanybrowser.ui
        ../framework/picklistbrowser.h
        ../framework/picklistbrowser.cpp
        ../framework/picklistbrowser.ui
        ../framework/selectclosewidget.h
        ../framework/selectclosewidget.cpp
        ../framework/selectclosewidget.ui
        ../framework/valuelistbrowser.h
        ../framework/valuelistbrowser.cpp
        ../framework/valuelistbrowser.ui
        ../framework/valuelistdetaildlg.h
        ../framework/valuelistdetaildlg.cpp
        ../framework/valuelistdetaildlg.ui

        ../security/loginform.h
        ../security/loginform.cpp
        ../security/loginform.ui
        ../security/accesscontroller.h
        ../security/authentication.h
        ../security/accesscontroller.cpp
        ../security/authentication.cpp
        ../security/user.h
        ../security/user.cpp
        ../security/userbrowser.h
        ../security/userbrowser.cpp
        ../security/userform.h
        ../security/userform.cpp
        ../security/contentauthbrowser.cpp
        ../security/contentauthbrowser.h
        ../security/contentauthbrowser.ui
        ../security/contentauthform.h
        ../security/contentauthform.cpp
        ../security/contentauthform.ui
        ../security/contentauth.cpp
        ../security/contentauth.h
        ../security/role.h
        ../security/role.cpp
        ../security/rolemember.h
        ../security/rolemember.cpp
        ../security/content.h
        ../security/content.cpp
        ../security/contentbrowser.h
        ../security/contentbrowser.cpp
        ../security/contentbrowser.ui
        ../security/contentform.h
        ../security/contentform.cpp
        ../security/contentform.ui
        ../security/contentdetailform.h
        ../security/contentdetailform.cpp
        ../security/contentdetailform.ui
        ../security/rolebrowser.h
        ../security/rolebrowser.cpp
        ../security/rolebrowser.ui
        ../security/roleform.h
        ../security/roleform.cpp
        ../security/roleform.ui

        ../utils/notificationbar.h
        ../utils/notificationitem.h
        ../utils/notificationbar.cpp
        ../utils/notificationitem.cpp
        ../utils/notificationitem.ui
        ../utils/daypartgrid.h
        ../utils/daypartgrid.cpp
        ../utils/daypartgrid.ui
        ../utils/qchecklist.h
)

#set(PROJECT_SOURCES
#        main.cpp
#        mainwindow.cpp
#        mainwindow.h
#        mainwindow.ui
#        tree.h
#        tree.cpp
#        treeviewmodel.h
#        treeviewmodel.cpp
#        schedulemantreeviewmodel.h
#        schedulemantreeviewmodel.cpp
#        scheduleform.h
#        scheduleform.cpp
#        scheduleform.ui
#        approverform.h
#        approverform.cpp
#        approverform.ui
#        bookingsegment.h
#        bookingsegment.cpp
#        bookingwizard.h
#        bookingwizard.cpp
#        bookingwizard.ui
#        breakcreateform.h
#        breakcreateform.cpp
#        breakcreateform.ui
#        breaklayout.h
#        breaklayout.cpp
#        breaklayoutbrowser.h
#        breaklayoutbrowser.cpp
#        breaklayoutbrowser.ui
#        breaklayoutform.h
#        breaklayoutform.cpp
#        breaklayoutform.ui
#        breaklayoutline.h
#        breaklayoutline.cpp
#        commlogtreeviewmodel.h
#        commlogtreeviewmodel.cpp
#        commlogform.h
#        commlogform.cpp
#        commlogform.ui
#        mergedbrowser.h
#        mergedbrowser.cpp
#        mergedbrowser.ui
#        order.h
#        order.cpp
#        orderapprover.h
#        orderapprover.cpp
#        orderbooking.h
#        orderbooking.cpp
#        orderbrowser.h
#        orderbrowser.cpp
#        orderbrowser.ui
#        orderform.h
#        orderform.cpp
#        orderform.ui
#        orderpackage.h
#        orderpackage.cpp
#        setupbrowser.h
#        setupbrowser.cpp
#        setupbrowser.ui
#        setupform.h
#        setupform.cpp
#        setupform.ui
#        spotaudio.h
#        spotaudio.cpp
#        spotaudiobrowser.h
#        spotaudiobrowser.cpp
#        spotaudiobrowser.ui
#        spotaudioform.h
#        spotaudioform.cpp
#        spotaudioform.ui
#        traffiksetup.h
#        traffiksetup.cpp
#        traffikrules.h
#        traffikrules.cpp
#        agent.h
#        agent.cpp
#        agentbrowser.h
#        agentbrowser.cpp
#        agentbrowser.ui
#        agentform.h
#        agentform.cpp
#        agentform.ui
#        brand.h
#        brand.cpp
#        brandbrowser.h
#        brandbrowser.cpp
#        brandbrowser.ui
#        brandform.h
#        brandform.cpp
#        brandform.ui
#        client.h
#        client.cpp
#        clientbrowser.h
#        clientbrowser.cpp
#        clientbrowser.ui
#        clientform.h
#        clientform.cpp
#        clientform.ui
#        clientgroup.h
#        clientgroup.cpp
#        clientgroupdetaildlg.h
#        clientgroupdetaildlg.cpp
#        clientgroupdetaildlg.ui
#        salesperson.h
#        salesperson.cpp
#        salespersonbrowser.h
#        salespersonbrowser.cpp
#        salespersonbrowser.ui
#        salespersonform.h
#        salespersonform.cpp
#        salespersonform.ui
#        spot.h
#        spot.cpp
#        spotbrowser.h
#        spotbrowser.cpp
#        spotbrowser.ui
#        spotform.h
#        spotform.cpp
#        spotform.ui
#        spottypeexclusion.h
#        spottypeexclusion.cpp
#        spotvoiceover.h
#        spotvoiceover.cpp
#        timeband.h
#        timeband.cpp
#        timebandbrowser.h
#        timebandbrowser.cpp
#        timebandbrowser.ui
#        timebandform.h
#        timebandform.cpp
#        timebandform.ui
#        typeexclusionbrowser.h
#        typeexclusionbrowser.cpp
#        typeexclusionbrowser.ui
#        typeexclusiondetails.h
#        typeexclusiondetails.cpp
#        typeexclusiondetails.ui
#        voiceover.h
#        voiceover.cpp
#        voiceoverbrowser.h
#        voiceoverbrowser.cpp
#        voiceoverbrowser.ui
#        voiceoverform.h
#        voiceoverform.cpp
#        voiceoverform.ui
#        bookingorderbrowser.h
#        bookingorderbrowser.cpp
#        bookingorderbrowser.ui
#        clientgroupdlg.h
#        clientgroupdlg.cpp
#        clientgroupdlg.ui
#        ../framework/entityregister.h
#        ../framework/relationmapper.h
#        ../framework/schedule.h
#        ../framework/schedule.cpp
#        ../framework/relationmapper.cpp
#        ../framework/datetimeselector.cpp
#        ../framework/letterfilterwidget.cpp
#        ../framework/letterfilterwidget.h
#        ../framework/letterfilterwidget.ui
#        ../framework/typeexclusion.h
#        ../framework/typeexclusion.cpp
#        ../framework/baseentity.cpp
#        ../framework/baseentitybrowserdlg.cpp
#        ../framework/baseentitydetaildlg.cpp
#        ../framework/databaseconnector.cpp
#        ../framework/databasemanager.cpp
#        ../framework/dataprovider.h
#        ../framework/dataprovider.cpp
#        ../framework/entitydatamodel.h
#        ../framework/entitydatamodel.cpp
#        ../framework/manytomany.h
#        ../framework/manytomany.cpp
#        ../framework/manytomanybrowser.h
#        ../framework/manytomanybrowser.cpp
#        ../framework/manytomanybrowser.ui
#        ../framework/picklistbrowser.h
#        ../framework/picklistbrowser.cpp
#        ../framework/picklistbrowser.ui
#        ../framework/ravenexception.h
#        ../framework/ravenexception.cpp
#        ../framework/selectclosewidget.h
#        ../framework/selectclosewidget.cpp
#        ../framework/selectclosewidget.ui
#        ../framework/valuelist.h
#        ../framework/valuelist.cpp
#        ../framework/valuelistbrowser.h
#        ../framework/valuelistbrowser.cpp
#        ../framework/valuelistbrowser.ui
#        ../framework/valuelistdetaildlg.h
#        ../framework/valuelistdetaildlg.cpp
#        ../framework/valuelistdetaildlg.ui
#        ../framework/datetimeselector.h
#        ../framework/baseentity.h
#        ../framework/baseentitybrowserdlg.h
#        ../framework/baseentitydetaildlg.h
#        ../framework/choicefield.h
#        ../framework/databaseconnector.h
#        ../framework/databasemanager.h
#        ../framework/datetimeselector.ui
#        ../framework/baseentitybrowserdlg.ui
#        ../framework/baseentitydetaildlg.ui
#        ../audio/artist.h
#        ../audio/audio.h
#        ../audio/audiofile.h
#        ../audio/audiotool.h
#        ../audio/audiofile.cpp
#        ../audio/audiotool.cpp
#        ../audio/artist.cpp
#        ../audio/audio.cpp
#        ../utils/qchecklist.h
#        ../utils/notificationbar.h
#        ../utils/notificationitem.h
#        ../utils/notificationbar.cpp
#        ../utils/notificationitem.cpp
#        ../utils/notificationitem.ui
#        ../utils/plainform.h
#        ../utils/tools.h
#        ../utils/types.h
#        ../utils/plainform.ui
#        ../utils/daypartgrid.h
#        ../utils/daypartgrid.cpp
#        ../utils/daypartgrid.ui
#        ../utils/plainform.cpp
#        ../security/accesscontroller.h
#        ../security/authentication.h
#        ../security/accesscontroller.cpp
#        ../security/authentication.cpp
#        ../security/loginform.h
#        ../security/loginform.cpp
#        ../security/loginform.ui
#        ../security/role.h
#        ../security/role.cpp
#        ../security/user.h
#        ../security/user.cpp
#        ../security/userbrowser.h
#        ../security/userbrowser.cpp
#        ../security/userform.h
#        ../security/userform.cpp
#        ../security/rolemember.h
#        ../security/rolemember.cpp
#        ../security/content.h
#        ../security/content.cpp
#        ../security/contentauth.cpp
#        ../security/contentauth.h
#        ../security/contentauthbrowser.cpp
#        ../security/contentauthbrowser.h
#        ../security/contentauthbrowser.ui
#        ../security/contentauthform.h
#        ../security/contentauthform.cpp
#        ../security/contentauthform.ui
#        ../security/contentbrowser.h
#        ../security/contentbrowser.cpp
#        ../security/contentbrowser.ui
#        ../security/contentdetailform.h
#        ../security/contentdetailform.cpp
#        ../security/contentdetailform.ui
#        ../security/contentform.h
#        ../security/contentform.cpp
#        ../security/contentform.ui
#        ../security/rolebrowser.h
#        ../security/rolebrowser.cpp
#        ../security/rolebrowser.ui
#        ../security/roleform.h
#        ../security/roleform.cpp
#        ../security/roleform.ui
#        ../security/userbrowser.ui
#        ../security/userform.ui
#)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Traffik
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Traffik SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Traffik
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Traffik PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(Traffik PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(Traffik PRIVATE ${rave_framework_INCLUDE_DIRS})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})
link_directories(${rave_audiolib_INCLUDE_DIRS})

target_link_libraries(Traffik PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Traffik PRIVATE ${PostgreSQL_LIBRARIES})
target_link_libraries(Traffik PRIVATE ${RAVE_AUDIO_LIBRARIES})
target_link_libraries(Traffik PRIVATE ${LIMEREPORT_LIBRARIES})

set_target_properties(Traffik PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Traffik)
endif()
