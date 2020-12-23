#pragma once
#include "shared_segment.h" 
#include <stdbool.h>
#define SHMKEY (key_t)1700269
#define PERMS 0600

#define ONION 1
#define PEPPER 2
#define TOMATO 3
#define SALADMAKERS 3
typedef char* String;
typedef int INGREDIENT;
#define FINE_CUT 1 
#define CUT_ERROR 0


typedef void* Pointer;
typedef char* String;
typedef unsigned int uint;
// Pointer to a function that compares 2 items "a" and "b" and returns the following values:
// < 0 if a < b
// = 0 if a = b 
// > 0  if a > b 
typedef int (*CompareFunc)(Pointer a, Pointer b); 

// Pointer to a function that destroys an element "value"
typedef void (*DestroyFunc)(Pointer value);
