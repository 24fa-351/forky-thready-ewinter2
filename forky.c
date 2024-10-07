#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void sleep_random1(int process_num) {
    srand(getpid());
    int sleep_time = rand() % 8 + 1;
    printf("Process %d (PID: %d) created. Sleeping for %d seconds.\n", process_num, getpid(), sleep_time);
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
           sleep_random1(i + 1);
           exit(0);
       }

       printf("Parent: created child %d (PID: %d)\n", i + 1, pids[i]);
   }

   printf("Pattern 1: All processes created\n");

   for (int i = 0; i < num_of_things; i++) {
       waitpid(pids[i], NULL, 0);
       printf("Process %d (PID: %d) is exiting\n", i + 1, pids[i]);
   }

    printf("Pattern 1: All children have exited\n");
}

void sleep_random2(int process_num) {
    srand(getpid());
    int sleep_time = rand() % 8 + 1;
    printf("%d seconds after creating child %d (pid to be determined)\n", sleep_time, process_num + 1);
    sleep(sleep_time);
}

void pattern2_wait(int process_num, pid_t pid) {
    waitpid(pid, NULL, 0);
    printf("Child %d (PID: %d), waiting for child %d (PID: %d)\n", process_num - 1, getpid(), process_num, pid);
}

//Pattern 2
void pattern2(int process_num, int num_of_things) {
    if (process_num < num_of_things) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("Fork Failed\n");
            exit(1);
        }
        else if (pid == 0) {
            printf("Child %d (PID: %d): starting\n", process_num, getpid());

            if (process_num < num_of_things) {
                printf("Child %d (PID: %d), sleeping ", process_num, getpid());
            } 
            else {
                printf("Child %d (PID: %d) has no more children to create.\n", process_num, getpid());
            }

            sleep_random2(process_num);

            if (process_num + 1 < num_of_things) {
                pattern2(process_num + 1, num_of_things);
            }

            printf("Child %d (PID: %d), exiting.\n", process_num, getpid());
            exit(0);
        }
        else {
            pattern2_wait(process_num, pid);
            printf("Parent: created child %d (pid %d)\n", process_num, pid);
        }
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
        printf("Pattern 1 Selected. Creating %d processes...\n", num_of_things);
        pattern1(num_of_things);
    }
    else if (pattern_num == 2) {
        printf("Pattern 2 Selected. Creating %d processes... \n", num_of_things);
        pattern2(1, num_of_things);
    }


    return 0;
}