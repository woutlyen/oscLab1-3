#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_TEMP 35
#define MIN_TEMP -10

void getTemp(){
    double range = (MAX_TEMP - MIN_TEMP);
    double div = RAND_MAX / range;
    double temp = MIN_TEMP + (rand()/div);

    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );

    printf("Temperature = %0.2f°C @ %s", temp, asctime(info));
    fflush(stdout);
    sleep(1);
}

int main(){
    srand(time(NULL));
    while (1){
        getTemp();
    }

    return 0;
}