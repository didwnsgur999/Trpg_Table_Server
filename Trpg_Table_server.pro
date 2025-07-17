QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Info.cpp \
    backend.cpp \
    chathandler.cpp \
    chatroomui.cpp \
    customer.cpp \
    customerui.cpp \
    debugui.cpp \
    logworker.cpp \
    main.cpp \
    mainwindow.cpp \
    mygraphicsview.cpp \
    order.cpp \
    orderui.cpp \
    product.cpp \
    productui.cpp \
    room.cpp \
    roommanager.cpp \
    serverchat.cpp \
    serveruser.cpp

HEADERS += \
    Info.h \
    backend.h \
    chathandler.h \
    chatroomui.h \
    customer.h \
    customerui.h \
    debugui.h \
    logworker.h \
    mainwindow.h \
    mygraphicsview.h \
    order.h \
    orderui.h \
    product.h \
    productui.h \
    room.h \
    roomitem.h \
    roommanager.h \
    serverchat.h \
    serveruser.h

FORMS += \
    chatroomui.ui \
    customerui.ui \
    debugui.ui \
    mainwindow.ui \
    orderui.ui \
    productui.ui

TRANSLATIONS += \
    Trpg_Table_ko_KR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
