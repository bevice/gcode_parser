/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#ifndef _GCODE_H
#define _GCODE_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#ifdef __cplusplus
extern "C" {
#endif


//#define _GCODE_USE_DYNAMIC_MEMORY   ///< Использовать динамическую память
#define _GCODE_CLEAN_ZEROES         ///< Убирать незначащие нули из кодов команд G01 -> G1

///< Максимальное количество аргументов команды GCode и коллбека без динамической памяти
#define _GCODE_MAX_ARGS 4
///Это значение передается в коллбек если соответствующий параметр команды был опущен в GCode
#define GCODE_NULL_VALUE (INFINITY)

/// тип аргумента передаваемого в коллбек
#ifndef gcode_gw_arg_t
typedef float gcode_hw_arg_t;
#endif

/**
 * Коды возврата
 */
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


/**
 * Прототип функции-коллбека
 * Вызывается с ссылкой на массив аргументов args и количество аргументов argv
 * порядок передачи и количество зависит от порядка в элементе arguments gcode_command_struct
 * описывающей команду
 */
typedef gcode_status_t (*gcode_callback_t)(size_t argc, gcode_hw_arg_t *args);


/**
 * Структура описывающая реализованные команды GCode
 */
typedef struct {
    char *code;                     ///< Код команды без значащих нулей "G1" "G0" "M300" и так далее
    char *arguments;                ///< Аргументы которые может принимать команда "XYZF"
    gcode_callback_t callback;      ///< Коллбек, который нужно дергать по команде
} gcode_command_struct;


/**
 * Инициализация парсера
 * в функцию передается массив gcode_command_struct и количество элементов массива
 * при определенном _GCODE_USE_DYNAMIC_MEMORY массив будет использован malloc() и memcpy для копирования
 * массива,
 * При неопределенной _GCODE_USE_DYNAMIC_MEMORY будет скопирован адрес переданного массива,
 * поэтому массив не должнен выходить за область видимости пока модуль работает.
 *
 * @param init_struct структура с командами и коллбеками
 * @param commands_count количество команд в структуре
 * @return
 */
gcode_status_t gcode_init(const gcode_command_struct *init_struct, uint8_t commands_count);


/**
 * Деинициализация модуля и освобождение ресурсов в случае использования динамической памяти
 * @return
 */
gcode_status_t gcode_deinit();


/**
 * Расшифровка кода ошибки
 * @param error код ошибки
 * @return строка с расшифровкой ошибки
 */
const char *gcode_get_error(gcode_status_t error);


/**
 * Парсить очередную GCode строку
 * @param line
 * @return GCODE_OK (0) или код ошибки
 */
gcode_status_t gcode_parse_line(char *line);

/**
 * * Обновить переменные значениями из args если они не равны GCODE_NULL_VALUE
 * @param args указатель на первый элемент массива аргументов
 * @param count количесво аргументов
 * @param ... адреса переменных для обновления
 * @example: gcode_modify_values(args, 3, &x, &y, &speed);
 */
void gcode_modify_values(gcode_hw_arg_t *args, int count, ...);
#ifdef __cplusplus
}
#endif


#endif //_GCODE_H
