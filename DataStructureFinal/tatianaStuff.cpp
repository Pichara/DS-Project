//Undo = Stack part
//Test Harness

#include "tatianaStuff.h"
#include "nickStuff.h"  

// FUNCTION: initStack
// DESCRIPTION: Initializes the pile to be empty
// PARAMETERS: stack - a pointer to the stack to be initialized
// RETURNS: none
void initStack(Pile* stack) {
    stack->top = NULL;
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
    PileNode* newNode = (PileNode*)malloc(sizeof(PileNode));
    if (newNode == NULL) {
        printf("Memory allocation error.\n");
        return;
    }

    newNode->action = action;
    newNode->next = stack->top;
    stack->top = newNode;

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
    if (isEmpty(actionHistory)) {
        printf("No actions to undo.\n");
        return;
    }

    // implementing...
}


// FUNCTION: clearStack
// DESCRIPTION: Clears all actions from the pile
// PARAMETERS: stack - a pointer to the pile to be cleared
// RETURNS: none
void clearStack(Pile* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
}

Pile actionHistory;
