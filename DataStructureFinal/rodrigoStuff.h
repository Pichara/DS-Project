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


//void saveUserInFile(HashTable* ht); Pichara Implementing
//void removeUserInFile(HashTable* ht); Pichara Implementing
//void saveBookInFile(HashTable* ht); Pichara Implementing
//void removeBookInFile(HashTable* ht); Pichara Implementing
//void updateBookInFile(HashTable* ht); Pichara Implementing

void removeBook(HashTable* ht);
void updateBook(HashTable* ht);
void removeUser(HashTable* ht);
void updateUser(HashTable* ht);
void checkOutMenu(HashTable* ht);
void returnBook(HashTable* ht);
void borrowBook(HashTable* ht);