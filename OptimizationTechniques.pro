#-------------------------------------------------
#
# Project created by QtCreator 2014-05-01T12:07:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OptimizationTechniques
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    factor.cpp \
    mydelegat.cpp \
    tablewidget.cpp \
    symplextable.cpp \
    regexpstring.cpp \
    solution.cpp

HEADERS  += mainwindow.h \
    factor.h \
    mydelegat.h \
    tablewidget.h \
    symplextable.h \
    regexpstring.h \
    solution.h

FORMS    += mainwindow.ui \
    solution.ui

RESOURCES += \
    icons.qrc
RC_FILE = AppIcon.rc
OTHER_FILES += \
    AppIcon.rc \
    AppIcon.rc
