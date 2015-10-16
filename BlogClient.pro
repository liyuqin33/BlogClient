#-------------------------------------------------
#
# Project created by QtCreator 2015-09-17T23:45:37
#
#-------------------------------------------------

QT       += core gui webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlogClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    titlebar.cpp \
    BlogEditArea.cpp \
    blogheadview.cpp \
    CustomFilterModel.cpp \
    customlistwidget.cpp \
    editview.cpp \
    MyUtility.cpp \
    WebCorrespond/article.cpp

HEADERS  += mainwindow.h \
    titlebar.h \
    BlogEditArea.h \
    blogheadview.h \
    CustomFilterModel.h \
    customlistwidget.h \
    editview.h \
    MyUtility.h \
    WebCorrespond/article.h

RESOURCES += \
	BlogClient.qrc

CONFIG += c++11

FORMS += \
    BlogEditArea.ui
