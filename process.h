/**
  * @file process.h
  *
  * @brief Header file for parsing helper funcitons
  *
  *
  * @version 1.10
  *          C.S Student (22 February 2017)
  *
  * @note Requires structures.h
  * @note Requires SimpleTimer.h
  */
#include "structures.h"
#include "SimpleTimer.h"
#include <pthread.h>


#ifndef PROCESS_H
#define PROCESS_H

#define LOG_BUFFER_SIZE 16384
#define MAX_LINE_LENGTH 100

#define PROCESS_SET_BLOCKED -1


typedef struct PCB
{
	enum {New, Ready, Running, Exit}states;
	MetaDataNode* current;
	int processID;
  int totalTime;
  struct PCB *next;

} PCB;
typedef struct QueueNode
{
  struct QueueNode *next;
  PCB* PCBNode;
  int priority;
} QueueNode;

typedef struct Thread
{
  int delay;
  PCB* ProcessThread;
  QueueNode** InterruptHead;

} Thread;

/**
  * @brief Function to initialize the PCB variables
  *
  * @details Initailizes which state that the PCB is in 
  *
  * @param[in] PCB
  *            Gets the process that we are going to start with
  *
  * @param[in] MetaDataNode
  *            Gets the beginning of the linked list within the data 
  *					structure
  *
  * @note None
  */
void InitProcess( PCB*, MetaDataNode*, ConfigFile*);

/**
  * @brief Function to runs the process within the program
  *
  * @details Runs the process frame by frame 
  *
  * @param[in] PCB
  *            Gets the process that we are going to start with
  *
  * @param[in] ConfigFile
  *            Gets the configfile that the data needs to execute 
  *				
  * @param[in] Char
  *				the timestr for keeping track of the time
  *
  * @param[in] QueueNode
  *       Gets the Queue and its head Node
  *
  * @param[in] Char
  *       gets the log buffer for the log issues
  *
  * @param[in] int
  *       gets the quantum time passes as an int 
  *         used for RR-P
  *
  *
  * @note None
  */
int RunProcess( PCB*, ConfigFile*, char*,QueueNode**,  char* , int*);
/**
  * @brief Function to runs the process within the program
  *
  * @details Runs the process frame by frame 
  *
  * @param[in] PCB
  *            Gets the process that we are going to start with
  *
  * @param[in] QueueNode
  *            Gets the head of the Queue
  *       
  * @param[in] Int
  *       Gets the priority of the Queue
  *
  * @note None
  */

int Enqueue( PCB*, QueueNode**, int);
/**
  * @brief Function to runs the process within the program
  *
  * @details Runs the process frame by frame 
  *
  * @param[in] PCB
  *            Gets the process that we are going to start with
  *
  * @param[in] QueueNode
  *           Gets the head of the Queue
  *       
  *
  * @note None
  */

int Dequeue( QueueNode**, PCB**);



#endif
