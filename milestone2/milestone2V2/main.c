#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#include <sys/types.h>

#include "sensor_db.h"
#include "logger.h"
#include "config.h"

/**
 * Opening and closing the same database file twice
 * */
void test1(){
    FILE *f = NULL;
    f = open_db("sensor_db.csv", true);
    f = open_db("sensor_db.csv", true);
    close_db(f);
    close_db(f);
}


/**
 * Inserting data of 100 sensors as fast as possible
 */
void test2(){
    FILE *f = open_db("sensor_db.csv", true);
    for (int i = 1; i <= 100; i++) {
        sensor_id_t id = i;
        sensor_value_t v = 0.001;
        sensor_ts_t ts = time(NULL);
        insert_sensor(f, id, v, ts);
    }
    close_db(f);
    sleep(1);
}


/**
 * Empty database name
 */
void test3(){
    FILE *f = open_db(NULL, true);
    sensor_id_t id = 10;
    sensor_value_t v = 0.003;
    sensor_ts_t ts = time(NULL);
    insert_sensor(f, id, v, ts);
    close_db(f);
}


/**
 * Inserting data in a not opened file e.g. NULL pointer
 */
void test4(){
    sensor_id_t id = 1;
    sensor_value_t v = 0.005;
    sensor_ts_t ts = time(NULL);
    insert_sensor(NULL, id, v, ts);

    FILE *f = open_db("sensor_db.csv", true);
    id = 1;
    v = 0.005;
    ts = time(NULL);
    insert_sensor(NULL, id, v, ts);
    close_db(f);
}


/**
 * Calling the functions out of order
 */
void test5(){
    FILE *f = NULL;
    close_db(f);
    sensor_id_t id = 1;
    sensor_value_t v = 0.005;
    sensor_ts_t ts = time(NULL);
    insert_sensor(NULL, id, v, ts);
    f = open_db("sensor_db.csv", true);
    close_db(f);
}

/**
 * Opening and closing the same database file twice with different pointers
 * */
void test6(){
    FILE *f1 = open_db("sensor_db.csv", true);
    FILE *f2 = open_db("sensor_db.csv", true);
    close_db(f1);
    close_db(f2);
}

/**
 * Provided test
 */
void test_main(){
    FILE *f = open_db("sensor_db.csv", true);

    sleep(1);
    sensor_id_t id = 1;
    sensor_value_t v = 0.001;
    sensor_ts_t ts = time(NULL);
    insert_sensor(f, id, v, ts);

    id = 2;
    v = 0.002;
    ts = time(NULL);
    insert_sensor(f, id, v, ts);
    id = 3;
    v = 0.003;
    ts = time(NULL);
    insert_sensor(f, id, v, ts);
    sleep(5);
    insert_sensor(f, 4, v, ts);
    close_db(f);
}

int main()
{

    test_main();
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();

    return 0;
}
