//Undo = Stack part
//Test Harness

#include "tatianaStuff.h"

// FUNCTION: initStack
// DESCRIPTION: Initializes the pile to be empty
// PARAMETERS: stack - a pointer to the stack to be initialized
// RETURNS: Pile
Pile* initPile() {
    Pile* newPile = (Pile*)malloc(sizeof(Pile));
    if (newPile == NULL) {
        printf("Memory allocation error.\n");
        return newPile;
    }

    newPile->top = NULL;
    return newPile;
}

// FUNCTION: initPileNode
// DESCRIPTION: 
// PARAMETERS: 
// RETURNS: 
PileNode* initPileNode() {

    PileNode* newPileNode = (PileNode*)malloc(sizeof(Pile));
    if (newPileNode == NULL) {
        printf("Memory allocation error.\n");
        return NULL;
    }
    
    newPileNode->next = NULL;
    return newPileNode;
}


// FUNCTION: isEmpty
// DESCRIPTION: Checks if the pile is empty
// PARAMETERS: stack - a pointer to the stack to check
// RETURNS: 1 if the pile is empty, 0 if not
int isEmpty(Pile* stack) {
    return stack->top == NULL;
}

// FUNCTION: push
// DESCRIPTION: Pushes an action onto the pile
// PARAMETERS: stack - a pointer to the pile
//             action - the action to be pushed onto the pile
// RETURNS: none
void push(Pile* stack, Action action) {
 
    PileNode* newPileNode = (PileNode*)malloc(sizeof(PileNode));
    newPileNode->action = action;
    newPileNode->next = stack->top;
    stack->top = newPileNode;

    printf("Pushed action: %s\n", action.description);
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

    printf("Popped action: %s\n", action.description);  // Debugging output
    return action;
}


// FUNCTION: undo_last_action
// DESCRIPTION: Undoes the last action by popping from the pile
// PARAMETERS: actionHistory - a pointer to the pile holding the action history
// RETURNS: none
void undo_last_action(Pile* actionHistory) {
    if (actionHistory == NULL) {
        printf("Error: actionHistory is NULL =(\n");
        return;
    }

    if (isEmpty(actionHistory)) {
        printf("No actions to undo.\n");
        return;
    }

    Action lastAction = pop(actionHistory);

    switch (lastAction.actionType) {
    case ADD_ACTION:
        printf("Reverted action: Added - %s\n", lastAction.description);
        break;
    case REMOVE_ACTION:
        printf("Reverted action: Removed - %s\n", lastAction.description);
        break;
    case UPDATE_ACTION:
        printf("Reverted action: Updated - %s\n", lastAction.description);
        break;
    case UNDO_ACTION:
        printf("Action undone: %s\n", lastAction.description);
        break;
    case SEARCH_ACTION:
        printf("Reverted action: Search - %s\n", lastAction.description);
        break;
    default:
        printf("Unknown action type. Cannot undo.\n");
    }
}

// FUNCTION: clearStack
// DESCRIPTION: Clears all actions from the pile
// PARAMETERS: stack - a pointer to the pile to be cleared
// RETURNS: none
void clearStack(Pile* stack) {

    if (stack == NULL) return; 
    while (!isEmpty(stack)) {

        pop(stack);

    }
}
