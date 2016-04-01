#-------------------------------------------------
#
# Project created by QtCreator 2015-04-21T12:32:13
#
#-------------------------------------------------

QT       += core gui
QT      += xml
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets

TARGET = indexcalculator
TEMPLATE = app
CONFIG +=gcc


LIBS+= -L"C:/LibQt/bin/" -lopenblas \

INCLUDEPATH += "C:/LibQt/OpenBLAS-v0.2.14-Win32/include" \
DEPENDPATH += "C:/LibQt/OpenBLAS-v0.2.14-Win32/include" \

INCLUDEPATH += "C:/LibQt/Armadillo/include" \
DEPENDPATH += "C:/LibQt/Armadillo/include" \


SOURCES += main.cpp\
    model/documento.cpp \
    model/keyword.cpp \
    model/database.cpp \
    model/query.cpp \
    model/match.cpp \
    model/querylist.cpp

HEADERS  += \
    model/documento.h \
    model/keyword.h \
    model/database.h \
    model/query.h \
    model/match.h \
    model/querylist.h \

FORMS    += mainwindow.ui
