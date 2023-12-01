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

/*
pthread_mutex_t lock;
pthread_cond_t cond;
int buffer_size = 0;
*/

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

        //pthread_mutex_lock(&lock);
        sbuffer_insert(buf, sensor_data);
        //buffer_size++;
        //pthread_cond_signal(&cond);
        //pthread_mutex_unlock(&lock);

        usleep(10000);
    }

    sensor_data->id = 0;
    sensor_data->value = 0;
    sensor_data->ts = 0;

    //pthread_mutex_lock(&lock);
    sbuffer_insert(buf, sensor_data);
    //buffer_size++;
    //pthread_cond_broadcast(&cond);
    //pthread_mutex_unlock(&lock);

    fclose(fptr);
    free(sensor_data);
    return NULL;
}

void *reader_thread(void *vargp){

    sbuffer_t *buf = (sbuffer_t*) vargp;
    sensor_data_t *sensor_data = malloc(sizeof(sensor_data_t));

    while(1){

        /*
        pthread_mutex_lock(&lock);
        if (buffer_size == 0){
            pthread_cond_wait(&cond, &lock);
        }
         */

        sbuffer_remove(buf,sensor_data);

        //printf("%d\n", buffer_size);
        if (sensor_data->id == 0){
            break;
        }

        //buffer_size--;
        //pthread_mutex_unlock(&lock);



        FILE *file = fopen("sensor_data_out.csv", "a");
        fprintf(file,"sensor id = %" PRIu16 " - temperature = %f - timestamp = %ld\n", sensor_data->id, sensor_data->value,
                (long int) sensor_data->ts);
        fclose(file);
        printf("sensor id = %" PRIu16 " - temperature = %f - timestamp = %ld\n", sensor_data->id, sensor_data->value, (long int) sensor_data->ts);
        usleep(25);

    }
    free(sensor_data);
    return NULL;
}

int main(){
    for (int i = 0; i < 1; i++) {
        pthread_t write_thread_id, reader_thread_id, reader_thread_id2;
        sbuffer_t *buffer;

        if (sbuffer_init(&buffer) == SBUFFER_FAILURE) {
            return -1;
        }

        /*
        if (pthread_mutex_init(&lock, NULL) != 0) {
            printf("\n mutex init has failed\n");
            return 1;
        }

        if (pthread_cond_init(&cond, NULL) != 0) {
            printf("\n condition init has failed\n");
            return 1;
        }
        */

        printf("Before Thread\n");
        pthread_create(&write_thread_id, NULL, writer_tread, (void *) buffer);
        pthread_create(&reader_thread_id, NULL, reader_thread, (void *) buffer);
        pthread_create(&reader_thread_id2, NULL, reader_thread, (void *) buffer);
        pthread_join(write_thread_id, NULL);
        pthread_join(reader_thread_id, NULL);
        pthread_join(reader_thread_id2, NULL);
        printf("After Thread\n");

        sbuffer_free(&buffer);

        //pthread_mutex_destroy(&lock);
    }
    return 0;
}