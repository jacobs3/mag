cmake_minimum_required(VERSION 3.2)
project(qnodeseditor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.cpp
    qneblock.cpp
    qneblock.h
    qneconnection.cpp
    qneconnection.h
    qnemainwindow.cpp
    qnemainwindow.h
    qneport.cpp
    qneport.h
    qnodeseditor.cpp
    qnodeseditor.h
    ui_qnemainwindow.h)

add_executable(qnodeseditor ${SOURCE_FILES})