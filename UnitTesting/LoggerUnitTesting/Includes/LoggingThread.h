

// #ifndef LOGGINGTHREAD_H_
// #define LOGGINGTHREAD_H_
#ifndef LOGGINGTHREAD_H_
#define LOGGINGTHREAD_H_

#include "main.h"

/* This function Opens the file with specified name or uses default value if not defined.
   Successful opening result in Success log. Else Error message is logged*/
uint8_t log_init(char* user_path);

/* Logs the particular thread in the source description with time stamp and 
   even the destination description with time stamp.*/
uint8_t log_file(char* user_path, ThreadStruct* StringToSend);

#endif /* LOGGINGTHREAD_H_ */
