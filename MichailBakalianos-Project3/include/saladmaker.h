/*
 * Header file for saladmaker program 
 */

#pragma once

#include <stdio.h>
#include "common_types.h"
/* This function is used to parse the arguments from the user*/
void loadSaladmakerArgs(String argv[], double* lb, double *ub, int *id);

/* This function is used to simulate the procedure of cutting the ingredients*/
int cutIngredients(SharedSegment shmem, int id);

/* This function is used to write [ waiting ] message in logfile */
void writeWaiting(int pid,int id,SharedSegment shmem,char curTime[]);

/* This function is used to write [ get ingredients ] message in logfile */
void writeGetIngredients(int pid,int id, SharedSegment shmem,char curTime[]);

/* This function is used to write [ start makign salad ]message in logfile */
void writeStartMakingSalad(int pid,int id, SharedSegment shmem,char curTime[]);

/* This function is used to write [ end making salad ] message in logfile */
void writeEndMakingSalad(int pid,int id, SharedSegment shmem,char curTime[]);

