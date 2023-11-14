#include<stdio.h>
#include<unistd.h>
#include <string.h>
#include <ctype.h>

int main() {
    int fd[2];
    int returnstatus;
    int pid;
    char *writemessage = "brOdeR";
    char readmessage[20];

    returnstatus = pipe(fd);

    if (returnstatus == -1) {
        printf("Unable to create pipe\n");
        return 1;
    }
    pid = fork();

    // Child process
    if (pid == 0) {
        read(fd[0], readmessage, sizeof(readmessage));

        for (int count = 0; count < strlen(readmessage); count++) {
            if(islower(readmessage[count])){
                readmessage[count] = (signed char)toupper(readmessage[count]);
            }
            else{
                readmessage[count] = (signed char)tolower(readmessage[count]);
            }
        }
        printf("Child Process - Reading from pipe – Message 1 is %s\n", readmessage);

    }


    else { //Parent process
        printf("Parent Process - Writing to pipe - Message 1 is %s\n", writemessage);
        write(fd[1], writemessage, strlen(writemessage));
    }


    return 0;
}