#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int i;
    int iterations;
    int p = 0;
    int n;

    if (argc != 2) {
        fprintf(stderr, "Usage: forkloop <iterations>\n");
        exit(1);
    }

    iterations = strtol(argv[1], NULL, 10);

    for (i = 0; i < iterations; i++) {
        if (p == 0) {
            n = fork();
            p = 1;
        }
        if (n == 0) {  
            p = 0;  
        }
        if (n < 0) {  
            perror("fork");  
            exit(1);  
        }  
        printf("ppid = %d, pid = %d, i = %d\n", getppid(), getpid(), i);
    }
    sleep(1);
    return 0;
}
