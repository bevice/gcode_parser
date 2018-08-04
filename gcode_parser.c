/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#include "gcode_parser.h"
#include "gcode_commands.h"
#include "gcode_tools.h"
#include <string.h>
#include "gcode.h"

static gcode_worker_f last_command = NULL;

static const gcode_parser_struct callbacks[] = {
        {"G00", gcode_g00},
        {"G0",  gcode_g00}, // для тупых
        {"G01", gcode_g01},
        {"G1",  gcode_g01}, // для тупых
        {"G04", gcode_g04},
        {"G4",  gcode_g04}, // для тупых
        {"G15", gcode_g15},
        {"G16", gcode_g16},
        {"G20", gcode_g20},
        {"G21", gcode_g21},
        {"G90", gcode_g90},
        {"G91", gcode_g91},
        {"G92", gcode_g92},
};


gcode_status_t gcode_run_code(char *line) {
    size_t code_len = gcode_argument_len(line);
    // найдем команду
    for (size_t i = 0; i < sizeof(callbacks) / sizeof(gcode_parser_struct); ++i)
        if (!strncmp(line, callbacks[i].code, code_len)) {
            last_command = callbacks[i].callback;
            gcode_clear_comment(line + code_len);
            return callbacks[i].callback(rstrip(line + code_len));
        }
    return GCODE_NOT_IMPLEMENTED;
}


gcode_status_t gcode_repeat_last(char *line) {
    if (!last_command) return GCODE_LAST_COMMAND_NOT_FOUND;
    return last_command(line);
}


gcode_status_t gcode_parce_line(char *line) {
    uint8_t code_len = 0;

    if (!line) return GCODE_ARGUMENT_ERROR; // нулевой указатель
    char *first_sym = rstrip(line);


    if (!strlen(first_sym)) return GCODE_OK; // пустая строка
    if (*first_sym == '(') // Это комментарий, выходим
        return GCODE_OK;

    gcode_clear_comment(line);
    switch (*first_sym) {

        case 'G':
        case 'g':
        case 'M':
        case 'm':
            return gcode_run_code(first_sym);
        case 'X':
        case 'x':
        case 'Y':
        case 'y':
        case 'Z':
        case 'z':
            return gcode_repeat_last(first_sym);
        default:
            return GCODE_UNKNOWN_LINE_TYPE;
    }


}