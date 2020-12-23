/*
 * Source file for common functions used from the whole program
 */

#include <stdio.h>
#include <stdlib.h>
#include "common_functions.h"
#include <math.h>

void printRed(const char* sentence){
    if(sentence != NULL){
        printf("\033[1;31m");
        printf("%s",sentence);
        printf("\033[0m");
    }
}

void printBlue(const char* sentence){
    if(sentence != NULL){
        printf("\033[0;34m");
        printf("%s",sentence);
        printf("\033[0m");
    }
}

void printGreen(const char* sentence){
    if(sentence != NULL){
        printf("\033[0;32m");
        printf("%s",sentence);
        printf("\033[0m");
    }
}

void printYellow(const char* sentence){
    if(sentence != NULL){
        printf("\033[0;33m");
        printf("%s",sentence);
        printf("\033[0m");
    }
}

void fetchCurTime(char str[]){
    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 1000;

    time_t rawtime;
    struct tm * timeinfo;
    char buffer [40];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    strftime(buffer, 44, "%H:%M:%S", timeinfo);
    char currentTime[TIME_LENGTH] = "";
    sprintf(currentTime, "%s.%d", buffer, milli);
    strcpy(str,currentTime);
    // printf("%s \n", currentTime);
}