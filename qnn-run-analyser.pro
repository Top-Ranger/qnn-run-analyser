#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T02:12:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qnn-run-analyser
TEMPLATE = app


SOURCES += src/main.cpp \
        src/analyser.cpp \
    src/plotter.cpp

HEADERS  += src/analyser.h \
    src/plotter.h

FORMS    += src/analyser.ui \
    src/plotter.ui

OTHER_FILES += \
    LICENSE.GPL3 \
    README.txt

unix|win32: LIBS += -lqcustomplot
