include(gtest_dependency.pri)

QT += core gui widgets


TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG += no_keywords

SOURCES += \
        TableModelTest.cpp \
        main.cpp

INCLUDEPATH += ../MiniExcel

SOURCES += \
    ../MiniExcel/MainToolBar/textAlignmentButtons.cpp \
    ../MiniExcel/cell.cpp \
    ../MiniExcel/tablemodel.cpp \
    ../MiniExcel/window.cpp

HEADERS += \
    ../MiniExcel/MainToolBar/textAlignmentButtons.h \
    ../MiniExcel/cell.h \
    ../MiniExcel/tablemodel.h \
    ../MiniExcel/window.h
