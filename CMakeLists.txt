cmake_minimum_required(VERSION 3.0)
project(gcode_parse_test)

set(CMAKE_CXX_STANDARD 14)

set(GCODE_FILES
        gcode.c gcode.h
        gcode_commands.c gcode_commands.h
        gcode_parser.c gcode_parser.h
        gcode_tools.c gcode_tools.h
        )

add_library(gcode_parser ${GCODE_FILES})
