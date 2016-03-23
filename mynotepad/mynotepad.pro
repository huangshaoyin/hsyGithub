#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T16:26:59
#
#-------------------------------------------------



QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mynotepad
TEMPLATE = app

 INCLUDEPATH+=e:\opencv\build\include\
                e:\opencv\build\include\opencv\
                 e:\opencv\build\include\opencv2

 LIBS+=e:\opencv\build\x86\vc11\lib\opencv_ts300d.lib\
       e:\opencv\build\x86\vc11\lib\opencv_world300d.lib



SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui



