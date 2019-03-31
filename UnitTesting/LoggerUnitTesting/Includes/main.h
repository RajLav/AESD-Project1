/*
*@File:main.h
*@Description_File: Header File for main function
*@Author:Raj Lavingia
*@Date: 03/23/2018
*/

/*Inclusion of Libraries for main.c*/
#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <math.h> //pow
#include <float.h>
#include <complex.h>
#include <time.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <malloc.h>


extern pthread_mutex_t lock, lock_var;
double GetCurrentTime();
extern int temp_file_des;
extern int lux_file_des;
extern uint8_t Temp_Error_Retry;
extern uint8_t Lux_Error_Retry;
extern uint8_t Temp_Sensor_State;
extern uint8_t Lux_Sensor_State;
extern uint8_t Temp_Warning;
extern uint8_t Lux_Warning;

extern uint8_t Socket_State;

#define Socket_Online		1
#define Socket_Offline	0

extern uint8_t Counter;

extern sig_atomic_t flag;
extern uint8_t LogKillSafe;
extern uint8_t AliveThreads;

#define Temperature_Signal	0xF0
#define Lux_Signal		0xF1

#define Celsius		1
#define Fahrenheit	2
#define Kelvin		3

#define Sensor_Online		1
#define Sensor_Offline	0

#define PORT 8005

#define Timer_Interval		250		// In ms
#define Sensor_Retry_Period		5000		// In ms
#define Counter_Threshold		(Sensor_Retry_Period / Timer_Interval)

#define Alive_Testing_Interval		2		// In sec

//Path of I2C Bus
#define I2C_BUS		(char *)"/dev/i2c-2"
#define DEBUG_PRINTF 1
// void signal_function(int value);
// void sig_setup(void);



#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

typedef enum
{
	Main = 1,
	Logging = 2,
	Socket = 3,
	Temp = 4,
	Lux = 5
} Sources;

typedef struct Pthread_ArgsStruct
{
    char LogFile_Path[100];			//Used to store the wanted logfile path
}Pthread_ArgsStruct;



/***************************************
 *  Log_error() Function Parameters:   *
 *        (found in POSIX_Qs.h)        *
 ***************************************/
#define Log_Native			0x01
#define LOGGING_ONLY				0x02
#define LOCAL_ONLY					0x03


/***************************************
 *  Alive Checking Defines:
 *  (Used in Main pThread)
 ***************************************/
#define LOGGING_ALIVE 				0b00000001
#define SOCKET_ALIVE 				0b00000010
#define TEMP_ALIVE 					0b00000100
#define LUX_ALIVE 					0b00001000


/***************************************
 *  Poorn Defines                      *
 ***************************************/
#define Temperature_Signal			0xF0
#define Lux_Signal					0xF1

#define Celsius						1
#define Fahrenheit					2
#define Kelvin						3

#define PORT 						8005

#define time_high 					0x02  //for 402ms
#define time_med 					0x01  //for 101ms
#define time_low 					0x00  //for 13ms

#define gain 						0x10  //for maximum gain

typedef struct 
{
	uint8_t Source;
	uint8_t Dest;
	char LogLevel[150];
	char Msg[150];
} ThreadStruct;

#define MSG_STRING  "Unknown Thread!"

#define MAIN_QUEUE					"/MAIN_POSIX_Q"
#define LOGGING_QUEUE				"/LOGGING_POSIX_Q"
#define SOCKET_QUEUE				"/SOCKET_POSIX_Q"
#define TEMP_QUEUE					"/TEMP_POSIX_Q"
#define LUX_QUEUE					"/LUX_POSIX_Q"

typedef enum{
	MAIN_QUEUE_E=0,
	LOGGING_QUEUE_E=1,
	SOCKET_QUEUE_E=2,
	TEMP_QUEUE_E=3,
	LUX_QUEUE_E=4
}QUEUE_ENUM;

char* QueueValues[]={"/MAIN_POSIX_Q","/LOGGING_POSIX_Q","/SOCKET_POSIX_Q","/TEMP_POSIX_Q","/LUX_POSIX_Q"};
char* SourceStrings[]={"Main Thread","Logging Thread","Socket Thread","Temp Thread","Lux Thread"};
//function prototypes
void SendToThreadQ(uint8_t Src, uint8_t Dst, char* Log, char* Message);
void Log_error(uint8_t Src, char* Err_Msg, int errnum, uint8_t SendToLogging);
char* ThreadString[]={" ","Main","Logging","Socket","Temperature","Lux","Unknown"};

#endif