//
// Created by woutlyen on 14/11/23.
//
#include "sensor_db.h"

FILE * open_db(char * filename, bool append){
    FILE *file = NULL;
    if (append){
        file = fopen(filename, "a");
    }
    else{

        file = fopen(filename, "a");
    }
    return file;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    return fprintf(f, "%hu, %f, %ld\n", id, value, ts);
}

int close_db(FILE * f){
    return fclose(f);
}
