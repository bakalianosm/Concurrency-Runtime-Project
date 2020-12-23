/*
 * Header file for chef program 
 */

#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <semaphore.h>
#include "common_types.h"
#include "shared_segment.h"
#include "common_functions.h"



/* This function is used to parse the arguments from the user*/
void loadChefArgs(String argv[], int *num, double *time);

/* This function is used to generate to 2 random numbers between 1 and 3*/
void pickIngredients(randNums ingredients);

/* This function is used to print what ingredients the chef picked*/
void printPickedIngredients(int num,randNums ingredients,SharedSegment shmem);

/* This function is used to find the correspondind saladmaker to send the ingredients*/
int findSaladmaker(randNums ingredients);

/* This function is used to pass the ingredients to the memory space
 * that we have to store each saladmaker's available resources
 */
void passIngredients(SharedSegment shmem, int saladmaker);

/* this function is used to compare ingredients */
int compareIngredients(randNums pastIngredients, randNums currIngredients);

/* this fucntion writes the notify message at the log file*/
void writeNotify(int id, int inf, SharedSegment shmem);

/* this fucntion writes the mantime message at the log file*/
void writeManTime(int id , SharedSegment shmem);

/* find the time intervals */
void findCommonIntervals(char name[]);