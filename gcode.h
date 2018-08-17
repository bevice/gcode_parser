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

typedef double gcode_hw_arg_t;
typedef void gcode_hw_return_t;

#define GCODE_NO_VALUE (9e99)
typedef enum {
    GCODE_OK = 0,
    GCODE_ARGUMENT_ERROR,
    GCODE_LAST_COMMAND_NOT_FOUND,
    GCODE_UNKNOWN_LINE_TYPE,
    GCODE_BUFFER_TOO_SHORT,
    GCODE_STUB,
    GCODE_NOT_IMPLEMENTED,
    GCODE_HARDWARE_DOES_NOT_SUPPORT,
} gcode_status_t;


typedef gcode_status_t (*gcode_worker_f)(char *args);


typedef struct {
    char *code;
    gcode_worker_f callback;
} gcode_parser_struct;

typedef struct {
    gcode_hw_return_t (*g00)(gcode_hw_arg_t x, gcode_hw_arg_t y, gcode_hw_arg_t z, gcode_hw_arg_t speed);
    gcode_hw_return_t (*g01)(gcode_hw_arg_t x, gcode_hw_arg_t y, gcode_hw_arg_t z, gcode_hw_arg_t speed);

    gcode_hw_return_t (*g04)(gcode_hw_arg_t time);

    gcode_hw_return_t (*g15)(void);

    gcode_hw_return_t (*g16)(void);

    gcode_hw_return_t (*g20)(void);

    gcode_hw_return_t (*g21)(void);

    gcode_hw_return_t (*g90)(void);

    gcode_hw_return_t (*g91)(void);

    gcode_hw_return_t (*g92)(gcode_hw_arg_t x, gcode_hw_arg_t y, gcode_hw_arg_t z);

    gcode_hw_return_t (*m51)(gcode_hw_arg_t r,gcode_hw_arg_t g, gcode_hw_arg_t b);
} gcode_hw_callbacks_struct;


void gcode_init(gcode_hw_callbacks_struct *hw_callbacks);

const char *gcode_get_error(gcode_status_t error);

#ifdef __cplusplus
}
#endif


#endif //_TEST_GCODE_H
