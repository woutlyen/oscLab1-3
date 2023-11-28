//
// Created by woutlyen on 28/11/23.
//
#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "config.h"
#include "sbuffer.h"


void *writer_tread(void *vargp){

    FILE *fptr;
    sbuffer_t *buf = (sbuffer_t*) vargp;

    fptr = fopen("sensor_data", "r");

    //Declaration of variables in the fp_sensor_data file
    sensor_data_t *sensor_data = (sensor_data_t*)malloc(sizeof(sensor_data_t));
    uint16_t *sensor_id_t = &sensor_data->id;
    double *sensor_value_t = &sensor_data->value;
    time_t *sensor_ts_t = &sensor_data->ts;

    //Reading and adding all the assigned sensor data in the list
    while(fread(sensor_id_t, sizeof(uint16_t), 1, fptr) != 0) {
        fread(sensor_value_t, sizeof(double), 1, fptr);
        fread(sensor_ts_t, sizeof(time_t), 1, fptr);
        sbuffer_insert(buf, sensor_data);
        usleep(10000);
    }

    sensor_data->id = 0;
    sensor_data->value = 0;
    sensor_data->ts = 0;
    sbuffer_insert(buf, sensor_data);


    fclose(fptr);
    free(sensor_data);
    return NULL;
}

void *reader_thread(void *vargp){

    sbuffer_t *buf = (sbuffer_t*) vargp;
    sensor_data_t *sensor_data = malloc(sizeof(sensor_data_t));

    sbuffer_remove(buf,sensor_data);
    while (sensor_data->id != 0){

        sbuffer_remove(buf,sensor_data);
        printf("sensor id = %" PRIu16 " - temperature = %f - timestamp = %ld\n", sensor_data->id, sensor_data->value,
               (long int) sensor_data->ts);

        usleep(25000);
    }

    free(sensor_data);
    return NULL;
}

int main(){

    pthread_t write_thread_id, reader_thread_id, reader_thread_id2;
    sbuffer_t *buffer;

    if (sbuffer_init(&buffer) == SBUFFER_FAILURE){
        return -1;
    }

    printf("Before Thread\n");
    pthread_create(&write_thread_id, NULL, writer_tread, (void*)buffer);
    pthread_create(&reader_thread_id, NULL, reader_thread, (void*)buffer);
    pthread_create(&reader_thread_id2, NULL, reader_thread, (void*)buffer);
    pthread_join(write_thread_id, NULL);
    pthread_join(reader_thread_id, NULL);
    pthread_join(reader_thread_id2, NULL);
    printf("After Thread\n");

    sbuffer_free(&buffer);

    return 0;
}