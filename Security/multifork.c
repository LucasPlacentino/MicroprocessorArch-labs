/**
 * multifork.c: Example for spawning processes.
 *
 * This needs to be compiled with -mrdrnd to enable architecture support for
 * RDRAND in GCC.
 * Command line: gcc -mrdrnd multifork.c
 * RDRAND is not supported by all architectures and if you are working on
 * this exercise on a processor without RDRAND, you may have to substitute
 * _rdrand16_step() below with a different source of randomness or with
 * deterministic behaviour.
**/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <immintrin.h>

#define MAXCHILDREN 3


// Create a global variable
int g = 0;

// This function is the workload for child processes
void myThreadFun(void)
{
    // Create a static variable
    static int s = 0;

    // Normal int variable
    int n = 0;

    ++s;
    ++g;
    ++n;

    // This construct sometimes crashes a child process due to a
    // NULL dereference. We use this to simulate a programming error.
    unsigned short r;
    _rdrand16_step (&r);      // RDRAND requires -mrdrnd
    if ((r % 10) + 1 > 7) {
        int* i = NULL;
        printf("crashing... (Process ID: %d)\n", getpid());
        printf("%d\n", *i);   // *i == NULL
    }

    // Output variable states
    printf("Process ID: %d, Static: %d, Global: %d, Normal: %d\n",
      getpid(), s, g, n);
    return;
}


int main()
{
    int i;
    pid_t p;
    pid_t Children[MAXCHILDREN];
    siginfo_t siginfo;

    // Create three child processes
    for (i = 0; i < MAXCHILDREN; i++) {
        p = fork();
        // This system call creates a new process by duplicating the calling process. The new process (child) and the calling process (parent) run concurrently from the exact instruction following the fork() call. To distinguish execution paths, fork() returns 0 to the child process and returns the child's Process ID (PID) to the parent. If it fails, it returns a value < 0.
        Children[i] = p;
        if (p < 0) {          // fork() failed
            perror ("fork fail\n");
            exit (1);
        } else if (p == 0) {  // p=0 means this is a child that is executing
          myThreadFun();
          break;              // remove this and explain
          // If removed, the child process does not exit the for loop after executing myThreadFun(). Instead, it will continue to the next iteration of the loop, call fork() itself, and spawn its own children. This creates a branching hierarchy of processes (a classic fork bomb vulnerability), rather than the intended flat architecture of one parent and three children.
        }
    }

    // Wait for all children to terminate
    if (p > 0) {              // parent is executing (p=0 if child is executing, and p is child's PID if parent is executing)
        int success = 0;
        int crashed = 0;

        for (i = 0; i < MAXCHILDREN; i++) {
            waitid (P_PID, Children[i], &siginfo, WEXITED);
            // This system call forces the parent process to suspend execution until a specified child changes state (e.g., terminates or crashes). In the provided code, waitid(P_PID, Children[i], &siginfo, WEXITED) specifies waiting for the exact PID stored in Children[i] to exit. It populates the siginfo_t structure (siginfo), which provides granular metadata about how the child process terminated.

            // Check termination status
            if (siginfo.si_code == CLD_EXITED) {
                success++;
            } else if (siginfo.si_code == CLD_KILLED || siginfo.si_code == CLD_DUMPED) {
                crashed++;
            }

        }

        printf("Successful children: %d / %d\n", success, MAXCHILDREN);
        printf("Crashed children: %d / %d\n", crashed, MAXCHILDREN);
        printf("That's it, folks!\n");
    }

    return EXIT_SUCCESS;
}
