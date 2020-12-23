
#include <stdio.h>
#include <errno.h>
#include "common_functions.h"
#include "common_types.h"
#include "semaphore.h"
#include "shared_segment.h"
#include <stdlib.h>
#include <string.h>

void initializeSharedSegment(SharedSegment shmem, int number, char name[15]){

     /* ------------------------------------------------------------
     * ------------------- LOG FILE  ------------------------------ 
     * ------------------------------------------------------------ 
     */
    strcpy(shmem->logfileName,name);
    FILE* log = fopen(shmem->logfileName,"w");
    fclose(log);
    /* ------------------------------------------------------------
     * ------------------- SEMAPHORES  ---------------------------- 
     * ------------------------------------------------------------ 
     */


    /* initalization of the semahores all with pshared value = 1 because
     * everyone can edit the */
    if (sem_init( (&shmem->sharedmem_mutex) , 1, 1) != 0) {
        perror("saladmakersSumSemaphore");
    }
    else{
        printGreen("Semaphore for mutex saldmakers created successfully\n");
    }
    
    if (sem_init(&(shmem->saladmakersSum),1,3) != 0) {
        perror("saladmakersSumSemaphore");
    }
    else{
        printGreen("Semaphore for 3 saldmakers created successfully\n");
    }

    if (sem_init( (&shmem->recieveMutex) , 1, 0) != 0) {
        perror("Recieve semaphore");
    }
    else{
        printGreen("Semaphore for recieve ingredientscreated successfully\n");
    }

    if (sem_init( (&shmem->logfileMutex) , 1, 1) != 0) {
        perror("Logfile");
    }
    else{
        printGreen("Semaphore for logfile mutex created successfully\n");
    }
    for (int i = 0 ; i < SALADMAKERS ; i++){
        if (sem_init( (&shmem->notifySaladmaker[i]) , 1, 0) != 0) {
            perror("Notify Saladmaker");
        }
        else{
            printGreen("Semaphore for notification of saladmakers created successfully\n");
        }

        // if (sem_init( (&shmem->readySaladmaker[i]) , 1, 0) != 0) {
        //     perror("saladmakersSumSemaphore");
        // }
        // else{
        //     printGreen("Semaphore for notification of saladmakers created successfully\n");
        // }
    }

    /* ----------------------- OTHER --------------------------------
     * ----------------------- SHARED -------------------------------
     * ----------------------- VALUES ------------------------------- 
     */

    /* every process need to know how many are the remaining salads*/
    shmem->remainingSalads = number; 
    
    /* just for easiest access */
    shmem->completedSalads = 0;
    /* how many saladmakers are active and preparing salads */
    shmem->activeSaladmakers = 0;

    shmem->pastIngredients = malloc(sizeof(shmem->pastIngredients));
    shmem->pastIngredients->firstIngredient = NOTEXISTS;
    shmem->pastIngredients->secondIngredient = NOTEXISTS;
    /* how many ingredients each saladmaker have */
    for (int i = 0 ; i < SALADMAKERS ; i++){
        if(i == 0){
            shmem->saladmakerResources[i].onion = EXISTS;
            shmem->saladmakerResources[i].pepper = NOTEXISTS;
            shmem->saladmakerResources[i].tomato =  NOTEXISTS ;
        }
        else if (i ==1){
            shmem->saladmakerResources[i].onion =  NOTEXISTS ;
            shmem->saladmakerResources[i].pepper  = EXISTS ;
            shmem->saladmakerResources[i].tomato =  NOTEXISTS ;
        }
        else if(i == 2){
            shmem->saladmakerResources[i].onion =  NOTEXISTS ;
            shmem->saladmakerResources[i].pepper =  NOTEXISTS ;
            shmem->saladmakerResources[i].tomato = EXISTS ;
        }
        shmem->preparedSalads[i] = 0;
    }

    strcpy(shmem->logfile1,"logs/saladmaker1.txt");
    strcpy(shmem->logfile2,"logs/saladmaker2.txt");
    strcpy(shmem->logfile3,"logs/saladmaker3.txt");



}

void destroySharedSegment(SharedSegment shmem,int memid){
    free(shmem->pastIngredients);

    /* destroy current semaphores */
    sem_destroy(&(shmem->sharedmem_mutex));
    sem_destroy(&(shmem->saladmakersSum));
    sem_destroy(&(shmem->recieveMutex));
    sem_destroy(&(shmem->logfileMutex));

    for (int i = 0 ; i < SALADMAKERS ; i++){
        sem_destroy(&(shmem->notifySaladmaker[i]));

    }
    
    
    /* detach shared memory */
    if( shmdt(shmem) == -1){
        perror("Shared memory detatch\n");
    }
    else{
        printGreen("Shared memory detatched successfully\n");
    }

    /* destroy shared memory */
    if(shmctl(memid,IPC_RMID,NULL)==1){
        perror("Shared memory destroy\n");
    }
    else{
        printGreen("Shared memory destroyed successfully\n");
    }

}
