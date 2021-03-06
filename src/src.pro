#-------------------------------------------------
#
# Project created by QtCreator 2020-03-11T17:25:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kylin-scanner
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#CONFIG += c++11
##加载gio库和gio-unix库，用于处理desktop文件
CONFIG        += link_pkgconfig \
                 C++11
PKGCONFIG     += gio-2.0 \
                 gio-unix-2.0 \
                 opencv4
LIBS +=-llept
LIBS +=-ltesseract

SOURCES += \
    funcBar.cpp \
    interruptDlg.cpp \
    kylinCmb.cpp \
    kylinLbl.cpp \
    kylinSane.cpp \
        main.cpp \
    realTimelbl.cpp \
    rectify.cpp \
    scanDisplay.cpp \
    scanSet.cpp \
    sendMail.cpp \
    titleBar.cpp \
    waterMarkDlg.cpp \
        widget.cpp \
    embelish.cpp

HEADERS += \
    funcBar.h \
    interruptDlg.h \
    kylinCmb.h \
    kylinLbl.h \
    kylinLog.h \
    kylinSane.h \
    realTimelbl.h \
    rectify.h \
    scanDisplay.h \
    scanSet.h \
    sendMail.h \
    titleBar.h \
    waterMarkDlg.h \
        widget.h \
    embelish.h

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

FORMS +=

unix:!macx: LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/ -lsane

INCLUDEPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../usr/lib/x86_64-linux-gnu

TRANSLATIONS = translations/kylin-scanner.zh_CN.ts
# !system($$PWD/translations/generate_translations_pm.sh): error("Failed to generate pm")
qm_files.files = translations/*.qm
qm_files.path = /usr/share/kylin-scanner/translations/

# target
target.files += $$TARGET
target.path = /usr/bin/

# icons
icons.files += data/scanner.png
icons.path = /usr/share/pixmaps/

# desktop
desktop.files += data/kylin-scanner.desktop
desktop.path = /usr/share/applications/

INSTALLS += target icons desktop qm_files

DISTFILES += \
    icon/scanner.png \
    translations/generate_translations_pm.sh \
    translations/local.zh_CN.qm

