//Avoiding multiple inclusions
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Importing nickStuff.h to use the HashTable struct and the function GetValidIntegerInput
#include "nickStuff.h"

typedef enum {
    BORROW_BOOK = 1,
	RETURN_BOOK,
} checkOutOptions;


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



//File IO functions
//void saveUserInFile(HashTable* ht); Pichara Implementing
//void removeUserInFile(HashTable* ht); Pichara Implementing
//void saveBookInFile(HashTable* ht); Pichara Implementing
//void removeBookInFile(HashTable* ht); Pichara Implementing
//void updateBookInFile(HashTable* ht); Pichara Implementing

//Basic Logic Hash Table functions
void removeBook(HashTable* ht);
void updateBook(HashTable* ht);
void removeUser(HashTable* ht);
void updateUser(HashTable* ht);
void processBookMenu(HashTable* ht);
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