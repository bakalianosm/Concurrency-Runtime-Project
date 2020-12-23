/*
 * Source file for chef program 
 */

#include "chef.h"

#include <sys/shm.h>
#include <sys/sem.h>

int main(int argc, char* argv[]){
    
    printf("Hello from Chef!\n");

    int saladsNum; 
    double timeToSpend;
    

    /* Parse the correct arguments to continue executing */
    if(argc != 5){
        printRed("Error. Wrong num of command line arguments.\n");
        exit(EXIT_FAILURE);
    }
    else{
        loadChefArgs(argv, &saladsNum, &timeToSpend); 
    }
    printf("Chef args : \t numOfSalads = %d, mantime = %lf\n",saladsNum,timeToSpend);
    /* Now the program have to start working clear*/ 


    /* Useful information variables */
    int sleepTime = (int)timeToSpend;
    char logfileName[15] = "logfile.txt";

    pid_t chefID = getpid();

    /* Here we hold the id of the shared memory segment */
    int shmid ;
    shmid = shmget(SHMKEY, sizeof(SharedSegment), IPC_CREAT | PERMS );
    if(shmid < 0 ) {
        perror("shmget");
        exit(1);
    }
    else{
        printGreen("Memory Segment created successfully with id =");
        printf("%d\n",shmid);
    }
    
    /* Attach the shared memory segment */
    
    SharedSegment shmem = shmat(shmid, (void*)0,0);
    if(shmem== NULL){
        perror("shmem");
        exit(1);
    }
    else{
        printGreen("Memory Segment attached successfully\n");
    }
    /*
     * The shared memory now is created and attatched to the
     * address space of this program. Later we will attach this 
     * memory segment to the saladmaker's program but at first 
     * we have to initialize the values contained in this segment.
     */



    /* Initialize shared memory values */
    initializeSharedSegment(shmem, saladsNum,logfileName);
    randNums ingredients = malloc(sizeof(ingredients));
    ingredients->firstIngredient = NOTEXISTS;
    ingredients->secondIngredient = NOTEXISTS;

    /* To simplify the situation */
    printf("Press enter to start looping around\n");
    getchar();
    do {
        // printf("Remaining salads are %d\n[1]",shmem->remainingSalads);
    
        sem_wait(&(shmem->sharedmem_mutex));
        if(shmem->remainingSalads==0) { sem_post(&(shmem->sharedmem_mutex)); exit(EXIT_SUCCESS); }
        sem_post(&(shmem->sharedmem_mutex));

        /* Pick random ingredients */
        pickIngredients(ingredients);

        /* And not the same as the previous picked ingredients */
        while(!compareIngredients(shmem->pastIngredients,ingredients)) pickIngredients(ingredients);
        
        /* Here writepicked ingredients to the main logfile  and we 
         * use the semaphores to block every other process that tries to 
         * write into this file */
        sem_wait(&(shmem->logfileMutex));
        printPickedIngredients(chefID,ingredients,shmem);
        sem_post(&(shmem->logfileMutex));

    
        /* Find which saladmaker have to inform */
        int inform = findSaladmaker(ingredients);
        
        printf("%d Chef Notify saladmaker #%d\n",chefID,inform);

        /* Here write notify message to the main logfile  and we 
         * use the semaphores to block every other process that tries to 
         * write into this file */
        sem_wait(&(shmem->logfileMutex));
        writeNotify(chefID,inform,shmem);
        sem_post(&(shmem->logfileMutex));

        /* Pass the ingredients from the chef to the corresponding saladmaker */
        passIngredients(shmem,inform);

        /* Notify the saladmaker that the ingredietns are on the table */
        sem_post(&(shmem->notifySaladmaker[inform-1]));
   

        /* Suspend till the saladmaker recieve the ingredients to continue with
           generating next 2 ingredients */
        sem_wait(&(shmem->recieveMutex));


        // --------- --------- --------- ---------   END TIME SEGMENT --------

        /* Pass the ingredients */
        // sem_wait(&(shmem->readySaladmaker[inform-1]));
        
        // getchar();

        printf("%d Chef Man time for resting\n",chefID);
        /* Here write man time message to the main logfile  and we 
         * use the semaphores to block every other process that tries to 
         * write into this file */
        sem_wait(&(shmem->logfileMutex));
        writeManTime(chefID,shmem);
        sem_post(&(shmem->logfileMutex));


       
        sleep(sleepTime);
    }while(shmem->remainingSalads>0);
    // } while(1);
    printf("Total #salads :[%d]\n",shmem->completedSalads);
    for(int i = 0; i< SALADMAKERS ; i++){
        sem_post(&(shmem->notifySaladmaker[i]));
        printf("#salads of salad_maker%d [%d] : [%d]\n",i+1,shmem->saladmakerPIDs[i],shmem->preparedSalads[i]);
    }

    printf("Time intervals: (in increasing order)\n");
    /* EXIT SEGMENT */
    free(ingredients);

    // findCommonIntervals(logfileName);
   
    /* here we need to remove & destroy the existing shared memory segment */
    destroySharedSegment(shmem, shmid);
    
    return 0;
}

 // sleep(1);