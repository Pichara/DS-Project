//Undo = Stack part

#include "tatianaStuff.h"
#include "nickStuff.h"

Pile* LastAction = NULL;  

// FUNCTION: initPile
// DESCRIPTION: Initializes the pile to be empty
// PARAMETERS: none
// RETURNS: Pointer to a new Pile
Pile* initPile() {
    Pile* newPile = (Pile*)malloc(sizeof(Pile));
    if (newPile == NULL) {
        printf("Memory allocation error.\n");
        return NULL;
    }

    newPile->top = NULL;
    return newPile;
}

// FUNCTION: isEmpty
// DESCRIPTION: Checks if the pile is empty
// PARAMETERS: stack - a pointer to the stack to check
// RETURNS: 1 if the pile is empty, 0 if not
int isEmpty(Pile* stack) {
    return stack == NULL || stack->top == NULL;
}

// FUNCTION: push
// DESCRIPTION: Pushes an action onto the pile
// PARAMETERS: stack - a pointer to the pile
//             action - the action to be pushed onto the pile
// RETURNS: none
void push(Pile* stack, Action action) {
    if (stack == NULL) {
        printf("Error: stack is NULL. Cannot push action.\n");
        return;
    }

    PileNode* newPileNode = (PileNode*)malloc(sizeof(PileNode));
    if (newPileNode == NULL) {
        printf("Memory allocation error.\n");
        return;
    }

    newPileNode->action = action;
    newPileNode->next = stack->top;
    stack->top = newPileNode;
}

// FUNCTION: pop
// DESCRIPTION: Pops an action from the pile
// PARAMETERS: stack - a pointer to the pile
// RETURNS: the action that was popped from the pile
Action pop(Pile* stack) {
    if (isEmpty(stack)) {
        printf("Pile is empty.\n");
        Action emptyAction = { UNKNOWN_ACTION, "", "" };  //return a default empty action if the pile is empty
        return emptyAction;
    }

    PileNode* tempNode = stack->top;
    Action action = tempNode->action;
    stack->top = stack->top->next;
    free(tempNode);

    return action;
}


// FUNCTION: undo_last_action
// DESCRIPTION: Undoes the last action by popping from the pile
// PARAMETERS: actionHistory - a pointer to the pile holding the action history
// RETURNS: none
void undo_last_action(Pile* actionHistory) {
    if (actionHistory == NULL || isEmpty(actionHistory)) {
        printf("No actions to undo.\n");
        return;
    }

    Action lastAction = pop(actionHistory);

    char revertDescription[256];
    sprintf_s(revertDescription, "Reverted action: %s", lastAction.description);
    logAction;

    if (lastAction.actionType == ADD_ACTION) {
        printf("Reverted action: User '%s %s' (ID: %d) was added.\n", lastAction.firstName, lastAction.lastName, lastAction.userId);
    }
    else if (lastAction.actionType == REMOVE_ACTION) {
        printf("Reverted action: User '%s %s' (ID: %d) was removed.\n", lastAction.firstName, lastAction.lastName, lastAction.userId);
    }
    else if (lastAction.actionType == UPDATE_ACTION) {
        printf("Reverted action: User '%s %s' (ID: %d) was updated.\n", lastAction.firstName, lastAction.lastName, lastAction.userId);
    }
    else {
        printf("Unknown action. No revert logic implemented.\n");
    }
    
    // Implementing...
    Action revertAction;
    revertAction.actionType = REVERT_ACTION;
    revertAction.userId = lastAction.userId;
    strcpy(revertAction.firstName, lastAction.firstName);
    strcpy(revertAction.lastName, lastAction.lastName);
    strcpy(revertAction.description, "Reverted action");
    strcpy(revertAction.details, "This action was reverted.");

    push(actionHistory, revertAction);

}

// FUNCTION: clearStack
// DESCRIPTION: Clears all actions from the pile and frees memory
// PARAMETERS: stack - a pointer to the pile to be cleared
// RETURNS: none
void clearStack(Pile** stack) {  
    if (stack == NULL || *stack == NULL) return;
    while (!isEmpty(*stack)) {
        pop(*stack);
    }
    free(*stack);
    *stack = NULL;
}

// FUNCTION   : recordAction
// DESCRIPTION: Registers an action (like add, remove, or update) into the action stack.
//              It stores information about the action in a structure and adds it to the stack.
// PARAMETERS : 
//     - LastAction: A pointer to the Pile where the action will be recorded.
//     - actionType: The type of the action (e.g., ADD_ACTION, REMOVE_ACTION, etc.)
//     - description: A short description of the action (e.g., "Add user", "Remove book").
//     - details: A more detailed description of the action (e.g., "Added user John Doe" or "Removed book ABC").
// RETURNS    : none
//
void recordAction(Pile* LastAction, ActionType actionType, const char* description, const char* details) {
    if (LastAction == NULL) {
        printf("Error: LastAction is NULL.\n");
        return;
    }

    Action action;
    action.actionType = actionType;

    strcpy_s(action.description, sizeof(action.description), description);
    strcpy_s(action.details, sizeof(action.details), details);

    action.description[sizeof(action.description) - 1] = '\0';
    action.details[sizeof(action.details) - 1] = '\0';

    push(LastAction, action);

    logAction(
        (actionType == ADD_ACTION) ? "Add" :
        (actionType == REMOVE_ACTION) ? "Remove" : "Update",
        description,
        details
    );
 
}


// FUNCTION   : showUndoOptions
// DESCRIPTION: displays the undo menu, al
// lowing users to view action history or undo the last action.
// PARAMETERS : actionHistory - The stack containing the action history.
// RETURNS    : none
void showUndoOptions(Pile* actionHistory) {
    int choice;

    printf("\nSelect an option:\n");
    printf("1. Action History\n");
    printf("2. Undo last action\n");
    printf("Enter your choice: ");

    choice = GetValidIntegerInput();

    switch (choice) {
    case 1:
        showActionHistory(actionHistory);
        break;
    case 2:
        undo_last_action(actionHistory);
        break;
    default:
        printf("Invalid choice. Please select 1 or 2.\n");
        break;
    }
}

// FUNCTION   : ActionHistory
// DESCRIPTION: 
// PARAMETERS : actionHistory 
// RETURNS    : none
void actionHistory(Pile* actionHistory) {
    if (actionHistory == NULL || isEmpty(actionHistory)) {
        printf("No actions in history.\n");
        return;
    }

    PileNode* topAction = actionHistory->top;
    if (topAction != NULL) {
        printf("Last action: %s\n", topAction->action.description);
    }
}

// FUNCTION   : showActionHistory
// DESCRIPTION: Displays the full history of actions stored in the action stack in a clear format.
// PARAMETERS : actionHistory - The stack containing the action history.
// RETURNS    : none
void showActionHistory(Pile* actionHistory) {
    if (actionHistory == NULL || isEmpty(actionHistory)) {
        printf("\nNo actions recorded in history.\n\n");
        return;
    }

    printf("\nAction History:\n");

    PileNode* current = actionHistory->top;

    while (current != NULL) {
        switch (current->action.actionType) {
        case ADD_ACTION:
            printf("Added: %s\n", current->action.description);
            break;
        case REMOVE_ACTION:
            printf("Removed: %s\n", current->action.description);
            break;
        case UPDATE_ACTION:
            printf("Updated: %s\n", current->action.description);
            break;
        case SEARCH_ACTION:
            printf("Searched for: %s\n", current->action.description);
            break;
        case PROCESS_ACTION:
            printf("Processed: %s\n", current->action.description);
            break;
        case DISPLAY_ACTION:
            printf("Displayed: %s\n", current->action.description);
            break;
        default:
            printf("Unknown action: %s\n", current->action.description);
            break;
        }

        current = current->next;
    }

    printf("\n");
}

// FUNCTION   : logAction
// DESCRIPTION: Saves an action to the history file without printing anything on screen
// PARAMETERS : action - The string describing the action
// RETURNS    : none
void logAction(const char* actionType, const char* description, const char* details) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    FILE* logFile = fopen("history.txt", "a");
    if (logFile == NULL) {
        printf("Error.\n");
        return;
    }

    char timeString[100];
    snprintf(timeString, sizeof(timeString), "%04d-%02d-%02d %02d:%02d:%02d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    char logMessage[512];
    snprintf(logMessage, sizeof(logMessage), "[%s] %s: %s - %s\n", timeString, actionType, description, details);

    fprintf(logFile, "%s", logMessage);

    printf("%s", logMessage);

    fclose(logFile);

} // Tatiana implementing...
