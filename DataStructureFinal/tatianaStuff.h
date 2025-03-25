//Avoiding multiple inclusions
//Undo = Stack part
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Importing nickStuff.h to use for ActionType and Action structures
#include "nickStuff.h"

typedef enum ActionType {
    ADD_ACTION = 1,
    REMOVE_ACTION,
    UPDATE_ACTION,
    UNDO_ACTION,
    SEARCH_ACTION,
    UNKNOWN_ACTION
} ActionType;

typedef struct Action {
    int actionType;
    char description[100];
    char details[100];
} Action;

typedef struct PileNode {
    Action action;
    struct PileNode* next;
} PileNode;

typedef struct Pile {
    PileNode* top;
} Pile;

Pile* initPile();
PileNode* initPileNode();
int isEmpty(Pile* stack);
void push(Pile* stack, Action action);
Action pop(Pile* stack);
void undo_last_action(Pile* actionHistory);
void clearStack(Pile* stack);
