//Avoiding multiple inclusions
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 50
#define MAX_NAME_LEN 50
#define TABLE_SIZE 1000
#define MAX_LOG_LEN 256

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