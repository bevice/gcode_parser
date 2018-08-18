/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#ifndef _TEST_GCODE_H
#define _TEST_GCODE_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ARGS 4

#define GCODE_USE_DYNAMIC_MEMORY

#define GCODE_NULL_VALUE (9e99)

typedef float gcode_hw_arg_t;

typedef enum {
    GCODE_OK = 0,
    GCODE_ARGUMENT_ERROR,
    GCODE_LAST_COMMAND_NOT_FOUND,
    GCODE_UNKNOWN_LINE_TYPE,
    GCODE_BUFFER_TOO_SHORT,
    GCODE_STUB,
    GCODE_NOT_IMPLEMENTED,
    GCODE_HARDWARE_DOES_NOT_SUPPORT,
    GCODE_CANT_ALLOCATE_MEMORY,
    GCODE_NOT_INITED,
} gcode_status_t;


typedef gcode_status_t (*gcode_callback_t)(gcode_hw_arg_t * args);


typedef struct {
    char *code;
    char *arguments;
    gcode_callback_t callback;
} gcode_command_struct;


gcode_status_t gcode_init(gcode_command_struct *init_struct, uint8_t commands_count);
gcode_status_t gcode_deinit();
const char *   gcode_get_error(gcode_status_t error);
gcode_status_t gcode_parse_line(char *line);

#ifdef __cplusplus
}
#endif


#endif //_TEST_GCODE_H
