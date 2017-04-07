/**
  * @file structures.c
  *
  * @brief Driver program for structure helper methods
  *
  * @details Allows for printing of structures and checking 
  *          if an entry is valid
  *
  * @version 1.10
  *          C.S Student 22 February 2017)
  *
  * @note Requires structures.h
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"


int CheckSchedulingCode(char* type)
{
    if (strcmp(type, "NONE") == 0)
    {
        strncpy(type, "FCFS-N\0", 7);
        return 0;
    }
    else if (strcmp(type, "FCFS-N") != 0 &&
             strcmp(type, "SJF-N") != 0 &&
             strcmp(type, "SRTF-P") != 0 &&
             strcmp(type, "FCFS-P") != 0 &&
             strcmp(type, "RR-P") != 0)
    {
        return INVALID_ENTRY_ERROR;
    }
    return 0;
}

int CheckLogTo(char* type)
{
    if (strcmp(type, "Monitor") != 0 &&
        strcmp(type, "File") != 0 &&
        strcmp(type, "Both") != 0)
    {
        return INVALID_ENTRY_ERROR;
    }
    return 0;
}

char* NodeToString(MetaDataNode* node)
{
  char* OutputString = malloc(50);

  switch (node->commandLetter)
  {
    case 'S':
      OutputString = "";
      break;
    case 'A':
      OutputString = "";
      break;
    case 'M':
      snprintf(OutputString, 50, "Memory Management %s", node->operation);
      break;
    case 'I':
      snprintf(OutputString, 50, "Input %s", node->operation);
      break;
    case 'O':
      snprintf(OutputString, 50, "Output %s", node->operation);
      break;
    case 'P':
      OutputString = "Run Operation";
      break;
  }
  return OutputString;
}

void PrintConfig(ConfigFile* configFile)
{
    printf("Start Simulator Configuration File:\n");
    printf("Version/Phase: %d\n", configFile->version);
    printf("File Path: %s\n", configFile->filePath);
    printf("CPU Scheduling Code: %s\n", configFile->schedulingCode);
    printf("Quantum Time (cycles): %d\n", configFile->quantumTime);
    printf("Memory Available (KB): %d\n", configFile->memoryAvailable);
    printf("Processor Cycle Time (msec): %d\n", configFile->pCycleTime);
    printf("I/O Cycle Time (msec): %d\n", configFile->ioCycleTime);
    printf("Log To: %s\n", configFile->logTo);
    printf("Log File Path: %s\n\n", configFile->logFilePath);
}

void AppendNewNode(MetaDataNode* head, MetaDataNode* newNode)
{
    // Iterate to last node
    MetaDataNode* curr = head;
    while (curr->next != NULL)
    {
        curr = curr->next;
    }

    // Append new node
    curr->next = newNode;
}

int CheckCommandLetter(char type)
{
    if (type != 'S' &&
        type != 'A' &&
        type != 'P' &&
        type != 'M' &&
        type != 'I' &&
        type != 'O')
    {
        return INVALID_ENTRY_ERROR;
    }
    return 0;
}

int CheckOperation(char* type)
{
    if (strcmp(type, "harddrive") == 0)
    {
        strncpy(type, "hard drive\0", 11);
        return 0;
    }
    else if (strcmp(type, "access") != 0 &&
             strcmp(type, "allocate") != 0 &&
             strcmp(type, "end") != 0 &&
             strcmp(type, "keyboard") != 0 &&
             strcmp(type, "printer") != 0 &&
             strcmp(type, "monitor") != 0 &&
             strcmp(type, "run") != 0 &&
             strcmp(type, "start") != 0)
    {
        return INVALID_ENTRY_ERROR;
    }
    return 0;
}

void PrintMetaData(MetaDataNode* head)
{
    MetaDataNode* curr = head;
    printf("Start Meta-Data Code:");
    while (curr != NULL)
    {
        printf("\nCommand letter: %c\n", curr->commandLetter);
        printf("Operation: %s\n", curr->operation);
        printf("Cycle Time: %d\n", curr->cycleTime);
        
        curr = curr->next;
    }
}
