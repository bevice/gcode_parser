cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

set(GCODE_FILES
        gcode.c gcode.h
        gcode_tools.c gcode_tools.h
        )

add_library(gcode_parser ${GCODE_FILES})
