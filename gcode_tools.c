/**
* \file
* \date 04.08.2018
* \authors Alexander A. Kuzkin <xbevice@gmail.com>
*/

#include "gcode_tools.h"
#include <string.h>

/// \brief Заменяет открывающую скобку '(' на конец строки
void gcode_clear_comment(char *line) {
    if (!line)return;
    do {
        if (*line == '(') {
            *line = 0;
            break;
        }
    } while (*++line);
}

/**
 * \brief Возвращает длинну аргумента (цифровую часть) включая символ кода
 *
 * \example
 * G01X10 вернет 3      <br/>
 * G01 Y10 вернет 3     <br/>
 * Y10.4Z10 вернет 5    <br/>
*/
size_t gcode_argument_len(const char *const line) {
    size_t i = 0;
    // находим первую цифру в строке
    while (line[i] && (line[i] < '0' || line[i] > '9'))
        i++;
    if (i >= strlen(line))return strlen(line); // строка состоит только из букв
    if (line[i] == '-')i++; // минус в начале тоже можно
    while (line[i] && ((line[i] >= '0' && line[i] <= '9') || line[i] == '.'))
        i++;

    return i;
}

/**
 * Возвращает указатель на первый символ (игнорируются пробелы вначале)
 * @return
 */
char *rstrip(char *const line) {
    char *first_sym = line;
    while (*first_sym == ' ' || *first_sym == '\r')
        first_sym++;
    return first_sym;
}

