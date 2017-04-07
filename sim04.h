
#ifndef SIM04_H
#define SIM04_H

#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "parsers.h"
#include "process.h"
#include "SimpleTimer.h"
/**
  * @brief A funtion that handles the log situations 
  *
  * @details Puts all the prints in a log and sends it to monitor and/or file
  *
  * @param[in] Char
  *            Gets the Print statement
  *
  * @param[in] Char
  *            Gets the Log buffer for the log 
  *				
  * @param[in] Char
  *				Gets the Log to parameter to print to either monitor or file or both
  *
  * @note None
  */
int PrintWrapper(char*, char*, char*);

#endif