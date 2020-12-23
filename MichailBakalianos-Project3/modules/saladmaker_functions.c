/*
 * Source file for functions used to simplify
 * the use of saladmaker program 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saladmaker.h"
#include "common_functions.h"
#include "common_types.h"
#include <unistd.h>

void loadSaladmakerArgs(String argv[], double* lb, double *ub, int *id){
    double lowerBound, upperBound;
    int idNum;

    if( (strcmp(argv[1], "-t1")==0) && (strcmp(argv[3], "-t2")==0) && (strcmp(argv[5], "-s")==0)  ){
            lowerBound = atof(argv[2]);
            upperBound = atof(argv[4]);
            idNum = atoi(argv[6]);
    }
    else if( (strcmp(argv[1], "-t1")==0) && (strcmp(argv[3], "-s")==0) && (strcmp(argv[5], "-t2")==0)  ){
        lowerBound = atof(argv[2]);
        upperBound = atof(argv[6]);
        idNum = atoi(argv[4]);
    }
    else if( (strcmp(argv[1], "-t2")==0) && (strcmp(argv[3], "-t1")==0) && (strcmp(argv[5], "-s")==0)  ){
        lowerBound = atof(argv[4]);
        upperBound = atof(argv[2]);
        idNum = atoi(argv[6]);
    }
    else if( (strcmp(argv[1], "-t2")==0) && (strcmp(argv[3], "-s")==0) && (strcmp(argv[5], "-t1")==0)  ){
        lowerBound = atof(argv[6]);
        upperBound = atof(argv[2]);
        idNum = atoi(argv[4]);
    }
    else if( (strcmp(argv[1], "-s")==0) && (strcmp(argv[3], "-t1")==0) && (strcmp(argv[5], "-t2")==0)  ){
        lowerBound = atof(argv[4]);
        upperBound = atof(argv[6]);
        idNum = atoi(argv[2]);
    }
    else if( (strcmp(argv[1], "-s")==0) && (strcmp(argv[3], "-t2")==0) && (strcmp(argv[5], "-t1")==0)  ){
        lowerBound = atof(argv[6]);
        upperBound = atof(argv[4]);
        idNum = atoi(argv[2]);
    }
    else{
        printRed("Invalid command line sequence.Exiting...\n");
        exit(EXIT_FAILURE);
    }
    *ub = upperBound;
    *lb = lowerBound;
    *id = idNum;

}


int cutIngredients(SharedSegment shmem, int id){
    if( (shmem->saladmakerResources[id].onion == EXISTS) &&
        (shmem->saladmakerResources[id].pepper ==  EXISTS) &&
        (shmem->saladmakerResources[id].tomato == EXISTS) ){
            
            if(id == 0){
                sem_wait(&(shmem->sharedmem_mutex));
                shmem->saladmakerResources[id].pepper = NOTEXISTS;
                shmem->saladmakerResources[id].tomato =  NOTEXISTS ;
                sem_post(&(shmem->sharedmem_mutex));
            }
            else if (id ==1){
                sem_wait(&(shmem->sharedmem_mutex));
                shmem->saladmakerResources[id].onion =  NOTEXISTS ;
                shmem->saladmakerResources[id].tomato =  NOTEXISTS ;
                sem_post(&(shmem->sharedmem_mutex));
            }
            else if(id == 2){
                sem_wait(&(shmem->sharedmem_mutex));
                shmem->saladmakerResources[id].onion =  NOTEXISTS ;
                shmem->saladmakerResources[id].pepper =  NOTEXISTS ;
                sem_post(&(shmem->sharedmem_mutex));
            }

            
            /* sleep for 400 milliseconds */
            usleep(400);
            return FINE_CUT;
        }
    else{
        
        return CUT_ERROR;
    }

}

void writeWaiting(int pid,int id,SharedSegment shmem,char curTime[]){
    FILE* logfile = NULL;

    /* open to append at the end of the file */
    logfile = fopen(shmem->logfileName, "a");
    fprintf(logfile,"%s %d Saladmaker%d Waiting for ingredients\n",curTime,pid,id);
    fclose(logfile);

}

void writeGetIngredients(int pid,int id, SharedSegment shmem,char curTime[]){
    FILE* logfile = NULL;

    /* open to append at the end of the file */
    logfile = fopen(shmem->logfileName, "a");
    fprintf(logfile,"%s %d Saladmaker%d Get ingredients\n",curTime,pid,id);
    fclose(logfile);
}

void writeStartMakingSalad(int pid,int id, SharedSegment shmem,char curTime[]){
    FILE* logfile = NULL;

    /* open to append at the end of the file */
    logfile = fopen(shmem->logfileName, "a");
    fprintf(logfile,"%s %d Saladmaker%d Start making salad\n",curTime,pid,id);
    fclose(logfile);

}

void writeEndMakingSalad(int pid,int id, SharedSegment shmem,char curTime[]){
    FILE* logfile = NULL;

    /* open to append at the end of the file */
    logfile = fopen(shmem->logfileName, "a");
    fprintf(logfile,"%s %d Saladmaker%d End making salad\n",curTime,pid,id);
    fclose(logfile);
}

