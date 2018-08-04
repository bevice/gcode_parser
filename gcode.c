/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#include "gcode.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "gcode_commands.h"

// test
const char *gcode_get_error(gcode_status_t error) {
    switch (error) {
        default:
            return "Unknown error";
        case GCODE_OK:
            return "No error";
        case GCODE_STUB:
            return "STUB";
        case GCODE_ARGUMENT_ERROR:
            return "Argument error";
        case GCODE_LAST_COMMAND_NOT_FOUND:
            return "Last command not found";
        case GCODE_BUFFER_TOO_SHORT:
            return "Buffer too short";
        case GCODE_NOT_IMPLEMENTED:
            return "Not implemented";
        case GCODE_HARDWARE_DOES_NOT_SUPPORT:
            return "Hardware doesn't support";
        case GCODE_UNKNOWN_LINE_TYPE:
            return "Unknown line type";
    }

}


void gcode_init(gcode_hw_callbacks_struct *hw_callbacks) {
    gcode_commands_init(hw_callbacks);
}

