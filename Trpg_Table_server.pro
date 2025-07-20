QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    mycore/Info.cpp \
    mycore/backend.cpp \
    mycore/chathandler.cpp \
    ui/chatroomui.cpp \
    mycore/customer.cpp \
    ui/customerui.cpp \
    ui/debugui.cpp \
    mycore/logworker.cpp \
    mycore/main.cpp \
    ui/mainwindow.cpp \
    graphics/mygraphicsview.cpp \
    mycore/order.cpp \
    ui/orderui.cpp \
    mycore/product.cpp \
    ui/productui.cpp \
    mycore/room.cpp \
    mycore/roommanager.cpp \
    mycore/serverchat.cpp \
    mycore/serveruser.cpp

HEADERS += \
    mycore/Info.h \
    mycore/backend.h \
    mycore/chathandler.h \
    ui/chatroomui.h \
    mycore/customer.h \
    ui/customerui.h \
    ui/debugui.h \
    mycore/logworker.h \
    ui/mainwindow.h \
    graphics/mygraphicsview.h \
    mycore/order.h \
    ui/orderui.h \
    mycore/product.h \
    ui/productui.h \
    mycore/room.h \
    mycore/roomitem.h \
    mycore/roommanager.h \
    mycore/serverchat.h \
    mycore/serveruser.h

FORMS += \
    ui/chatroomui.ui \
    ui/customerui.ui \
    ui/debugui.ui \
    ui/mainwindow.ui \
    ui/orderui.ui \
    ui/productui.ui

TRANSLATIONS += \
    Trpg_Table_ko_KR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
