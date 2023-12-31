cmake_minimum_required(VERSION 3.16)

project(Application)

set(SOURCES
    Model
    Model/Detection
    Model/Detection/LicensePlateDetection
    View 
    Controller
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

qt_standard_project_setup()

include_directories(include)

set(SRC_FILES "")
set(HEADER_FILES "")
set(UI_FILES "")

foreach(dir ${SOURCES})
    # Add source files for this directory
    file(GLOB DIR_SRC "source/${dir}/*.cpp")
    file(GLOB DIR_HEADER "include/${dir}/*.h")
    file(GLOB DIR_UI "source/${dir}/*.ui")
    list(APPEND SRC_FILES ${DIR_SRC})
    list(APPEND HEADER_FILES ${DIR_HEADER})
    list(APPEND UI_FILES ${DIR_UI})
    # Create source group for this directory
    source_group("Source Files/${dir}" FILES ${DIR_SRC})
    source_group("Header Files/${dir}" FILES ${DIR_HEADER})
    source_group("Resource Files/${dir}" FILES ${DIR_UI})
endforeach()

file(GLOB MAIN_HEADER_FILES "include/*.h")
file(GLOB MAIN_SRC_FILES "source/*.cpp")
list(APPEND SRC_FILES ${MAIN_HEADER_FILES})
list(APPEND HEADER_FILES ${MAIN_SRC_FILES})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(Application ${HEADER_FILES} ${SRC_FILES} ${UI_FILES})

target_link_libraries(Application PRIVATE ${OpenCV_LIBS} Qt6::Widgets)