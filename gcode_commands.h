/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#ifndef _GCODE_COMMANDS_H
#define _GCODE_COMMANDS_H

#include "gcode.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

gcode_status_t gcode_commands_init(gcode_hw_callbacks_struct *callbacks);
gcode_status_t gcode_g00(char *args);

gcode_status_t gcode_g01(char *args);

gcode_status_t gcode_g04(char *args);

gcode_status_t gcode_g15(char *args);

gcode_status_t gcode_g16(char *args);

gcode_status_t gcode_g20(char *args);

gcode_status_t gcode_g21(char *args);

gcode_status_t gcode_g90(char *args);

gcode_status_t gcode_g91(char *args);

gcode_status_t gcode_g92(char *args);

gcode_status_t gcode_m51(char *args);

gcode_status_t gcode_m52(char *args);

#ifdef __cplusplus
}
#endif


#endif //_GCODE_COMMANDS_H
