cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Licenta)

enable_testing()

add_subdirectory(Application)
add_subdirectory(Resources)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Application)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui GuiTools CoreTools)
find_package(OpenCV REQUIRED)

set(Qt6 CMAKE_AUTOMOC True)
set(Qt6 CMAKE_AUTOUIC True)
set(Qt6 CMAKE_AUTORCC True)

qt_standard_project_setup()
