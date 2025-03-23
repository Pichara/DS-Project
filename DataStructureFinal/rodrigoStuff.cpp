#include "rodrigoStuff.h"

//Including nickstuff to use the user and book structures
#include "nickStuff.h"

//
// FUNCTION   : removeBook
// DESCRIPTION: removes a book from the hash table, using the title to find it               
// PARAMETERS : Pointer to the hash table 
// RETURNS    : none
//
void removeBook(HashTable* ht) {
    char title[MAX_TITLE_LEN];
    printf("Enter the title of the book to remove: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0';

    int bookHashCode = generateBookHash(title);
    int index = bookHashCode % TABLE_SIZE;

    Book* current = ht->table[index];
    Book* prev = NULL;

    //Search the linked list at that index for the matching book
    while (current != NULL) {
        if (current->hashCode == bookHashCode && strcmp(current->title, title) == 0) {
            if (prev == NULL) {
                ht->table[index] = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current);
            printf("The book '%s' was removed successfully\n", title);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Book '%s' not found\n", title);

	//Remove the book from the file | Pichara implementing..
}


//
// FUNCTION   : updateBook
// DESCRIPTION: Lets you edit a book's title/author. We re-hash if the title is changed.          
// PARAMETERS : Pointer to the hash table 
// RETURNS    : none
//
void updateBook(HashTable* ht) {
    char oldTitle[MAX_TITLE_LEN];

    printf("Enter the current title of the book to update: ");
    fgets(oldTitle, sizeof(oldTitle), stdin);
    oldTitle[strcspn(oldTitle, "\n")] = '\0';

    int oldBookHash = generateBookHash(oldTitle);
    int index = oldBookHash % TABLE_SIZE;

    Book* current = ht->table[index];
    while (current != NULL) {
		//If the book is found, asks for new title and author
        if (current->hashCode == oldBookHash && strcmp(current->title, oldTitle) == 0) {
            char newTitle[MAX_TITLE_LEN], newAuthor[MAX_AUTHOR_LEN];

            printf("Enter new title (leave blank to keep current): ");
            fgets(newTitle, sizeof(newTitle), stdin);
            newTitle[strcspn(newTitle, "\n")] = '\0';

            printf("Enter new author (leave blank to keep current): ");
            fgets(newAuthor, sizeof(newAuthor), stdin);
            newAuthor[strcspn(newAuthor, "\n")] = '\0';

            //If the user typed in a new title, it 're-hash' it
            if (strlen(newTitle) > 0) {
                //First remove from old index
                Book* temp = current;
                //If it's at the head of the chain
                if (ht->table[index] == temp) {
                    ht->table[index] = temp->next;
                }
                else {
                    //Unlink by searching
                    Book* prev = ht->table[index];
                    while (prev->next != temp) {
                        prev = prev->next;
                    }
                    prev->next = temp->next;
                }

                //Update the title
                strcpy_s(current->title, newTitle);

                //Recalculate the hash and new index
                current->hashCode = generateBookHash(newTitle);
                int newIndex = current->hashCode % TABLE_SIZE;

                //Insert at the new index
                current->next = ht->table[newIndex];
                ht->table[newIndex] = current;
            }
            //If the user typed in a new author
            if (strlen(newAuthor) > 0) {
                strcpy_s(current->author, newAuthor);
            }
            printf("Book updated successfully\n");
            return;
        }
        current = current->next;
    }
    printf("Book '%s' not found\n", oldTitle);

	//Update the book in the file | Pichara implementing...
}


//
// FUNCTION   : removeUser
// DESCRIPTION: Removes a user from the hash table, using the last name to find them     
// PARAMETERS : Pointer to the hash table 
// RETURNS    : none
//
void removeUser(HashTable* ht) {
    char lastName[MAX_NAME_LEN];
    
    printf("Enter the last name of the user to remove: ");
    fgets(lastName, sizeof(lastName), stdin);
    lastName[strcspn(lastName, "\n")] = '\0';

    int userHashCode = generateUserHash(lastName);
    int index = userHashCode % TABLE_SIZE;

    User* current = ht->users[index];
    User* prev = NULL;

    while (current != NULL) {
        //Check matching userId and last name
        if (current->userId == userHashCode && strcmp(current->lastName, lastName) == 0) {
            if (prev == NULL) {
                ht->users[index] = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current);
            printf("User '%s' removed successfully\n", lastName);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("User '%s' not found\n", lastName);

	//Remove the user from the file | Pichara implementing...
}

//
// FUNCTION   : updateUser
// DESCRIPTION: Removes a user from the hash table, using the last name to find them     
// PARAMETERS : Pointer to the hash table 
// RETURNS    : none
//
void updateUser(HashTable* ht) {
    char oldLastName[MAX_NAME_LEN];

    printf("Enter the last name of the user to update: ");
    fgets(oldLastName, sizeof(oldLastName), stdin);
    oldLastName[strcspn(oldLastName, "\n")] = '\0';

    int oldUserHash = generateUserHash(oldLastName);
    int index = oldUserHash % TABLE_SIZE;

    User* current = ht->users[index];
    while (current != NULL) {
        if (current->userId == oldUserHash && strcmp(current->lastName, oldLastName) == 0) {
            //Ask user for the updates
            char newFirstName[MAX_NAME_LEN], newLastName[MAX_NAME_LEN];
            printf("Enter new first name (leave blank to keep current): ");
            fgets(newFirstName, sizeof(newFirstName), stdin);
            newFirstName[strcspn(newFirstName, "\n")] = '\0';

            printf("Enter new last name (leave blank to keep current): ");
            fgets(newLastName, sizeof(newLastName), stdin);
            newLastName[strcspn(newLastName, "\n")] = '\0';

            if (strlen(newFirstName) > 0) {
                strcpy_s(current->firstName, newFirstName);
            }

            //If last name changes, remove from old index, re-hash, and reinsert
            if (strlen(newLastName) > 0) {
                User* temp = current;

                //Unlink from old slot
                if (ht->users[index] == temp) {
                    ht->users[index] = temp->next;
                }
                else {
                    User* prev = ht->users[index];
                    while (prev->next != temp) {
                        prev = prev->next;
                    }
                    prev->next = temp->next;
                }

                //Update last name
                strcpy_s(temp->lastName, newLastName);
                temp->userId = generateUserHash(temp->lastName);

                //Insert in the new slot
                int newIndex = temp->userId % TABLE_SIZE;
                temp->next = ht->users[newIndex];
                ht->users[newIndex] = temp;
            }
            printf("User updated successfully\n");
            return;
        }
        current = current->next;
    }
    printf("User '%s' not found\n", oldLastName);
}


// ================== QUEUE FUNCTIONS ==================


//
// FUNCTION   : borrowBook
// DESCRIPTION: Called from checkOutMenu option 1  
// PARAMETERS : Pointer to the hash table 
// RETURNS    : none
//
void borrowBook(HashTable* ht) {
    char lastName[MAX_NAME_LEN];
    printf("Enter the last name of the user who wants to borrow: ");
    fgets(lastName, sizeof(lastName), stdin);
    lastName[strcspn(lastName, "\n")] = '\0';

    char bookTitle[MAX_TITLE_LEN];
    printf("Enter the title of the book to borrow: ");
    fgets(bookTitle, sizeof(bookTitle), stdin);
    bookTitle[strcspn(bookTitle, "\n")] = '\0';

    //Look up user and book
    int userHash = generateUserHash(lastName);
    User* user = searchUserByHash(ht, userHash);
    if (!user) {
        printf("User '%s' not found\n", lastName);
        return;
    }

    int bookHash = generateBookHash(bookTitle);
    Book* book = searchBookByHash(ht, bookHash);
    if (!book) {
        printf("Book '%s' not found\n", bookTitle);
        return;
    }

    //If it's borrowed, queue the user. Otherwise, assign borrowedBy
    if (book->borrowedBy) {
        printf("Book is currently borrowed; user '%s %s' added to the waiting queue\n",
            user->firstName, user->lastName);
        enqueueUser(book, user);
    }
    else {
        book->borrowedBy = user;
        printf("Book '%s' is now borrowed by '%s %s'\n",
            book->title, user->firstName, user->lastName);
    }
}


//
// FUNCTION   : borrowBook
// DESCRIPTION: Called from checkOutMenu option 2
// PARAMETERS : Pointer to the hash table 
// RETURNS    : none
//
void returnBook(HashTable* ht) {
    char bookTitle[MAX_TITLE_LEN];

    printf("Enter the title of the book to return: ");
    fgets(bookTitle, sizeof(bookTitle), stdin);
    bookTitle[strcspn(bookTitle, "\n")] = '\0';

    int bookHash = generateBookHash(bookTitle);
    Book* book = searchBookByHash(ht, bookHash);
    if (!book) {
        printf("Book '%s' not found.\n", bookTitle);
        return;
    }

    //If it was borrowed, free it up. Then see if the queue has anyone waiting
    if (book->borrowedBy) {
        printf("Book '%s' returned by '%s %s'\n",
            book->title,
            book->borrowedBy->firstName,
            book->borrowedBy->lastName);

        book->borrowedBy = NULL;

        //Dequeue the next waiting user, if any
        User* nextUser = dequeueUser(book);
        if (nextUser) {
            //Immediately assign the book to the next user in queue
            book->borrowedBy = nextUser;
            printf("Book '%s' is now automatically borrowed by '%s %s'\n", book->title, nextUser->firstName, nextUser->lastName);
        }
    }
    else {
        printf("That book was not borrowed\n");
    }
}


//
// FUNCTION   : borrowBook
// DESCRIPTION: Display a menu for borrowing or returning a book
// PARAMETERS : Pointer to the hash table 
// RETURNS    : none
//
void processBookMenu(HashTable* ht) {
	checkOutOptions choice;
    printf("Please choose an option:\n");
    printf("1. Borrow a book\n");
    printf("2. Return a book\n");
    printf("Enter your choice: ");
    choice = (checkOutOptions)GetValidIntegerInput();
    switch (choice) {
    case BORROW_BOOK:
        borrowBook(ht);
        break;
    case RETURN_BOOK:
        returnBook(ht);
        break;
    default:
        printf("Invalid choice!\n");
    }
}

//
// FUNCTION   : enqueueUser
// DESCRIPTION: Adds a user to the end of the queue for a book
// PARAMETERS : book - the book to queue the user for
// RETURNS    : none
//
void enqueueUser(Book* book, User* user) {
    //Make a new node
    BookQueueNode* newNode = (BookQueueNode*)malloc(sizeof(BookQueueNode));
	if (newNode == NULL) {
		printf("Memory allocation failed\n");
		return;
	}

    newNode->user = user;
    newNode->next = NULL;

    if (book->queueRear == NULL) {
        //Empty queue, front = rear = new node
        book->queueFront = newNode;
        book->queueRear = newNode;
    }
    else {
        //Attach new node at the rear
        book->queueRear->next = newNode;
        book->queueRear = newNode;
    }
}


//
// FUNCTION   : dequeueUser
// DESCRIPTION: Removes a user from the front of the queue for a book
// PARAMETERS : book - the book to dequeue the user from
// RETURNS    : the user that was dequeued, or NULL if the queue was empty
//
User* dequeueUser(Book* book) {
    if (book->queueFront == NULL) {
        //Queue is empty
        return NULL;
    }
    //Take the user from the front
    BookQueueNode* frontNode = book->queueFront;
    User* frontUser = frontNode->user;

    //Advance the front pointer
    book->queueFront = frontNode->next;
    if (book->queueFront == NULL) {
        //Queue is now empty
        book->queueRear = NULL;
    }

    free(frontNode);
    return frontUser;
}


//
// FUNCTION   : databaseMenu    
// DESCRIPTION: Secondary menu to display the results of the users or books                    
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

// ================== BINARY SEARCH TREE FUNCTIONS ==================
// These functions are used to sort and display the users and books in a BST ( please dont touch it, risk of stop working by your own =))


//
// FUNCTION   : compareUsersById
// DESCRIPTION: Compares two users by their userId(hash), for sorting purposes
// PARAMETERS : u1 - first user, u2 - second user  
// RETURNS    : -1 if u1 comes before u2, 0 if they are the same, 1 if u1 comes after u2
//
int compareUsersById(User* u1, User* u2) {
    //Sorting by userId (the "hash", the index)
    if (u1->userId < u2->userId) {
        return -1;
    }
    if (u1->userId > u2->userId) {
        return 1;
    }
    return 0;
}


//
// FUNCTION   : compareBooksByIndex   
// DESCRIPTION: Compares two books by their hashCode, for sorting purposes                 
// PARAMETERS : b1 - first book, b2 - second book  
// RETURNS    : -1 if b1 comes before b2, 0 if they are the same, 1 if b1 comes after b2
//
int compareBooksByIndex(Book* b1, Book* b2) {
	//Checking if the book is borrowed or not
    if (b1->borrowedBy == NULL && b2->borrowedBy != NULL) {
        return -1;
    }
    if (b1->borrowedBy != NULL && b2->borrowedBy == NULL) {
        return 1;
    }

    //If both are either available or borrowed, compare hashCode
	//Negative if b1 comes before b2,
    //Zero if both are the same
    //Positive if b1 comes after b2
    return b1->hashCode - b2->hashCode;
}


//
// FUNCTION   : insertUserBST
// DESCRIPTION: Inserts a user into a binary search tree, sorted by userId   
// PARAMETERS : root - the root of the tree, user - the user to insert
// RETURNS    : the new root of the tree
//
UserBSTNode* insertUserBST(UserBSTNode* root, User* user) {
    if (!root) {
        //Creates new node
        UserBSTNode* newNode = (UserBSTNode*)malloc(sizeof(UserBSTNode));

		//Check if the new node is NULL
		if (newNode == NULL) {
			printf("Memory allocation failed\n");
			return NULL;
		}
        newNode->data = user;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

	//Place the value in the right place of the tree
    int cmp = compareUsersById(user, root->data);
    if (cmp < 0) {
        root->left = insertUserBST(root->left, user);
    }
    else {
        root->right = insertUserBST(root->right, user);
    }
    return root;
}

//
// FUNCTION   : insertBookBST   
// DESCRIPTION: Inserts a book into a binary search tree, sorted by hashCode                 
// PARAMETERS : root - the root of the tree, book - the book to insert  
// RETURNS    : the new root of the tree
//
BookBSTNode* insertBookBST(BookBSTNode* root, Book* book) {
    if (!root) {
        //Creates new node
        BookBSTNode* newNode = (BookBSTNode*)malloc(sizeof(BookBSTNode));
		
        //Check if the new node is NULL
        if (newNode == NULL) {
			printf("Memory allocation failed\n");
			return NULL;
		}
		
        newNode->data = book;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

	//Place the value in the right place of the tree
    int cmp = compareBooksByIndex(book, root->data);
    if (cmp < 0) {
        root->left = insertBookBST(root->left, book);
    }
    else {
        root->right = insertBookBST(root->right, book);
    }
    return root;
}

//
// FUNCTION   : inOrderPrintUsers
// DESCRIPTION: Prints all users in a BST in ascending order of userId using recursive functions
// PARAMETERS : root - the root of the tree
// RETURNS    : none
//
void inOrderPrintUsers(UserBSTNode* root) {
    if (!root) {
        return;
    }
    
    inOrderPrintUsers(root->left);

    printf("User ID: %d | Name: %s %s\n",
        root->data->userId,
        root->data->firstName,
        root->data->lastName);

    inOrderPrintUsers(root->right);
}

//
// FUNCTION   : freeUserBST   
// DESCRIPTION: Frees all nodes in a user BST, but not the actual user objects                 
// PARAMETERS : root - the root of the tree  
// RETURNS    : none
//
void freeUserBST(UserBSTNode* root) {
    if (!root) {
        return;
    }
    freeUserBST(root->left);
    freeUserBST(root->right);
    free(root);
}

//
// FUNCTION   : inOrderPrintBooks   
// DESCRIPTION: Prints all books in a BST in ascending order of hashCode using recursive functions               
// PARAMETERS : root - the root of the tree  
// RETURNS    : none
//
void inOrderPrintBooks(BookBSTNode* root) {
    if (!root) {
        return;
    }

    inOrderPrintBooks(root->left);

    //Print info
    printf("BookID: %d | Title: %s | Author: %s | %s\n",
        root->data->hashCode,
        root->data->title,
        root->data->author,
        (root->data->borrowedBy ? "Borrowed" : "Available"));

    inOrderPrintBooks(root->right);
}

//
// FUNCTION   : freeBookBST   
// DESCRIPTION: Frees all nodes in a book BST, but not the actual book objects                 
// PARAMETERS : root - the root of the tree   
// RETURNS    : none
//
void freeBookBST(BookBSTNode* root) {
    if (!root) return;
    freeBookBST(root->left);
    freeBookBST(root->right);
    free(root);
}


//
// FUNCTION   : printUsers   
// DESCRIPTION: Prints all users in the hash table in ascending order of userId                 
// PARAMETERS : ht - the hash table  
// RETURNS    : none
//
void printUsers(HashTable* ht) {
    printf("Listing all users:\n");

    UserBSTNode* root = NULL;

    //Insert users from each index into BST
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = ht->users[i];
        while (current) {
            root = insertUserBST(root, current);
            current = current->next;
        }
    }

    inOrderPrintUsers(root);

    freeUserBST(root);
}

//
// FUNCTION   : printBooks   
// DESCRIPTION: Prints all books in the hash table, first available then borrowed, in ascending order of hashCode                 
// PARAMETERS : ht - the hash table  
// RETURNS    : none
//
void printBooks(HashTable* ht) {
    printf("Listing all books:\n");

    BookBSTNode* root = NULL;

    //Insert books from each index into BST
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = ht->table[i];
        while (current) {
            root = insertBookBST(root, current);
            current = current->next;
        }
    }

    inOrderPrintBooks(root);

    freeBookBST(root);
}
