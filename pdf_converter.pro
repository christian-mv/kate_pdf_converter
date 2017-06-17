
#-------------------------------------------------
#
# Project created by QtCreator 2016-12-19T19:15:32
#
#-------------------------------------------------

QT       += core gui
qtHaveModule(printsupport): QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kate_pdf_converter
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += main.cpp\
        mainwindow.cpp \
    mypixmaptopdfwriter.cpp \
    mypixmapitem.cpp \
    mygraphicsview.cpp \
    qfileinfoandsettings.cpp \
    mydialogsettings.cpp \
    mygeneralsettings.cpp

HEADERS  += mainwindow.h \
    mypixmaptopdfwriter.h \
    mypixmapitem.h \
    mygraphicsview.h \
    qfileinfoandsettings.h \
    mydialogsettings.h \
    mygeneralsettings.h

FORMS    += mainwindow.ui \
    mydialogsettings.ui

RESOURCES += \
    myresources.qrc
