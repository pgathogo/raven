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

SOURCES += \
    ../framework/baseentity.cpp \
    ../framework/baseentitybrowserdlg.cpp \
    ../framework/baseentitydetaildlg.cpp \
    ../framework/databasemanager.cpp \
    ../framework/dataprovider.cpp \
    ../framework/entitydatamodel.cpp \
    ../framework/entityfields.cpp \
    clientgroup.cpp \
    clientgroupdetaildlg.cpp \
    clientgroupdlg.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../framework/baseentity.h \
    ../framework/baseentitybrowserdlg.h \
    ../framework/baseentitydetaildlg.h \
    ../framework/databasemanager.h \
    ../framework/dataprovider.h \
    ../framework/entitydatamodel.h \
    ../framework/entityfields.h \
    ../framework/queryset.h \
    ../utils/tools.h \
    ../utils/types.h \
    clientgroup.h \
    clientgroupdetaildlg.h \
    clientgroupdlg.h \
    mainwindow.h

FORMS += \
    ../framework/baseentitybrowserdlg.ui \
    ../framework/baseentitydetaildlg.ui \
    clientgroupdetaildlg.ui \
    clientgroupdlg.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

#win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files/PostgreSQL/9.5/lib/' -llibpq
win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files/PostgreSQL/9.5/lib/' -llibpq
else:unix: LIBS += -L'C:/Program Files/PostgreSQL/9.5/lib/' -llibpq

INCLUDEPATH += 'C:/Program Files/PostgreSQL/9.5/include'
DEPENDPATH += 'C:/Program Files/PostgreSQL/9.5/include'

#INCLUDEPATH += 'D:/home/PMS/Raven/rave/lib/fmt'
#DEFINES += FMT_HEADER_ONLY
