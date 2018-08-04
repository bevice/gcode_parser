/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#ifndef _TEST_GCODE_TOOLS_H
#define _TEST_GCODE_TOOLS_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t gcode_argument_len(const char *const line);

void gcode_clear_comment(char *line);

char *rstrip(char *line);

char to_upper(char v);
#ifdef __cplusplus
}
#endif

#endif //_TEST_GCODE_TOOLS_H
