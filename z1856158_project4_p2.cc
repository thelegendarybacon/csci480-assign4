/********************************************************************
CSCI 480 - Assignment 4 (part 2) - Spring 2023

Progammer: Alec Bakker
Section: 0001
TA: Daniel Feller 
Date Due:  4/11/2023

Purpose: Implements a solution to the reader/writer problem using
         the pthread library.
*********************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

int READ_THREADS = 1;               // number of read threads
int WRITE_THREADS;                  // number of write threads

sem_t a_sem;                       // used for readers and writers
sem_t b_sem;                       // used for critical sections for readers

char phrase[] = "Have you heard the tragedy of Darth Plagueis the Wise.";


void *writer(void *data)
{
    // variables
    long w_id = (long)data;

    // loops through string
    for(int i = 0; strlen(phrase) > 0; i++)
    {
        sem_wait(&a_sem);

        // print currently writing
        printf("writer %d is writing...\n", w_id);

        // cut off last character of string
        phrase[strlen(phrase) - 1] = '\0';

        sem_post(&b_sem);

        // sleep 1 second
        sleep(1);
    }

    printf("writer %d is exiting...\n", w_id);
    pthread_exit(NULL);
}

void *reader(void *data)
{
    // Initialize variables
    long r_id = (long)data;

    for(int i = 0; strlen(phrase) > 0; i++)
    {
        sem_wait(&b_sem);

        printf("reader %d is reading ... content : %s\n", r_id, phrase);
        
        for(int i = 0; i < (WRITE_THREADS - READ_THREADS); i++)
            sem_post(&a_sem);

        sleep(1);
    }

    printf("reader %d is exiting...\n", r_id);
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
    // Check for errors with passed arguments
    if(argc == 3)
    {
        READ_THREADS = atoi(argv[1]);
        WRITE_THREADS = atoi(argv[2]);
    }
    else
    {
        printf("Invalid arguments. Usage: ./filename.exe (# read threads) (# write threads)\n");
        printf("Example: ./filename.exe 5 3\n");
        exit(-1);
    }

    // Initial Output
    printf("***** Reader-Writer Problem Simulation *****\n");
    printf("Number of reader threads: %d\n", READ_THREADS);
    printf("Number of writer threads: %d\n\n", WRITE_THREADS);

    // Initialize semaphores
    if(sem_init(&a_sem, 0, 1) == -1)
    {
        printf("Error: Failed to initialize semaphore\n");
        exit(-1);
    }
    if(sem_init(&b_sem, 0, 0) == -1)
    {
        printf("Error: Failed to initialize semaphore\n");
        exit(-1);
    }

    // Create read/write threads
    pthread_t read[READ_THREADS], write[WRITE_THREADS];
    int rc1, rc2;

    for(long a = 0; a < READ_THREADS; a++)
    {
        rc1 = pthread_create(&read[a], NULL, reader, (void *)a);
        if(rc1)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc1);
            exit(-1);
        }
    }

    for(long b = 0; b < WRITE_THREADS; b++)
    {
        rc2 = pthread_create(&write[b], NULL, writer, (void *)b);
        if(rc2)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc2);
            exit(-1);
        }
    }


    // Wait for read & write threads to finish
    for (int i = 0; i< READ_THREADS; i++)
        pthread_join(read[i], NULL);

    for (int i = 0; i < WRITE_THREADS; i++)
        pthread_join(write[i], NULL);

    printf("All threads are done\n");

    // Cleanup & exit
    sem_destroy(&rw_sem);
    sem_destroy(&cs_sem);

    printf("Resources cleaned up\n");

    pthread_exit(NULL);
}