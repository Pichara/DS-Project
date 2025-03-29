#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "nickStuff.h"

//Structures
SnapshotStack* initSnapshotStack(void);
void freeSnapshotStack(SnapshotStack** stack);
void pushSnapshot(HashTable* ht, SnapshotStack* stack);
int popSnapshot(HashTable* destinationHT, SnapshotStack* stack);
HashTable* copyHashTable(HashTable* original);
void overwriteHashTable(HashTable* destinationHT, HashTable* sourceHT);
void undo_last_action(HashTable* ht, SnapshotStack* stack);
void logAction(const char* actionType, const char* details);

