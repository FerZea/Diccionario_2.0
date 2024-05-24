#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "attribute.h"

#define MAIN_HEADER_POSITION 0L
#define ENTITY_NAME_LENGTH 50

typedef struct DataEntity {
    long FileDirection;
    char Name[ENTITY_NAME_LENGTH];
} Entity;


int appendEntity(FILE *dataDictionary, Entity* dataEntity);
long getDataDictionaryHeader(FILE *dataDictionary);
void reassignEntityPointers(FILE *dataDictionary, long currentHeaderPointer, const char* newEntityName, long newEntityDirection);
void printEntityData(FILE *dataDictionary, long currentHeaderPointer);
int removeEntity(FILE * dataDictionary, long currentEntityPointer, const char *entityName);
long searchDataEntity(FILE *dataDictionary, long header, const char *entityName);

#endif