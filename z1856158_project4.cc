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
int readcount = 0;                  // number of active readers

char* phrase = "General Kenobi";


void *writer(void *data)
{
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
    pthread_exit(NULL);
}

void *reader(void *data)
{
    readcount++;

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


    // Wait for read threads to finish
    pthread_join(read, NULL);

    // Wait for write threads to finish
    pthread_join(write, NULL);

    // Cleanup & exit

    pthread_exit(NULL);
}