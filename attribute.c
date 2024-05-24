#include "attribute.h"

int appendAttribute(FILE* dataDictionary, Attribute *attribute){
    
    
    int operationResult = EXIT_SUCCESS;
    long newAttributePosition = 0;
    const long nullPointer = -1L;
    
    fseek(dataDictionary, 0, SEEK_END);

    newAttributePosition = ftell(dataDictionary);
    attribute->FileDirection = newAttributePosition;

    // Add the attribute name
    fwrite(attribute->name, sizeof(char), sizeof(attribute->name), dataDictionary);
    
    //type int
    fwrite(&attribute->Type, sizeof(int), 1, dataDictionary);
    
    //length
    fwrite(&attribute->length, sizeof(int), 1, dataDictionary);

    fwrite(&nullPointer, sizeof(long), 1, dataDictionary);


    printf("New attribute \"%s\" added at position %ld\n", attribute->name, attribute->FileDirection);


    return operationResult;
}

void printAttributeData(FILE *dataDictionary,long currentAtttributePointer)
{

    long headerValue = -1;
    
    printf("Current pointer: %ld\n", currentAtttributePointer);
    
    fseek(dataDictionary, currentAtttributePointer, SEEK_SET);
    fread(&headerValue, sizeof(headerValue), 1, dataDictionary);

    if(headerValue == -1L) {
        return;
    }
    else {
        char currentAttributeName[ATTRIBUTE_NAME_LENGTH];
        long nextAttributePointer;

        // Go to the Entity location and read its data.
        fseek(dataDictionary, headerValue, SEEK_SET);
        // Read the name for the entity at the current position.
        fread(&currentAttributeName, sizeof(char), ATTRIBUTE_NAME_LENGTH, dataDictionary); 

        printf("Attribute: %s\n", currentAttributeName);
        nextAttributePointer = ftell(dataDictionary) + sizeof(int)*2;
        int type, lenght;
        fread(&type, sizeof(int), 1, dataDictionary);
        fread(&lenght, sizeof(int), 1, dataDictionary);
        printf("Type: %d\n", type);
        printf("Lenght: %d\n", lenght);

        printAttributeData(dataDictionary, nextAttributePointer);
    } 



}


void reassingAttributePointer(FILE *dataDictionary, long currentAttributePointer, Attribute *newAttribute){
    
    long headerValue = -1;
    
    fseek(dataDictionary, currentAttributePointer, SEEK_SET);
    fread(&headerValue, sizeof(headerValue), 1, dataDictionary);

    if(headerValue == -1L) 
    {
       
        fseek(dataDictionary, currentAttributePointer, SEEK_SET);
        fwrite(&newAttribute->FileDirection, sizeof(long), 1, dataDictionary);
        printf("Attribute \"%s\" added at position %ld\n", newAttribute->name, newAttribute->FileDirection);
        
    }
    else {
        char currentAttributeName[ATTRIBUTE_NAME_LENGTH];
        long nextAttributeDirection;
        long nextHeaderPointer;

        // Go to the Attribute location and read its data.
        fseek(dataDictionary, headerValue, SEEK_SET);
        // Read the name for the attribute at the current position.
        fread(&currentAttributeName, sizeof(char), ATTRIBUTE_NAME_LENGTH, dataDictionary); 
        nextHeaderPointer = ftell(dataDictionary) + sizeof(int)*2;
        // Compare the Attribute names to determine whether the current attribute should be before the new one or not.
        if(strcmp(currentAttributeName, newAttribute->name) < 0) {
            // Current attribute is before the new one.
            
            reassingAttributePointer(dataDictionary, nextHeaderPointer, newAttribute);
        }
        else {
            // New attribute should be before the current one.
            fseek(dataDictionary, currentAttributePointer, SEEK_SET);
            fwrite(&newAttribute->FileDirection, sizeof(long), 1, dataDictionary);
            
            fseek(dataDictionary, newAttribute->FileDirection + ATTRIBUTE_NAME_LENGTH + sizeof(int)*2, SEEK_SET);
            fwrite(&headerValue, sizeof(long), 1, dataDictionary);

    
        }
    }
    
    
}



int removeAttribute(FILE *dataDictionary, long pointerAttribute, const char *AttributeName)
{

    long headerValue = -1;
    long nextAttributePointer;
    char currentAttributeName[ATTRIBUTE_NAME_LENGTH];
    char currentEntityName[50];
   

    //pointerAttribute es el de la entidad a buscar
    fseek(dataDictionary, pointerAttribute, SEEK_SET);
    fread(&headerValue, sizeof(headerValue), 1, dataDictionary);

    if (headerValue == -1L)
    {
        printf("Attribute not found\n");
        return EXIT_FAILURE;
    }
    else
    {
        
        fseek(dataDictionary, headerValue, SEEK_SET);

        fread(&currentAttributeName, sizeof(char), ATTRIBUTE_NAME_LENGTH, dataDictionary);
       
        nextAttributePointer = ftell(dataDictionary) + sizeof(int)*2;
  
        if (strcmp(currentAttributeName, AttributeName) == 0)
        {

            printf("Attribute found\n");
            
            fseek(dataDictionary, nextAttributePointer, SEEK_SET);
            long valueNextAttribute;
            fread(&valueNextAttribute, sizeof(long), 1, dataDictionary); 

            fseek(dataDictionary, pointerAttribute, SEEK_SET);
            fwrite(&valueNextAttribute, sizeof(long), 1, dataDictionary);

            return EXIT_SUCCESS;
        }
        else
        {
            
            return removeAttribute(dataDictionary, nextAttributePointer, AttributeName);
        }
    }
}