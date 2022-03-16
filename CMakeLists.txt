cmake_minimum_required(VERSION 3.21)
project(Przebiegi)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Concurrent
        Qml
        Quick
        Charts
        REQUIRED)

set(SOURCES
        main.cpp
        src/Utils.h
        src/DataInterface.h
        src/Generator.h
        src/DataObject.h
        src/DataObject.cpp
        src/Generator.cpp
        src/Waveforms.cpp
        src/Waveforms.h)
qt5_add_resources(SOURCES qml.qrc)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(
        ${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Concurrent
        Qt5::Qml
        Qt5::Quick
        Qt5::Charts
        )

