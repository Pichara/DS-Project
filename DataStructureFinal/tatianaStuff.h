//Avoiding multiple inclusions
//Undo = Stack part
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Importing nickStuff.h to use for ActionType and Action structures
#include "nickStuff.h"

typedef enum {
    ADD_ACTION = 1,
    REMOVE_ACTION,
    UPDATE_ACTION,
    UNKNOWN_ACTION  
} ActionType;

typedef struct {
    ActionType actionType;
    char description[100];
    char details[100];
} Action;

typedef struct PileNode {
    Action action;
    struct PileNode* next;
} PileNode;

typedef struct {
    PileNode* top;
} Pile;

Pile actionHistory = { NULL }; 

void initStack(Pile* stack);
int isEmpty(Pile* stack);
void push(Pile* stack, Action action);
Action pop(Pile* stack);
//void undo_last_action(HashTable* ht); 
void clearStack(Pile* stack);
