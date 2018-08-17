/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#include "gcode_commands.h"
#include "gcode_tools.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

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

#define MAX_VARGS 4

gcode_status_t gcode_parse_vargs(char *find, char *args, ...) {
    va_list vl;
    gcode_hw_arg_t *values[MAX_VARGS];
    uint8_t i = 0;

    va_start(vl, args);
    while (find[i]) {
        gcode_hw_arg_t *x = va_arg(vl, gcode_hw_arg_t*);
        if (i < MAX_VARGS)
            values[i] = x;
        i++;
    }
    va_end(vl);

    while (*args && *args != '\n' && *args != '\r') {
        char argument = to_upper(*args);
        uint8_t n = 0xFF;
        //найдем аргумент в строке
        for (i = 0; i < strlen(find); ++i)
            if (to_upper(find[i]) == argument) {
                n = i;
                break;
            }
        gcode_hw_arg_t *t = NULL;
        if (n != 0xFF)
            t = values[n];
        args = get_arg_value(args, t);
        if (!args)
            return GCODE_ARGUMENT_ERROR;
    }
    return GCODE_OK;


}

gcode_status_t gcode_g00(char *args) {
    static gcode_hw_arg_t x = 0, y = 0, z = 0, speed = 0;
    if (hw_callbacks->g00) {
        gcode_status_t r = gcode_parse_vargs("XYZF", args, &x, &y, &z, &speed);
        if (r != GCODE_OK)
            return r;
        hw_callbacks->g00(x, y, z, speed);
        return GCODE_OK;
    }

    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}

gcode_status_t gcode_g01(char *args) {
    static gcode_hw_arg_t x = 0, y = 0, z = 0, speed = 0;
    if (hw_callbacks->g01) {
        gcode_status_t r = gcode_parse_vargs("XYZF", args, &x, &y, &z, &speed);
        if (r != GCODE_OK)
            return r;
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
        gcode_status_t r = gcode_parse_vargs("PF", args, &time, &time);
        if (r != GCODE_OK)
            return r;
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
        gcode_status_t r = gcode_parse_vargs("XYZ", args, &x, &y, &z);
        if (r != GCODE_OK)
            return r;
        hw_callbacks->g92(x, y, z);
        return GCODE_OK;
    }

    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}

gcode_status_t gcode_m51(char *args) {
    static gcode_hw_arg_t
            r = 0,
            g = 0,
            b = 0;
    if (hw_callbacks->m51) {
        gcode_status_t res = gcode_parse_vargs("RGB", args, &r, &g, &b);
        if (res != GCODE_OK)
            return res;
        hw_callbacks->m51(r, g, b);

    }
    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}

gcode_status_t gcode_m52(char *args) {
    static gcode_hw_arg_t
            r = 0,
            g = 0,
            b = 0,
            time = 1;
    if (hw_callbacks->m52) {
        gcode_status_t res = gcode_parse_vargs("RGBT", args, &r, &g, &b, &time);
        if (res != GCODE_OK)
            return res;
        hw_callbacks->m52(r, g, b, time);

    }
    return GCODE_HARDWARE_DOES_NOT_SUPPORT;
}
