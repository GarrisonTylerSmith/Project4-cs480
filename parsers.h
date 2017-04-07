/**
  * @file parsers.h
  *
  * @brief Header file for parsing helper funcitons
  *
  * @details Return error codes: FILE_OPEN_ERROR, MALFORMED_DATA_ERROR
  *
  * @version 1.10
  *          C.S Student (22 February 2017)
  *
  * @note Requires structures.h
  */

#ifndef PARSER_H
#define PARSER_H

#include "structures.h"

#define FILE_OPEN_ERROR -1
#define MALFORMED_DATA_ERROR -2

/**
  * @brief Function to parse the configuration file
  *
  * @details Parses and saves the configuration file into the 
  *          ConfigFile passed as a parameter
  *
  * @param[in] configFile
  *            the configuration file structure instance to save into
  *
  * @param[in] pathToFile
  *            the file path of the configuration file to parse
  *
  * @return FILE_OPEN_ERROR, INVALID_ENTRY_ERROR, MALFORMED_DATA_ERROR
  *         on failure, 0 on success
  *
  * @note None
  */
int ParseConfig(ConfigFile*, char*);

/**
  * @brief Function to parse the meta data file
  *
  * @details Parses and saves the meta data file info into
  *          the meta data linked list that is given
  *
  * @param[in] head
  *            the head node of the meta data linked list to save to
  *
  * @param[in] pathToFile
  *            the file path of the meta data file to parse
  *
  * @return FILE_OPEN_ERROR, INVALID_ENTRY_ERROR, MALFORMED_DATA_ERROR
  *         on failure, 0 on success
  *
  * @note None
  */
int ParseMetaData(MetaDataNode*, char*);

#endif  // PARSER_H
