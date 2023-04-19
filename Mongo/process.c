#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define RESULTS_PATH "/home/ubuntu/results/";
const char* PROCESS[] = {"a", "b", "c"};

int maint () {
    // Pour chaque workload
    for (int i = 0; i< 3; i++) {
        if(fork()){
            char filepath[] = strcat(RESULTS_PATH,PROCESS[i]); 
            execl("mkdir", filepath);
        }

        int docker_status_pid = fork();

        if (docker_status_pid == 0) {
            execl("python", PROCESS[i]); // execute le process de docker-stats;
        } else {
            for(int i= 0; i< 5; i ++) {
                int run_workload_pid = fork();

                if (run_workload_pid == 0) {
                    int load_workload_pid = fork();

                    if(load_workload_pid == 0) {
                         // exec le load
                        execl("");
                    } else {
                        // attend la fin du load
                        wait(NULL); 
                        // exec le run
                        execl("");
                    }
                } else {
                    wait(NULL);
                }
            }

            kill(docker_status_pid);
        }
    }
}
