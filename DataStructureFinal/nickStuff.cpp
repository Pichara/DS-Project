#include "mainHeader.h"

//
// FUNCTION   : mainMenu      
// DESCRIPTION: Acts as the main function in the program
//              Initlaizes data structures
//              Has a switch statement that will call upon different menus
//                    
// PARAMETERS : none
// RETURNS    : none    
//
void mainMenu(void) {

	//Initialize the hash table and start going into the main menu
    HashTable ht;
    initHashTable(&ht);
    SnapshotStack* myUndoStack = initSnapshotStack();
    loadDatabase(&ht, "database.txt");
    mainMenuOptions choice;
            do {
                displayMainMenu();
                choice = (mainMenuOptions)GetValidIntegerInput();
                switch (choice) {
                case MANAGE_USER:
                    printf("\n======= Manage Users =======\n");
					manageUserMenu(&ht, myUndoStack);
                    break;
                case MANAGE_BOOK:
                    printf("\n======= Manage Books =======\n");
                    manageBookMenu(&ht, myUndoStack);
                    break;
                case SEARCH_SYS:
                    printf("\n======= Search System =======\n");
                    searchMenu(&ht);
                    break;
                case CHECK_BOOKOUT:
			        printf("\n======= Process book =======\n");
					processBookMenu(&ht, myUndoStack);
                    break;
                case DISPLAY_DB:
                    printf("\n======= Displaying Database =======\n");
                    databaseMenu(&ht);
                    break;
                case UNDO:
			        printf("\n======= Last Actions =======\n");
                    lastActionMenu(&ht, myUndoStack);
                    break;
                case EXIT:
                    printf("\nExiting program...\n");
                    freeHashTable(&ht);
                    return;
                default:
                    printf("\nInvalid choice! Try again.\n");
                    break;
                }
            } while (choice != EXIT);

}

//
// FUNCTION   : displayMainMenu    
// DESCRIPTION: Displays the inital choices of the system to the user for selection
//                    
// PARAMETERS : none  
// RETURNS    : none
//
void displayMainMenu() {
    printf("\nLibAid Book System v2.00\n");
    printf("1. Manage users\n");
    printf("2. Manage books\n");
    printf("3. Search system\n");
    printf("4. Process book\n");
    printf("5. Display database\n");
    printf("6. Last action\n");
    printf("7. Exit the program\n");
    printf("Enter your choice: ");
}

//
// FUNCTION   : manageUserMenu  
// DESCRIPTION: Secondary menu used when the user has selected they want to manage a book
//                    
// PARAMETERS : Pointer to the hash table   
// RETURNS    : none    
//
void manageUserMenu(HashTable* ht, SnapshotStack* undoStack) {
    manageUserOptions choice;
    do {
    printf("\nPlease choose an option:\n");
    printf("1. Add a new user\n");
    printf("2. Remove exisitng user\n");
	printf("3. Update existing user\n");
    printf("4. Back\n");
    printf("Enter your choice: ");
    choice = (manageUserOptions)GetValidIntegerInput();
    switch (choice) {
    case ADD_USER:
        pushSnapshot(ht, undoStack);
        addUser(ht);
        break;
    case REMOVE_USER:
        pushSnapshot(ht, undoStack);
		removeUser(ht); //Pichara implementing...
		break;
    case UPDATE_USER:
        pushSnapshot(ht, undoStack);
		updateUser(ht); //Pichara implementing...
		break;
    case BACK_USER:
        break;
    default:
        printf("Please only enter the valid integer options (1,2,3)\n");
    }
    } while (choice != BACK_USER);
}

//
// FUNCTION   : manageBookMenu  
// DESCRIPTION: Secondary menu used when the user has selected they want to manage a user
//                    
// PARAMETERS : Pointer to the hash table   
// RETURNS    : none    
//
void manageBookMenu(HashTable* ht, SnapshotStack* undoStack) {
    manageBookOptions choice;
    printf("\nPlease choose an option:\n");
    printf("1. Add a books\n");
    printf("2. Remove existing book\n");
	printf("3. Update existing book\n");
    printf("4. Back\n");
    printf("Enter your choice: ");
    choice = (manageBookOptions)GetValidIntegerInput();
    switch (choice) {
    case ADD_BOOK:
        pushSnapshot(ht, undoStack);
        addBook(ht);
        break;
    case REMOVE_BOOK:
		printf("Remove book\n");
		pushSnapshot(ht, undoStack);
		removeBook(ht);
        break;
	case UPDATE_BOOK:
		printf("Update book\n");
		pushSnapshot(ht, undoStack);
		updateBook(ht);
        break;
    case BACK_BOOK:
        break;
    default:
        printf("Please only enter the valid integer options (1,2,3)\n");
    }
}

//
// FUNCTION   : searchMenu    
// DESCRIPTION: Secondary menu to display chopice of searching books by id or users
//                    
// PARAMETERS : Pointer to the hash table    
// RETURNS    : none   
//
void searchMenu(HashTable* ht) {
    searchOptions choice;
    do {
    printf("\nPlease choose an option:\n");
    printf("1. Search for books\n");
    printf("2. Search for users\n");
    printf("3. Back\n");
    printf("Enter your choice: ");
    choice = (searchOptions)GetValidIntegerInput();
    switch (choice) {
    case SEARCH_BOOK:
        searchforBookByHash(ht);
        break;
    case SEARCH_USER:
        searchForUserByHash(ht);
        break;
    case BACK_SEARCH:
        break;
    default:
        printf("Please only enter the valid integer options (1,2)\n");
	}
	} while (choice != BACK_SEARCH);
}


//
// FUNCTION   : GetValidIntegerInput     
// DESCRIPTION: Makes sure that the input read from keyboard is a single int only between 1-6 inclusive  
//                    
// PARAMETERS : none     
// RETURNS    : value, which is the number isolated and meets the range of 1-6 inclusive.     
//
int GetValidIntegerInput(void) {
    //Declare the local variables
    int value;
    char buffer[100];

    while (1) {

        //Read the input from the user, make sure it is within bounds. 
        if (fgets(buffer, sizeof(buffer), stdin)) {

            //Use sscanf_s to parse the integer out of the input
            if (sscanf_s(buffer, "%d", &value) == 1) {

                //After parsing, initalize the pointer be equal to the buffer to get data.
                char* extra_char = buffer;

                //Skip over the number if it is more, increament the extra counter
                while (*extra_char >= '0' && *extra_char <= '9') {
                    extra_char++;
                }

                //Check to see if after skipping the number, there are actual characters at the end.
                while (*extra_char != '\0') {
                    if (*extra_char != ' ' && *extra_char != '\n') {
                        break;
                    }
                    extra_char++;
                }

                //Check to confirm that the number entered ends with null\newline
                if (*extra_char == '\0' || *extra_char == '\n') {
                    return value; // Valid input
                }
            }
        }
        printf("Invalid input! Please enter a valid number:\n");
    }
}

//
// FUNCTION   : addUser   
// DESCRIPTION: Adds a user into the single linked list
//              Whoever is actually responsible for this will need to edit this to be a sorted linked list
//              Nick simply added it in for testing purposes of his hash table
//                    
// PARAMETERS : Pointer to the hash table 
// RETURNS    : none    
//
void addUser(HashTable* ht) {
    //Initlaize variables
    char firstName[MAX_NAME_LEN], lastName[MAX_NAME_LEN];
    int userId;
    char log_message[MAX_LOG_LEN];

    //Prompt the user for their first name
    printf("Enter first name: ");
    fgets(firstName, sizeof(firstName), stdin);
    firstName[strcspn(firstName, "\n")] = '\0';

    //Prompt the user for their last name
    printf("Enter last name: ");
    fgets(lastName, sizeof(lastName), stdin);
    lastName[strcspn(lastName, "\n")] = '\0';

    //Generate the hash for the user based on the last name (this is the userId)
    userId = generateUserHash(lastName); 
    printf("Generated UserID (Hash): %d\n", userId); 

    //Allocate memory for the new user
    User* newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        printf("Memory allocation failed for new user!\n");
        return;
    }

    //Copy the provided first name and last name to the new user structure
    strcpy_s(newUser->firstName, sizeof(newUser->firstName), firstName);
    strcpy_s(newUser->lastName, sizeof(newUser->lastName), lastName);

    //Assign the generated hash as the userId
    newUser->userId = userId;

    //Check to make sure that this user is not a duplciate
    int index = userId % TABLE_SIZE; 
    User* current = ht->users[userId];
    while (current != NULL) {

        //If a user with the same first name, last name, and userId exists, it's a duplicate. Throw error and do not add.
        if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0 && current->userId == userId) {

            printf("Error: Duplicate user found! User %s %s with ID %d already exists.\n", firstName, lastName, userId);
            free(newUser);
            return;
        }
        current = current->next;
    }

    // Insert the new user at the beginning of the linked list at the appropriate index
    newUser->next = ht->users[userId];

    // Insert the new user into the hash table's linked list at the calculated index
    ht->users[userId] = newUser;

    //Confirm the user was added
    printf("User %s %s with ID %d has been added.\n", firstName, lastName, userId);

    //Logging
    snprintf(log_message, sizeof(log_message), "User %s added", lastName);
    logAction("Add User", log_message);

	//Add the data to a file for storage 
    syncDatabaseToFile(ht, "database.txt");
}


//
// FUNCTION   : addBook    
// DESCRIPTION: Adds a book into the single linked list
//              Whoever is actually responsible for this will need to edit this to be a sorted linked list
//              Nick simply added it in for testing purposes of his hash table
//                    
// PARAMETERS : Pointer to the hash table   
// RETURNS    : none
//
void addBook(HashTable* ht) {
    //Initalize variables.
    char title[MAX_TITLE_LEN], author[MAX_AUTHOR_LEN]; 
    char log_message[MAX_LOG_LEN];

    //Prompt the user for the book title
    printf("Enter book title: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';

    //Prompt the user for the book author
    printf("Enter book author: ");
    fgets(author, sizeof(author), stdin);
    author[strcspn(author, "\n")] = '\0';

    //Generate the index number by the hash.
    unsigned int index = generateBookHash(title);

    //Allocate memory for book.
    Book* newBook = (Book*)malloc(sizeof(Book));
    if (!newBook) {
        printf("Memory allocation failed!\n");
        return;
    }

    //Add the values to the data members for the Book. 
    newBook->hashCode = generateBookHash(title);
    strcpy_s(newBook->title, sizeof(newBook->title), title);
    strcpy_s(newBook->author, sizeof(newBook->author), author);
    newBook->borrowedBy = NULL;

	//Initialize the queue pointers
	newBook->queueFront = NULL;
	newBook->queueRear = NULL;

    //Check to make sure that this user is not a duplciate
    Book* current = ht->table[index];
    while (current != NULL) {
        // If a book with the same title and hashCode exists, it's a duplicate. Throw error and delete.
        if (strcmp(current->title, title) == 0 && current->hashCode == newBook->hashCode) {
            printf("Error: Duplicate book found! The book '%s' already exists.\n", title);
            free(newBook);
            return;
        }
        current = current->next;
    }
    
	//Add the entry into the hash table
    newBook->next = ht->table[index];
    ht->table[index] = newBook;

    //Confirm the process was successful.
    printf("Book '%s' by '%s' has been added at index %d.\n", title, author, index);


    //Logging
    snprintf(log_message, sizeof(log_message), "Book %s added", title);
    logAction("Add Book", log_message);

	//Add the data to a file for storage
    syncDatabaseToFile(ht, "database.txt");
}

//
// FUNCTION   : searchBookByHash    
// DESCRIPTION: Looks through the hash table to find the book that matches the hash
//                    
// PARAMETERS : pointer to the hash table and the hash code    
// RETURNS    : the pointer to the book with its information, should it exist
//
Book* searchBookByHash(const HashTable* ht, unsigned int hashCode) {
    unsigned int index = hashCode % TABLE_SIZE;

    //Traverse the linked list at that index
    Book* current = ht->table[index];
    while (current != NULL) {
        if (current->hashCode == hashCode) {  
            return current;
        }
        current = current->next;
    }

    return NULL;
}

//
// FUNCTION   : searchUserByHash   
// DESCRIPTION: Looks through the hash table to find the book that matches the hash
//                    
// PARAMETERS : pointer to the hash table and the hash code    
// RETURNS    : the pointer to the book with its information, should it exist
//
User* searchUserByHash(HashTable* ht, int userHashCode) {
    int index = userHashCode % TABLE_SIZE; 

    User* current = ht->users[index];
    while (current) {
        if (userHashCode == generateUserHash(current->lastName)) { 
            return current; 
        }
        current = current->next; 
    }
    return NULL;
}

//
// FUNCTION   : searchBookByHash    
// DESCRIPTION: Checks to see if the book exists in the hash table by taking the hash code (index) from user.
//              
//                    
// PARAMETERS : Pointer to the hash table 
// RETURNS    : none
//
void searchforBookByHash(HashTable* ht) {

    int bookHashCode;
    char title[MAX_TITLE_LEN];
    //Get users last name for the search
    printf("Enter the title you want to search for: \n");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';
    bookHashCode = generateBookHash(title);

    //Search for the book using the hash code
    Book* foundBook = searchBookByHash(ht, bookHashCode);
    if (foundBook != NULL) {
        printf("Book found: Title: %s, Author: %s\n", foundBook->title, foundBook->author);
    }
    else {
        printf("No book found with hash code %u\n", bookHashCode);
    }
}

//
// FUNCTION   : searchForUserByHash   
// DESCRIPTION: Searches for the user by their hash code
//                    
// PARAMETERS : Pointer to the hash table   
// RETURNS    : none    
//
void searchForUserByHash(HashTable* ht) {

    //Initialize the variable
    int userHashCode = 0;
    char lastName[MAX_NAME_LEN];

    //Get users last name for the search
    printf("Enter the user's last name to search for: \n");
    fgets(lastName, sizeof(lastName), stdin);  
    lastName[strcspn(lastName, "\n")] = '\0';   
    userHashCode = generateUserHash(lastName);
    User* user = searchUserByHash(ht, userHashCode);

    if (user) {
        printf("User found: %s %s\n", user->firstName, user->lastName);
    }
    else {
        printf("User not found.\n");
    }
}
//
// FUNCTION   : generateBookHash     
// DESCRIPTION: Sums the ASCII value of each character and then takes the modulus 
//              with the table size to ensure the hash value is within range.
// PARAMETERS : Pointer to the title and the table size   
// RETURNS    : an integer value which is the hash key or index
//
int generateBookHash(const char* title) {
    unsigned int hash = 0;

    // Process each character in the title
    while (*title) {
        // Sum the ASCII value of the current character
        hash += *title;
        title++;
    }

    // Return the hash value constrained to the table size
    return hash % TABLE_SIZE;
}



//
// FUNCTION   : generateUserHash   
// DESCRIPTION: Generates the user hash by adding the ASCII value of each character in last name
//              Modulused by the tablesize to ensure value in range.
//                    
// PARAMETERS : pointer to the last name and the table size  
// RETURNS    : an integer value with the hash value (index)     
//
int generateUserHash(const char* lastName) {
    unsigned int hash = 0;
    while (*lastName) {
        hash += *lastName;
        lastName++;
    }
    return hash % TABLE_SIZE;
}

//
// FUNCTION   : initHashTable     
// DESCRIPTION: Initializes the ahsh table and the array for the books and the users
//                    
// PARAMETERS : Pointer to the hashtable 
// RETURNS    : none   
//
void initHashTable(HashTable* ht) {

    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
        ht->users[i] = NULL;
    }
}


//
// FUNCTION   : freeHashTable     
// DESCRIPTION: Frees the memory allocated by the hash table
//                    
// PARAMETERS : Pointer to the hash table    
// RETURNS    : none
//
void freeHashTable(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = ht->table[i];
        while (current) {
            Book* temp = current;
            current = current->next;
            free(temp);
        }
        ht->table[i] = NULL;
    }
}
