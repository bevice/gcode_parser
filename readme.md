# GCode Parser

Как использовать:
* прочитать файл gcode.h
* добавить в проект


````
add_subdirectory(gcode_parser)
target_link_libraries(project_name gcode_parser)
````



## Пример:

```c

gcode_status_t g00(size_t argv, gcode_hw_arg_t *args) {
    static gcode_hw_arg_t x = 0, y = 0, speed = 0;
    gcode_modify_values(args, 3, &x, &y, &speed);

    printf("G00 to x=%.2f, y=%.2f, speed=%.2f\n", x, y, speed);
    return GCODE_OK;
}

gcode_status_t g01(size_t argv, gcode_hw_arg_t *args) {
        static gcode_hw_arg_t x = 0, y = 0, speed = 0;
        gcode_modify_values(args, 3, &x, &y, &speed);

    printf("G01 to x=%.2f, y=%.2f, speed=%.2f\n", x, y, speed);
    return GCODE_OK;
}

gcode_status_t g04(size_t argv, gcode_hw_arg_t *args) {
    if (*args == GCODE_NULL_VALUE)
        return GCODE_ARGUMENT_ERROR;
    printf("G04 sleep time = %0.2f\n", *args);
    return GCODE_OK;
}


int main(){
    char * lines[lines_count] = {...};

    gcode_command_struct commands[] = {
        {"G0", "XYF", g00},
        {"G1", "XYF", g01},
        {"G4", "P",   g04},
    };

    gcode_init(commands, sizeof(commands) / sizeof(gcode_command_struct));

    for(size_t i=0;i<lines_count;++i){
        gcode_status_t res = gcode_parse_line(lines[i]);
        if(res)
            printf("Error %d: %s", res, gcode_get_error(res));

        }
}

