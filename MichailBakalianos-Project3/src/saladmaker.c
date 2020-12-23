/*
 * Source file for saladmaker program 
 */

#include <unistd.h>
#include "shared_segment.h"
#include <stdlib.h>
#include "saladmaker.h"
#include "common_functions.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
int main(int argc, char* argv[]){
    printf("Hello from Saladmaker!\n");
    double timeFrom,timeTo;
    int memId;

    if(argc != 7){
        printRed("Error. Wrong num of command line arguments.\n");
        exit(EXIT_FAILURE);
    }
    else{
        loadSaladmakerArgs(argv, &timeFrom, &timeTo, &memId); 
    }
    
    printf("Salad maker args :\t t1= %lf, t2= %lf, sharedMemoryId= %d\n",timeFrom,timeTo,memId);
    
    /* get the process id */
    pid_t saladmakerPID = getpid();
    
    /* generate a random float number between lower and upper bound */
    srand(time(NULL));
    double randomTimeSegment  = timeFrom + (double)rand()/(double)(RAND_MAX/(timeTo-timeFrom));
    printf("Random segment is %lf\n",randomTimeSegment);

    /* split the rest time into half */ 
    int sleepTime = (int)randomTimeSegment;
    int rest = (int)(sleepTime/2);

    /* open shared memory segment*/
    key_t memKey = (key_t)memId;
    int shmid;
    shmid = shmget(memKey, sizeof(SharedSegment), IPC_CREAT | PERMS );
    if(shmid < 0 ) {
        perror("shmget");
        exit(1);
    }
    else{
        printGreen("Memory Segment opened successfully with id =");
        printf("%d\n",shmid);
    }

    SharedSegment shmem = shmat(shmid, (void*)0,0);
    if(shmem== NULL){
        perror("shmem");
        exit(1);
    }
    else{
        printGreen("Memory Segment attached successfully\n");
    }


    /* decrease saladmaker semaphore. We want maximum 3 saladmakers to run concurrent*/
    sem_wait(&(shmem->saladmakersSum));

    /* find ID of saladmaker */
    shmem->activeSaladmakers++;
    int saladmakerID = shmem->activeSaladmakers;

    /* log file for the saladmaker */
    FILE * log = NULL;
    char logName[30];

    /* initialize log files */
    switch(saladmakerID){
        
        case ONION:
            strcpy(logName,shmem->logfile1);
            log = fopen(shmem->logfile1,"w");
            //printf("Saladmaker %d has onion\n", saladmakerID);
            fclose(log);
            break;

        case PEPPER:
            strcpy(logName,shmem->logfile2);
            log = fopen(shmem->logfile2,"w");
            fclose(log);
            //printf("Saladmaker %d has green pepper\n", saladmakerID);
            break;

        case TOMATO:
            strcpy(logName,shmem->logfile3);
            log = fopen(shmem->logfile3,"w");
            fclose(log);
            //printf("Saladmaker %d has tomato\n", saladmakerID);
            break;
    }
    
    /* this is a string that stores the timestamp */
    char curTime[50];

    /* store the id of this process in the corresponding variable at shared memory*/
    sem_wait(&(shmem->sharedmem_mutex));
    shmem->saladmakerPIDs[saladmakerID-1] = saladmakerPID;
    sem_post(&(shmem->sharedmem_mutex));


    do{
        /* suspend till the chef notify the corresponding saladmaker*/    
     
        sem_wait(&(shmem->notifySaladmaker[saladmakerID-1]));
        
        /* block into sharedmem_mutex to check the current remaining salads number */
        sem_wait(&(shmem->sharedmem_mutex));
        if(shmem->remainingSalads==0) { sem_post(&(shmem->sharedmem_mutex)); exit(EXIT_SUCCESS); }
        sem_post(&(shmem->sharedmem_mutex));


        // ------------------------ TIME SEGMENT FOR JOB #1--------------------------
       
        fetchCurTime(curTime);
        /* write message to the stdout */
        printf("%s %d Saladmaker%d Waiting for ingredients\n",curTime,saladmakerPID,saladmakerID);
        
        /* write message to the saladmakers log file*/
        log = fopen(logName,"a");
        fprintf(log,"%s %d Saladmaker%d Waiting for ingredients\n",curTime,saladmakerPID,saladmakerID);
        fclose(log);

        /* write message to the main logfile */
        sem_wait(&(shmem->logfileMutex));
        writeWaiting(saladmakerPID,saladmakerID,shmem,curTime);
        sem_post(&(shmem->logfileMutex));
        //----------------------------------------------------------------       
        
        
       

        // ------------------- TIME SEGMENT FOR JOB #2--------------------------------
        fetchCurTime(curTime);
        /* write message to the stdout */
        printf("%s %d Saladmaker%d Get ingredients\n",curTime,saladmakerPID,saladmakerID);
        
        /* write message to the saladmakers log file*/
        log = fopen(logName,"a");
        fprintf(log,"%s %d Saladmaker%d Get ingredients\n",curTime,saladmakerPID,saladmakerID);
        fclose(log);

        /* write message to the main logfile */
        sem_wait(&(shmem->logfileMutex));
        writeGetIngredients(saladmakerPID,saladmakerID,shmem,curTime);
        sem_post(&(shmem->logfileMutex));
        // ------------------------------------------------------------------

        /* increase recieveMutex. Saladmaker recieved the ingredients and now
           chef can pick ingredients for the next saladmaker that is waiting */
        sem_post(&(shmem->recieveMutex));
        
        /* sleep for 500 millisecs between [Get ingredients] and [start making salad] */
        usleep(500);
        

        // ------------------ TIME SEGMENT FOR JOB #3-----------------------------------
        fetchCurTime(curTime);
        /* write message to the stdout */
        printf("%s %d Saladmaker%d Start making salad\n",curTime,saladmakerPID,saladmakerID);
        
        /* write message to the saladmakers log file*/
        log = fopen(logName,"a");
        fprintf(log,"%s %d Saladmaker%d Start making salad\n",curTime,saladmakerPID,saladmakerID);
        fclose(log);
        
        /* write message to the main logfile */
        sem_wait(&(shmem->logfileMutex));
        writeStartMakingSalad(saladmakerPID,saladmakerID,shmem,curTime);
        sem_post(&(shmem->logfileMutex));
        // --------------------------------------------------------------------
        
        /* sleep for the half time of the random time that has to do the work */
        sleep(rest);

        /* cut the ingredients that the chef placed in the table */
        int cut = cutIngredients(shmem,saladmakerID-1);
        if(cut==FINE_CUT) {
            

            sem_wait(&(shmem->sharedmem_mutex));
            shmem->completedSalads++;
            shmem->remainingSalads--;
            shmem->preparedSalads[saladmakerID-1]++;
            sem_post(&(shmem->sharedmem_mutex));
   

            //--------------------------TIME SEGMENT FOR JOB #4---------------------------
            fetchCurTime(curTime);

            /* write message to the stdout */
            printf("%s %d Saladmaker%d End making salad\n",curTime,saladmakerPID,saladmakerID);
            
            /* write message to the saladmakers log file*/
            log = fopen(logName,"a");
            fprintf(log,"%s %d Saladmaker%d End making salad\n",curTime,saladmakerPID,saladmakerID);
            fclose(log);

            /* write message to the main logfile */
            sem_wait(&(shmem->logfileMutex));
            writeEndMakingSalad(saladmakerPID,saladmakerID,shmem,curTime);
            sem_post(&(shmem->logfileMutex));
            // -----------------------------------------------------------------
        }
        else {
           exit(EXIT_FAILURE);

        }
        
        // placeInDish();
        // giveToAWaiter();
        // sleep(10)
        sleep(rest);
        printRed("------------------------------\n");
    } while(shmem->remainingSalads>0);   

    /* detach the shared memory segment */
    sem_post(&(shmem->saladmakersSum));
    shmdt(shmem);
    return 0;
}