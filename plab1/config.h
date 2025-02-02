/**
 * \author {AUTHOR}
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define RUN_AVG_LENGTH 5
#define SET_MAX_TEMP 30
#define SET_MIN_TEMP 0

#include <stdint.h>
#include <time.h>

typedef uint16_t sensor_id_t;
typedef double sensor_value_t;
typedef time_t sensor_ts_t;         // UTC timestamp as returned by time() - notice that the size of time_t is different on 32/64 bit machine

/**
 * structure to hold sensor data
 */
typedef struct {
    sensor_id_t id;         /** < sensor id */
    sensor_value_t value;   /** < sensor value */
    sensor_ts_t ts;         /** < sensor timestamp */
} sensor_data_t;


#endif /* _CONFIG_H_ */
