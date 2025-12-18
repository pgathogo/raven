cmake_minimum_required(VERSION 3.20)

string(REGEX REPLACE "/W[3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")

set(MSVS_BUILD_NUM "14.42.34433")

set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/${MSVS_BUILD_NUM}/bin/Hostx64/x64/cl.exe")

project(traffik VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
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

set(CMAKE_EXE_LINKER_FLAGS "-static")
#set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")


# set(LIMEREPORT_INCLUDE_DIR "../lib/limerport/include")
# set(LIMEREPORT_LIB "../lib/limereport")

# find_library(LIMEREPORT_LIBRARY limereport ${LIMEREPORT_LIB})
# if (LIMEREPORT_LIB)
#         message(STATUS "Limereport library found.")
# endif()

# include_directories(${LIMEREPORT_INCLUDE_DIR})
# set(LIMEREPORT_LIBRARIES ${LIMEREPORT_LIBRARY})

set(RAVE_AUDIO_EDITOR "../audio/editor")
set(RAVE_FRAMEWORK "../framework")
set(RAVE_SECURITY "../security")
set(UTILS_DIR "../utils")
set(RAVE_CLUSTER_DIR "../../ravecluster/rave/clusterman")


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
    traffiktree.h
    traffiktree.cpp
    traffiktreeviewmodel.h
    traffiktreeviewmodel.cpp

    ../audio/artist.h
    ../audio/audio.h
    ../audio/audiofile.h
    ../audio/audiofile.cpp
    ../audio/audiotool.h
    ../audio/audiotool.cpp
    ../audio/artist.cpp
    ../audio/audio.cpp
    ../audio/audiometer.h
    ../audio/audiometer.cpp
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
    ../audio/audiolibitem.h
    ../audio/audiolibitem.cpp
    ../audio/audiolibrary.h
    ../audio/audiolibrary.cpp
    ../audio/audiofolder.h
    ../audio/audiofolder.cpp

    ${RAVE_AUDIO_EDITOR}/audiographic.cpp
    ${RAVE_AUDIO_EDITOR}/audiographic.h
    ${RAVE_AUDIO_EDITOR}/audioplayer.h
    ${RAVE_AUDIO_EDITOR}/audioplayer.cpp
    ${RAVE_AUDIO_EDITOR}/audiothread.h
    ${RAVE_AUDIO_EDITOR}/audiothread.cpp
    ${RAVE_AUDIO_EDITOR}/audiowaveform.h
    ${RAVE_AUDIO_EDITOR}/audiowaveform.cpp
    ${RAVE_AUDIO_EDITOR}/subjectobserver.cpp
    ${RAVE_AUDIO_EDITOR}/vumeter.h
    ${RAVE_AUDIO_EDITOR}/vumeter.cpp
    ${RAVE_AUDIO_EDITOR}/markindicator.h
    ${RAVE_AUDIO_EDITOR}/subjectobserver.h

    ../AudioExplorer/audioform.h
    ../AudioExplorer/audioform.cpp
    ../AudioExplorer/audioexplorer.qrc

    ../framework/tree.h
    ../framework/tree.cpp
    ../framework/treeviewmodel.h
    ../framework/treeviewmodel.cpp

    ../framework/ravensetup.h
    ../framework/ravensetup.cpp
    ../framework/entityregister.h
    ../framework/schedule.h
    ../framework/schedule.cpp
    ../framework/relationmapper.h
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
    ../framework/datetimeselector.h
    ../framework/datetimeselector.cpp
    ../framework/comboboxitemdelegate.h
    ../framework/comboboxitemdelegate.cpp
    ../framework/readonlydelegate.h
    ../framework/readonlydelegate.cpp
    ../framework/spinboxdelegate.h
    ../framework/spinboxdelegate.cpp
    ../framework/timeeditdelegate.h
    ../framework/timeeditdelegate.cpp
    ../framework/logger.h
    ../framework/logger.cpp
    ../framework/dowwidget.h
    ../framework/dowwidget.cpp
       traffik.qrc

    ../security/loginform.h
    ../security/loginform.cpp
    ../security/loginform.ui
    ../security/accesscontroller.h
    ../security/authentication.h
    ../security/accesscontroller.cpp
    ../security/authentication.cpp
    ../security/baserole.h
    ../security/baserole.cpp
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
    ../security/loginform.h
    ../security/loginform.cpp
    ../security/selectstationform.h
    ../security/selectstationform.cpp
    ../security/userconfig.h
    ../security/userconfig.cpp
     ../rave.qrc

    ../utils/notificationbar.h
    ../utils/notificationitem.h
    ../utils/notificationbar.cpp
    ../utils/notificationitem.cpp
    ../utils/notificationitem.ui
    ../utils/daypartgrid.h
    ../utils/daypartgrid.cpp
    ../utils/daypartgrid.ui
    ../utils/qchecklist.h
    ../utils/tools.h
    ../utils/togglebutton.h
    ../utils/togglebutton.cpp
    ../utils/cachehandler.h
    ../utils/cachehandler.cpp

    ${RAVE_CLUSTER_DIR}/clustercontroller.h
    ${RAVE_CLUSTER_DIR}/clustercontroller.cpp
    ${RAVE_CLUSTER_DIR}/server.h
    ${RAVE_CLUSTER_DIR}/server.cpp
    ${RAVE_CLUSTER_DIR}/station.h
    ${RAVE_CLUSTER_DIR}/station.cpp
    ${RAVE_CLUSTER_DIR}/cluster.h
    ${RAVE_CLUSTER_DIR}/cluster.cpp
    ${RAVE_CLUSTER_DIR}/useraccess.h
    ${RAVE_CLUSTER_DIR}/useraccess.cpp


)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(traffik
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ../AudioExplorer/audioexplorer.qrc
    )
else()
    if(ANDROID)
        add_library(traffik SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(traffik
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if (MSVC)
    target_compile_options(traffik PRIVATE "/W0")
    target_compile_options(traffik PUBLIC "/wd4716")
    target_compile_options(traffik PUBLIC "/Zc:__cplusplus")
    target_compile_options(traffik PUBLIC "/EHsc")
 else()
     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type")
 endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    #target_compile_options(traffik PRIVATE "/MD")
    set_property(TARGET traffik PROPERTY
      MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    #target_compile_options(traffik PRIVATE "/MT")
    set_property(TARGET traffik PROPERTY
      MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Releae>:Release>")
endif()

#target_link_libraries(traffik PRIVATE dbghelp.dll)

#set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")
#target_link_libraries(traffik PRIVATE -static-libstdc++)
target_compile_options(traffik PRIVATE /MT)
target_link_options(traffik PRIVATE /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)

target_link_libraries(traffik PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


target_link_libraries(traffik PRIVATE ${PostgreSQL_LIBRARIES})

target_include_directories(traffik PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(traffik PRIVATE ${rave_framework_INCLUDE_DIRS})

#target_link_libraries(traffik PRIVATE ${LIMEREPORT_LIBRARIES})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})

#link_directories(${AUDIO_LIBRARY_DIR})

#set(AUDIO_INCLUDE_DIR "../audiolib/headers")
#set(AUDIO_LIBRARY_DIR "../audiolib/lib_debug")
#find_library(AUDIO_LIBRARY cueeditord ${AUDIO_LIBRARY_DIR})

#if (AUDIO_LIBRARY)
#        message(STATUS "CueEditor library is found.")
#endif()

#target_link_directories(traffik PRIVATE ${AUDIO_LIBRARY_DIR})
#target_link_libraries(traffik PRIVATE ${AUDIO_LIBRARY})


set(BASS_INCLUDE_DIR "../lib/BassLib/Include")
set(BASS_LIBRARY_DIR "../lib/BassLib/Lib")
find_library(BASS_LIBRARY bass ${BASS_LIBRARY_DIR})
if (BASS_LIBRARY)
        message(STATUS "Bass library found.")
endif()
target_link_directories(traffik PRIVATE ${BASS_LIBRARY_DIR})
target_link_libraries(traffik PRIVATE ${BASS_LIBRARY})

find_library(DbgHelp_LIBRARY dbghelp ".")
if (DbgHelp_LIBRARY)
    message(STATUS "DbgHelp library found")
endif()
target_link_libraries(traffik PRIVATE ${DbgHelp_LIBRARY})

set(sqlite3_INCLUDE_DIR "../lib/sqlite3/include")
set(sqlite3_LIBRARY_DIR "../lib/sqlite3/lib")

find_library(SQLite3_LIBRARY sqlite3 ${sqlite3_LIBRARY_DIR})
if (SQLite3_LIBRARY)
    message(STATUS "SQLite3 library found.")
endif()

include_directories(${sqlite3_INCLUDE_DIR})
target_link_directories(traffik PRIVATE ${sqlite3_LIBRARY_DIR})
target_link_libraries(traffik PRIVATE ${SQLite3_LIBRARY})


set_target_properties(traffik PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(traffik)
endif()
