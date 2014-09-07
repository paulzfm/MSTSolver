#-------------------------------------------------
#
# Project created by QtCreator 2014-09-02T23:37:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MST
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    add_point_dialog.cpp \
    add_points_dialog.cpp \
    sample_generator_dialog.cpp \
    qutil.cpp \
    mst/Prim.cpp \
    mst/SampleGenerator.cpp \
    mst/SampleReader.cpp \
    edit_point_dialog.cpp \
    graphic.cpp \
    delaunay/Clarkson-Delaunay.cpp \
    scale_set_dialog.cpp \
    preference_dialog.cpp

HEADERS  += mainwindow.h \
    add_point_dialog.h \
    add_points_dialog.h \
    sample_generator_dialog.h \
    qutil.h \
    mst/Prim.h \
    mst/SampleGenerator.h \
    mst/SampleReader.h \
    mst/util.h \
    edit_point_dialog.h \
    graphic.h \
    delaunay/Clarkson-Delaunay.h \
    scale_set_dialog.h \
    preference_dialog.h

FORMS    += mainwindow.ui \
    add_point_dialog.ui \
    add_points_dialog.ui \
    sample_generator_dialog.ui \
    edit_point_dialog.ui \
    scale_set_dialog.ui \
    preference_dialog.ui

RESOURCES += \
    resources.qrc
