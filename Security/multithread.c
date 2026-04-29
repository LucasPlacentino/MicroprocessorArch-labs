/**
 * multithread.c: Example for POSIX Threads.
 *
 * This needs to be compiled with the pthread library.
 * Command line: gcc -lpthread multithread.c
 * In CodeBlocks: in the project build options, go in the "Linker settings"
 * tab and add -lpthread in the "Other linker options" area.
**/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <immintrin.h>
#include <time.h>

#define MAXTHREADS 3


// Define a structure for the thread arguments
typedef struct Args Args;
struct Args {
    int i;
};

// Time per thread
struct timespec ttimes[MAXTHREADS + 1];

// Create a global variable
int g = 0;

// Create a global mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// This function is the entry point of the thread
void *myThreadFun(Args *vargp)
{
    // Store the argument passed to this thread
    Args *arg = vargp;

    // Create a static variable
    static int s = 0;

    // Normal int variable
    int n = 0;

    ++s;
    pthread_mutex_lock(&mutex);
    ++g;
    pthread_mutex_unlock(&mutex);
    ++n;

    printf("Thread ID: %03d, Static: %d, Global: %d, Normal: %d\n",
      arg->i, s, g, n);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ttimes[arg->i]);
    
    pthread_exit(NULL);
}


int main()
{
    int i;

    // Create thread IDs
    pthread_t *tid = malloc(MAXTHREADS * sizeof(pthread_t));
    assert (tid != NULL);

    // Create three threads
    for (i = 0; i < MAXTHREADS; i++) {
        Args *arg = (Args*) malloc(sizeof(Args));
        assert (arg != NULL);
        arg->i = i;
        pthread_create(&tid[i], NULL, (void *) myThreadFun, arg);
    }

    // Wait for all the threads to finish
    for (i = 0; i < MAXTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    free (tid);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ttimes[MAXTHREADS]);
    double elapsed_threads = 0;
    for (i = 0; i <= MAXTHREADS; i++) {
      long seconds = ttimes[i].tv_sec;
      long nanoseconds = ttimes[i].tv_nsec;
      double elapsed = seconds + nanoseconds*1e-9;
      if (i < MAXTHREADS) {
        elapsed_threads += elapsed;
        printf("Time %03d: %.9f seconds.\n", i, elapsed); }
      else {
        printf("Total threads: %.9f seconds.\n", elapsed_threads);
        printf("Total process: %.9f seconds.\n", elapsed); }
    }

    printf("That's it, folks!\n");

    return EXIT_SUCCESS;
}
