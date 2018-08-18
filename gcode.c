/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#include "gcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gcode_tools.h"

static gcode_command_struct *commands_ptr = NULL;
static uint8_t cmds_count;

static gcode_command_struct *last_command = NULL;

gcode_status_t gcode_run_code(char *line);

gcode_status_t gcode_repeat_last(char *line);


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
        case GCODE_CANT_ALLOCATE_MEMORY:
            return "Can't allocate dynamic memory";
    }

}


gcode_status_t gcode_init(const gcode_command_struct *init_struct, uint8_t commands_count) {
#ifdef _GCODE_USE_DYNAMIC_MEMORY
    if (commands_ptr)
        free(commands_ptr); // если уже инициализировались - забудем.
    size_t s = commands_count * sizeof(gcode_command_struct);
    commands_ptr = malloc(s);
    // если память не выделилась - вернем ошибку
    if (!commands_ptr)
        return GCODE_CANT_ALLOCATE_MEMORY;
    // скопируем структуру
    memcpy(commands_ptr, init_struct, commands_count * sizeof(gcode_command_struct));
    int test = memcmp(commands_ptr, init_struct, commands_count * sizeof(gcode_command_struct));

#else
    commands_ptr = (gcode_command_struct*)init_struct;
#endif
    cmds_count = commands_count;
    return GCODE_OK;
}

char *get_arg_value(char *args, gcode_hw_arg_t *value) {

    gcode_hw_arg_t dummy_value;
    if (!value) value = &dummy_value;
    if (!args)return NULL;
    if (*args == '\r' || *args == '\n')return ++args;
    // находим первую цифру в строке
    while (*args && (*args < '0' || *args > '9') && *args != '-')
        args++;
    if (!*args) // вышли из цикла по концу строки, цифра не найдена
        return NULL; // возвращаемся с ошибкой

    // args указывает на начало аргумента


    uint8_t flags = 0; // 0 бит - нашли точку, 1 бит - число отрицательное
    float d = 10;
    // начинаем разбирать
    if (*args == '-') {
        flags |= 1 << 1;
        args++;
    }
    *value = 0;
    while (*args && ((*args >= '0' && *args <= '9') || *args == '.')) {
        if (*args == '.') {
            if (flags & 1)
                return NULL; // точка уже нашлась - ошибка аргумента
            flags |= 1 << 0;
        } else {
            // приехала очередная цифра
            if (flags & 1) // точка уже нашлась
            {
                *value += (*args - '0') / (float) d;
                d *= 10.0;
            } else
                *value = *value * 10 + (*args - '0');
        }
        args++;
    }
    if (flags & 1 << 1)
        *value = -*value;
    return rstrip(args);
}


gcode_status_t gcode_parse_line(char *line) {

    if (!line) return GCODE_ARGUMENT_ERROR; // нулевой указатель
    char *first_sym = rstrip(line);


    if (!strlen(first_sym)) return GCODE_OK; // пустая строка
    if (*first_sym == '(') // Это комментарий, выходим
        return GCODE_OK;

    gcode_clear_comment(line);
    *first_sym = to_upper(*first_sym);
    switch (*first_sym) {

        case 'G':
        case 'M':
            return gcode_run_code(first_sym);
        case 'X':
        case 'Y':
        case 'Z':
        case 'F':
        case 'T':
            return gcode_repeat_last(first_sym);
        default:
            return GCODE_UNKNOWN_LINE_TYPE;
    }


}

gcode_status_t gcode_deinit() {
#ifdef _GCODE_USE_DYNAMIC_MEMORY
    if (commands_ptr)
        free(commands_ptr);
#endif
    commands_ptr = NULL;
    cmds_count = 0;
    return GCODE_OK;
}

gcode_status_t gcode_command(gcode_command_struct *command, char *args) {
    // есть строка с аргументами
    size_t argv = strlen(command->arguments);
#ifdef _GCODE_USE_DYNAMIC_MEMORY
    gcode_hw_arg_t *data = malloc(sizeof(gcode_hw_arg_t) * argv);
    if (!data)
        return GCODE_CANT_ALLOCATE_MEMORY;
#else // _GCODE_USE_DYNAMIC_MEMORY
    if(_GCODE_MAX_ARGS<argv)
        return GCODE_BUFFER_TOO_SHORT;
    gcode_hw_arg_t data[_GCODE_MAX_ARGS] = {0};
#endif // _GCODE_USE_DYNAMIC_MEMORY

    // заполняем значениями по-умолчанию
    for (uint8_t i = 0; i < strlen(command->arguments); ++i)
        data[i] = GCODE_NULL_VALUE;

    // разбираем аргументы
    while (*args && *args != '\n' && *args != '\r') {
        char argument = to_upper(*args);
        uint8_t n = 0xFF;
        //найдем аргумент в строке
        for (uint8_t i = 0; i < strlen(command->arguments); ++i)
            if (to_upper(command->arguments[i]) == argument) {
                n = i;
                break;
            }
        gcode_hw_arg_t *t = NULL;
        if (n != 0xFF)
            t = &data[n];
        args = get_arg_value(args, t);
        if (!args)
            return GCODE_ARGUMENT_ERROR;
    }
    gcode_status_t res = command->callback(argv, data);

#ifdef _GCODE_USE_DYNAMIC_MEMORY
    free(data); // не забудем освободить память

#endif //_GCODE_USE_DYNAMIC_MEMORY
    return res;

}

gcode_status_t gcode_run_code(char *line) {

    // найдем команду

    if (!commands_ptr || !cmds_count)
        return GCODE_NOT_INITED;
    size_t code_len = gcode_argument_len(line);
#ifdef _GCODE_CLEAN_ZEROES
    // Уберем незначащие нули
    while (code_len>2 && *(line+1) == '0'){
        for(uint8_t i=1;i<code_len-1;i++)
            *(line+i) = *(line+i+1);
        code_len--;
        *(line+code_len) = ' ';
#endif // _GCODE_CLEAN_ZEROES

    }

    for (uint8_t i = 0; i < cmds_count; ++i) {
        if (!strncmp(line, (commands_ptr + i)->code, code_len)) {
            gcode_clear_comment(line + code_len);
            last_command = commands_ptr + i;
            return gcode_command(commands_ptr + i, rstrip(line + code_len));
        }
    }
    return GCODE_NOT_IMPLEMENTED;
}


gcode_status_t gcode_repeat_last(char *line) {
    if (!last_command) return GCODE_LAST_COMMAND_NOT_FOUND;
    return gcode_command(last_command, rstrip(line));
}
