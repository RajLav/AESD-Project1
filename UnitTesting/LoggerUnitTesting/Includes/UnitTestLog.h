#ifndef UNITTESTLOG_H_
#define UNITTESTLOG_H_

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <float.h>
#include <complex.h>
#include <stdint.h>
#include <time.h>
#include <mqueue.h>
#include <stdio.h>
#include "main.h"
#include "LoggingThread.h"

#define HB_PORT_ADR 5000
#define IP_ADR      "127.0.0.1"

#define	MAIN_TASK	(0X00)
#define	LIGHT_TASK	(0X01)
#define	TEMP_TASK	(0X02)
#define	SOCKET_TASK	(0X03)

#define	VALUE 		(0X00)
#define	STRING 		(0X01)

pthread_t logger_id, light_id, temp_id, socket_id;

char file_name[50];

FILE *fptr;

struct threadParam
{
char *filename;
};

ThreadStruct STRUCT; 

typedef enum {Success=0,FilePtrErr=1,StrCmpErr=2,ValCmpErr=3,DataValErr=4,SourceIdErr=5}ErrorState;
char* ErrorStates[]={"Success\n","File Pointer Error\n","String Compare Error\n","Value Compare Error\n","Data Value Error\n","Source ID Error\n"};
// int logger_task();

char* TaskArray[]={"MainTask\n","Light Task\n","Temp Task\n","Socket Task\n"};
void InitializeStruct(int Source, int Dest, char* LogLevel, char* Msg);

#endif