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
#include <unistd.h>
#include <semaphore.h>

using namespace std;

int READ_THREADS;                   // number of read threads
int WRITE_THREADS;                  // number of write threads
int read_count = 0;                 // number of active readers
int write_count = 0;                // number of active writers

sem_t rw_sem;                       // used for readers and writers
sem_t cs_sem;                       // used for critical sections for readers

char phrase[] = "General Kenobi";


void *writer(void *data)
{
    sem_wait(&rw_sem);

    // variables
    char *temp = (char*)data;
    int w_id = write_count;
    write_count++;

    // loops through string
    for(int i = 0; strlen(temp) > 0; i++)
    {
        // print currently writing
        printf("writer %d is writing...\n", w_id);

        // cut off last character of string
        temp[strlen(temp) - 1] = '\0';

        // sleep 1 second
        sleep(1);
    }

    sem_post(&rw_sem);

    printf("writer %d is exiting...\n", w_id);
    pthread_exit(NULL);
}

void *reader(void *data)
{
    sem_wait(&cs_sem);          // ENTER CRITICAL

    int r_id = read_count;
    read_count++;

    printf("read_count incremented to: %d\n", read_count);

    if(read_count == 1)
        sem_wait(&rw_sem);
    
    sem_post(&cs_sem);          // EXIT CRITICAL

    // Perform reading
    char *temp = (char*)data;
    for(int i = 0; strlen(temp) > 0; i++)
    {
        printf("reader %d is reading ... content : %s\n", r_id, temp);

        sleep(1);
    }

    sem_wait(&cs_sem);          // ENTER CRITICAL

    read_count--;
    printf("read_count decremented to: %d\n", read_count);

    if(read_count == 0)
        sem_post(&rw_sem);
    
    sem_post(&cs_sem);          // EXIT CRITICAL

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

    for(int a = 0; a < READ_THREADS; a++)
    {
        rc1 = pthread_create(&read[a], NULL, reader, (void *)phrase);
        if(rc1)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc1);
            exit(-1);
        }
    }

    for(int b = 0; b < WRITE_THREADS; b++)
    {
        rc2 = pthread_create(&write[b], NULL, writer, (void *)phrase);
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