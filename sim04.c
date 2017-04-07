/**
  * @file sim02.c
  *
  * @brief Driver program for an operating system simulator
  *
  * @details Uses methods from parsers and structures to parse and store
  *          meta-data information
  *
  * @version 1.10
  *          C.S Student (22 February 2017)
  *
  * @note Requires structures.h parsers.h
  */

#include <stdlib.h>
#include <stdio.h>
#include "SimpleTimer.h"
#include "structures.h"
#include "parsers.h"
#include "process.h"
#include "sim04.h"


int main(int argc, char* argv[])
{
  // Variables declared for use throughout the function
    char toPrint[MAX_LINE_LENGTH];
    char* logBuffer = malloc(LOG_BUFFER_SIZE);
    FILE *logFP;

    QueueNode* qhead = malloc(sizeof(QueueNode));
    qhead->priority = -1;
    QueueNode* InterruptHead = malloc(sizeof(QueueNode));
    InterruptHead->priority = -1;

    PCB *phead = malloc(sizeof(PCB));
    phead->processID = -1;
    PCB* runningPCB = malloc(sizeof(PCB));
    PCB* processInterate = phead;
    int quantumCycle = 0;

    char timestr[50];

    logBuffer[0] = '\0';
    setbuf(stdout,NULL);


    // Check usage
    if (argc < 2)
    {
        printf("Usage: sim04 (config file path)\n");
        return EXIT_FAILURE;
    }

    PrintWrapper("\n Operating System Simulator \n", logBuffer, NULL);
    PrintWrapper("=========================\n", logBuffer, NULL);

    // Create new config file and parse
    ConfigFile config = {};
    PrintWrapper("Loading configuration file\n", logBuffer, NULL);
    int configError = ParseConfig(&config, argv[1]);

    if (configError == FILE_OPEN_ERROR)
    {
        printf("Error: Unable to open configuration file, check file path.\n");
        return EXIT_FAILURE;
    }
    if (configError == MALFORMED_DATA_ERROR)
    {
        printf("Error: Incorrectly formed configuration file.\n");
        return EXIT_FAILURE;
    }
    if (configError == INVALID_ENTRY_ERROR)
    {
        printf("Error: Incorrectly formed meta-data file. (Invalid entry)\n");
        return EXIT_FAILURE;
    }

    PrintWrapper("Loading Meta-data file\n", logBuffer, NULL);
    
    // Create head meta data node and pass to parse
    MetaDataNode* metaDataHead = malloc(sizeof(MetaDataNode));
    int metaError = ParseMetaData(metaDataHead, config.filePath);

    if (metaError == FILE_OPEN_ERROR) {
        printf("Error: Unable to open meta-data file, check file path.\n");
        return EXIT_FAILURE;
    }
    if (metaError == MALFORMED_DATA_ERROR)
    {
        printf("Error: Incorrectly formed meta-data file.\n");
        return EXIT_FAILURE;
    }
    if (metaError == INVALID_ENTRY_ERROR)
    {
        printf("Error: Incorrectly formed meta-data file. (Invalid entry)\n");
        return EXIT_FAILURE;
    }


    PrintWrapper("=======================\n", logBuffer, NULL);
    printf("\n");

    // Initilize the Process
    InitProcess(phead, metaDataHead, &config);

    PrintWrapper("\nBegin Simulation\n", logBuffer, config.logTo);
    // Started the time here and went through the process and data info
    accessTimer(START_TIMER, timestr);
    accessTimer(GET_TIME_DIFF, timestr);

    // Hard coded the OS systems I couldnt fix it myself
    snprintf(toPrint, MAX_LINE_LENGTH, "Time: %9s, System Start\n", timestr);
    PrintWrapper(toPrint, logBuffer, config.logTo);

    snprintf(toPrint, MAX_LINE_LENGTH, "Time: %9s, OS: Begin PCB Creation\n", timestr);
    PrintWrapper(toPrint, logBuffer, config.logTo);

    snprintf(toPrint, MAX_LINE_LENGTH, "Time: %9s, OS: All processes initialized in New State\n", timestr);
    PrintWrapper(toPrint, logBuffer, config.logTo);

    snprintf(toPrint, MAX_LINE_LENGTH, "Time: %9s, OS: All processes now set in Ready State\n\n", timestr);
    PrintWrapper(toPrint, logBuffer, config.logTo);


    // Checks what Shceduling code to use and then runs in that order
    while(processInterate->next != NULL)
    {
      accessTimer(GET_TIME_DIFF, timestr);


      if(strcmp(config.schedulingCode, "FCFS-P") == 0 || strcmp(config.schedulingCode, "FCF-N") == 0)
        {
          Enqueue(processInterate, &qhead, processInterate->processID);
        }
      else if(strcmp(config.schedulingCode , "SJF-N") == 0 || strcmp(config.schedulingCode , "SRTF-P")==0)
      {
        Enqueue(processInterate, &qhead, processInterate->totalTime);
      }

      // check round robin right here
      else
      {
        Enqueue(processInterate, &qhead, 5);
      }

      processInterate = processInterate->next;

    }


    // for loop to run through the process of the metadata
    Boolean WorkMeta = True;
    int ReturnValue;
    // runs through the multiple processes
    while(qhead->priority != -1)
    {
      Dequeue(&qhead, &runningPCB);

      snprintf(toPrint, MAX_LINE_LENGTH, "Time: %9s, OS: %s, Strategy selects Process %d with time: %d\n",
        timestr, config.schedulingCode, runningPCB->processID, runningPCB->totalTime);
      PrintWrapper(toPrint, logBuffer, config.logTo);

      snprintf(toPrint, MAX_LINE_LENGTH, "Time: %9s, OS: Process %d set in Running state\n", 
        timestr, runningPCB->processID);
      PrintWrapper(toPrint, logBuffer, config.logTo);

      // gets the quantum time when running RR-P
      quantumCycle = config.quantumTime;

      WorkMeta = True;
      while( WorkMeta )
      {

        ReturnValue = RunProcess(runningPCB, &config, timestr,&InterruptHead, logBuffer, &quantumCycle);

        if(ReturnValue == 1)
        {
          WorkMeta = False;
        }

      }
      snprintf(toPrint, MAX_LINE_LENGTH, "Time: %9s, OS: Process %d set to Exit state\n\n", 
        timestr, runningPCB->processID);
      PrintWrapper(toPrint, logBuffer, config.logTo);
    }


    snprintf(toPrint, MAX_LINE_LENGTH, "Time: %9s, System Stop\n", 
      timestr);
    PrintWrapper(toPrint, logBuffer, config.logTo);
    PrintWrapper("\n", logBuffer, config.logTo);
    PrintWrapper("End Simulation\n", logBuffer, config.logTo);

    //Write to log file conditionally
    if(strcmp(config.logTo, "File")==0 || strcmp(config.logTo, "Both")==0)
    {
      logFP = fopen(config.logFilePath, "w+");
      fprintf(logFP, "%s", logBuffer);
      fclose(logFP);
    }
    PrintWrapper("===========================\n", logBuffer, config.logTo);

    
    free(metaDataHead);
    return EXIT_SUCCESS;
}
// Print wrapper helps with the log issues that need to be maintained
int PrintWrapper(char* toPrint, char* buffer, char* logTo)
{
  strcat(buffer, toPrint);
  if(logTo == NULL || strcmp(logTo, "Monitor") == 0 || strcmp(logTo,"Both") == 0)
  {
    printf("%s", toPrint);
  }
  return 0;
}

