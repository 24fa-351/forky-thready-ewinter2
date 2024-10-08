#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void sleep_random1(int process_num, FILE *file) {
    srand(getpid());
    int sleep_time = rand() % 8 + 1;
    fprintf(file, "Process %d (PID: %d) created. Sleeping for %d seconds.\n", process_num, getpid(), sleep_time);
    sleep(sleep_time);
}

//Pattern 1
void pattern1(int num_of_things, FILE *file) {
   pid_t pids[num_of_things];

   for (int i = 0; i < num_of_things; i++) {

       fflush(file);

       pids[i] = fork();

       if(pids[i] < 0) {
           fprintf(file, "Fork Failed\n");
           exit(1);
       }
       else if (pids[i] == 0) {
           sleep_random1(i + 1, file);
           exit(0);
       }

       fprintf(file, "Parent: created child %d (PID: %d)\n", i + 1, pids[i]);
   }

   fprintf(file, "Pattern 1: All processes created\n");

   for (int i = 0; i < num_of_things; i++) {
       waitpid(pids[i], NULL, 0);
       fprintf(file, "Process %d (PID: %d) is exiting\n", i + 1, pids[i]);
   }

    fprintf(file, "Pattern 1: All children have exited\n");
}

void sleep_random2(int process_num, FILE* file) {
    srand(getpid());
    int sleep_time = rand() % 8 + 1;
    fprintf(file, "%d seconds after creating child %d (pid to be determined)\n", sleep_time, process_num + 1);
    sleep(sleep_time);
}

void pattern2_wait(int process_num, pid_t pid, FILE* file) {
    waitpid(pid, NULL, 0);
    fprintf(file, "Child %d (PID: %d), waiting for child %d (PID: %d)\n", process_num - 1, getpid(), process_num, pid);
}

//Pattern 2
void pattern2(int process_num, int num_of_things, FILE* file) {
    if (process_num + 1 <= num_of_things) {
        fflush(file);

        pid_t pid = fork();

        if (pid < 0) {
            fprintf(file, "Fork Failed\n");
            exit(1);
        }
        else if (pid == 0) {
            fprintf(file, "Child %d (PID: %d): starting\n", process_num, getpid());

            if (process_num < num_of_things) {
                fprintf(file, "Child %d (PID: %d), sleeping ", process_num, getpid());
            } 
            else {
                fprintf(file, "Child %d (PID: %d) has no more children to create.\n", process_num, getpid());
            }

            sleep_random2(process_num, file);

            if (process_num + 1 < num_of_things) {
                pattern2(process_num + 1, num_of_things, file);
            }

            fprintf(file, "Child %d (PID: %d), exiting.\n", process_num, getpid());
            exit(0);
        }
        else {
            pattern2_wait(process_num, pid, file);
            fprintf(file, "Parent: created child %d (PID: %d)\n", process_num, pid);
        }
    }
}


int main(int argc, char* argv[]) {

    FILE *file = fopen("result.txt", "w");

    if (argc < 3) {
        fprintf(file, "Usage: %s <command> <args>\n", argv[0]);
        return 1;
    }

    int num_of_things = atoi(argv[1]);
    int pattern_num = atoi(argv[2]);

    if (num_of_things < 1 || num_of_things > 256 || pattern_num < 1 || pattern_num > 2) {
        fprintf(file, "Invalid input\n");
        return 1;
    }

    if (pattern_num == 1) {
        fprintf(file, "Pattern 1 Selected. Creating %d processes...\n", num_of_things);
        pattern1(num_of_things, file);
    }
    else if (pattern_num == 2) {
        fprintf(file, "Pattern 2 Selected. Creating %d processes... \n", num_of_things);
        pattern2(1, num_of_things + 1, file);
    }

    fclose(file);

    return 0;
}