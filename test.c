#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void signalHandler(int signum) {
    printf("Received Ctrl+C. Terminating processes...\n");
    exit(0);
}

int main() {
    signal(SIGINT, signalHandler);

    pid_t child1, child2;

    if ((child1 = fork()) == 0) {
        int a = 10;
        int *b = (int*)malloc(sizeof(int));
        while (1) {
            printf("I am the first child process (PID: %d)\n", getpid());
            printf("address of a is %p\n",&a);
            printf("address of b is %p\n",b);
            sleep(1);
        }
    }
 
    if ((child2 = fork()) == 0) {
        while (1) {
            printf("I am the second child process (PID: %d)\n", getpid());
            sleep(1); 
        }
    }
  
    while (1) {
        printf("I am the parent process (PID: %d)\n", getpid());
        sleep(1); 
    }

    return 0;
}
