//
// Created by Wout Lyen on 14/11/23.
//
#include <unistd.h>
#include <string.h>
#include "sensor_db.h"
#include "logger.h"

#define SIZE 1

int pipe_write_end = -1;
FILE *db_file = NULL;
bool child_created = false;

#define OPEN 1
#define OPEN_ERROR 2
#define INSERT 3
#define INSERT_ERROR 4
#define CLOSE 5
#define CLOSE_ERROR 6
#define NO_MESSAGE 7
#define END 8
#define FILENAME_EMPTY 9
#define ALREADY_CHILD 10


FILE * open_db(char * filename, bool append){

    // Check if no child exists
    if (!child_created){

        // Create new child
        pipe_write_end = create_log_process();

        // Check if child creation succeeded
        if(pipe_write_end != -1){
            child_created = true;
        }
        else{
            child_created = false;
            return NULL;
        }

    }
    else{ // Child already exists

        // Log message
        char message = ALREADY_CHILD;
        write(pipe_write_end, &message, SIZE);
        return db_file;
    }

    if (filename == NULL){
        char message = FILENAME_EMPTY;
        write(pipe_write_end, &message, SIZE);
        message = END;
        write(pipe_write_end, &message, SIZE);
        child_created = false;
        close(pipe_write_end);
        return NULL;
    }

    FILE *file = NULL;
    if (append){
        file = fopen(filename, "a");
    }
    else{
        file = fopen(filename, "w");
    }

    if(file != NULL){
        char message = OPEN;
        write(pipe_write_end, &message, SIZE);
        db_file = file;
    } else{
        char message = OPEN_ERROR;
        write(pipe_write_end, &message, SIZE);
        db_file = NULL;
        child_created = false;
        close(pipe_write_end);
    }
    return file;
}



int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){

    // Check if child is created
    if (!child_created){
        printf("No log file opened (use open_db() first): No database file opened\n");
        return -1;
    }

    // Check if filename is NULL
    if (f == NULL){
        char message = FILENAME_EMPTY;
        write(pipe_write_end, &message, SIZE);
        return -1;
    }

    int code = fprintf(f, "%hu, %f, %ld\n", id, value, ts);

    if (code >= 0) {
        char message = INSERT;
        write(pipe_write_end, &message, SIZE);
    }
    else{
        char message = INSERT_ERROR;
        write(pipe_write_end, &message, SIZE);
    }

    return code;
}

int close_db(FILE * f){

    // Check if child is created
    if (!child_created){
        printf("No log file opened (use open_db() first): No database file opened\n");
        return -1;
    }


    if (f == NULL){
        char message = FILENAME_EMPTY;
        write(pipe_write_end, &message, SIZE);
        return -1;
    }

    int code = fclose(f);
    if (code == 0){
        char message = CLOSE;
        write(pipe_write_end, &message, SIZE);
    }
    else{
        char message = CLOSE_ERROR;
        write(pipe_write_end, &message, SIZE);
    }
    close(pipe_write_end);
    child_created = false;
    db_file = NULL;
    return code;
}
