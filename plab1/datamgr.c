/**
 * \Author: Wout Lyen
 */

#define _GNU_SOURCE
#include "datamgr.h"
#include "lib/dplist.h"

#include <assert.h>

typedef struct {
    uint16_t sensorID;
    uint16_t roomID;
    float avg;
    time_t last_modified;
} element_t;


void * element_copy(void * element) {
    element_t* copy = malloc(sizeof (element_t));
    //char* new_name;
    //asprintf(&new_name,"%s",((element_t*)element)->name); //asprintf requires _GNU_SOURCE
    assert(copy != NULL);
    copy->roomID = ((element_t*)element)->roomID;
    copy->sensorID = ((element_t*)element)->sensorID;
    copy->avg = ((element_t*)element)->avg;
    copy->last_modified = ((element_t*)element)->last_modified;
    return (void *) copy;
}

void element_free(void ** element) {
    free(*element);
    *element = NULL;
}

int element_compare(void * x, void * y) {
    //TODO implement this feedback function
    //return ((((element_t*)x)->id < ((element_t*)y)->id) ? -1 : (((element_t*)x)->id == ((element_t*)y)->id) ? 0 : 1);
    return -1;
}


void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data){

    //Malloc one element_t to temporarily store the data of one line in the room_sensor_map file
    element_t *element = (element_t*)malloc(sizeof(element_t));

    //Initialise dplist with feedback functions
    dplist_t *list = dpl_create(element_copy, element_free, element_compare);

    //Add all the assigned sensorIDs to a roomID in the list
    char line[255];
    while (fgets(line, 255, fp_sensor_map) != NULL){
        sscanf(line,"%hu %hu", &(element->roomID), &(element->sensorID));
        dpl_insert_at_index(list, element, dpl_size(list), true);
        //printf("%hu %hu\n", element->roomID, element->sensorID );
    }


    uint16_t sensorID;
    double temperature;
    time_t timestamp;

    fread(&sensorID, sizeof(uint16_t), 1, fp_sensor_data);
    fread(&temperature, sizeof(double), 1, fp_sensor_data);
    fread(&timestamp, sizeof(time_t), 1, fp_sensor_data);
    printf("%s", asctime(gmtime(&timestamp)));
}


void datamgr_free(){

}


uint16_t datamgr_get_room_id(sensor_id_t sensor_id){

}


sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){

}


time_t datamgr_get_last_modified(sensor_id_t sensor_id){

}


int datamgr_get_total_sensors(){

}