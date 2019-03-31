
/*
*@File:LoggingThread.h
*@Description_File: Header File for main functionLog Thread
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/23/2018
*/

#ifndef LOGGINGTHREAD_H_
#define LOGGINGTHREAD_H_

#include "main.h"
#include "LoggingThread.h"
#include "main.h"
#include "POSIX_Qs.h"

pthread_mutex_t lock;

sig_atomic_t flag;
uint8_t LogKillSafe;
uint8_t AliveThreads;

void * LoggingThread(void * args);

char* ThreadString[]={" ","Main","Logging","Socket","Temperature","Lux","Unknown"};

/* This function Opens the file with specified name or uses default value if not defined.
   Successful opening result in Success log. Else Error message is logged*/
void log_init(char* user_path);

/* Logs the particular thread in the source description with time stamp and
   even the destination description with time stamp.*/
void log_file(char* user_path, ThreadStruct* StringToSend);

#endif
