#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include<sys/wait.h>


char* RESULTS_PATH = "/home/ubuntu/results/";
char PROCESS[] = {'a', 'b', 'c'};

int main() {
    int docker_status_pid = fork();
    if (docker_status_pid == 0) {
            execl("/usr/bin/bash", "bin/bash", "-c", "python /home/ubuntu/LOG-8430-TP3/Mongo/script.py", NULL); // execute le process de docker-stats;
    }

    // Pour chaque workload
    for (int i = 0; i< 3; i++) {
            sleep(3);
            for(int j= 0; j< 5; j ++) {
		char* path = "/home/ubuntu/results/";
                char filepath[200];
	        sprintf(filepath, "%s%c",path, PROCESS[i]); 

                char postfix [500];
                sprintf(postfix, "mongodb-async -s -P /home/ubuntu/ycsb-0.17.0/workloads/workload%c -p recordcount=1000 -p mongodb.upsert=true -p mongodb.url=mongodb://primary:27017,secondary1:27017,secondary2:27017,secondary3:27017/?replicaSet=myReplicaSet", PROCESS[i]);
		
		char outputpath[600];
		sprintf(outputpath, "%s/output.csv", filepath);

                int load_pid = fork();
                if (load_pid == 0){
			char execCommand[600];
                        sprintf(execCommand, "%s%s%s%c%s", "/home/ubuntu/ycsb-0.17.0/bin/ycsb load ", postfix, ">> /home/ubuntu/results/", PROCESS[i], "/output.csv");
                        execl("/usr/bin/bash", "/bin/bash", "-c", execCommand, NULL);
                }
                wait(NULL);
                sleep(3);
                int run_pid = fork();
                if (run_pid == 0) {
			char execCommand[600];
			sprintf(execCommand, "%s%s%s%c%s", "/home/ubuntu/ycsb-0.17.0/bin/ycsb run ", postfix, ">> /home/ubuntu/results/", PROCESS[i], "/output.csv");
			execl("/usr/bin/bash", "/bin/bash", "-c", execCommand, NULL);
                        
                }
                wait(NULL);
                sleep(3);
            }
   }
   kill(docker_status_pid, 1);
} 
