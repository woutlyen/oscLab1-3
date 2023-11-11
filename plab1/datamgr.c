/**
 * \Author: Wout Lyen
 */

#include "datamgr.h"
#include "lib/dplist.h"

#include <assert.h>

typedef struct {
    uint16_t sensorID;
    uint16_t roomID;
    double avg;
    time_t last_modified;
} element_t;

dplist_t *list;

void * element_copy(void * element) {
    element_t* copy = malloc(sizeof (element_t));
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
    return (((element_t*)x)->sensorID == ((element_t*)y)->sensorID ? 0 : 1);
}


void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data){
    //TODO logging data to the stderr file
    //TODO calculating the average temperature
    //TODO implement ERROR_HANDLER

    //Malloc one element_t to temporarily store the data of one line in the room_sensor_map file
    element_t *element = (element_t*)malloc(sizeof(element_t));


    //Initialise dplist with feedback functions
    list = dpl_create(element_copy, element_free, element_compare);


    //Reading and adding all the assigned sensorIDs to roomIDs in the list
    char line[255];
    while (fgets(line, 255, fp_sensor_map) != NULL){
        sscanf(line,"%hu %hu", &(element->roomID), &(element->sensorID));
        dpl_insert_at_index(list, element, dpl_size(list), true);
        //printf("%hu %hu\n", element->roomID, element->sensorID );
    }


    //Declaration of variables in the fp_sensor_data file
    sensor_data_t *sensor_data = (sensor_data_t*)malloc(sizeof(sensor_data_t));
    uint16_t *sensor_id_t = &sensor_data->id;
    double *sensor_value_t = &sensor_data->value;
    time_t *sensor_ts_t = &sensor_data->ts;

    //Reading and adding all the assigned sensor data in the list
    while(fread(sensor_id_t, sizeof(uint16_t), 1, fp_sensor_data) != 0) {
        fread(sensor_value_t, sizeof(double), 1, fp_sensor_data);
        fread(sensor_ts_t, sizeof(time_t), 1, fp_sensor_data);

        //Print the time_t in human language
        //printf("%s", asctime(gmtime(&timestamp)));

        for (int count = 0; count < dpl_size(list); count++) {

            element_t *element_at_index = (element_t*)dpl_get_element_at_index(list, count);

            if (element_at_index->sensorID == *sensor_id_t){
                element_at_index->avg = *sensor_value_t;
                element_at_index->last_modified = *sensor_ts_t;
                printf("%hu %f %lld \n", element_at_index->sensorID, element_at_index->avg, (long long)element_at_index->last_modified);
                break;
            }

            if (count == dpl_size(list)-1){
                printf("%s %hu %s\n", "sensorID ", *sensor_id_t, " not found");
            }
        }
    }

    free(element);
}


void datamgr_free(){
    dpl_free(&list, true);
}


uint16_t datamgr_get_room_id(sensor_id_t sensor_id){

        for (int count = 0; count < dpl_size(list); count++) {
            element_t *element = dpl_get_element_at_index(list, count);
        if(element->sensorID == sensor_id){
            return element->roomID;
        }
    }
    ERROR_HANDLER(true, "Invalid sensor_id");
}


sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){

}


time_t datamgr_get_last_modified(sensor_id_t sensor_id){

}


int datamgr_get_total_sensors(){

}