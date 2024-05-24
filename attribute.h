#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ATTRIBUTE_NAME_LENGTH 50

typedef enum AttributeType{
    CHAR,
    INT,
    VARCHAR
}AttributeType;

typedef struct DataAttribute
{
    long FileDirection;
    char name[ATTRIBUTE_NAME_LENGTH];
    AttributeType Type;
    int length;
}Attribute;

int appendAttribute(FILE* dataDictionary, Attribute *attribute);
void reassingAttributePointer(FILE *dataDictionary, long currentAttributePointer, Attribute *newAttribute);
void printAttributeData(FILE *dataDictionary,long currentAtttributePointer);
