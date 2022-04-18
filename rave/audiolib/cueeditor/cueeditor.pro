QT += core gui widgets

TEMPLATE = lib
DEFINES += CUEEDITOR_LIBRARY

CONFIG(debug, debug|release){
        TARGET = cueeditord
} else {
        TARGET = cueeditor
}

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../audio/audiofile.cpp \
    ../../audio/audiotool.cpp \
    audiographic.cpp \
    audioplayer.cpp \
    audiothread.cpp \
    audiowaveform.cpp \
    cueeditor.cpp \
    subjectobserver.cpp \
    vumeter.cpp

HEADERS += \
    ../../audio/audiofile.h \
    ../../audio/audiotool.h \
    audiographic.h \
    audioplayer.h \
    audiothread.h \
    audiowaveform.h \
    cueeditor_global.h \
    cueeditor.h \
    markindicator.h \
    subjectobserver.h \
    vumeter.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    audiowaveform.ui

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib_debug/ -laudiofile
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib_debug/ -laudiofiled
#else:unix: LIBS += -L$$PWD/../lib_debug/ -laudiofile

#INCLUDEPATH += $$PWD/../lib_debug
#DEPENDPATH += $$PWD/../lib_debug

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/BassLib/Lib/ -lbass
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/BassLib/Lib/ -lbass
else:unix: LIBS += -L$$PWD/../../lib/BassLib/Lib/ -lbass

INCLUDEPATH += $$PWD/../../lib/BassLib/Include
DEPENDPATH += $$PWD/../../lib/BassLib/Lib

RESOURCES += \
    ../../raven.qrc
