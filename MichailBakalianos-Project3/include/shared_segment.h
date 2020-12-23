#pragma once

#include <semaphore.h>
#include "common_types.h"
#include <sys/shm.h>
#include <sys/sem.h>
#define SALADMAKERS 3
#define EXISTS 1 
#define NOTEXISTS 0


typedef struct random_numbers{
    int firstIngredient;
    int secondIngredient;
} randNumbers;

typedef randNumbers* randNums;

typedef struct saladmaker_info{
    
    int tomato;
    int onion;
    int pepper;

} saladmaker_info;
typedef struct shared_segment{
    /* ------------------------------------------------------------
     * ------------------- SEMAPHORES  ---------------------------- 
     * ------------------------------------------------------------ 
     */

   
    /* this semaphore is used to obtain mutual exclusion in Shared Memory */
    sem_t sharedmem_mutex;
    
    /* this semaphore is used to maintain how many processes can run
     * in this program . For this assignment max is 3.
     */
    sem_t saladmakersSum;
    
    /* this semaphore is used to block chef even the ingredients that
    produced havent recieved from the corresponding saladmaker */
    sem_t recieveMutex;


    /* this semaphore informs the corresponding saladmaker that the needed
     * ingredients are ready to pick . 
     */
    sem_t notifySaladmaker[SALADMAKERS];

    //sem_t readySaladmaker[SALADMAKERS];

     /* mutex for log file */
    sem_t logfileMutex;

    /* ----------------------- OTHER --------------------------------
     * ----------------------- SHARED -------------------------------
     * ----------------------- VALUES ------------------------------- 
     */
    
    
    int completedSalads;

    /* how many salads remaining */
    int remainingSalads;

    /* an integer value to hold how many saladmakers we have run*/
    int activeSaladmakers ;

    /* holds for each saldmaker how many salads hae prepared */
    int preparedSalads[SALADMAKERS];
    /* holds for each saladmaker how many ingrendients are here*/
    saladmaker_info saladmakerResources[SALADMAKERS];

    /* this struct is used to store the previous generated ingredients*/
    randNums pastIngredients;
    /* holds for each saladmaker its process id*/
    pid_t saladmakerPIDs[SALADMAKERS];

    /* the name of the logfile */
    char logfileName[15];

    char logfile1[30];
    char logfile2[30];
    char logfile3[30];
} shared_segment;


typedef struct shared_segment* SharedSegment;

/* functionality of shared memory  */


/* 
 * This function is used to initialize the memory segment 
 * The "number" argument represents the number of the salads.
 */
void initializeSharedSegment(SharedSegment segment, int number, char name[]);

/* This function is used to destroy the shared segment */
void destroySharedSegment(SharedSegment shmem, int memid);