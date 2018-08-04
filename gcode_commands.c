/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#include "gcode_commands.h"
#include "gcode_tools.h"
#include <stdio.h>
#include <string.h>

static gcode_hw_callbacks_struct *hw_callbacks = NULL;


char *get_arg_value(char *args, gcode_hw_arg_t *value) {

    gcode_hw_arg_t dummy_value;
    if (!value) value = &dummy_value;
    if (!args)return NULL;
    if (*args == '\r' || *args == '\n')return args++;
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

gcode_status_t gcode_g00(char *args) {
    static gcode_hw_arg_t x = 0, y = 0, z = 0, speed = 0;
    if (hw_callbacks->g00) {
        char *pargs = rstrip(args);
        // достаем координаты из аргументов
        while (pargs && *pargs && *pargs != '\n') {
            gcode_hw_arg_t *t = NULL;
            switch (*pargs) {
                case 'X':
                case 'x':
                    t = &x;
                    break;
                case 'Y':
                case 'y':
                    t = &y;
                    break;
                case 'F':
                case 'f':
                    t = &speed;
                    break;
                case 'Z':
                case 'z':
                    t = &z;
                    break;
                default:
                    t = NULL;
                    break;
            }
            pargs = get_arg_value(pargs, t);
            if (!pargs) // если функция вернула пустой указатель, значит что-то не распарсилось!
                return GCODE_ARGUMENT_ERROR;
        }
        hw_callbacks->g00(x, y, z, speed);
        return GCODE_OK;
    }

    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}

gcode_status_t gcode_g01(char *args) {
    static gcode_hw_arg_t x = 0, y = 0, z = 0, speed = 0;
    if (hw_callbacks->g01) {
        char *pargs = rstrip(args);
        // достаем координаты из аргументов
        while (pargs && *pargs && *pargs != '\n') {
            gcode_hw_arg_t *t = NULL;
            switch (*pargs) {
                case 'X':
                case 'x':
                    t = &x;
                    break;
                case 'Y':
                case 'y':
                    t = &y;
                    break;
                case 'F':
                case 'f':
                    t = &speed;
                    break;
                case 'Z':
                case 'z':
                    t = &z;
                    break;
                default:
                    t = NULL;
                    break;
            }
            pargs = get_arg_value(pargs, t);
            if (!pargs) // если функция вернула пустой указатель, значит что-то не распарсилось!
                return GCODE_ARGUMENT_ERROR;
        }
        hw_callbacks->g01(x, y, z, speed);
        return GCODE_OK;
    }

    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}

gcode_status_t gcode_commands_init(gcode_hw_callbacks_struct *callbacks) {
    hw_callbacks = callbacks;
    if (!callbacks)
        return GCODE_ARGUMENT_ERROR;
    return GCODE_OK;
}

gcode_status_t gcode_g04(char *args) {
    static gcode_hw_arg_t time = 0;
    if (hw_callbacks->g04) {
        char *pargs = rstrip(args);
        // достаем координаты из аргументов
        while (pargs && *pargs && *pargs != '\n') {
            gcode_hw_arg_t *t = NULL;
            switch (*pargs) {
                case 'F':
                case 'f':
                    t = &time;
                    break;
                case 'P':
                case 'p':
                    t = &time;
                    break;
                default:
                    t = NULL;
                    break;
            }
            pargs = get_arg_value(pargs, t);
            if (!pargs) // если функция вернула пустой указатель, значит что-то не распарсилось!
                return GCODE_ARGUMENT_ERROR;
        }
        hw_callbacks->g04(time);
        return GCODE_OK;
    }

    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}

gcode_status_t gcode_g15(char *args) {
    if (hw_callbacks->g15) {
        hw_callbacks->g15();
        return GCODE_OK;
    }
    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}


gcode_status_t gcode_g16(char *args) {
    if (hw_callbacks->g16) {
        hw_callbacks->g16();
        return GCODE_OK;
    }
    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}


gcode_status_t gcode_g20(char *args) {
    if (hw_callbacks->g20) {
        hw_callbacks->g20();
        return GCODE_OK;
    }
    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}

gcode_status_t gcode_g21(char *args) {
    if (hw_callbacks->g21) {
        hw_callbacks->g21();
        return GCODE_OK;
    }
    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}

gcode_status_t gcode_g90(char *args) {
    if (hw_callbacks->g90) {
        hw_callbacks->g90();
        return GCODE_OK;
    }
    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}

gcode_status_t gcode_g91(char *args) {
    if (hw_callbacks->g91) {
        hw_callbacks->g91();
        return GCODE_OK;
    }
    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}

gcode_status_t gcode_g92(char *args) {
    gcode_hw_arg_t x = GCODE_NO_VALUE, y = GCODE_NO_VALUE, z = GCODE_NO_VALUE;
    if (hw_callbacks->g92) {
        char *pargs = rstrip(args);
        // достаем координаты из аргументов
        while (pargs && *pargs && *pargs != '\n') {
            gcode_hw_arg_t *t = NULL;
            switch (*pargs) {
                case 'X':
                case 'x':
                    t = &x;
                    break;
                case 'Y':
                case 'y':
                    t = &y;
                    break;
                case 'Z':
                case 'z':
                    t = &z;
                    break;
                default:
                    t = NULL;
                    break;
            }
            pargs = get_arg_value(pargs, t);
            if (!pargs) // если функция вернула пустой указатель, значит что-то не распарсилось!
                return GCODE_ARGUMENT_ERROR;
        }
        hw_callbacks->g92(x, y, z);
        return GCODE_OK;
    }

    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}
