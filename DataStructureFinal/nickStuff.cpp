#include "nickStuff.h"

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

    //Initialize data strcture(s)
    HashTable ht;
    initHashTable(&ht);
    mainMenuOptions choice;
    do {
        displayMainMenu();
        choice = (mainMenuOptions)GetValidIntegerInput(); // Get user input
        switch (choice) {
        case MANAGE_USER:
            printf("Manage Users:\n");
            addUser(&ht);
            break;
        case MANAGE_BOOK:
            printf("Manage Books:\n");
            addBook(&ht);
            break;
        case SEARCH_SYS:
            printf("Search System:\n");
            {
                searchMenu(&ht);
            }
            break;
        case CHECK_BOOKOUT:

            break;
        case DISPLAY_DB:
            printf("Displaying Database:\n");
            databaseMenu(&ht);
            break;
        case UNDO:

            break;
        case EXIT:
            printf("Exiting program...\n");
            freeHashTable(&ht);
            return;
        default:
            printf("Invalid choice! Try again.\n");
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
    printf("\nLibAid Book System v1.01\n");
    printf("1. Manage users\n");
    printf("2. Manage books\n");
    printf("3. Search system\n");
    printf("4. Process book for sign out.\n");
    printf("5. Display database\n");
    printf("6. Undo last action\n");
    printf("7. Exit the program\n");
    printf("Enter your choice: ");
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
    \
        //Initlaize variables
        char firstName[MAX_NAME_LEN], lastName[MAX_NAME_LEN];
    int userId;

    //Prompt the user for their first name
    printf("Enter first name: ");
    fgets(firstName, sizeof(firstName), stdin);
    firstName[strcspn(firstName, "\n")] = '\0';

    //Prompt the user for their last name
    printf("Enter last name: ");
    fgets(lastName, sizeof(lastName), stdin);
    lastName[strcspn(lastName, "\n")] = '\0';

    //Generate the hash for the user based on the last name (this is the userId)
    userId = generateUserHash(lastName, TABLE_SIZE); // Pass table size for hashing
    printf("Generated UserID (Hash): %d\n", userId); // Debugging output

    //Allocate memory for the new user
    User* newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        printf("Memory allocation failed for new user!\n"); // Debugging output
        return;
    }

    //Copy the provided first name and last name to the new user structure
    strcpy_s(newUser->firstName, sizeof(newUser->firstName), firstName);
    strcpy_s(newUser->lastName, sizeof(newUser->lastName), lastName);

    //Assign the generated hash as the userId
    newUser->userId = userId;

    //Insert the user at the beginning of the linked list at the appropriate index
    newUser->next = ht->users[userId];

    //Insert the new user into the hash table's linked list at the calculated index
    ht->users[userId] = newUser;

    //Confirm the user was added
    printf("User %s %s with ID %d has been added.\n", firstName, lastName, userId);
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

    //Prompt the user for the book title
    printf("Enter book title: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';

    //Prompt the user for the book author
    printf("Enter book author: ");
    fgets(author, sizeof(author), stdin);
    author[strcspn(author, "\n")] = '\0';

    //Generate the index number by the hash.
    unsigned int index = generateBookHash(title, TABLE_SIZE);

    //Allocate memory for book.
    Book* newBook = (Book*)malloc(sizeof(Book));
    if (!newBook) {
        printf("Memory allocation failed!\n");
        return;
    }

    //Add the values to the data memebrs for the Book. 
    newBook->hashCode = generateBookHash(title, TABLE_SIZE);
    strcpy_s(newBook->title, sizeof(newBook->title), title);
    strcpy_s(newBook->author, sizeof(newBook->author), author);
    newBook->borrowedBy = NULL;
    newBook->next = ht->table[index];
    ht->table[index] = newBook;

    printf("Book '%s' by '%s' has been added at index %d.\n", title, author, index);
}

//
// FUNCTION   : searchBookByHash    
// DESCRIPTION: Looks through the hash table to find the book that matches the hash
//                    
// PARAMETERS : pointer to the hash table and the hash code    
// RETURNS    : the pointer to the book with its information, should it exist
//
Book* searchBookByHash(const HashTable* ht, unsigned int hashCode) {
    unsigned int index = hashCode % TABLE_SIZE; // Calculate the index using the hash code

    //Traverse the linked list at that index
    Book* current = ht->table[index];
    while (current != NULL) {
        if (current->hashCode == hashCode) {  
            return current;
        }
        current = current->next;
    }

    return NULL;  // Return NULL if no book with that hash is found
}

//
// FUNCTION   : searchUserByHash   
// DESCRIPTION: Looks through the hash table to find the book that matches the hash
//                    
// PARAMETERS : pointer to the hash table and the hash code    
// RETURNS    : the pointer to the book with its information, should it exist
//
User* searchUserByHash(HashTable* ht, int userHashCode) {
    unsigned int index = userHashCode % TABLE_SIZE; 

    User* current = ht->users[index];
    while (current) {
        if (userHashCode == generateUserHash(current->lastName, TABLE_SIZE)) { 
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

    //Initalize hash code variable and get input
    unsigned int bookHashCode;
    printf("Enter hash code to search for the book: ");
    bookHashCode = GetValidIntegerInput();

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
    int userHashCode;

    //Get hash code
    printf("Enter the user hash code to search for: ");
    userHashCode = GetValidIntegerInput();

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
// DESCRIPTION: Multiplies the char ASCII value by 31 and adds it to total
//              Then moduluses the result by the size of the table to ensure its in range
//                    
// PARAMETERS : Pointer to the title and the tablesize   
// RETURNS    : an integer value which is the hash key or index
//
int generateBookHash(const char* title, int tableSize) {
    unsigned int hash = 0;
    while (*title) {
        hash = (hash * 31) + *title;
        title++;
    }
    return hash % tableSize; 
}

//
// FUNCTION   : generateUserHash   
// DESCRIPTION: Generates the user hash by adding the ASCII value of each character in last name
//              Modulused by the tablesiZe to ensure value in range.
//                    
// PARAMETERS : pointer to the last name and the table size  
// RETURNS    : an integer value with the hash value (index)     
//
int generateUserHash(const char* lastName, int tableSize) {
    unsigned int hash = 0;
    while (*lastName) {
        hash += *lastName;
        lastName++;
    }
    return hash % tableSize;
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
// FUNCTION   : printBooks     
// DESCRIPTION: prints out all of the books that are in the list
//              THIS IS FOR DEBUG.  WHOEVER IS DOING TREES NEEDS TO MAKE A TREE TO DISPLAY RESULTS
//                    
// PARAMETERS : Pointer to the hash table   
// RETURNS    : none
//
void printBooks(const HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = ht->table[i];
        while (current) {
            printf("BookID: %d | Title: %s | Author: %s | %s\n", current->hashCode, current->title, current->author,
                (current->borrowedBy ? "Borrowed" : "Available"));
            current = current->next;
        }
    }
}

//
// FUNCTION   : printUsers   
// DESCRIPTION: prints out all of the users that are in the list
//              THIS IS FOR DEBUG.  WHOEVER IS DOING TREES NEEDS TO MAKE A TREE TO DISPLAY RESULTS
//                    
// PARAMETERS : Pointer to the hash table    
// RETURNS    : none    
//
void printUsers(HashTable* ht) {
    printf("Listing all users:\n");

    //Iterate over the entire hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = ht->users[i];

        //If there are users at this index, print their details
        while (current != NULL) {
            printf("User ID: %d, Name: %s %s\n", current->userId, current->firstName, current->lastName);
            current = current->next;
        }
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

//
// FUNCTION   : databaseMenu    
// DESCRIPTION: Secondary menu to display the results of the users or books.
//              
//                    
// PARAMETERS : Pointer to the hash table    
// RETURNS    : none   
//
void databaseMenu(HashTable* ht) {
    displayOptions choice;
    printf("Please choose an option:\n");
    printf("1. View all books\n");
    printf("2. View all users\n");
    printf("Enter your choice: ");
    choice = (displayOptions)GetValidIntegerInput();
    switch (choice) {
    case DISP_BOOK:
        printBooks(ht);
        break;
    case DISP_USER:
        printUsers(ht);
        break;
    default:
        printf("Enter 1 or 2 only.\n");
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
    printf("Please choose an option:\n");
    printf("1. Search for books\n");
    printf("2. Search for users\n");
    printf("Enter your choice: ");
    choice = (searchOptions)GetValidIntegerInput();
    switch (choice) {
    case SEARCH_BOOK:
        searchforBookByHash(ht);
        break;
    case SEARCH_USER:
        searchForUserByHash(ht);
        break;
    default:
        printf("Enter 1 or 2 only.\n");
    }
}