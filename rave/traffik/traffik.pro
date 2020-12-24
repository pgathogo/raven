QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../raven.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    traffik.qrc

#win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files/PostgreSQL/9.5/lib/' -llibpq
win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files/PostgreSQL/9.5/lib/' -llibpq
else:unix: LIBS += -L'C:/Program Files/PostgreSQL/9.5/lib/' -llibpq

INCLUDEPATH += 'C:/Program Files/PostgreSQL/9.5/include'
DEPENDPATH += 'C:/Program Files/PostgreSQL/9.5/include'

#INCLUDEPATH += 'D:/home/PMS/Raven/rave/lib/fmt'
#DEFINES += FMT_HEADER_ONLY

HEADERS += \
    ../framework/entityregister.h \ \
    ../security/accesscontroller.h \
    ../security/authentication.h \
    approverform.h \
    bookingsegment.h \
    bookingwizard.h \
    breakcreateform.h \
    breaklayout.h \
    breaklayoutbrowser.h \
    breaklayoutform.h \
    breaklayoutline.h \
    order.h \
    orderapprover.h \
    orderbooking.h \
    orderbookingbrowser.h \
    orderbrowser.h \
    orderform.h \
    orderpackage.h \
    schedule.h \
    scheduleform.h \
    setupform.h \
    spotaudio.h \
    traffiksetup.h \
    tree.h \
    treeviewmodel.h

SOURCES += \
    ../security/accesscontroller.cpp \
    ../security/authentication.cpp \
    approverform.cpp \
    bookingsegment.cpp \
    bookingwizard.cpp \
    breakcreateform.cpp \
    breaklayout.cpp \
    breaklayoutbrowser.cpp \
    breaklayoutform.cpp \
    breaklayoutline.cpp \
    order.cpp \
    orderapprover.cpp \
    orderbooking.cpp \
    orderbookingbrowser.cpp \
    orderbrowser.cpp \
    orderform.cpp \
    orderpackage.cpp \
    schedule.cpp \
    scheduleform.cpp \
    setupform.cpp \
    spotaudio.cpp \
    traffiksetup.cpp \
    tree.cpp \
    treeviewmodel.cpp

FORMS += \
    approverform.ui \
    bookingwizard.ui \
    breakcreateform.ui \
    breaklayoutbrowser.ui \
    breaklayoutform.ui \
    orderbookingbrowser.ui \
    orderbrowser.ui \
    orderform.ui \
    scheduleform.ui \
    setupform.ui
