// Program Information ////////////////////////////////////////////////////////
/**
 * @file SimpleTimer.h
 *
 * @brief Header file for micro-second precision timer
 * 
 * @author Michael Leverington
 * 
 * @details Specifies all member methods of the SimpleTimer
 *
 * @version 2.00 (13 January 2017)
 *          1.00 (11 September 2015)
 *
 * @Note None
 */

// Precompiler directives /////////////////////////////////////////////////////

#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

// Header files ///////////////////////////////////////////////////////////////

#include <sys/time.h>
#include <string.h>
#include <math.h>

// Global Constants  //////////////////////////////////////////////////////////

#define RADIX_POINT '.'
#define NULL_CHAR '\0'

enum TIMER_CTRL_CODES { START_TIMER, STOP_TIMER, RESET_TIMER, GET_TIME_DIFF };
typedef enum Boolean { False, True } Boolean;

// Function Prototyp  /////////////////////////////////////////////////////////

double accessTimer( int controlCode, char *timeStr );
void timeToString( int secTime, int uSecTime, char *timeStr );
// Added a delay function that makes it so the process executes in real time
void delay( int );

#endif // ifndef SIMPLETIMER_H
