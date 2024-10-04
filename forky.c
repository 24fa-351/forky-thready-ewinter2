#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sleep_random(int process_num) {
    int sleep_time = rand() % 8 + 1;
    printf("Process %d (PID: %d) beginning. Sleeping for %d seconds.\n", process_num, getpid(), sleep_time);
    sleep(sleep_time);
}

//Pattern 1: fork all processes first, then wait for them 
void pattern1(int num_of_things) {
   pid_t pids[num_of_things];

   for (int i = 0; i < num_of_things; i++) {
       pids[i] = fork();

       if(pids[i] < 0) {
           printf("Fork Failed\n");
           exit(1);
       }
       else if (pids[i] == 0) {
           sleep_random(i + 1);
           exit(0);
       }
   }

   for (int i = 0; i < num_of_things; i++) {
       printf("Process %d (PID: %d) waiting for process %d (PID: %d)\n", i + 1, getppid(), i + 2, pids[i]);
       waitpid(pids[i], NULL, 0);
       printf("Process %d (PID: %d) is exiting\n", i + 1, pids[i]);
   }

}

//Pattern 2
void pattern2(int process_num, int num_of_things) {
    sleep_random(process_num);

    if (process_num < num_of_things) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("Fork Failed\n");
            exit(1);
        }
        else if (pid == 0) {
            printf("Process %d (PID: %d) is creating Process %d (PID %d)\n", process_num, getpid(), process_num + 1, getpid());
            pattern2(process_num + 1, num_of_things);
        }
    }
    else {
        printf("Process %d (PID: %d) has no more children to create.\n", process_num, getpid());
        printf("Process %d (PID: %d) exiting.\n", process_num, getpid());
    }
}


int main(int argc, char* argv[]) {

    if (argc < 3) {
        printf("Usage: %s <command> <args>\n", argv[0]);
        return 1;
    }

    int num_of_things = atoi(argv[1]);
    int pattern_num = atoi(argv[2]);

    if (num_of_things < 1 || num_of_things > 256 || pattern_num < 1 || pattern_num > 2) {
        printf("Invalid input\n");
        return 1;
    }

    if (pattern_num == 1) {
        printf("Pattern 1 Selected.\n");
        pattern1(num_of_things);
    }
    else if (pattern_num == 2) {
        printf("Pattern 2 Selected.\n");
        pattern2(1, num_of_things);
    }


    return 0;
}