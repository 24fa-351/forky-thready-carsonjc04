#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void pattern1(int things, FILE *file) {
    fprintf(file, "Hello, World! pattern: 1, processes: %d\n", things);
    
    for (int i = 1; i <= things; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            fprintf(file, "Child process %d created (PID: %d).\n", i, getpid());
            fclose(file);  
            exit(0);
        } else {
            wait(NULL);
        }
    }
}

void pattern2(int things, FILE *file) {
    fprintf(file, "Hello, World! pattern: 2, processes: %d\n", things);

    for (int i = 1; i <= things; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        if (pid == 0) {
            fprintf(file, "Process %d (PID %d) created by parent PID %d.\n", i, getpid(), getppid());
            fclose(file);  
            exit(0);
        } else {
            wait(NULL);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <things> <pattern number>\n", argv[0]);
        exit(1);
    }

    int things = atoi(argv[1]);
    int pattern_num = atoi(argv[2]);

    if (things < 1 || things > 256) {
        fprintf(stderr, "Invalid number of things: %d\n", things);
        exit(1);
    }

    FILE *file = fopen("results.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    if (pattern_num == 1) {
        pattern1(things, file);
    } else if (pattern_num == 2) {
        pattern2(things, file);
    } else {
        fprintf(stderr, "Invalid pattern number: %d\n", pattern_num);
        fclose(file);
        exit(1);
    }

    fclose(file);
    return 0;
}
