#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T02:12:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qnn-run-analyser
TEMPLATE = app


SOURCES += main.cpp\
        analyser.cpp \
    plotter.cpp

HEADERS  += analyser.h \
    plotter.h

FORMS    += analyser.ui \
    plotter.ui

OTHER_FILES += \
    LICENSE.GPL3 \
    README.txt

unix|win32: LIBS += -lqcustomplot
