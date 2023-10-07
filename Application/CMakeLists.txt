cmake_minimum_required(VERSION 3.16)

project(Application)

set(SOURCES
    Model
    Model/Detection
    Model/Detection/LicensePlateDetection
    View 
    Controller
)

find_package(OpenCV REQUIRED)

include_directories(include)

set(SRC_FILES "")
set(HEADER_FILES "")

foreach(dir ${SOURCES})
    # Add source files for this directory
    file(GLOB DIR_SRC "source/${dir}/*.cpp")
    file(GLOB DIR_HEADER "include/${dir}/*.h")
    list(APPEND SRC_FILES ${DIR_SRC})
    list(APPEND HEADER_FILES ${DIR_HEADER})
    # Create source group for this directory
    source_group("Source Files/${dir}" FILES ${DIR_SRC})
    source_group("Header Files/${dir}" FILES ${DIR_HEADER})
endforeach()

file(GLOB MAIN_HEADER_FILES "include/*.h")
file(GLOB MAIN_SRC_FILES "source/*.cpp")
list(APPEND SRC_FILES ${MAIN_HEADER_FILES})
list(APPEND HEADER_FILES ${MAIN_SRC_FILES})

add_executable(Application ${HEADER_FILES} ${SRC_FILES})

target_link_libraries(Application PRIVATE ${OpenCV_LIBS})