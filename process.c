#include "process.h"
#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sim04.h"
/**
  * @file process.c
  *
  * @brief Driver program process structures to run processes
  *
  * @details Allows to intiialize the process and then run the process
  *          
  *
  * @version 1.10
  *          C.S Student (22 February 2017)
  *
  * @note Requires process.h
  * @note Requires structures.h
  */
// Delcaring the Thread process because no other file needs it
void ThreadProcess(void*);

void InitProcess( PCB* phead, MetaDataNode* mhead, ConfigFile* config )
{
    MetaDataNode* current;
    int totalTime;
    PCB* newProcess = malloc(sizeof(PCB));
    MetaDataNode* prevCurrent;
    current = mhead;
    int processIncrement = 0;
    PCB* lastProcess;
    // loop to check if we have data in the file
    while(current != NULL)
    {
      // checks the start of the metadata file
        if( current->commandLetter == 'A' && strcmp(current->operation,"start") == 0 )
        {
          // makes a new process if we dont have one
            if(phead->processID == -1)
            {
                phead->states = New;
                phead->current = current->next;
                phead->processID = processIncrement++;
                phead->next = NULL;
                lastProcess = phead;
            }
            // if we do have one add a new one to the existing one
            else
            {
                newProcess->next = NULL;
                newProcess->states = New;
                newProcess->current = current->next;
                newProcess->processID = processIncrement++;
                lastProcess->next = newProcess;
                lastProcess = newProcess;
                newProcess = malloc(sizeof(PCB));
            }
        }
        if( current->commandLetter == 'I' || current->commandLetter == 'O')
        {
          totalTime += current->cycleTime * config->ioCycleTime;
        }
        else if( current->commandLetter == 'P')
        {
          totalTime += current->cycleTime * config->pCycleTime;
        }

        prevCurrent = current;
        current = current->next;
        // gets the end of the metadata file
        if( prevCurrent->commandLetter == 'A' && strcmp(prevCurrent->operation,"end") == 0 )
        {
          lastProcess->totalTime = totalTime;
          totalTime = 0;
          prevCurrent->next = NULL;
        } 
    }
}

int RunProcess( PCB* process, ConfigFile* config, char* timestr, 
  QueueNode** InterruptHead,  char* logBuffer, int* quantumCycle)
{
    MetaDataNode* current = process->current;
    char* outputString = NodeToString(current);
    char toPrint[MAX_LINE_LENGTH]; 
    pthread_t ioThreads;
    int cyclesToRun;
    Thread* threadData = malloc(sizeof(Thread));

    if(current->cycleTime != 0)
    {
        accessTimer(GET_TIME_DIFF, timestr);
        snprintf(toPrint, MAX_LINE_LENGTH, "Time: %9s, Process %d, %s start\n", 
            timestr, process->processID, outputString);
        PrintWrapper(toPrint, logBuffer, config->logTo);
        if(current->commandLetter == 'M')
        {
          // Memory checking
        }
        else if(current->commandLetter == 'I' || current->commandLetter == 'O')
        {
          int delayValue = (current->cycleTime * config->ioCycleTime);
          threadData->delay = delayValue;
          threadData->InterruptHead = InterruptHead;
          threadData->ProcessThread = process;

          pthread_create(&ioThreads, NULL, (void*) ThreadProcess, (void*) threadData);
          pthread_join(ioThreads, NULL);
        }
        else
        {

          if(current->cycleTime > config->quantumTime)
          {
            cyclesToRun = *quantumCycle;
          }
          else
          {
            cyclesToRun = current->cycleTime;
          }
          while(cyclesToRun != 0)
          {
            delay(config->pCycleTime);
            quantumCycle --;
            cyclesToRun --;
            current->cycleTime --;
            process->totalTime -= config->pCycleTime;

          }
        }
        snprintf(toPrint, MAX_LINE_LENGTH, "Time: %9s, Process %d, %s end\n", 
            timestr, process->processID, outputString);
        PrintWrapper(toPrint, logBuffer, config->logTo);
        
    }
    process->current = current->next;

    if(process->current == NULL)
    {
        return 1;
    }

    return 0;
}
// Adds to a Priority Queue
int Enqueue(PCB* PCBNode, QueueNode** Queuehead, int priority)
{
  // Declarted variables to use throughout the function
  QueueNode* new_node;
  new_node = malloc(sizeof(QueueNode));
  new_node->PCBNode = PCBNode;
  new_node->priority = priority;
  new_node->next = NULL;
  QueueNode* qhead = *Queuehead;

  if(qhead->priority == -1)
  {
    qhead->PCBNode = PCBNode;
    qhead->priority = priority;
    qhead->next = NULL;
  }
  else if(priority < qhead->priority)
  {
    new_node->next = qhead;
    *Queuehead = new_node;
  }
  else
  {
    while(qhead->next != NULL && priority > qhead->next->priority)
    {
      qhead = qhead->next;
    }
    new_node->next = qhead->next;
    qhead->next = new_node;
  }

  return 0;
}
// takes processes of the queue
int Dequeue(QueueNode** Queuehead, PCB** output)
{
  // delcarated vzriables used throughout the function
  QueueNode* head = *Queuehead;

  if(head->priority == -1)
  {
    output = NULL;
    return 0;
  }

  if(head->next == NULL)
  {
    (*Queuehead)->priority = -1;
    (*Queuehead)->next = NULL;
  }
  else
  {
    *Queuehead = head->next;
  }
  *output = head->PCBNode;
  return 0;
}
// Handles the Threads in the process
void ThreadProcess(void* data)
{
  Thread *threadData = (Thread*) data;
  delay(threadData->delay);
}