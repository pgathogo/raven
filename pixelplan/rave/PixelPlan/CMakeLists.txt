cmake_minimum_required(VERSION 3.20)

string(REGEX REPLACE "/W[3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")

set(MSVS_BUILD_NUM "14.42.34433")

set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/${MSVS_BUILD_NUM}/bin/Hostx64/x64/cl.exe")

project(pixelplan VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(rave_framework_INCLUDE_DIRS "../../../rave/framework")
set(rave_security_INCLUDE_DIRS "../../../rave/security")
set(rave_audio_INCLUDE_DIRS "../../../rave/audio")
#set(rave_audiolib_INCLUDE_DIRS "../audiolib/headers")

set(Bass_INCLUDE_DIRS "../../../rave/lib/BassLib/Include")

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

set(RAVE_DIR "../../../rave")
set(AUDIO_LIB_DIR "../../../rave/audio")
set(RAVE_FRAMEWORK_DIR "../../../rave/framework")
set(RAVE_SECURITY_DIR "../../../rave/security")
set(UTILS_DIR "../../../rave/utils")
set(RAVE_CLUSTER_DIR "../../../ravecluster/rave/clusterman")


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
    tvprogrambrowser.h
    tvprogrambrowser.cpp
    tvprogrambrowser.ui
    tvprogram.h
    tvprogram.cpp
    tvprogramform.h
    tvprogramform.cpp
    tvprogramform.ui

    ${AUDIO_LIB_DIR}/artist.h
    ${AUDIO_LIB_DIR}/audio.h
    ${AUDIO_LIB_DIR}/audiofile.h
    ${AUDIO_LIB_DIR}/audiofile.cpp
    ${AUDIO_LIB_DIR}/audiotool.h
    ${AUDIO_LIB_DIR}/audiotool.cpp
    ${AUDIO_LIB_DIR}/artist.cpp
    ${AUDIO_LIB_DIR}/audio.cpp
    ${AUDIO_LIB_DIR}/audiometer.h
    ${AUDIO_LIB_DIR}/audiometer.cpp
    ${AUDIO_LIB_DIR}/audioconverter.h
    ${AUDIO_LIB_DIR}/audioconverter.cpp
    ${AUDIO_LIB_DIR}/mp3oggconverter.h
    ${AUDIO_LIB_DIR}/mp3oggconverter.cpp
    ${AUDIO_LIB_DIR}/oggtooggconverter.h
    ${AUDIO_LIB_DIR}/oggtooggconverter.cpp
    ${AUDIO_LIB_DIR}/mtstomp3converter.h
    ${AUDIO_LIB_DIR}/mtstomp3converter.cpp
    ${AUDIO_LIB_DIR}/audiowaveformgenerator.h
    ${AUDIO_LIB_DIR}/audiowaveformgenerator.cpp
    ${AUDIO_LIB_DIR}/audiofileinfo.h
    ${AUDIO_LIB_DIR}/audiofileinfo.cpp

    ${AUDIO_LIB_DIR}/editor/audiographic.cpp
    ${AUDIO_LIB_DIR}/editor/audiographic.h
    ${AUDIO_LIB_DIR}/editor/audioplayer.h
    ${AUDIO_LIB_DIR}/editor/audioplayer.cpp
    ${AUDIO_LIB_DIR}/editor/audiothread.h
    ${AUDIO_LIB_DIR}/editor/audiothread.cpp
    ${AUDIO_LIB_DIR}/editor/audiowaveform.h
    ${AUDIO_LIB_DIR}/editor/audiowaveform.cpp
    ${AUDIO_LIB_DIR}/editor/subjectobserver.cpp
    ${AUDIO_LIB_DIR}/editor/vumeter.h
    ${AUDIO_LIB_DIR}/editor/vumeter.cpp
    ${AUDIO_LIB_DIR}/editor/markindicator.h
    ${AUDIO_LIB_DIR}/editor/subjectobserver.h

    ${AUDIO_LIB_DIR}/trackbrowser.h
    ${AUDIO_LIB_DIR}/trackbrowser.cpp
    ${AUDIO_LIB_DIR}/trackpickerdialog.h
    ${AUDIO_LIB_DIR}/trackpickerdialog.cpp
    ${AUDIO_LIB_DIR}/audiodatamodel.h
    ${AUDIO_LIB_DIR}/audiodatamodel.cpp
    ${AUDIO_LIB_DIR}/audiofolderwidget.h
    ${AUDIO_LIB_DIR}/audiofolderwidget.cpp
    ${AUDIO_LIB_DIR}/audiosearchwidget.h
    ${AUDIO_LIB_DIR}/audiosearchwidget.cpp
    ${AUDIO_LIB_DIR}/audiolibwidget.h
    ${AUDIO_LIB_DIR}/audiolibwidget.cpp
    ${AUDIO_LIB_DIR}/audiotrackwidgettoolbar.h
    ${AUDIO_LIB_DIR}/audiotrackwidgettoolbar.cpp
    ${AUDIO_LIB_DIR}/audiotrackwidget.h
    ${AUDIO_LIB_DIR}/audiotrackwidget.cpp
    ${AUDIO_LIB_DIR}/audiotrackviewer.h
    ${AUDIO_LIB_DIR}/audiotrackviewer.cpp
    ${AUDIO_LIB_DIR}/audiolibitem.h
    ${AUDIO_LIB_DIR}/audiolibitem.cpp
    ${AUDIO_LIB_DIR}/audiolibrary.h
    ${AUDIO_LIB_DIR}/audiolibrary.cpp
    ${AUDIO_LIB_DIR}/audiofolder.h
    ${AUDIO_LIB_DIR}/audiofolder.cpp

    ${RAVE_DIR}/AudioExplorer/audioform.h
    ${RAVE_DIR}/AudioExplorer/audioform.cpp

    ${RAVE_FRAMEWORK_DIR}/tree.h
    ${RAVE_FRAMEWORK_DIR}/tree.cpp
    ${RAVE_FRAMEWORK_DIR}/treeviewmodel.h
    ${RAVE_FRAMEWORK_DIR}/treeviewmodel.cpp


    ${RAVE_FRAMEWORK_DIR}/ravensetup.h
    ${RAVE_FRAMEWORK_DIR}/ravensetup.cpp
    ${RAVE_FRAMEWORK_DIR}/entityregister.h
    ${RAVE_FRAMEWORK_DIR}/schedule.h
    ${RAVE_FRAMEWORK_DIR}/schedule.cpp
    ${RAVE_FRAMEWORK_DIR}/relationmapper.h
    ${RAVE_FRAMEWORK_DIR}/relationmapper.cpp
    ${RAVE_FRAMEWORK_DIR}/entitydatamodel.h
    ${RAVE_FRAMEWORK_DIR}/entitydatamodel.cpp
    ${RAVE_FRAMEWORK_DIR}/baseentity.cpp
    ${RAVE_FRAMEWORK_DIR}/baseentitybrowserdlg.cpp
    ${RAVE_FRAMEWORK_DIR}/baseentitydetaildlg.cpp
    ${RAVE_FRAMEWORK_DIR}/baseentitydetaildlg.ui
    ${RAVE_FRAMEWORK_DIR}/databaseconnector.cpp
    ${RAVE_FRAMEWORK_DIR}/databasemanager.cpp
    ${RAVE_FRAMEWORK_DIR}/entityfields.h
    ${RAVE_FRAMEWORK_DIR}/entityfields.cpp
    ${RAVE_FRAMEWORK_DIR}/queryset.h
    ${RAVE_FRAMEWORK_DIR}/dataprovider.h
    ${RAVE_FRAMEWORK_DIR}/dataprovider.cpp
    ${RAVE_FRAMEWORK_DIR}/ravenexception.h
    ${RAVE_FRAMEWORK_DIR}/ravenexception.cpp
    ${RAVE_FRAMEWORK_DIR}/letterfilterwidget.cpp
    ${RAVE_FRAMEWORK_DIR}/letterfilterwidget.h
    ${RAVE_FRAMEWORK_DIR}/letterfilterwidget.ui
    ${RAVE_FRAMEWORK_DIR}/choicefield.h
    ${RAVE_FRAMEWORK_DIR}/valuelist.h
    ${RAVE_FRAMEWORK_DIR}/valuelist.cpp
    ${RAVE_FRAMEWORK_DIR}/manytomany.h
    ${RAVE_FRAMEWORK_DIR}/manytomany.cpp
    ${RAVE_FRAMEWORK_DIR}/typeexclusion.h
    ${RAVE_FRAMEWORK_DIR}/typeexclusion.cpp
    ${RAVE_FRAMEWORK_DIR}/manytomanybrowser.h
    ${RAVE_FRAMEWORK_DIR}/manytomanybrowser.cpp
    ${RAVE_FRAMEWORK_DIR}/manytomanybrowser.ui
    ${RAVE_FRAMEWORK_DIR}/picklistbrowser.h
    ${RAVE_FRAMEWORK_DIR}/picklistbrowser.cpp
    ${RAVE_FRAMEWORK_DIR}/picklistbrowser.ui
    ${RAVE_FRAMEWORK_DIR}/selectclosewidget.h
    ${RAVE_FRAMEWORK_DIR}/selectclosewidget.cpp
    ${RAVE_FRAMEWORK_DIR}/selectclosewidget.ui
    ${RAVE_FRAMEWORK_DIR}/valuelistbrowser.h
    ${RAVE_FRAMEWORK_DIR}/valuelistbrowser.cpp
    ${RAVE_FRAMEWORK_DIR}/valuelistbrowser.ui
    ${RAVE_FRAMEWORK_DIR}/valuelistdetaildlg.h
    ${RAVE_FRAMEWORK_DIR}/valuelistdetaildlg.cpp
    ${RAVE_FRAMEWORK_DIR}/valuelistdetaildlg.ui
    ${RAVE_FRAMEWORK_DIR}/applicationcontext.h
    ${RAVE_FRAMEWORK_DIR}/applicationcontext.cpp
    ${RAVE_FRAMEWORK_DIR}/datetimeselector.h
    ${RAVE_FRAMEWORK_DIR}/datetimeselector.cpp
    ${RAVE_FRAMEWORK_DIR}/comboboxitemdelegate.h
    ${RAVE_FRAMEWORK_DIR}/comboboxitemdelegate.cpp
    ${RAVE_FRAMEWORK_DIR}/readonlydelegate.h
    ${RAVE_FRAMEWORK_DIR}/readonlydelegate.cpp
    ${RAVE_FRAMEWORK_DIR}/spinboxdelegate.h
    ${RAVE_FRAMEWORK_DIR}/spinboxdelegate.cpp
    ${RAVE_FRAMEWORK_DIR}/timeeditdelegate.h
    ${RAVE_FRAMEWORK_DIR}/timeeditdelegate.cpp
    ${RAVE_FRAMEWORK_DIR}/logger.h
    ${RAVE_FRAMEWORK_DIR}/logger.cpp
    ${RAVE_FRAMEWORK_DIR}/dowwidget.h
    ${RAVE_FRAMEWORK_DIR}/dowwidget.cpp
    traffik.qrc

    ${RAVE_SECURITY_DIR}/loginform.h
    ${RAVE_SECURITY_DIR}/loginform.cpp
    ${RAVE_SECURITY_DIR}/loginform.ui
    ${RAVE_SECURITY_DIR}/accesscontroller.h
    ${RAVE_SECURITY_DIR}/authentication.h
    ${RAVE_SECURITY_DIR}/accesscontroller.cpp
    ${RAVE_SECURITY_DIR}/authentication.cpp
    ${RAVE_SECURITY_DIR}/baserole.h
    ${RAVE_SECURITY_DIR}/baserole.cpp
    ${RAVE_SECURITY_DIR}/user.h
    ${RAVE_SECURITY_DIR}/user.cpp
    ${RAVE_SECURITY_DIR}/userbrowser.h
    ${RAVE_SECURITY_DIR}/userbrowser.cpp
    ${RAVE_SECURITY_DIR}/userform.h
    ${RAVE_SECURITY_DIR}/userform.cpp
    ${RAVE_SECURITY_DIR}/contentauthbrowser.cpp
    ${RAVE_SECURITY_DIR}/contentauthbrowser.h
    ${RAVE_SECURITY_DIR}/contentauthbrowser.ui
    ${RAVE_SECURITY_DIR}/contentauthform.h
    ${RAVE_SECURITY_DIR}/contentauthform.cpp
    ${RAVE_SECURITY_DIR}/contentauthform.ui
    ${RAVE_SECURITY_DIR}/contentauth.cpp
    ${RAVE_SECURITY_DIR}/contentauth.h
    ${RAVE_SECURITY_DIR}/role.h
    ${RAVE_SECURITY_DIR}/role.cpp
    ${RAVE_SECURITY_DIR}/rolemember.h
    ${RAVE_SECURITY_DIR}/rolemember.cpp
    ${RAVE_SECURITY_DIR}/content.h
    ${RAVE_SECURITY_DIR}/content.cpp
    ${RAVE_SECURITY_DIR}/contentbrowser.h
    ${RAVE_SECURITY_DIR}/contentbrowser.cpp
    ${RAVE_SECURITY_DIR}/contentbrowser.ui
    ${RAVE_SECURITY_DIR}/contentform.h
    ${RAVE_SECURITY_DIR}/contentform.cpp
    ${RAVE_SECURITY_DIR}/contentform.ui
    ${RAVE_SECURITY_DIR}/contentdetailform.h
    ${RAVE_SECURITY_DIR}/contentdetailform.cpp
    ${RAVE_SECURITY_DIR}/contentdetailform.ui
    ${RAVE_SECURITY_DIR}/rolebrowser.h
    ${RAVE_SECURITY_DIR}/rolebrowser.cpp
    ${RAVE_SECURITY_DIR}/rolebrowser.ui
    ${RAVE_SECURITY_DIR}/roleform.h
    ${RAVE_SECURITY_DIR}/roleform.cpp
    ${RAVE_SECURITY_DIR}/roleform.ui
    ${RAVE_SECURITY_DIR}/loginform.h
    ${RAVE_SECURITY_DIR}/loginform.cpp
    ${RAVE_SECURITY_DIR}/selectstationform.h
    ${RAVE_SECURITY_DIR}/selectstationform.cpp
    ${RAVE_SECURITY_DIR}/userconfig.h
    ${RAVE_SECURITY_DIR}/userconfig.cpp
    ${RAVE_DIR}/rave.qrc

    ${UTILS_DIR}/notificationbar.h
    ${UTILS_DIR}/notificationitem.h
    ${UTILS_DIR}/notificationbar.cpp
    ${UTILS_DIR}/notificationitem.cpp
    ${UTILS_DIR}/notificationitem.ui
    ${UTILS_DIR}/daypartgrid.h
    ${UTILS_DIR}/daypartgrid.cpp
    ${UTILS_DIR}/daypartgrid.ui
    ${UTILS_DIR}/qchecklist.h
    ${UTILS_DIR}/tools.h
    ${UTILS_DIR}/togglebutton.h
    ${UTILS_DIR}/togglebutton.cpp
    ${UTILS_DIR}/cachehandler.h
    ${UTILS_DIR}/cachehandler.cpp

    ${RAVE_CLUSTER_DIR}/clustercontroller.h
    ${RAVE_CLUSTER_DIR}/clustercontroller.cpp
    ${RAVE_CLUSTER_DIR}/server.h
    ${RAVE_CLUSTER_DIR}/server.cpp
    ${RAVE_CLUSTER_DIR}/station.h
    ${RAVE_CLUSTER_DIR}/station.cpp
    ${RAVE_CLUSTER_DIR}/cluster.h
    ${RAVE_CLUSTER_DIR}/cluster.cpp

    ${RAVE_DIR}/AudioExplorer/audioexplorer.qrc

)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pixelplan
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${RAVE_DIR}/AudioExplorer/audioexplorer.qrc
        tvprogramform.h tvprogramform.cpp tvprogramform.ui
        tvprogrambrowser.h tvprogrambrowser.cpp tvprogrambrowser.ui
    )
else()
    if(ANDROID)
        add_library(pixelplan SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(pixelplan
            ${PROJECT_SOURCES}
        )
    endif()
endif()

if (MSVC)
    target_compile_options(pixelplan PRIVATE "/W0")
    target_compile_options(pixelplan PUBLIC "/wd4716")
    target_compile_options(pixelplan PUBLIC "/Zc:__cplusplus")
    target_compile_options(pixelplan PUBLIC "/EHsc")
 else()
     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type")
 endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    #target_compile_options(pixelplan PRIVATE "/MD")
    set_property(TARGET pixelplan PROPERTY
      MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    #target_compile_options(traffik PRIVATE "/MT")
    set_property(TARGET pixelplan PROPERTY
      MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Releae>:Release>")
endif()

#target_link_libraries(pixelplan PRIVATE dbghelp.dll)

#set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")
#target_link_libraries(pixelplan PRIVATE -static-libstdc++)
target_compile_options(pixelplan PRIVATE /MT)
target_link_options(pixelplan PRIVATE /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)

target_link_libraries(pixelplan PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


target_link_libraries(pixelplan PRIVATE ${PostgreSQL_LIBRARIES})

target_include_directories(pixelplan PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(pixelplan PRIVATE ${rave_framework_INCLUDE_DIRS})

#target_link_libraries(pixelplan PRIVATE ${LIMEREPORT_LIBRARIES})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})

#link_directories(${AUDIO_LIBRARY_DIR})

#set(AUDIO_INCLUDE_DIR "../audiolib/headers")
#set(AUDIO_LIBRARY_DIR "../audiolib/lib_debug")
#find_library(AUDIO_LIBRARY cueeditord ${AUDIO_LIBRARY_DIR})

#if (AUDIO_LIBRARY)
#        message(STATUS "CueEditor library is found.")
#endif()

#target_link_directories(PixelPlan PRIVATE ${AUDIO_LIBRARY_DIR})
#target_link_libraries(PixelPlan PRIVATE ${AUDIO_LIBRARY})


set(BASS_INCLUDE_DIR "../../../rave/lib/BassLib/Include")
set(BASS_LIBRARY_DIR "../../../rave/lib/BassLib/Lib")
find_library(BASS_LIBRARY bass ${BASS_LIBRARY_DIR})
if (BASS_LIBRARY)
        message(STATUS "Bass library found.")
endif()
target_link_directories(pixelplan PRIVATE ${BASS_LIBRARY_DIR})
target_link_libraries(pixelplan PRIVATE ${BASS_LIBRARY})

find_library(DbgHelp_LIBRARY dbghelp ".")
if (DbgHelp_LIBRARY)
    message(STATUS "DbgHelp library found")
endif()
target_link_libraries(pixelplan PRIVATE ${DbgHelp_LIBRARY})

set(sqlite3_INCLUDE_DIR "../../../rave/lib/sqlite3/include")
set(sqlite3_LIBRARY_DIR "../../../rave/lib/sqlite3/lib")

find_library(SQLite3_LIBRARY sqlite3 ${sqlite3_LIBRARY_DIR})
if (SQLite3_LIBRARY)
    message(STATUS "SQLite3 library found.")
endif()

include_directories(${sqlite3_INCLUDE_DIR})
target_link_directories(pixelplan PRIVATE ${sqlite3_LIBRARY_DIR})
target_link_libraries(pixelplan PRIVATE ${SQLite3_LIBRARY})


set_target_properties(pixelplan PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pixelplan)
endif()
