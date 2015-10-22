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
    WebCorrespond/article.cpp \
    BlogEditArea/BlogEditArea.cpp \
    BlogEditArea/blogheadview.cpp \
    BlogEditArea/CustomFilterModel.cpp \
    BlogEditArea/customlistwidget.cpp \
    BlogEditArea/editview.cpp \
    BlogEditArea/MyUtility.cpp \
    MessageWidget/itemwidget.cpp \
    MessageWidget/piclabel.cpp \
    MessageWidget/statewidget.cpp \
    MessageWidget/widget.cpp \
    Login/login.cpp

HEADERS  += \
    WebCorrespond/article.h \
    BlogEditArea/BlogEditArea.h \
    BlogEditArea/blogheadview.h \
    BlogEditArea/CustomFilterModel.h \
    BlogEditArea/customlistwidget.h \
    BlogEditArea/editview.h \
    BlogEditArea/MyUtility.h \
    MessageWidget/itemwidget.h \
    MessageWidget/piclabel.h \
    MessageWidget/statewidget.h \
    MessageWidget/widget.h \
    Login/login.h

RESOURCES += \
        BlogClient.qrc


CONFIG += c++11

FORMS += \
    BlogEditArea/BlogEditArea.ui \
    Login/login.ui
