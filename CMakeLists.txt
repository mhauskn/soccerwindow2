cmake_minimum_required (VERSION 2.8.12)
project(soccerwindow2)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(LIBRCSC_INCLUDE_DIR CACHE FILEPATH "librcsc include directory")
set(LIBRCSC_LINK_DIR CACHE FILEPATH "librcsc link directory")
if(NOT LIBRCSC_INCLUDE_DIR)
    message(FATAL_ERROR "LIBRCSC_INCLUDE_DIR required but not set.")
endif(NOT LIBRCSC_INCLUDE_DIR)
if(NOT LIBRCSC_LINK_DIR)
    message(FATAL_ERROR "LIBRCSC_LINK_DIR required but not set.")
endif(NOT LIBRCSC_LINK_DIR)
include_directories(${LIBRCSC_INCLUDE_DIR})
link_directories(${LIBRCSC_LINK_DIR})

find_package(Qt4 REQUIRED COMPONENTS QTCORE QTGUI QTNETWORK)
set(CMAKE_AUTOMOC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${QT_INCLUDES})
add_definitions(-DHAVE_CONFIG_H)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(RCSC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rcsc)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB MODEL_SOURCES ${SRC_DIR}/model/*.cpp)
file(GLOB WINDOW_SOURCES ${SRC_DIR}/qt4/*.cpp)
include_directories(${SRC_DIR}/model ${SRC_DIR}/qt4)

add_executable(soccerwindow2 ${MODEL_SOURCES} ${WINDOW_SOURCES})
target_link_libraries(soccerwindow2 Qt4::QtGui Qt4::QtNetwork rcsc_agent rcsc_ann rcsc_geom rcsc_gz rcsc_net rcsc_param rcsc_rcg rcsc_time)
