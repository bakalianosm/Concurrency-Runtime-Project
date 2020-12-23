/*
 * Header file for common functions used 
 * from the whole project 3.
 */

#pragma once
#include "common_types.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#define TIME_LENGTH 44
//This function prints the text in "sentence" in red color 
void printRed(const char* sentence);

//This function prints the text in "sentence" in blue color 
void printBlue(const char* sentence);

//This function prints the text in "sentence" in blue color 
void printGreen(const char* sentence);

//This function prints the text in "sentence" in yellow color 
void printYellow(const char* sentence);

// This function stores the current date and time with milliseconds in str array
void fetchCurTime(char str[]);