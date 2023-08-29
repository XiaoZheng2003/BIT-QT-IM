QT       += core gui \
            network  \
            sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    add.cpp \
    avatar.cpp \
    chat.cpp \
    dbmanager.cpp \
    edit.cpp \
    emoji.cpp \
    file.cpp \
    filerec.cpp \
    group.cpp \
    heartbeatbroadcaster.cpp \
    heartbeatreceiver.cpp \
    history.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    networktool.cpp \
    threadmanager.cpp

HEADERS += \
    add.h \
    avatar.h \
    chat.h \
    dbmanager.h \
    edit.h \
    emoji.h \
    file.h \
    filerec.h \
    group.h \
    heartbeatbroadcaster.h \
    heartbeatreceiver.h \
    history.h \
    login.h \
    mainwindow.h \
    networktool.h \
    threadmanager.h

FORMS += \
    add.ui \
    avatar.ui \
    chat.ui \
    edit.ui \
    emoji.ui \
    file.ui \
    filerec.ui \
    group.ui \
    history.ui \
    login.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
