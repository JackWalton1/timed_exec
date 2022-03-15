#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>    
#include <unistd.h>  
#include <signal.h>   
#include <string.h>

#define INTERVAL 400 //give or take 500 ms, half a second intervals
float remaining_seconds = 0;

void printClock(void) {
    fflush(stdin);
    fflush(stdout);
    char message[100];
    if(remaining_seconds < 0.0){
        return;
    }
    else if(remaining_seconds == 0.0){
        fprintf(stderr, "//Start of other program\n");
    }
    else{
        int i;
        for(i = 0;i<(int)(remaining_seconds*2);i++){
            message[i] = '.';
        }
        message[i] = '\0';
        char sub_message[20];
        strcat(message, "//");
        sprintf(sub_message, "%.2f", remaining_seconds);
        strcat(message, sub_message);
        strcat(message, " seconds until START\n");
        fprintf(stderr, "%s", message);
    }
    remaining_seconds = remaining_seconds - 0.5;
    memset(&message[0], 0, sizeof(message)); // sets to list of 0's
    //printf("Timer went off.\n");

}

int main(int argc, char *argv[]){
    remaining_seconds = atof(argv[1]);
    int total_iterations = (int)remaining_seconds*2;
    struct itimerval timer; //timer
    struct timeval s, e; //time the timer
    if (signal(SIGALRM, (void (*)(int)) printClock) == SIG_ERR) {
        perror("errror: did not catch signal");
        exit(1);
    }
    timer.it_value.tv_sec =     INTERVAL/1000;
    timer.it_value.tv_usec =    (INTERVAL*1000) % 1000000;   
    timer.it_interval = timer.it_value;
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("error setting timer");
        exit(1);
    }
    gettimeofday(&s, NULL);
    int i = 0;
    while(i<total_iterations+1){
        pause();
        i++;
    }
    pause();
    gettimeofday(&e, NULL);
    // long seconds = (e.tv_sec - s.tv_sec);
    // long micros = ((seconds * 1000000) + e.tv_usec) - (s.tv_usec);
    // printf("Time taken for timer (SS.MS):\n\t %ld.%ld\n", seconds, micros);
    int pid;

    char* program_name = argv[2];
    // fprintf(stderr,"Program name: %s\n", program_name);
    // fprintf(stderr,"Argc: %d\n", argc);
    char* arg_list[100] = {NULL}; //Can have up to 100 cmd line args
    for(int i = 3; i<argc; i++){
        arg_list[i-3] = argv[i];
    }
    pid = fork();
    if(!pid){
        execv(program_name, arg_list);
        return 0;
    }
    // if(argc>3){
    //     //MUST SPECIFY EXACTLY TWO ARGS FOR THIS CALL
    //     char* prog2arg1 = argv[3];
    //     char* prog2arg2 = argv[4];
    //     pid = fork();
    //     if(!pid){
    //         execv(program_name, (char*[]){prog2arg1, prog2arg2,NULL});
    //         return 0;
    //     }
    // }else{
    //     pid = fork();
    //     if(!pid){
    //         execv(program_name, (char*[]){NULL});
    //         return 0;
    //     }
    // }
    sleep(1);
    return 0;
    
}
