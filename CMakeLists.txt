cmake_minimum_required(VERSION 3.28)
project(gomoku)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(gomoku main.cpp
        src/mainwindow.cpp
        src/board.cpp
        src/sensor.cpp)

target_link_libraries(gomoku
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

