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
    UPDATE_USER
} manageUserOptions;

typedef enum {
    ADD_BOOK = 1,
    REMOVE_BOOK,
    UPDATE_BOOK
} manageBookOptions;

typedef enum {
    SEARCH_BOOK = 1,
    SEARCH_USER
} searchOptions;

typedef enum {
    DISP_BOOK = 1,
    DISP_USER
} displayOptions;

#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 50
#define MAX_NAME_LEN 50
#define TABLE_SIZE 1000 

typedef struct Book {
    int hashCode;
    char title[100];
    char author[100];
    struct User* borrowedBy;
    struct Book* next;
} Book;

typedef struct User {
    char firstName[50];
    char lastName[50];
    int userId;
    struct User* next; 
} User;

typedef struct HashTable {
    Book* table[TABLE_SIZE];
    User* users[TABLE_SIZE]; 
} HashTable;

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
void printBooks(const HashTable* ht);
void printUsers(HashTable* ht);
void freeHashTable(HashTable* ht);
void databaseMenu(HashTable* ht);
void searchMenu(HashTable* ht);