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
    ERROR_HANDLER(!copy, "Malloc of the element_t copy struct failed");
    //assert(copy != NULL);
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
    return (((element_t*)x)->sensorID == ((element_t*)y)->sensorID ? 0 : 1);
}


typedef struct {
    uint16_t sensorID;
    int counter;
    double sum;
} avg_t;

dplist_t *avg_list;

void * element_copy_avg(void * element) {
    avg_t * copy = malloc(sizeof (avg_t));
    ERROR_HANDLER(!copy, "Malloc of the element_t copy struct failed");
    //assert(copy != NULL);
    copy->sensorID = ((avg_t *)element)->sensorID;
    copy->counter = ((avg_t *)element)->counter;
    copy->sum = ((avg_t *)element)->sum;
    return (void *) copy;
}


void datamgr_parse_sensor_files(FILE *fp_sensor_map, FILE *fp_sensor_data){
    //TODO logging data to the stderr file

    //Malloc one element_t to temporarily store the data of one line in the room_sensor_map file
    element_t *element = (element_t*)malloc(sizeof(element_t));
    ERROR_HANDLER(!element, "Malloc of the element_t struct failed");
    avg_t *avg = (avg_t*)malloc(sizeof(avg_t));
    ERROR_HANDLER(!avg, "Malloc of the avg_t struct failed");

    //Initialise dplist with feedback functions
    list = dpl_create(element_copy, element_free, element_compare);
    avg_list = dpl_create(element_copy_avg, element_free, element_compare);

    //Reading and adding all the assigned sensorIDs to roomIDs in the list
    char line[255];
    while (fgets(line, 255, fp_sensor_map) != NULL){
        sscanf(line,"%hu %hu", &(element->roomID), &(element->sensorID));
        dpl_insert_at_index(list, element, dpl_size(list), true);

        avg->sensorID = element->sensorID;
        avg->counter = 0;
        dpl_insert_at_index(avg_list, avg, dpl_size(avg_list), true);
        //printf("%hu %hu\n", element->roomID, element->sensorID );
    }


    //Declaration of variables in the fp_sensor_data file
    sensor_data_t *sensor_data = (sensor_data_t*)malloc(sizeof(sensor_data_t));
    ERROR_HANDLER(!sensor_data, "Malloc of the sensor_data Struct failed");
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
                avg_t *avg_at_index = (avg_t*)dpl_get_element_at_index(avg_list, count);
                if(avg_at_index->counter < RUN_AVG_LENGTH){
                    avg_at_index->counter = avg_at_index->counter+1;
                    avg_at_index->sum = avg_at_index->sum+(*sensor_value_t);
                    element_at_index->avg = 0;

                    if (avg_at_index->counter == RUN_AVG_LENGTH){
                        element_at_index->avg = (avg_at_index->sum)/RUN_AVG_LENGTH;
                    }
                }
                else{
                    element_at_index->avg = ((RUN_AVG_LENGTH-1.0)/RUN_AVG_LENGTH)*(element_at_index->avg)+(1.0/RUN_AVG_LENGTH)*(*sensor_value_t);
                }
                element_at_index->last_modified = *sensor_ts_t;
                //printf("%hu %f %lld \n", element_at_index->sensorID, element_at_index->avg, (long long)element_at_index->last_modified);
                break;
            }

            if (count == dpl_size(list)-1){
                printf("%s %hu %s\n", "sensorID ", *sensor_id_t, " not found");
            }
        }
    }

    free(element);
    free(avg);
    free(sensor_data);
}


void datamgr_free(){
    dpl_free(&list, true);
    dpl_free(&avg_list, true);
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
    for (int count = 0; count < dpl_size(list); count++) {
        element_t *element = dpl_get_element_at_index(list, count);
        if(element->sensorID == sensor_id){
            return element->avg;
        }
    }
    ERROR_HANDLER(true, "Invalid sensor_id");
}


time_t datamgr_get_last_modified(sensor_id_t sensor_id){
    for (int count = 0; count < dpl_size(list); count++) {
        element_t *element = dpl_get_element_at_index(list, count);
        if(element->sensorID == sensor_id){
            return element->last_modified;
        }
    }
    ERROR_HANDLER(true, "Invalid sensor_id");
}


int datamgr_get_total_sensors(){
    return dpl_size(list);
}