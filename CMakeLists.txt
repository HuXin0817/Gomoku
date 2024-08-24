cmake_minimum_required(VERSION 3.28)
project(chess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/Users/huxin/Qt/6.7.2/macos/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(chess main.cpp
        mainwindow.cpp
        board.cpp)
target_link_libraries(chess
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

