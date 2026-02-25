cmake_minimum_required(VERSION 3.20)

string(REGEX REPLACE "/W[3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")

set(MSVS_BUILD_NUM "14.44.35207")

set(CMAKE_CXX_COMPILER "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/${MSVS_BUILD_NUM}/bin/Hostx64/x64/cl.exe")


if (CMAKE_GENERATOR MATCHES "Ninja")
    file(
        WRITE "${CMAKE_BINARY_DIR}/GNUMakeRulesOverwrite.cmake"
            "STRING(REPLACE \"-MD\" \"-MMD\" CMAKE_DEPFILE_FLAGS_C \"\${CMAKE_DEPFILE_FLAGS_C}\")\n"
            "STRING(REPLACE \"-MD\" \"-MMD\" CMAKE_DEPFILE_FLAGS_CXX \"\${CMAKE_DEPFILE_FLAGS_CXX}\")\n"
    )
    set(CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_BINARY_DIR}/GNUMakeRulesOverwrite.cmake" CACHE INTERNAL "")
endif()


project(pixelplan VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(rave_framework_INCLUDE_DIRS "../../../rave/framework")
set(rave_security_INCLUDE_DIRS "../../../rave/security")
set(rave_audio_INCLUDE_DIRS "../../../rave/audio")
#set(rave_audiolib_INCLUDE_DIRS "../audiolib/headers")

set(Bass_INCLUDE_DIRS "../../../rave/lib/BassLib/Include")

set(postgreSQL_ROOT "C:/Program Files/PostgreSQL/14")
find_package(PostgreSQL REQUIRED)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets xml)

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
set(QXLSX_DIR "third_party/QXlsx/QXlsx")

# Ninja built-in workaround for Windows timestamp issues
#set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E touch_nocreate <TARGET>")

if(WIN32)
    set(CMAKE_CL_SHOWINCLUDES_PREFIX "Note: including file: ")
    set(CMAKE_DEPFILE_FLAGS_C "/showIncludes")
    set(CMAKE_DEPFILE_FLAGS_CXX "/showIncludes")
endif()

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
    bookedcommercialtvm.h
    bookedcommercialtvm.cpp
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
    advertmedia.h
    advertmedia.cpp
    tvprogramform.h
    tvprogramform.cpp
    tvprogramform.ui
    tvprogrambrowser.h
    tvprogrambrowser.cpp
    tvprogrambrowser.ui
    advertmediaform.h
    advertmediaform.cpp
    advertmediaform.ui
    mediabrowser.h
    mediabrowser.cpp
    mediabrowser.ui
    playlistform.h
    playlistform.cpp
    playlistform.ui
    playlist.h
    playlist.cpp
    commloganalyzer.h
    commloganalyzer.cpp
    commloganalyzer.ui
    configmanager.h
    configmanager.cpp
    reasonvoidbooking.h
    reasonvoidbooking.cpp
    voidreasonbrowser.h
    voidreasonbrowser.cpp
    voidreasonbrowser.ui
    voidbookingform.h
    voidbookingform.cpp
    ctsgenerator.h
    ctsgenerator.cpp
    formatselectorwidget.h
    formatselectorwidget.cpp

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
    ${RAVE_CLUSTER_DIR}/useraccess.h
    ${RAVE_CLUSTER_DIR}/useraccess.cpp

    ${RAVE_DIR}/AudioExplorer/audioexplorer.qrc

    ${QXLSX_DIR}/header/xlsxabstractooxmlfile.h
    ${QXLSX_DIR}/header/xlsxabstractooxmlfile_p.h
    ${QXLSX_DIR}/header/xlsxabstractsheet.h
    ${QXLSX_DIR}/header/xlsxabstractsheet_p.h
    ${QXLSX_DIR}/header/xlsxcell.h
    ${QXLSX_DIR}/header/xlsxcell_p.h
    ${QXLSX_DIR}/header/xlsxcellformula.h
    ${QXLSX_DIR}/header/xlsxcellformula_p.h
    ${QXLSX_DIR}/header/xlsxcelllocation.h
    ${QXLSX_DIR}/header/xlsxcellrange.h
    ${QXLSX_DIR}/header/xlsxcellreference.h
    ${QXLSX_DIR}/header/xlsxchart.h
    ${QXLSX_DIR}/header/xlsxchart_p.h
    ${QXLSX_DIR}/header/xlsxchartsheet.h
    ${QXLSX_DIR}/header/xlsxchartsheet_p.h
    ${QXLSX_DIR}/header/xlsxcolor_p.h
    ${QXLSX_DIR}/header/xlsxconditionalformatting.h
    ${QXLSX_DIR}/header/xlsxconditionalformatting_p.h
    ${QXLSX_DIR}/header/xlsxcontenttypes_p.h
    ${QXLSX_DIR}/header/xlsxdatavalidation.h
    ${QXLSX_DIR}/header/xlsxdatavalidation_p.h
    ${QXLSX_DIR}/header/xlsxdatetype.h
    ${QXLSX_DIR}/header/xlsxdocpropsapp_p.h
    ${QXLSX_DIR}/header/xlsxdocpropscore_p.h
    ${QXLSX_DIR}/header/xlsxdocument.h
    ${QXLSX_DIR}/header/xlsxdocument_p.h
    ${QXLSX_DIR}/header/xlsxdrawing_p.h
    ${QXLSX_DIR}/header/xlsxdrawinganchor_p.h
    ${QXLSX_DIR}/header/xlsxformat.h
    ${QXLSX_DIR}/header/xlsxformat_p.h
    ${QXLSX_DIR}/header/xlsxglobal.h
    ${QXLSX_DIR}/header/xlsxmediafile_p.h
    ${QXLSX_DIR}/header/xlsxnumformatparser_p.h
    ${QXLSX_DIR}/header/xlsxreadsax.h
    ${QXLSX_DIR}/header/xlsxrelationships_p.h
    ${QXLSX_DIR}/header/xlsxrichstring.h
    ${QXLSX_DIR}/header/xlsxrichstring_p.h
    ${QXLSX_DIR}/header/xlsxsharedstrings_p.h
    ${QXLSX_DIR}/header/xlsxsimpleooxmlfile_p.h
    ${QXLSX_DIR}/header/xlsxstyles_p.h
    ${QXLSX_DIR}/header/xlsxtheme_p.h
    ${QXLSX_DIR}/header/xlsxutility_p.h
    ${QXLSX_DIR}/header/xlsxworkbook.h
    ${QXLSX_DIR}/header/xlsxworkbook_p.h
    ${QXLSX_DIR}/header/xlsxworksheet.h
    ${QXLSX_DIR}/header/xlsxworksheet_p.h
    ${QXLSX_DIR}/header/xlsxzipreader_p.h
    ${QXLSX_DIR}/header/xlsxzipwriter_p.h

    ${QXLSX_DIR}/source/xlsxabstractooxmlfile.cpp
    ${QXLSX_DIR}/source/xlsxabstractsheet.cpp
    ${QXLSX_DIR}/source/xlsxcell.cpp
    ${QXLSX_DIR}/source/xlsxcellformula.cpp
    ${QXLSX_DIR}/source/xlsxcelllocation.cpp
    ${QXLSX_DIR}/source/xlsxcellrange.cpp
    ${QXLSX_DIR}/source/xlsxcellreference.cpp
    ${QXLSX_DIR}/source/xlsxchart.cpp
    ${QXLSX_DIR}/source/xlsxchartsheet.cpp
    ${QXLSX_DIR}/source/xlsxcolor.cpp
    ${QXLSX_DIR}/source/xlsxconditionalformatting.cpp
    ${QXLSX_DIR}/source/xlsxcontenttypes.cpp
    ${QXLSX_DIR}/source/xlsxdatavalidation.cpp
    ${QXLSX_DIR}/source/xlsxdatetype.cpp
    ${QXLSX_DIR}/source/xlsxdocpropsapp.cpp
    ${QXLSX_DIR}/source/xlsxdocpropscore.cpp
    ${QXLSX_DIR}/source/xlsxdocument.cpp
    ${QXLSX_DIR}/source/xlsxdrawing.cpp
    ${QXLSX_DIR}/source/xlsxdrawinganchor.cpp
    ${QXLSX_DIR}/source/xlsxformat.cpp
    ${QXLSX_DIR}/source/xlsxmediafile.cpp
    ${QXLSX_DIR}/source/xlsxnumformatparser.cpp
    ${QXLSX_DIR}/source/xlsxreadsax.cpp
    ${QXLSX_DIR}/source/xlsxrelationships.cpp
    ${QXLSX_DIR}/source/xlsxrichstring.cpp
    ${QXLSX_DIR}/source/xlsxsharedstrings.cpp
    ${QXLSX_DIR}/source/xlsxsimpleooxmlfile.cpp
    ${QXLSX_DIR}/source/xlsxstyles.cpp
    ${QXLSX_DIR}/source/xlsxtheme.cpp
    ${QXLSX_DIR}/source/xlsxutility.cpp
    ${QXLSX_DIR}/source/xlsxworkbook.cpp
    ${QXLSX_DIR}/source/xlsxworksheet.cpp
    ${QXLSX_DIR}/source/xlsxzipreader.cpp
    ${QXLSX_DIR}/source/xlsxzipwriter.cpp

)

#playlistform.h playlistform.cpp playlistform.ui

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set(RC_FILE "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")
    set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/ravenicon64.ico")

    # Verify files exists
    if(NOT EXISTS ${APP_ICON})
       message(WARNING "Icon file not found! ${APP_ICON}")
    endif()

    if (NOT EXISTS ${RC_FILE})
       message(WARNING "RC file not found! ${RC_FILE}")
    endif()

    list(APPEND PROJECT_SOURCES ${RC_FILE})

endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pixelplan
        WIN32          # Critical: Tells Windows it's a GUI app
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        voidreasonbrowser.h voidreasonbrowser.cpp voidreasonbrowser.ui
        voidbookingreasonform.h voidbookingreasonform.cpp voidbookingreasonform.ui
    )
else()
    if(ANDROID)
        add_library(pixelplan SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(pixelplan
           WIN32
            ${PROJECT_SOURCES}
            ${RC_FILE}
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

target_link_libraries(pixelplan PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Xml)


target_link_libraries(pixelplan PRIVATE ${PostgreSQL_LIBRARIES})

target_include_directories(pixelplan PRIVATE ${postgreSQL_INCLUDE_DIRS})
target_include_directories(pixelplan PRIVATE ${rave_framework_INCLUDE_DIRS})

#target_link_libraries(pixelplan PRIVATE ${LIMEREPORT_LIBRARIES})

link_directories(${rave_framework_INCLUDE_DIRS})
link_directories(${rave_security_INCLUDE_DIRS})

find_package(Qt6 COMPONENTS Multimedia MultimediaWidgets REQUIRED)
target_link_libraries(pixelplan PUBLIC Qt::Multimedia Qt::MultimediaWidgets)

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

add_subdirectory("third_party/QXlsx/QXlsx")
target_link_libraries(pixelplan PRIVATE QXlsx::QXlsx)

set_target_properties(pixelplan PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pixelplan)
endif()

