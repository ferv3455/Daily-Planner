QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calendar.cpp \
    dayrecord.cpp \
    main.cpp \
    mainwindow.cpp \
    schedule.cpp \
    scheduletimer.cpp \
    settings.cpp \
    trayicon.cpp

HEADERS += \
    calendar.h \
    dayrecord.h \
    mainwindow.h \
    schedule.h \
    scheduletimer.h \
    settings.h \
    trayicon.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Daily-Planner_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
