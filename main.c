#include "main.h"


typedef enum DictionaryMenuOption {
    NO_SELECTION,
    NEW_DICTIONARY,
    OPEN_DICTIONARY,
    CLOSE_PROGRAM
} DictionaryMenuOption;


typedef enum DictionarySubOption{
    PRINT_DICT = 4,
    NEW_ENT,
    DELETE_ENT,
    MOD_ENT,
    NEW_ATR,
    DELETE_ATR,
    MOD_ATR,
    RETURN_BACK
}DictionarySubOption;

int main(int argc, char** argv) {
    int currentUserSelection = NO_SELECTION;
    int status;
    while (currentUserSelection != CLOSE_PROGRAM)
    {
        do
        {
        
        printMainMenu();
        currentUserSelection = readUserInput();
        status = processUserSelection(currentUserSelection);
        if(status == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        } 

        }while(currentUserSelection > 3);

        //1system("clear");

        while (currentUserSelection != RETURN_BACK && currentUserSelection != CLOSE_PROGRAM)
        {
            printSubMenu();
            currentUserSelection = readUserInput();
            status = processUserSelection(currentUserSelection);

              if(status == EXIT_FAILURE)
                {
                    return EXIT_FAILURE;
                } 

        }

       
    }

    return EXIT_SUCCESS;
}

void printMainMenu() {
    printf("----Data Dictionary----\n");
    printf("%d) Create a new dictionary.\n", NEW_DICTIONARY);
    printf("%d) Open an existing dictionary.\n", OPEN_DICTIONARY);
    printf("%d) Exit.\n", CLOSE_PROGRAM);
}

void printSubMenu() {
    printf("----SUB MENU----\n");
    printf("%d) Print the dictionary.\n", PRINT_DICT);
    printf("-------\n");
    printf("%d) Create a new entity.\n", NEW_ENT);
    printf("%d) Delete a entity.\n", DELETE_ENT);
    printf("%d) Modify a entity.\n", MOD_ENT);
    printf("-------\n");
    printf("%d) Create a new Attribute.\n", NEW_ATR);
    printf("%d) Delete a Attribute.\n",DELETE_ATR);
    printf("%d) Modify a Attribute.\n", MOD_ATR);
    printf("-------\n");
    printf("%d) Return <-\n",RETURN_BACK);
   
}

int readUserInput() {
    int userSelection;
    printf("Please select an option: ");

    scanf("%d", &userSelection);

    clearInputBuffer();

    return userSelection;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int processUserSelection(DictionaryMenuOption userSelection) {
    static char fileName[MAX_NAME_SIZE];
    char entityName[MAX_NAME_SIZE];
    char newEntityName[MAX_NAME_SIZE];
    Attribute dataAttribute;
    char newAttributeName[MAX_NAME_SIZE];

    switch (userSelection)
    {
        case NEW_DICTIONARY:
            readUserString("Enter the name of the new dictionary: ", fileName, sizeof(fileName));
            if(initializeDataDictionary(fileName)==EXIT_SUCCESS)
            {
                return EXIT_SUCCESS;
            }
            else
            {
                return EXIT_FAILURE;
            }
            break;

        case OPEN_DICTIONARY:
            readUserString("Enter the name of the dictionary to open: ", fileName, sizeof(fileName));
            if(openDataDictionary(fileName)==EXIT_SUCCESS)
            {
                printf("Dictionary opened successfully\n");
                return EXIT_SUCCESS;
            }
            else
            {
                printf("Error opening the dictionary\n");
                return EXIT_FAILURE;
            }
            break;

        case CLOSE_PROGRAM:
            printf("Exiting the program...\n");
            return EXIT_SUCCESS;
            break;

        case PRINT_DICT:
            printDataDictionary(fileName);
            break;

        case NEW_ENT:
            readUserString("Enter the name of the new entity: ", entityName, sizeof(entityName));
            if(newDataEntity(fileName, entityName)==EXIT_SUCCESS)
            {
                printf("Entity created successfully\n");
                return EXIT_SUCCESS;
            }
            else
            {
                printf("Error creating the entity\n");
                return EXIT_FAILURE;
            }
            break;

        case DELETE_ENT:
            readUserString("Enter the name of the entity to delete: ", entityName, sizeof(entityName));
            if(removeDataEntity(fileName, entityName)==EXIT_SUCCESS)
            {
                printf("Entity deleted successfully\n");
                return EXIT_SUCCESS;
            }
            else
            {
                printf("Error deleting the entity\n");
                return EXIT_FAILURE;
            }
            break;
        case MOD_ENT:
            readUserString("Enter the name of the entity to modify: ", entityName, sizeof(entityName));
            readUserString("Enter the new name of the entity: ", newEntityName, sizeof(newEntityName));
            if(modifyEntity(fileName, MAIN_HEADER_POSITION,entityName,newEntityName)==EXIT_SUCCESS)
            {
                printf("Entity modified successfully\n");
                return EXIT_SUCCESS;
            }
            else
            {
                printf("Error modifying the entity\n");
                return EXIT_FAILURE;
            }
            break;
        case NEW_ATR:
            readUserString("Enter the name of the entity to add the attribute: ", entityName, sizeof(entityName));
            readUserString("Enter the name of the new attribute: ", dataAttribute.name, sizeof(dataAttribute.name));
            int typeAsInt; // Variable temporal para almacenar el valor del enum como int
            fscanf(stdin, "%d", &typeAsInt);
            dataAttribute.Type = (AttributeType)typeAsInt; // Convertir el int a AttributeType
            fscanf(stdin, "%d", &dataAttribute.length);
            
            if(newDataAttribute(fileName, entityName, &dataAttribute)==EXIT_SUCCESS)
            {
                printf("Attribute created successfully\n");
                return EXIT_SUCCESS;
            }
            else
            {
                printf("Error creating the attribute\n");
                return EXIT_FAILURE;
            }
        case DELETE_ATR:
            readUserString("Enter the name of the entity to delete the attribute: ", entityName, sizeof(entityName));
            readUserString("Enter the name of the attribute to delete: ", dataAttribute.name, sizeof(dataAttribute.name));
            if(deleteDataAttribute(fileName,entityName,dataAttribute.name)==EXIT_SUCCESS)
            {
                printf("Attribute deleted successfully\n");
                return EXIT_SUCCESS;
            }
            else
            {
                printf("Error deleting the attribute\n");
                return EXIT_FAILURE;
            }
            break;
        case MOD_ATR:
            readUserString("Enter the name of the entity to modify the attribute: ", entityName, sizeof(entityName));
            readUserString("Enter the name of the attribute to modify: ", dataAttribute.name, sizeof(dataAttribute.name));
            readUserString("Enter the new name of the attribute: ", newAttributeName, sizeof(newAttributeName));
            if(modifyAttribute(fileName,entityName,dataAttribute.name,newAttributeName)==EXIT_SUCCESS)
            {
                printf("Attribute modified successfully\n");
                return EXIT_SUCCESS;
            }
            else
            {
                printf("Error modifying the attribute\n");
                return EXIT_FAILURE;
            }
            break;

        default:
            printf("Invalid option!");
            break;
    }


 
    return EXIT_SUCCESS;
   
}

void readUserString(const char* prompt, char* buffer, size_t size) 
{
    if (prompt != NULL) {
        printf("%s", prompt);
    }

    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';  // Handle EOF or read error by clearing buffer
        printf("2. Error reading input\n");
        return;
    }

    // Remove newline character if present
    buffer[strcspn(buffer, "\n")] = '\0';
}