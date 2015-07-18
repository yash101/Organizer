project(Organizer)
cmake_minimum_required(VERSION 2.8)

IF(WIN32)
ELSE()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread")
ENDIF()

file(GLOB_RECURSE SOURCES_MAIN "source/*.cpp")
file(GLOB_RECURSE HEADERS_MAIN "source/*.h")

add_executable(${PROJECT_NAME} ${SOURCES_MAIN} ${HEADERS_MAIN})
