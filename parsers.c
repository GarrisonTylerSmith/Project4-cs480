/**
  * @file parsers.c
  *
  * @brief Driver program parse structures
  *
  * @details Allows for parsing of the ConfigFile structure
  *          and MetaDataNode linked list
  *
  * @version 1.10
  *          C.S Student (22 February 2017)
  *
  * @note Requires parsers.h
  */

#define LINE_MAX_LENGTH 256

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parsers.h"


int ParseMetaDataLine(MetaDataNode* head, char* line);
int RemoveSpaces(char* string);


int ParseConfig(ConfigFile* configFile, char* pathToFile)
{
    // Attempt to open file path
    FILE* configFP = fopen(pathToFile, "r");
    if (configFP == NULL)
    {
        return FILE_OPEN_ERROR;
    }

    // Create variables to save config data into
    int version = -1;
    char filePath[STRING_MAX_SIZE] = "";
    char schedulingCode[STRING_MAX_SIZE] = "";
    int quantumTime = -1;
    int memoryAvailable = -1;
    int pCycleTime = -1;
    int ioCycleTime = -1;
    char logTo[STRING_MAX_SIZE] = "";
    char logFilePath[STRING_MAX_SIZE] = "";

    // Iterate through each line and find identifiers
    char line[LINE_MAX_LENGTH];
    while (fgets(line, sizeof(line), configFP))
    {
        sscanf(line, "Version/Phase: %d", &version);
        sscanf(line, "File Path: %s", filePath);
        sscanf(line, "CPU Scheduling Code: %s", schedulingCode);
        sscanf(line, "Quantum Time (cycles): %d", &quantumTime);
        sscanf(line, "Memory Available (KB): %d", &memoryAvailable);
        sscanf(line, "Processor Cycle Time (msec): %d", &pCycleTime);
        sscanf(line, "I/O Cycle Time (msec): %d", &ioCycleTime);
        sscanf(line, "Log To: %s", logTo);
        sscanf(line, "Log File Path: %s", logFilePath);
    }

    int schedulingError = CheckSchedulingCode(schedulingCode);
    int logError = CheckLogTo(logTo);

    if (schedulingError == INVALID_ENTRY_ERROR ||
        logError == INVALID_ENTRY_ERROR)
    {
        return INVALID_ENTRY_ERROR;
    }

    // Check if any variable was not found
    if (version == -1 ||
        strcmp(filePath, "") == 0 ||
        strcmp(schedulingCode, "") == 0 ||
        quantumTime == -1 ||
        memoryAvailable == -1 ||
        pCycleTime == -1 ||
        ioCycleTime == -1 ||
        strcmp(logTo, "") == 0 ||
        strcmp(logFilePath, "") == 0)
    {
        return MALFORMED_DATA_ERROR;
    }

    // Save values into the configFile structure
    configFile->version = version;
    strcpy(configFile->filePath, filePath);
    strcpy(configFile->schedulingCode, schedulingCode);
    configFile->quantumTime = quantumTime;
    configFile->memoryAvailable = memoryAvailable;
    configFile->pCycleTime = pCycleTime;
    configFile->ioCycleTime = ioCycleTime;
    strcpy(configFile->logTo, logTo);
    strcpy(configFile->logFilePath, logFilePath);

    fclose(configFP);
    return 0;
}

int ParseMetaData(MetaDataNode* head, char* pathToFile)
{
    // Attempt to open file path
    FILE* metaFP = fopen(pathToFile, "r");
    if (metaFP == NULL)
    {
        return FILE_OPEN_ERROR;
    }

    // Initialize head 
    head->commandLetter = '\0';
    strcpy(head->operation, "");
    head->cycleTime = 0; 

    // Iterate through each line in the file
    char line[LINE_MAX_LENGTH];
    while (fgets(line, sizeof(line), metaFP))
    {
        RemoveSpaces(line);

        if (strcmp(line, "StartProgramMeta-DataCode:") != 0 &&
            strcmp(line, "EndProgramMeta-DataCode.") != 0)
        {
            int lineError = ParseMetaDataLine(head, line);

            if (lineError == MALFORMED_DATA_ERROR)
            {
                return MALFORMED_DATA_ERROR;
            }
            if (lineError == INVALID_ENTRY_ERROR)
            {
                return INVALID_ENTRY_ERROR;
            }
        }
    }

    fclose(metaFP);
    return 0;
}

int ParseMetaDataLine(MetaDataNode* head, char* line)
{
    // Split the line by semi colons
    char* currOperation = strtok(line, ";.");
    while(currOperation != NULL) 
    {
        // Find parenthesis and store their memory address
        char* openParen = strchr(currOperation, '(');
        char* closeParen = strchr(currOperation, ')');
        size_t range = closeParen - openParen;

        if (openParen == NULL || 
            closeParen == NULL ||
            currOperation[1] != '(' ||
            range <= 1 ||
            closeParen[1] == '\0')
        {
            return MALFORMED_DATA_ERROR;
        }

        // Store the command letter, operation, and cycle time
        char commandLetter = currOperation[0];
        char* operation = malloc(sizeof(char)* range);
        strncpy(operation, openParen + 1, range);
        operation[range-1] = '\0';
        int cycleTime = atoi(closeParen + 1);

        int commandError = CheckCommandLetter(commandLetter);
        int operationError = CheckOperation(operation);

        if (commandError == INVALID_ENTRY_ERROR ||
            operationError == INVALID_ENTRY_ERROR)
        {
            return INVALID_ENTRY_ERROR;
        }

        // Create a head if it doesn't already exist
        if (strcmp(head->operation, "") == 0)
        {
            head->commandLetter = commandLetter;
            strcpy(head->operation, operation);
            head->cycleTime = cycleTime;
            head->next = NULL;
        }
        else
        {
            MetaDataNode* node = malloc(sizeof(MetaDataNode));
            node->commandLetter = commandLetter;
            strcpy(node->operation, operation);
            node->cycleTime = cycleTime;
            node->next = NULL;

            AppendNewNode(head, node);
        }

        currOperation = strtok(NULL, ";.");
    }

    return 0;
}

int RemoveSpaces(char* string)
{
    char* newString = string;
    char* workString = string;

    // Skip white space and new line characters when making new string
    while(*workString != 0)
    {
        *newString = *workString++;
        if(*newString != ' ' && *newString != '\n') {
            newString++;
        }
    }

    *newString = 0;
    return 0;
}
