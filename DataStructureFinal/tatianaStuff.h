//Avoiding multiple inclusions
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
    ADD_ACTION,
    REMOVE_ACTION,
    UPDATE_ACTION,
    UNDO_ACTION,
    SEARCH_ACTION,
    UNKNOWN_ACTION,
    PROCESS_ACTION,
    DISPLAY_ACTION,
    REVERT_ACTION,
} ActionType;

//Structure to store an action
typedef struct Action {
    int actionType;
    char description[100];
    char details[100];
    int userId;             
    char firstName[50];    
    char lastName[50];
} Action;

//Node structure for the action stack (pile)
typedef struct PileNode {
    Action action;
    struct PileNode* next;
} PileNode;

//Structure for the stack (pile) to store actions

typedef struct Pile {
    PileNode* top;
} Pile;

//A struct for showing undo options (can store actions in a more structured way if needed)

typedef struct ShowUndoOptions {
    Action* actions;
    int size;
    int capacity;
} Undo;


extern struct Pile* LastAction;

Pile* initPile();
int isEmpty(Pile* stack);
void push(Pile* stack, Action action);
Action pop(Pile* stack);
void undo_last_action(Pile* actionHistory);
void clearStack(Pile** stack);
void recordAction(Pile* LastAction, ActionType actionType, const char* description, const char* details);
void showUndoOptions(Pile* actionHistory);
void actionHistory(Pile* actionHistory);
void showActionHistory(Pile* actionHistory);
void logAction(const char* actionType, const char* description, const char* details);
