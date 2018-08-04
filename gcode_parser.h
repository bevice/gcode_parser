/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#ifndef _GCODE_PARSER_H
#define _GCODE_PARSER_H

#include "gcode.h"

#ifdef __cplusplus
extern "C" {
#endif

gcode_status_t gcode_parce_line(char *line);


#ifdef __cplusplus
}
#endif

#endif //_GCODE_PARSER_H
