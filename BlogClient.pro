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
    Login/login.cpp \
    Login/tray.cpp \
	Login/accountitem.cpp \
	Login/nofocusframedelegate.cpp \
	Base/MyUtility.cpp \
    MessageWidget/itemwidget.cpp \
    MessageWidget/messagedialog.cpp \
    MessageWidget/piclabel.cpp \
    MessageWidget/statewidget.cpp \
    MessageWidget/widget.cpp

HEADERS  += \
    WebCorrespond/article.h \
    BlogEditArea/BlogEditArea.h \
    BlogEditArea/blogheadview.h \
    BlogEditArea/CustomFilterModel.h \
    BlogEditArea/customlistwidget.h \
    BlogEditArea/editview.h \
    Login/login.h \
    Login/tray.h \
    Login/accountitem.h \
    Login/nofocusframedelegate.h \
    Base/MyUtility.h \
    MessageWidget/itemwidget.h \
    MessageWidget/message.h \
    MessageWidget/messagedialog.h \
    MessageWidget/piclabel.h \
    MessageWidget/statewidget.h \
    MessageWidget/widget.h

RESOURCES += \
        BlogClient.qrc


CONFIG += c++11

FORMS += \
    BlogEditArea/BlogEditArea.ui \
    Login/login.ui

DISTFILES +=
