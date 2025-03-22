#include "nickStuff.h"
#include "rodrigoStuff.h"

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
    mainMenuOptions choice;
    do {
        displayMainMenu();
        choice = (mainMenuOptions)GetValidIntegerInput();
        switch (choice) {
        case MANAGE_USER:
            printf("\nManage Users:\n");
            manageUserMenu(&ht);
            break;
        case MANAGE_BOOK:
            printf("\nManage Books:\n");
            manageBookMenu(&ht);
            break;
        case SEARCH_SYS:
            printf("\nSearch System:\n");
            searchMenu(&ht);
            break;
        case CHECK_BOOKOUT:
			printf("\nProcess book for sign out:\n");
			checkOutMenu(&ht); //Pichara implementing...
            break;
        case DISPLAY_DB:
            printf("\nDisplaying Database:\n");
            databaseMenu(&ht);
            break;
        case UNDO:
			printf("\nUndoing last action:\n");
			//Tatiana will implement this
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
    printf("\nLibAid Book System v1.02\n");
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
// FUNCTION   : manageUserMenu  
// DESCRIPTION: Secondary menu used when the user has selected they want to manage a book
//                    
// PARAMETERS : Pointer to the hash table   
// RETURNS    : none    
//
void manageUserMenu(HashTable* ht) {
    manageUserOptions choice;
    printf("Please choose an option:\n");
    printf("1. Add a new user\n");
    printf("2. Remove exisitng user\n");
	printf("3. Update existing user\n");
    printf("Enter your choice: ");
    choice = (manageUserOptions)GetValidIntegerInput();
    switch (choice) {
    case ADD_USER:
        addUser(ht);
        break;
    case REMOVE_USER:
		removeUser(ht); //Pichara implementing...
		break;
    case UPDATE_USER:
		updateUser(ht); //Pichara implementing...
		break;
    default:
        printf("Please only enter the valid integer options (1,2,3)\n");
    }
}

//
// FUNCTION   : manageBookMenu  
// DESCRIPTION: Secondary menu used when the user has selected they want to manage a user
//                    
// PARAMETERS : Pointer to the hash table   
// RETURNS    : none    
//
void manageBookMenu(HashTable* ht) {
    manageBookOptions choice;
    printf("Please choose an option:\n");
    printf("1. Add a books\n");
    printf("2. Remove existing book\n");
	printf("3. Update existing book\n");
    printf("Enter your choice: ");
    choice = (manageBookOptions)GetValidIntegerInput();
    switch (choice) {
    case ADD_BOOK:
        addBook(ht);
        break;
    case REMOVE_BOOK:
		printf("Remove book\n");
		removeBook(ht); //Pichara implementing...
        break;
	case UPDATE_BOOK:
		updateBook(ht); //Pichara implementing...
        break;
    default:
        printf("Please only enter the valid integer options (1,2,3)\n");
    }
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

    //Insert the user at the beginning of the linked list at the appropriate index
    newUser->next = ht->users[userId];

    //Insert the new user into the hash table's linked list at the calculated index
    ht->users[userId] = newUser;

    //Confirm the user was added
    printf("User %s %s with ID %d has been added.\n", firstName, lastName, userId);

	//Add the data to a file for storage | Pichara implementing...
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
    newBook->next = ht->table[index];
    ht->table[index] = newBook;

    printf("Book '%s' by '%s' has been added at index %d.\n", title, author, index);

	//Add the data to a file for storage | Pichara implementing...
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
// DESCRIPTION: Multiplies the char ASCII value by 31 and adds it to total
//              Then moduluses the result by the size of the table to ensure its in range
//                    
// PARAMETERS : Pointer to the title and the tablesize   
// RETURNS    : an integer value which is the hash key or index
//
int generateBookHash(const char* title) {
    unsigned int hash = 0;
    while (*title) {
        hash += *title;
        title++;
    }

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
        printf("Please only enter the valid integer options (1,2)\n");
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
        printf("Please only enter the valid integer options (1,2)\n");
    }
}