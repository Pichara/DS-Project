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
    return stack->top == NULL;
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
    if (lastAction.actionType == ADD_ACTION) {
        printf("Reverted action: Added - %s\n", lastAction.description);
    }
    else if (lastAction.actionType == REMOVE_ACTION) {
        printf("Reverted action: Removed - %s\n", lastAction.description);
    }
    else if (lastAction.actionType == UPDATE_ACTION) {
        printf("Reverted action: Updated - %s\n", lastAction.description);
    }
    else {
        printf("Unknown action. No revert logic implemented.\n");
    }
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

    snprintf(action.description, sizeof(action.description), "%s", description);
    snprintf(action.details, sizeof(action.details), "%s", details);

    action.description[sizeof(action.description) - 1] = '\0';
    action.details[sizeof(action.details) - 1] = '\0';

    push(LastAction, action);

}

// FUNCTION   : showUndoOptions
// DESCRIPTION: Muestra las opciones para deshacer la última acción o ver el historial de acciones.
// PARAMETERS : None
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
        printf("Invalid choice. Please select again.\n");
        break;
    }
}

// FUNCTION   : ActionHistory
// DESCRIPTION: Muestra el historial de acciones almacenadas en la pila de acciones.
// PARAMETERS : actionHistory - La pila de acciones que contiene el historial de acciones.
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
// DESCRIPTION: Muestra el historial de acciones almacenadas en la pila de acciones.
// PARAMETERS : actionHistory - La pila de acciones que contiene el historial de acciones.
// RETURNS    : none
void showActionHistory(Pile* actionHistory) {
    if (actionHistory == NULL || isEmpty(actionHistory)) {
        printf("No actions in history.\n");
        return;
    }

    PileNode* current = actionHistory->top;
    printf("\nAction History:\n");
    while (current != NULL) {
        printf("Action: %s - %s\n", current->action.description, current->action.details);
        current = current->next;
    }
}
