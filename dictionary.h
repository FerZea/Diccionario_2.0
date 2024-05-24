#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entity.h"



int initializeDataDictionary(const char * filename);
int openDataDictionary(const char * filename);

int newDataEntity(const char * filename, const char * entityName);
void printDataDictionary(const char * filename);
int removeDataEntity(const char * filename, const char * entityName);
int modifyEntity(const char* nameDataDictionary, long currentEntityPointer, const char* oldEntity, const char* newEntity);
int newDataAttribute(const char * filename, const char* entityName, Attribute *dataAttribute);
int deleteDataAttribute(const char * filename, const char* entityName, const char* attributeName);
int modifyAttribute(const char* nameDataDictionary, const char *entityName, const char* oldAttribute, const char* newAttribute);

#endif