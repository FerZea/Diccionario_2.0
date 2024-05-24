#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

#define MAX_NAME_SIZE 50

typedef enum DictionaryMenuOption DictionaryMenuOption;

void printMainMenu();
int readUserInput();
int processUserSelection(DictionaryMenuOption userSelection);
void printSubMenu();

void readUserString(const char* prompt, char* buffer, size_t size);
void clearInputBuffer();

#endif