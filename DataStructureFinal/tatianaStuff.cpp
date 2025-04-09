#include "mainHeader.h"


// FUNCTION   : initSnapshotStack
// DESCRIPTION: Initializes a new SnapshotStack
// PARAMETERS : none
// RETURNS    : Pointer to the new SnapshotStack
SnapshotStack* initSnapshotStack(void) {
    SnapshotStack* stack = (SnapshotStack*)malloc(sizeof(SnapshotStack));
    if (!stack) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    stack->top = NULL;
    return stack;
}

// FUNCTION   : freeSnapshotStack
// DESCRIPTION: Frees all memory associated with a SnapshotStack
// PARAMETERS : Pointer to the SnapshotStack pointer
// RETURNS    : none
void freeSnapshotStack(SnapshotStack** stackRef) {
    if (!stackRef || !(*stackRef)) return;
    SnapshotStack* stack = *stackRef;

    SnapshotNode* current = stack->top;
	//Free all nodes
    while (current) {
        SnapshotNode* temp = current;
        current = current->next;

        if (temp->snapshot) {
            freeHashTable(temp->snapshot); 
            free(temp->snapshot);
        }
        free(temp);
    }

    free(stack);
    *stackRef = NULL;
}

// FUNCTION   : pushSnapshot
// DESCRIPTION: Stores a snapshot of the HashTable in the stack
// PARAMETERS : Pointer to the HashTable, Pointer to the SnapshotStack
// RETURNS    : none
void pushSnapshot(HashTable* ht, SnapshotStack* stack) {
    if (!stack || !ht) return;

    HashTable* copied = copyHashTable(ht);
    if (!copied) {
        printf("Error copying\n");
        return;
    }

    SnapshotNode* node = (SnapshotNode*)malloc(sizeof(SnapshotNode));
    if (!node) {
        printf("Memory allocation failed!\n");
        return;
    }
    node->snapshot = copied;
    node->next = stack->top;
    stack->top = node;
}

// FUNCTION   : popSnapshot
// DESCRIPTION: Restores the HashTable to the state it was in when the last snapshot was taken
// PARAMETERS : Pointer to the HashTable, Pointer to the SnapshotStack
// RETURNS    : 1 if successful, 0 if there are no snapshots to undo
int popSnapshot(HashTable* destinationHT, SnapshotStack* stack) {
    if (!stack) {
        return 0;
    }
    if (!stack->top) {
        printf("No actions to undo!\n");
        return 0;
    }

    SnapshotNode* topNode = stack->top;
    stack->top = topNode->next;


    overwriteHashTable(destinationHT, topNode->snapshot);

    freeHashTable(topNode->snapshot);
    free(topNode->snapshot);
    free(topNode);

	//logAction("POP", "SnapshotSaved!"); No need to log when you save the database, it will be saved in the file anyway

    return 1;
}


// FUNCTION   : copyHashTable
// DESCRIPTION: Creates a deep copy of a HashTable
// PARAMETERS : Pointer to the original HashTable
// RETURNS    : Pointer to the new HashTable
HashTable* copyHashTable(HashTable* original) {
    if (!original) return NULL;

    HashTable* newHT = (HashTable*)malloc(sizeof(HashTable));
    if (!newHT) {
        printf("Memory allocation failed!.\n");
        return NULL;
    }
    initHashTable(newHT);
	//Copy users
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* cur = original->users[i];
        User** tailPtr = &newHT->users[i];
        while (cur) {
            User* newUser = (User*)malloc(sizeof(User));
            if (!newUser) {
                printf("Memory allocation failed!.\n");
                return newHT;
            }
            strcpy_s(newUser->firstName, cur->firstName);
            strcpy_s(newUser->lastName, cur->lastName);
            newUser->userId = cur->userId;
            newUser->next = NULL;

            *tailPtr = newUser;
            tailPtr = &newUser->next;

            cur = cur->next;
        }
    }

	//Copy books
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* cur = original->table[i];
        Book** tailPtr = &newHT->table[i];
        while (cur) {
            Book* newBook = (Book*)malloc(sizeof(Book));
            if (!newBook) {
                printf("Memory allocation failed!\n");
                return newHT;
            }
            newBook->hashCode = cur->hashCode;
            strcpy_s(newBook->title, cur->title);
            strcpy_s(newBook->author, cur->author);

            if (cur->borrowedBy) {
                int userId = cur->borrowedBy->userId;
                newBook->borrowedBy = searchUserByHash(newHT, userId);
            }
            else {
                newBook->borrowedBy = NULL;
            }

            newBook->queueFront = NULL; 
            newBook->queueRear = NULL;
            newBook->next = NULL;

            // Inserir
            *tailPtr = newBook;
            tailPtr = &newBook->next;

            cur = cur->next;
        }
    }

    return newHT;
}

// FUNCTION   : overwriteHashTable
// DESCRIPTION: Overwrites the contents of a HashTable with the contents of another HashTable
// PARAMETERS : Pointer to the destination HashTable, Pointer to the source HashTable
// RETURNS    : none
void overwriteHashTable(HashTable* destinationHT, HashTable* sourceHT) {
    if (!destinationHT || !sourceHT) return;

    freeHashTable(destinationHT);

    for (int i = 0; i < TABLE_SIZE; i++) {
        destinationHT->users[i] = NULL;
        destinationHT->table[i] = NULL;
    }

	//Copy users
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* cur = sourceHT->users[i];
        User** tailPtr = &destinationHT->users[i];
        while (cur) {
            User* newUser = (User*)malloc(sizeof(User));
            if (!newUser) {
                printf("Memory allocation failed!\n");
                return;
            }
            strcpy_s(newUser->firstName, cur->firstName);
            strcpy_s(newUser->lastName, cur->lastName);
            newUser->userId = cur->userId;
            newUser->next = NULL;

            *tailPtr = newUser;
            tailPtr = &newUser->next;

            cur = cur->next;
        }
    }

	//Copy books
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* cur = sourceHT->table[i];
        Book** tailPtr = &destinationHT->table[i];
    
        while (cur) {
            Book* newBook = (Book*)malloc(sizeof(Book));
            if (!newBook) {
                printf("Memory allocation failed!\n");
                return;
            }
            newBook->hashCode = cur->hashCode;
            strcpy_s(newBook->title, cur->title);
            strcpy_s(newBook->author, cur->author);

            if (cur->borrowedBy) {
                int userId = cur->borrowedBy->userId;
                newBook->borrowedBy = searchUserByHash(destinationHT, userId);
            }
            else {
                newBook->borrowedBy = NULL;
            }
            newBook->queueFront = NULL;
            newBook->queueRear = NULL;
            newBook->next = NULL;

            *tailPtr = newBook;
            tailPtr = &newBook->next;

            cur = cur->next;
        }
    }
}


// FUNCTION   : undo_last_action
// DESCRIPTION: Undoes the last action taken on the HashTable
// PARAMETERS : Pointer to the HashTable, Pointer to the SnapshotStack
// RETURNS    : none
void undo_last_action(HashTable* ht, SnapshotStack* stack) {
	//If popSnapshot returns 0, it means there are no snapshots to undo
    if (!popSnapshot(ht, stack)) {
		printf("No actions to undo!\n");
    }
    else {
		getLastLog();
        printf("\nReverted the system to the last action\n");
        //Implementation of printing the last log in the file



        syncDatabaseToFile(ht, "database.txt");
    }
}


//Logging

// FUNCTION   : logAction
// DESCRIPTION: Logs an action taken on the HashTable to a file
// PARAMETERS : The type of action taken, Details about the action
// RETURNS    : none
void logAction(const char* actionType, const char* details) {
    FILE* file = NULL;
    errno_t err = fopen_s(&file, "log.txt", "a");
    if (err != 0 || !file) {
        printf("Error opening log.txt\n");
        return;
    }

    time_t t = time(NULL);
    struct tm timeInfo;
    localtime_s(&timeInfo, &t);

    char timeString[64];
    snprintf(timeString, sizeof(timeString), "%04d-%02d-%02d %02d:%02d:%02d",
        timeInfo.tm_year + 1900,
        timeInfo.tm_mon + 1,
        timeInfo.tm_mday,
        timeInfo.tm_hour,
        timeInfo.tm_min,
        timeInfo.tm_sec);

    fprintf(file, "[%s] %s: %s\n", timeString, actionType, details);
    fclose(file);
}

// FUNCTION   : getLastLog
// DESCRIPTION: Prints the last log in the log.txt file and removes it from the file, 
// this function uses a logic to minimize the ammount of memory in the stack that the function uses, saving (0.025mb)
// PARAMETERS : none
// RETURNS    : none
void getLastLog(void)
{
    FILE* file = NULL;
    char line[MAX_LOG_LEN];
    char lastLine[MAX_LOG_LEN] = "";
    int lineCount = 0;

	//Open the file in read mode to get the last log
    if (fopen_s(&file, "log.txt", "r") != 0 || file == NULL) {
        perror("Error opening log.txt");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        lineCount++;
        strcpy_s(lastLine, sizeof(lastLine), line);
    }
    fclose(file);

    if (lineCount == 0) {
        printf("File is empty\n");
        return;
    }

	//Reopen the file in read mode to remove the last log
    if (fopen_s(&file, "log.txt", "r") != 0 || file == NULL) {
        perror("Error reopening log.txt");
        return;
    }

	//Create a temporary file to store the contents without the last line
    FILE* tempFile = NULL;
    if (fopen_s(&tempFile, "temp.txt", "w") != 0 || tempFile == NULL) {
        perror("Error creating temp.txt");
        fclose(file);
        return;
    }

	//Writes the file again without the last line and then rename it to the original file
    int currentLine = 0;
    while (fgets(line, sizeof(line), file)) {
        currentLine++;
        if (currentLine < lineCount) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("log.txt");
    (void)rename("temp.txt", "log.txt");

    printf("%s", lastLine);
}

void lastActionMenu(HashTable* ht, SnapshotStack* stack) {
	lastActionOptions choice;
	do {
		printf("\nLast Action Menu:\n");
		printf("1. Undo last action\n");
		printf("2. Print logs\n");
		printf("3. Back\n");
		printf("Enter your choice: ");
		choice = (lastActionOptions)GetValidIntegerInput();
		
        switch (choice) {
		case LAST_ACTION:
			undo_last_action(ht, stack);
			break;
		case PRINT_LOGS:
			printLogs();
			break;
		case BACK_LAST_ACTION:
			break;
		default:
			printf("Please only enter the valid integer options (1,3)\n");
			break;
		}
	} while (choice != BACK_LAST_ACTION);
}

void printLogs(void) {
	FILE* file = NULL;
	char line[MAX_LOG_LEN];
	int lineCount = 0;
    int maxPrintLogs = 10; //The amount of lines of the log that this functions is going to print

	if (fopen_s(&file, "log.txt", "r") != 0 || file == NULL) {
		perror("Error opening file");
		return;
	}
	
    while (fgets(line, sizeof(line), file)) {
		lineCount++;
		printf("%s", line);
		if (lineCount >= maxPrintLogs) {
			break;
		}
	}
	
    fclose(file);
}

