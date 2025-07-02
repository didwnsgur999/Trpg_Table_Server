QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Info.cpp \
    Product.cpp \
    backend.cpp \
    customer.cpp \
    main.cpp \
    mainwindow.cpp \
    order.cpp \
    room.cpp \
    roommanager.cpp \
    serverchat.cpp

HEADERS += \
    Info.h \
    Product.h \
    backend.h \
    customer.h \
    mainwindow.h \
    order.h \
    room.h \
    roommanager.h \
    serverchat.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Trpg_Table_ko_KR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
