/*
 * Source file for functions used to simplify
 * the use of chef program 
 */

#include <stdio.h>
#include <stdlib.h>
#include "chef.h"
#include <time.h>
#include "common_types.h"
#include "common_functions.h"
#define ONION 1
#define PEPPER 2
#define TOMATO 3
#define BUFFER_SIZE 200
void loadChefArgs(String argv[], int *num, double *time){
    int saladsNum;
    double timeToSpend;

    if( (strcmp(argv[1], "-n")==0) && (strcmp(argv[3], "-m")==0) ){
            saladsNum = atoi(argv[2]);
            timeToSpend = atof(argv[4]);      
    }
    else if( (strcmp(argv[1], "-m")==0) && (strcmp(argv[3], "-n")==0) ){
        saladsNum = atoi(argv[4]);
        timeToSpend = atof(argv[2]);
    }
    else{
        printRed("Invalid command line sequence.Exiting...\n");
        exit(EXIT_FAILURE);
    }
    *num = saladsNum;
    *time = timeToSpend;

}

void pickIngredients(randNums ingredients){
  
    srand(time(NULL));
    
    int num1, num2;

    num1 = 1 + rand()%3;
    num2 = 1 + rand()%3;
    
    while(num1 == num2){
        num2 = 1 + rand()%3;
    }

    ingredients->firstIngredient = num1;
    ingredients->secondIngredient = num2;

}

void printPickedIngredients(int num,randNums ingredients,SharedSegment shmem){
    char curTime[TIME_LENGTH];
    fetchCurTime(curTime);
    FILE* logfile = NULL;

    /* open to append at the end of the file */
    logfile = fopen(shmem->logfileName, "a");
    /* never will be executed but anyways!*/
    if (ingredients->firstIngredient == ingredients->secondIngredient){
        printRed("ERROR. Chef sent same ingredients\n");
    }

    if (
        ((ingredients->firstIngredient == PEPPER) && (ingredients->secondIngredient == TOMATO)) ||
        ((ingredients->firstIngredient == TOMATO) && (ingredients->secondIngredient == PEPPER))
        ){
        printf("%s %d Chef Selecting ingredients tomato-pepper\n",curTime,num);
        fprintf(logfile,"%s %d Chef Selecting ingredients tomato-pepper\n",curTime,num);
    }
    else if (
        ((ingredients->firstIngredient == TOMATO) && (ingredients->secondIngredient == ONION)) ||
        ((ingredients->firstIngredient == ONION) && (ingredients->secondIngredient == TOMATO)) 
        ){
        printf("%s %d Chef Selecting ingredients tomato-onion\n",curTime,num);
        fprintf(logfile,"%s %d Chef Selecting ingredients tomato-onion\n",curTime,num);
    }
    else if (
        ((ingredients->firstIngredient == ONION) && (ingredients->secondIngredient == PEPPER)) ||
        ((ingredients->firstIngredient == PEPPER) && (ingredients->secondIngredient == ONION)) 
        ){
        printf("%s %d Chef Selecting ingredients onion-pepper\n",curTime,num); 
        fprintf(logfile,"%s %d Chef Selecting ingredients onion-pepper\n",curTime,num);
    }

    fclose(logfile);
}


int findSaladmaker(randNums ingredients){
    int informSaladmaker;
    
    if (
        ((ingredients->firstIngredient == PEPPER) && (ingredients->secondIngredient == TOMATO)) ||
        ((ingredients->firstIngredient == TOMATO) && (ingredients->secondIngredient == PEPPER))
        ){
        informSaladmaker = 1;
    }
    else if (
        ((ingredients->firstIngredient == TOMATO) && (ingredients->secondIngredient == ONION)) ||
        ((ingredients->firstIngredient == ONION) && (ingredients->secondIngredient == TOMATO)) 
        ){
        informSaladmaker = 2;
    }
    else if (
        ((ingredients->firstIngredient == ONION) && (ingredients->secondIngredient == PEPPER)) ||
        ((ingredients->firstIngredient == PEPPER) && (ingredients->secondIngredient == ONION)) 
        ){
        informSaladmaker = 3;
    }

    return informSaladmaker;
}

void passIngredients(SharedSegment shmem, int saladmaker){
    
    // int pos = saladmaker -1;
    
    if(saladmaker == 1){
        
        shmem->saladmakerResources[0].pepper = EXISTS;
        shmem->saladmakerResources[0].tomato= EXISTS;
    }
    else if(saladmaker == 2 ){
        shmem->saladmakerResources[1].tomato = EXISTS;
        shmem->saladmakerResources[1].onion = EXISTS;
    }
    else if(saladmaker == 3){
        shmem->saladmakerResources[2].pepper = EXISTS;
        shmem->saladmakerResources[2].onion = EXISTS;
    }
}

int compareIngredients(randNums pastIngredients, randNums currIngredients){
    if( (pastIngredients->firstIngredient == NOTEXISTS) &&
        (pastIngredients->secondIngredient == NOTEXISTS) ){
            pastIngredients->firstIngredient = currIngredients->firstIngredient;
            pastIngredients->secondIngredient = currIngredients->secondIngredient;
            return -1;
        }
    else if( ((pastIngredients->firstIngredient == currIngredients->firstIngredient) || (pastIngredients->secondIngredient == currIngredients->firstIngredient)) &&
      ((pastIngredients->firstIngredient == currIngredients->secondIngredient) || (pastIngredients->secondIngredient == currIngredients->secondIngredient))){
          return 0;
      }
    else{
        pastIngredients->firstIngredient = currIngredients->firstIngredient;
        pastIngredients->secondIngredient = currIngredients->secondIngredient;
        return -1;
    }

}

void writeNotify(int id, int inf, SharedSegment shmem){
    FILE* logfile = NULL;

    char curTime[TIME_LENGTH];
    fetchCurTime(curTime);

    /* open to append at the end of the file */
    logfile = fopen(shmem->logfileName, "a");

    fprintf(logfile,"%s %d Chef Notify saladmaker #%d \n",curTime,id,inf);

    fclose(logfile);

}

void writeManTime(int id , SharedSegment shmem){

    FILE* logfile = NULL;
    char curTime[TIME_LENGTH];
    
    fetchCurTime(curTime);
    /* open to append at the end of the file */
    logfile = fopen(shmem->logfileName, "a");

    fprintf(logfile,"%s %d Chef Man time for resting\n",curTime,id);
    fclose(logfile);
}

void findTimeIntervals(char logfile[]){
    
    bool sal[3] = {false, false, false};

    FILE* fp = NULL;
    fp = fopen(logfile,"r");

    char hour[50];
    char pid[8];
    char who[10];
    char message[80];
    char str[200];

    int pos;

    while(fgets(str,200,fp) != 0 ){

        sscanf(str, "%s %s %s %[^\t\n]",hour,pid,who,message);
        // printf("%s \n",hour);

        if(strcmp(who,"Chef")!=0) {
            sscanf(who,"%*[^123]%d",&pos );
            
            if(strcmp(message,"Get ingredients")==0){
                sal[pos-1] = true;

                if(sal[0] && sal[1] && sal[2]){

                    printf("[%s, ",hour);
                }
                else if(sal[0] && sal[1]){
                    printf("[%s, ",hour);
                }
                else if(sal[1] && sal[2]){
                    printf("[%s, ",hour);
                }
                else if(sal[0] && sal[2]){
                    printf("[%s, ",hour);
                }
            }
            else if(strcmp(message,"End making salad")==0){

                sal[pos-1] = false;
                
                if(sal[0] && sal[1]){
                    printf("%s ] : 2 concurrent processes\n",hour);
                }
                else if(sal[1] && sal[2]){
                    printf("%s ] : 2 concurrent processes\n",hour);
                }
                else if(sal[0] && sal[2]){
                    printf("%s ] : 2 concurrent processes\n",hour);
                }
            }
        }
    }

    fclose(fp);
}