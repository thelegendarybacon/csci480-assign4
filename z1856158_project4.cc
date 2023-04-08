/********************************************************************
CSCI 480 - Assignment 4 (part 1) - Spring 2023

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
#include <semaphore.h>

using namespace std;

int READ_THREADS;                   // number of read threads
int WRITE_THREADS;                  // number of write threads
int read_count = 0;                  // number of active readers

sem_t rw_sem;                   // used for readers and writers
sem_t cs_sem;                   // used for critical sections for readers

char* phrase = "General Kenobi";


void *writer(void *data)
{
    sem_wait(&rw_sem);          // ENTER CRITICAL

    // variables
    char *temp = (char*)data;

    // loops through string
    for(int i = 0; temp[i] != '\0'; i++)
    {
        // print currently writing
        printf("writer %d is writing...\n", VAR);

        // cut off last character of string
        temp[strlen(temp)-1] = '\0';

        // sleep 1 second
        sleep(1);
    }

    sem_post(&rw_sem);          // EXIT CRITICAL
    pthread_exit(NULL);
}

void *reader(void *data)
{
    char *temp = (char*)data;
    read_count++;
    printf("read_count incremented to: %d\n", read_count);

    for(int i = 0; temp[i] != '\0'; i++)
    {
        printf("reader %d is reading ... content : %s\n", VAR, temp);
    }

    read_count--;
    printf("read_count decremented to: %d\n", read_count);

}

int main (int argc, char *argv[])
{
    // Check for errors with passed arguments
    if(argc == 3)
    {
        READ_THREADS = atoi(argv[1]);
        WRITE_THREADS = atoi(argv[2]);
    }



    // Initial Output
    printf("***** Reader-Write Problem Simulation *****\n");
    printf("Number of reader threads: %d\n", READ_THREADS);
    printf("Number of writer threads: %d\n\n", WRITE_THREADS);

    // Initialize semaphores
    if(sem_init(&rw_sem, 0, 1) == -1)
    {
        printf("Error: Failed to initialize semaphore\n");
        exit(-1);
    }
    if(sem_init(&cs_sem, 0, 1) == -1)
    {
        printf("Error: Failed to initialize semaphore\n");
        exit(-1);
    }

    // Create read/write threads
    pthread_t read[READ_THREADS], write[WRITE_THREADS];
    int rc1, rc2;

    for(long a = 0; a < READ_THREADS; a++)
    {
        rc1 = pthread_create(&read[a], NULL, reader, (void *)phrase);
        if(rc1)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc1);
            exit(-1);
        }
    }

    for(long b = 0; b < READ_THREADS; b++)
    {
        rc2 = pthread_create(&write[b], NULL, writer, (void *)phrase);
        if(rc2)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc2);
            exit(-1);
        }
    }


    // Wait for read & write threads to finish
    pthread_join(read, NULL);
    pthread_join(write, NULL);

    // Cleanup & exit
    sem_destroy(&rw_sem);
    sem_destroy(&cs_sem);
    pthread_exit(NULL);
}