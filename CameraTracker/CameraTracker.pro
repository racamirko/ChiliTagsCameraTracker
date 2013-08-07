#-------------------------------------------------
#
# Project created by QtCreator 2013-08-05T17:16:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = CameraTracker
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -L/home/raca/repo/outside_projects/chilitags/build/src/lib -lchilitags -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_video -lopencv_imgproc -lboost_system -lboost_filesystem -lopencv_calib3d -lglog -lboost_unit_test_framework
INCLUDEPATH += /home/raca/repo/outside_projects/chilitags/src/lib/include /usr/local/include

# remove or add -DTESTING to enable boost::unit_tests suite

QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -DNDEBUG
QMAKE_CXXFLAGS += -std=c++0x -DUNIT_TESTING

SOURCES += main.cpp \
    inputParsers.cpp \
    data/CTagCoords.cpp \
    unittests/CTestCase.cpp

HEADERS += \
    globalInclude.h \
    inputParsers.h \
    data/CTagCoords.h \
    unittests/CTestCase.h
