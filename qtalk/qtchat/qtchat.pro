QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatwindows.cpp \
    dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    mwin.cpp \
    widget.cpp
    dialog.cpp
    chatwindows.cpp
    widget.cpp


HEADERS += \
    chatwindows.h \
    dialog.h \
    mainwindow.h \
    mwin.h \
    widget.h
    dialog.h
    chatwindows.h
    widget.h


FORMS += \
    chatwindows.ui \
    dialog.ui \
    mainwindow.ui \
    widget.ui
    dialog.ui
    chatwindows.ui
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    CMakeLists.txt.user \
    qtchat.pro.user
