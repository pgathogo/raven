cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")

project(Traffik VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(rave_framework_INCLUDE_DIRS "../framework")
set(rave_security_INCLUDE_DIRS "../framework")
set(rave_audio_INCLUDE_DIRS "../audio")
#set(rave_audiolib_INCLUDE_DIRS "../audiolib/headers")

set(Bass_INCLUDE_DIRS "../../lib/BassLib/Include")

set(postgreSQL_ROOT "C:/Program Files/PostgreSQL/14")
find_package(PostgreSQL REQUIRED)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})


#set(LIMEREPORT_INCLUDE_DIR "../lib/limerport/include")
#set(LIMEREPORT_LIB "../lib/limereport")

#find_library(LIMEREPORT_LIBRARY limereport ${LIMEREPORT_LIB})
#if (LIMEREPORT_LIB)
#        message(STATUS "Limereport library found.")
#endif()

#include_directories(${LIMEREPORT_INCLUDE_DIR})
#set(LIMEREPORT_LIBRARIES ${LIMEREPORT_LIBRARY})


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
    traffik.qrc

    ../audio/artist.h
    ../audio/audio.h
    ../audio/audiofile.h
    ../audio/audiofile.cpp
    ../audio/audiotool.h
    ../audio/audiotool.cpp
    ../audio/artist.cpp
    ../audio/audio.cpp
    ../audio/audioconverter.h
    ../audio/audioconverter.cpp
    ../audio/mp3oggconverter.h
    ../audio/mp3oggconverter.cpp
    ../audio/oggtooggconverter.h
    ../audio/oggtooggconverter.cpp
    ../audio/audiowaveformgenerator.h
    ../audio/audiowaveformgenerator.cpp
    ../audio/audiofileinfo.h
    ../audio/audiofileinfo.cpp

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

    ../framework/ravensetup.h
    ../framework/ravensetup.cpp
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
    ../framework/applicationcontext.h
    ../framework/applicationcontext.cpp

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
target_link_libraries(Traffik PRIVATE ${PostgreSQL_LIBRARIES})

target_include_directories(Traffik PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(Traffik PRIVATE ${rave_framework_INCLUDE_DIRS})

#target_link_libraries(Traffik PRIVATE ${LIMEREPORT_LIBRARIES})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})

#link_directories(${AUDIO_LIBRARY_DIR})

#set(AUDIO_INCLUDE_DIR "../audiolib/headers")
#set(AUDIO_LIBRARY_DIR "../audiolib/lib_debug")
#find_library(AUDIO_LIBRARY cueeditord ${AUDIO_LIBRARY_DIR})

#if (AUDIO_LIBRARY)
#        message(STATUS "CueEditor library is found.")
#endif()

#target_link_directories(Traffik PRIVATE ${AUDIO_LIBRARY_DIR})
#target_link_libraries(Traffik PRIVATE ${AUDIO_LIBRARY})


set(BASS_INCLUDE_DIR "../lib/BassLib/Include")
set(BASS_LIBRARY_DIR "../lib/BassLib/Lib")
find_library(BASS_LIBRARY bass ${BASS_LIBRARY_DIR})
if (BASS_LIBRARY)
        message(STATUS "Bass library found.")
endif()
target_link_directories(Traffik PRIVATE ${BASS_LIBRARY_DIR})
target_link_libraries(Traffik PRIVATE ${BASS_LIBRARY})

set_target_properties(Traffik PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Traffik)
endif()
