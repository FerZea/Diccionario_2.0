#include"dictionary.h"

int initializeDataDictionary(const char * filename) {
    long header = -1L;
    FILE *file = fopen(filename, "wb");

    if (file == NULL) {
        fprintf(stderr, "Failed to create the data dictionary.\n");
        return EXIT_FAILURE;
    }

    if (fwrite(&header, sizeof(header), 1, file) != 1) {
        fprintf(stderr, "Failed to initialize the data dicitonary.\n");
        return EXIT_FAILURE;
    }

    fclose(file);
    return EXIT_SUCCESS;
    
}

int openDataDictionary(const char * filename) {
    int opeationResult = EXIT_SUCCESS;
    long header;
    FILE *file = fopen(filename, "rb");

    if(file) {
        if (fread(&header, sizeof(header), 1, file) != 1) {
            fprintf(stderr, "Failed to read data.\n");
            opeationResult = EXIT_FAILURE;
        }
        else {
            printf("The header value is: %ld\n", header);
        }
    }
    else {
        fprintf(stderr, "Failed to open the data dictionary.\n");
        opeationResult = EXIT_FAILURE;
    }

    fclose(file);

    return opeationResult;
}

int newDataEntity(const char * filename, const char * entityName) {
    int operationResult = EXIT_SUCCESS;
    FILE *file = fopen(filename, "r+b");

    if(file) {
        Entity dataEntity;

        strcpy(dataEntity.Name, entityName);
        if(appendEntity(file, &dataEntity) == EXIT_SUCCESS) {
          
            reassignEntityPointers(file, MAIN_HEADER_POSITION, dataEntity.Name, dataEntity.FileDirection);
        }
        else {
            fprintf(stderr, "Failed to create the \"%s\" entity.\n", entityName);
            operationResult = EXIT_FAILURE;
        }
    }
    else {
        fprintf(stderr, "Failed to open the data dictionary.\n");
        operationResult = EXIT_FAILURE;
    }

    fclose(file);

    return operationResult;
}


int removeDataEntity(const char * filename, const char * entityName) {
    int operationResult = EXIT_SUCCESS;
    FILE *file = fopen(filename, "r+b");

    if(file) {

         if(removeEntity(file, MAIN_HEADER_POSITION, entityName) == EXIT_SUCCESS) {
            printf("Entity \"%s\" removed successfully.\n", entityName);
         }
         else {
            fprintf(stderr, "Failed to remove the \"%s\" entity.\n", entityName);
            operationResult = EXIT_FAILURE;
         }

        fclose(file);
    }
    else {
        fprintf(stderr, "Failed to open the data dictionary.\n");
        operationResult = EXIT_FAILURE;
    }

}



int modifyEntity(const char* nameDataDictionary, long currentEntityPointer, const char* oldEntity, const char* newEntity)
{
    FILE *dataDictionary = fopen(nameDataDictionary, "r+b");
    long currentEntityDirection = -1;
    
    
    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary);

    if (currentEntityDirection == -1L)
    {
        printf("Entity not found.\n");
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

        if (strcmp(currentEntityName, oldEntity) == 0)
        {
            printf("Entity found.\n");
            fseek(dataDictionary, currentEntityDirection + ENTITY_NAME_LENGTH, SEEK_SET);
            long dataDirection;
            fread(&dataDirection, sizeof(long), 1, dataDictionary);
            printf("dataDirection: %ld\n", dataDirection);

            fseek(dataDictionary, currentEntityDirection + ENTITY_NAME_LENGTH + sizeof(long), SEEK_SET);
            long attributeDirection;
            fread(&attributeDirection, sizeof(long), 1, dataDictionary);
            printf("attributeDirection: %ld\n", attributeDirection);
            
            fclose(dataDictionary);

            // lo cierra y lo abre de nuevo para que se guarden los cambios
            removeDataEntity(nameDataDictionary, oldEntity);
            newDataEntity(nameDataDictionary, newEntity);

            dataDictionary = fopen(nameDataDictionary, "r+b");

            long newEntityDirection = searchDataEntity(dataDictionary, MAIN_HEADER_POSITION, newEntity);
           
          
            if(newEntityDirection == -1)
            {
                return EXIT_FAILURE;
            }
        
            printf("newEntityDirection: %ld\n", newEntityDirection);
            fseek(dataDictionary, newEntityDirection + ENTITY_NAME_LENGTH, SEEK_SET);
            fwrite(&dataDirection, sizeof(long), 1, dataDictionary);
            fwrite(&attributeDirection, sizeof(long), 1, dataDictionary);
            

           fclose(dataDictionary);
            return(EXIT_SUCCESS);

        }
        //estara bien ?? si es recursivo pasando el nombre
        fclose(dataDictionary);
        return modifyEntity(nameDataDictionary, nextEntityPointer, oldEntity, newEntity);
    }
}





void printDataDictionary(const char * filename) {
    FILE *file = fopen(filename, "rb");

    if(file) {
        Entity dataEntity;

        printf("Dictionary name: %s\n", filename);
        printEntityData(file, MAIN_HEADER_POSITION);
    }
    else {
        fprintf(stderr, "Failed to open the data dictionary.\n");
    }

    fclose(file);
}


int newDataAttribute(const char * filename, const char* entityName, Attribute *dataAttribute)
{
    int operationResult = EXIT_SUCCESS;
    FILE *file = fopen(filename, "r+b");

    long attributePointer = searchDataEntity(file, MAIN_HEADER_POSITION, entityName);

    if(attributePointer == -1)
    {
        fprintf(stderr, "Entity not found.\n");
        return EXIT_FAILURE;
    }

    attributePointer += ENTITY_NAME_LENGTH + sizeof(int)*2;
    

    if(file) {
      
            if(appendAttribute(file, dataAttribute) == EXIT_SUCCESS)
            {
        
            //fseek(file, attributePointer, SEEK_SET);
            //long attributeHeader = ftell(file);
            
            //reassingAttributePointer(filename, attributeHeader, dataAttribute);
            reassingAttributePointer(file, attributePointer, dataAttribute);

            }
            else
            {
                fprintf(stderr, "Failed to append the attribute.\n");
                operationResult = EXIT_FAILURE;
            }
    
    }
    else {
        fprintf(stderr, "Failed to open the data dictionary.\n");
        operationResult = EXIT_FAILURE;
    }

    if(file) {
        fclose(file);
    }

    return operationResult;
}


int deleteDataAttribute(const char * filename, const char* entityName, const char* attributeName)
{
    int operationResult = EXIT_SUCCESS;
    FILE *file = fopen(filename, "r+b");

    long attributePointer = searchDataEntity(file, MAIN_HEADER_POSITION, entityName);

    if(attributePointer == -1)
    {
        fprintf(stderr, "Entity not found.\n");
        return EXIT_FAILURE;
    }

    attributePointer += ENTITY_NAME_LENGTH + sizeof(int)*2;
    

    if(file) {
      
        if(removeAttribute(file, attributePointer,attributeName) != EXIT_SUCCESS)
        {
           fprintf(stderr, "Failed to remove the attribute.\n");
           operationResult = EXIT_FAILURE;
        }
            
    }
    else {
        fprintf(stderr, "Failed to open the data dictionary.\n");
        operationResult = EXIT_FAILURE;
    }

    if(file) {
        fclose(file);
    }

    return operationResult;
}



int modifyAttribute(const char* nameDataDictionary, const char *entityName, const char* oldAttribute, const char* newAttribute)
{
    FILE *dataDictionary = fopen(nameDataDictionary, "r+b");
    long headerValue = -1;
    
    long currentAttributePointer = searchDataEntity(dataDictionary, MAIN_HEADER_POSITION, entityName);
    if (currentAttributePointer == -1L)
    {
        printf("Entity not found.\n");
        return EXIT_FAILURE;
    }

    currentAttributePointer += ENTITY_NAME_LENGTH + sizeof(long);

    fseek(dataDictionary, currentAttributePointer , SEEK_SET);
    fread(&headerValue, sizeof(long), 1, dataDictionary);

    if(headerValue == -1)
    {
        printf("No attributes.\n");
    }
    else
    {

        fseek(dataDictionary, headerValue, SEEK_SET);
        long nextAttributePointer;
        char currentAttributeName[ATTRIBUTE_NAME_LENGTH];
        fread(&currentAttributeName, sizeof(char), ATTRIBUTE_NAME_LENGTH, dataDictionary);
        nextAttributePointer = ftell(dataDictionary) + sizeof(int)*2;

        while(strcmp(currentAttributeName, oldAttribute) != 0 && nextAttributePointer != -1L)
        {
            fseek(dataDictionary, nextAttributePointer, SEEK_SET);
            fread(&headerValue, sizeof(long), 1, dataDictionary);

            fseek(dataDictionary, headerValue, SEEK_SET);
            fread(&currentAttributeName, sizeof(char), ATTRIBUTE_NAME_LENGTH, dataDictionary);
            nextAttributePointer = ftell(dataDictionary) + sizeof(int)*2;
        }
       
       if(nextAttributePointer == -1L)
         {
              printf("Attribute not found.\n");
              return EXIT_FAILURE;
         }

        fseek(dataDictionary, headerValue + ATTRIBUTE_NAME_LENGTH, SEEK_SET);
        int type , length;
        fread(&type, sizeof(int), 1, dataDictionary);
        fread(&length, sizeof(int), 1, dataDictionary);
        
        fclose(dataDictionary);

        deleteDataAttribute(nameDataDictionary, entityName, oldAttribute);
        Attribute newAttributeStruct;
        newAttributeStruct.Type = type;
        newAttributeStruct.length = length;
        strcpy(newAttributeStruct.name, newAttribute);
        newDataAttribute(nameDataDictionary, entityName, &newAttributeStruct);


    
    }

}