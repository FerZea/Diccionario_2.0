#include "entity.h"

int appendEntity(FILE *dataDictionary, Entity* dataEntity) {
    int operationResult = EXIT_SUCCESS;
    long newEntityPosition = 0;
    const long nullPointer = -1L;
    
    fseek(dataDictionary, MAIN_HEADER_POSITION, SEEK_END);

    newEntityPosition = ftell(dataDictionary);
    dataEntity->FileDirection = newEntityPosition;

    // Add the entity name
    fwrite(dataEntity->Name, sizeof(char), sizeof(dataEntity->Name), dataDictionary);
    // Data pointer
    fwrite(&nullPointer, sizeof(long), 1, dataDictionary);
    // Attributes pointer
    fwrite(&nullPointer, sizeof(long), 1, dataDictionary);
    // Next entity pointer
    fwrite(&nullPointer, sizeof(long), 1, dataDictionary);

    printf("New entity \"%s\" added at position %ld\n", dataEntity->Name, dataEntity->FileDirection);

    return operationResult;
}

long getDataDictionaryHeader(FILE *dataDictionary) {
    long header;

    fseek(dataDictionary, MAIN_HEADER_POSITION, SEEK_SET);
    fread(&header, sizeof(header), 1, dataDictionary);

    return header;
}

void reassignEntityPointers(FILE *dataDictionary, long currentHeaderPointer, const char* newEntityName, long newEntityDirection) {
    long headerValue = -1;
    
    fseek(dataDictionary, currentHeaderPointer, SEEK_SET);
    fread(&headerValue, sizeof(headerValue), 1, dataDictionary);

    if(headerValue == -1L) {
        // No more entities to iterate. Set the current pointer to the new entity direction.

        printf("in the header value %ld it will be %ld\n", headerValue, newEntityDirection);
        fseek(dataDictionary, currentHeaderPointer, SEEK_SET);
        fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);
    }
    else {
        char currentEntityName[ENTITY_NAME_LENGTH];
        long nextEntityDirection;
        long nextHeaderPointer;

        // Go to the Entity location and read its data.
        fseek(dataDictionary, headerValue, SEEK_SET);
        // Read the name for the entity at the current position.
        fread(&currentEntityName, sizeof(char), ENTITY_NAME_LENGTH, dataDictionary); 
        nextHeaderPointer = ftell(dataDictionary) + (sizeof(long) * 2);
        // Compare the Entity names to determine whether the current entity should be before the new one or not.
        if(strcmp(currentEntityName, newEntityName) < 0) {
            // Current entity is before the new one.
            reassignEntityPointers(dataDictionary, nextHeaderPointer, newEntityName, newEntityDirection);
        }
        else {
            // Reassign first pointer.
            fseek(dataDictionary, currentHeaderPointer, SEEK_SET);
            fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);

            printf("Direction changed \"%s\" %ld\n", currentEntityName, headerValue);

            // Reassign new entity pointer.
            fseek(dataDictionary, newEntityDirection + ENTITY_NAME_LENGTH + (sizeof(long) * 2), SEEK_SET);
            fwrite(&headerValue, sizeof(long), 1, dataDictionary);

            printf("Entity \"%s\" repositioned at position %ld\n", newEntityName, newEntityDirection);
        }
    } 
}

void printEntityData(FILE *dataDictionary, long currentHeaderPointer) {
    long headerValue = -1;
    
    fseek(dataDictionary, currentHeaderPointer, SEEK_SET);
    fread(&headerValue, sizeof(headerValue), 1, dataDictionary);

    if(headerValue == -1L) {
        return;
    }
    else {
        char currentEntityName[ENTITY_NAME_LENGTH];
        long nextHeaderPointer;

        // Go to the Entity location and read its data.
        fseek(dataDictionary, headerValue, SEEK_SET);
        // Read the name for the entity at the current position.
        fread(&currentEntityName, sizeof(char), ENTITY_NAME_LENGTH, dataDictionary); 

        printf("\n");
        printf("Entity: %s\n\n", currentEntityName);
        
        nextHeaderPointer = ftell(dataDictionary) + (sizeof(long) * 2);

        
        printAttributeData(dataDictionary, headerValue+ENTITY_NAME_LENGTH+sizeof(long));
        printEntityData(dataDictionary, nextHeaderPointer);
    } 
}


int removeEntity(FILE * dataDictionary, long currentEntityPointer, const char *entityName)
{
    
    long currentEntityDirection = -1;

    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary);

    if (currentEntityDirection == -1L)
    {
      
        fclose(dataDictionary);
        return EXIT_FAILURE;
    }
    else
    {
        char currentEntityName[ENTITY_NAME_LENGTH];
        long nextEntityPointer;

        // Go to the Entity location and read its data.
        fseek(dataDictionary, currentEntityDirection, SEEK_SET);
        // Read the name for the entity at the current position.
        fread(&currentEntityName, sizeof(char), ENTITY_NAME_LENGTH, dataDictionary);
        nextEntityPointer = ftell(dataDictionary) + (sizeof(long) * 2);

        if (strcmp(currentEntityName, entityName) == 0)
        {
            long nextEntityDirection;

            fseek(dataDictionary, nextEntityPointer, SEEK_SET); 
            fread(&nextEntityDirection, sizeof(nextEntityDirection), 1, dataDictionary);

            fseek(dataDictionary, currentEntityPointer, SEEK_SET); 
            fwrite(&nextEntityDirection, sizeof(nextEntityDirection), 1, dataDictionary);
            
            fprintf(stdout, "Entity %s removed\n", entityName);

            return EXIT_SUCCESS;
        }
       
        return removeEntity(dataDictionary, nextEntityPointer, entityName);
    }
}

long searchDataEntity(FILE *dataDictionary, long header, const char *entityName)
{
    
    long headerValue;

    fseek(dataDictionary, header, SEEK_SET);
    fread(&headerValue, sizeof(long), 1, dataDictionary);
    printf("Header value %ld of searchEntity \n", headerValue);
    if (headerValue == -1L)
    {

       // Close the file before returning
        return headerValue;
    }
    else
    {
        char currentEntityName[ENTITY_NAME_LENGTH];
       
        long nextHeaderPointer;

        // Go to the Entity location and read its data.
        fseek(dataDictionary, headerValue, SEEK_SET);
        fread(&currentEntityName, sizeof(char), ENTITY_NAME_LENGTH, dataDictionary);


        if (strcmp(currentEntityName, entityName) == 0)
        {
           // Close the file before returning
            return headerValue;
        }
        nextHeaderPointer = ftell(dataDictionary) + (sizeof(long) * 2);
        // Close the file before recursive call

        return searchDataEntity(dataDictionary, nextHeaderPointer, entityName);
    }
}

