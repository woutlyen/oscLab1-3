//
// Created by Wout Lyen on 15/11/23.
//

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "logger.h"

#define SIZE 1
#define READ_END 0
#define WRITE_END 1

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


int fd[2];

int write_to_log_process(char *msg){

    // Initialize the sequence_number
    static int sequence_number = 0;

    // Open the log file
    FILE *file = fopen("gateway.log", "a");

    // Check if file opened properly
    if(file == NULL){
        return -1;
    }

    // Timestamp calculation
    time_t tm;
    time(&tm);
    char* time = ctime(&tm);
    time[strlen(time)-1] = '\0';

    // Print to the log file
    int value = fprintf(file, "%d - %s - %s\n", sequence_number, time, msg);

    // Check if print succeeded
    if(value <=0){
        return -1;
    }

    // Close the log file and increment the sequence number
    sequence_number++;
    return fclose(file);
}

int create_log_process(){

    // Initialize the pid variable
    pid_t pid;

    // create the pipe
    if (pipe(fd) == -1){
        if(write_to_log_process("Error while setting up the pipe.") == -1){
            printf("No log file opened: Pipe failed\n");
        }
        return -1;
    }

    // Fork the child
    pid = fork();

    // Fork error
    if (pid < 0){
        if(write_to_log_process("Error while setting up the fork.") == -1){
            printf("No log file opened: Fork failed\n");
        }
        return -1;
    }

    // Parent process
    if (pid > 0){

        // Close the read end of the parent
        close(fd[READ_END]);
    }
    else{ // Child process

        // Close the write end of the child
        close(fd[WRITE_END]);

        char msg = NO_MESSAGE;
        while (msg != END){

            long length = read(fd[READ_END], &msg, SIZE);
            if (length <= 0){
                write_to_log_process("Log process ended. Data file not closed.");
                msg = END;
            }

            //printf("%c\n", msg+48);
            if (msg == INSERT){
                write_to_log_process("Data inserted.");
            }
            else if (msg == INSERT_ERROR){
                write_to_log_process("ERROR: Failed to insert data.");
            }
            else if (msg == CLOSE){
                write_to_log_process("Data file closed.");
                msg = END;
            }
            else if (msg == CLOSE_ERROR){
                write_to_log_process("ERROR: Failed to close file.");
                msg = END;
            }
            else if (msg == OPEN){
                write_to_log_process("Data file opened.");
            }
            else if (msg == OPEN_ERROR){
                write_to_log_process("ERROR: Failed to open file.");
                msg = END;
            }
            else if (msg == FILENAME_EMPTY){
                write_to_log_process("ERROR: No filename.");
            }
            else if (msg == ALREADY_CHILD){
                write_to_log_process("ERROR: Database file already opened. First close the previous one. Pointer to already opened file returned.");
            }
        }
        end_log_process();
    }

    return fd[WRITE_END];
}

int end_log_process(){
    close(fd[READ_END]);
    exit(0);
    //return 0;
}