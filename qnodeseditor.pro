#-------------------------------------------------
#
# Project created by QtCreator 2012-04-16T14:33:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qnodeseditor
TEMPLATE = app


SOURCES +=\
    main.cpp \
    Editor.cpp \
    MainWindow.cpp \
    Site.cpp \
    RouterView.cpp \
    Router.cpp \
    ConnectionView.cpp \
    PortView.cpp \
    Port.cpp \
    Connection.cpp \
    RouterManagementDialog.cpp \
    IObservable.cpp \
    TableDialog.cpp \
    SimulateTrafficDialog.cpp \
    MplsPacketView.cpp \
    NextHopCalculator.cpp


HEADERS  += \
    IController.hpp \
    Editor.hpp \
    UiMainWindow.hpp \
    MainWindow.hpp \
    Site.hpp \
    Typedefs.hpp \
    RouterView.hpp \
    Router.hpp \
    IdGenerator.hpp \
    ConnectionView.hpp \
    Connection.hpp \
    PortView.hpp \
    Port.hpp \
    RouterManagementDialog.hpp \
    IObserver.hpp \
    IObservable.hpp \
    MplsIlmTable.hpp \
    MplsFtnTable.hpp \
    MplsNhlfeTable.hpp \
    TableDialog.hpp \
    SimulateTrafficDialog.hpp \
    MplsPacketView.hpp \
    NextHopCalculator.hpp

    

DISTFILES +=

