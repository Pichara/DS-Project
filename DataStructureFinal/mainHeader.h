#pragma once

//============================== Includes ==============================\\

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//============================== Constants ==============================\\

#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 50
#define MAX_NAME_LEN 50
#define TABLE_SIZE 1000
#define MAX_LOG_LEN 256
#define MAX_LINES 100

//============================== Enums ==============================\\

typedef enum {
    MANAGE_USER = 1,
    MANAGE_BOOK,
    SEARCH_SYS,
    CHECK_BOOKOUT,
    DISPLAY_DB,
    UNDO,
    EXIT
} mainMenuOptions;

typedef enum {
    ADD_USER = 1,
    REMOVE_USER,
    UPDATE_USER,
    BACK_USER
} manageUserOptions;

typedef enum {
    ADD_BOOK = 1,
    REMOVE_BOOK,
    UPDATE_BOOK,
    BACK_BOOK
} manageBookOptions;

typedef enum {
    SEARCH_BOOK = 1,
    SEARCH_USER,
    BACK_SEARCH
} searchOptions;

typedef enum {
    BORROW_BOOK = 1,
    RETURN_BOOK,
    BACK_PROCESS
} checkOutOptions;

typedef enum {
    DISP_BOOK = 1,
    DISP_USER,
    BACK_DISP
} displayOptions;

typedef enum {
	LAST_ACTION = 1,
	PRINT_LOGS,
	BACK_LAST_ACTION
} lastActionOptions;

//============================== Structs ==============================\\

typedef struct User {
    char firstName[50];
    char lastName[50];
    int userId;
    struct User* next;
} User;

typedef struct BookQueueNode {
    User* user;
    struct BookQueueNode* next;
} BookQueueNode;

typedef struct Book {
    int hashCode;
    char title[100];
    char author[100];
    struct User* borrowedBy;
    BookQueueNode* queueFront;
    BookQueueNode* queueRear;
    struct Book* next;
} Book;

typedef struct HashTable {
    Book* table[TABLE_SIZE];
    User* users[TABLE_SIZE];
} HashTable;

typedef struct SnapshotNode {
    struct HashTable* snapshot;
    struct SnapshotNode* next;
} SnapshotNode;

typedef struct SnapshotStack {
    SnapshotNode* top;
} SnapshotStack;

//BST for USERS
typedef struct UserBSTNode {
    User* data;
    struct UserBSTNode* left;
    struct UserBSTNode* right;
} UserBSTNode;

//BST for BOOKS
typedef struct BookBSTNode {
    Book* data;
    struct BookBSTNode* left;
    struct BookBSTNode* right;
} BookBSTNode;


//============================== Prototypes ==============================\\

//--------------------------- rodrigoStuff ---------------------------\\

//File IO functions
void loadDatabase(HashTable* ht, const char* filename);
void syncDatabaseToFile(HashTable* ht, const char* filename);

//Basic Logic Hash Table functions
void removeBook(HashTable* ht);
void updateBook(HashTable* ht);
void removeUser(HashTable* ht);
void updateUser(HashTable* ht);
void processBookMenu(HashTable* ht, SnapshotStack* UndoStack);
void returnBook(HashTable* ht);
void borrowBook(HashTable* ht);

//Stack Functions
void enqueueUser(Book* book, User* user);
User* dequeueUser(Book* book);


//Binary Search Tree functions
void databaseMenu(HashTable* ht);
int compareUsersById(User* u1, User* u2);
int compareBooksByIndex(Book* b1, Book* b2);
UserBSTNode* insertUserBST(UserBSTNode* root, User* user);
UserBSTNode* insertUserBST(UserBSTNode* root, User* user);
void inOrderPrintUsers(UserBSTNode* root);
void freeUserBST(UserBSTNode* root);
void inOrderPrintBooks(BookBSTNode* root);
void freeBookBST(BookBSTNode* root);
void printUsers(HashTable* ht);
void printBooks(HashTable* ht);


//--------------------------- nickStuff ---------------------------\\

void mainMenu(void);
void displayMainMenu(void);
int GetValidIntegerInput(void);
void addUser(HashTable* ht);
void addBook(HashTable* ht);
Book* searchBookByHash(const HashTable* ht, unsigned int hashCode);
User* searchUserByHash(HashTable* ht, int userHashCode);
void searchforBookByHash(HashTable* ht);
void searchForUserByHash(HashTable* ht);
int generateBookHash(const char* title);
int generateUserHash(const char* lastName);
void initHashTable(HashTable* ht);
void freeHashTable(HashTable* ht);
void searchMenu(HashTable* ht);
void manageUserMenu(HashTable* ht, SnapshotStack* undoStack);
void manageBookMenu(HashTable* ht, SnapshotStack* undoStack);

//--------------------------- tatianaStuff ---------------------------\\

SnapshotStack* initSnapshotStack(void);
void freeSnapshotStack(SnapshotStack** stack);
void pushSnapshot(HashTable* ht, SnapshotStack* stack);
int popSnapshot(HashTable* destinationHT, SnapshotStack* stack);
HashTable* copyHashTable(HashTable* original);
void overwriteHashTable(HashTable* destinationHT, HashTable* sourceHT);
void undo_last_action(HashTable* ht, SnapshotStack* stack);
void logAction(const char* actionType, const char* details);
void getLastLog(void);
void lastActionMenu(HashTable* ht, SnapshotStack* stack);
void printLogs(void);