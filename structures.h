/**
  * @file structures.h
  *
  * @brief Header file for structures and structure helper methods
  *
  * @details Return error codes: INVALID_ENTRY_ERROR
  *          Structures: ConfigFile, MetaDataNode
  *
  * @version 1.10
  *          C.S Student 22 February 2017)
  *
  * @note None
  */

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define INVALID_ENTRY_ERROR -3
#define STRING_MAX_SIZE 50

typedef struct ConfigFile
{
    int version;
    char filePath[STRING_MAX_SIZE];
    char schedulingCode[STRING_MAX_SIZE];
    int quantumTime;
    int memoryAvailable;
    int pCycleTime;
    int ioCycleTime;
    char logTo[STRING_MAX_SIZE];
    char logFilePath[STRING_MAX_SIZE];
} ConfigFile;

typedef struct MetaDataNode
{
    char commandLetter;
    char operation[STRING_MAX_SIZE];
    int cycleTime;
    struct MetaDataNode *next;
} MetaDataNode;

/**
  * @brief Function checks whether the scheduling code is valid
  *
  * @details Ensures that the input scheduling code is correct and 
  *          changes NONE to FCFS-N
  *
  * @param[in] type
  *            the current scheduling code to be checked
  *
  * @return INVALID_ENTRY_ERROR on failure, 0 on success
  *
  * @note None
  */
int CheckSchedulingCode(char*);

/**
  * @brief Function checks whether the log to string is valid
  *
  * @details Ensures that the input log to string is correct
  *
  * @param[in] type
  *            the current log to string to be checked
  *
  * @return INVALID_ENTRY_ERROR on failure, 0 on success
  *
  * @note None
  */
int CheckLogTo(char*);

/**
  * @brief Function used to print the configuration structure
  *
  * @details Prints each element of the configuration file structure
  *          in the format of the original file
  *
  * @param[in] configFile
  *            the ConfigFile structure to be printed
  *
  * @return void
  *
  * @note None
  */
void PrintConfig(ConfigFile*);

/**
  * @brief Function used to add a new node to the meta data linked list
  *
  * @details Adds a new node to the tail end of the linked list
  *
  * @param[in] head
  *            the head of the meta data linked list
  *
  * @param[in] newNode
  *            the new node to be added to the linked list
  *
  * @return void
  *
  * @note None
  */
void AppendNewNode(MetaDataNode*, MetaDataNode*);

/**
  * @brief Function checks whether the command letter is valid
  *
  * @details Ensures that the input command letter is correct
  *
  * @param[in] type
  *            the current command letter to be checked
  *
  * @return INVALID_ENTRY_ERROR on failure, 0 on success
  *
  * @note None
  */
int CheckCommandLetter(char);

/**
  * @brief Function checks whether the operation string is valid
  *
  * @details Ensures that the input operation string is correct and 
  *          changes harddrive to hard drive
  *
  * @param[in] type
  *            the current operation string to be checked
  *
  * @return INVALID_ENTRY_ERROR on failure, 0 on success
  *
  * @note None
  */
int CheckOperation(char*);

/**
  * @brief Function used to print the meta data linked list
  *
  * @details Iterates through the linked list, printing each node along the way
  *
  * @param[in] head
  *            the head of the meta data linked list to be printed
  *
  * @return void
  *
  * @note None
  */
void PrintMetaData(MetaDataNode*);

char* NodeToString(MetaDataNode*);

#endif  // STRUCTURES_H
