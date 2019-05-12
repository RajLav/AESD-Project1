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
#include <math.h>
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
#include <stdbool.h>


/************** Global Variables *********************/
extern pthread_mutex_t lock, lock_var;
extern uint8_t IF_RETRY;
extern uint8_t IF_RETRY_LUX;
extern uint8_t TEMP_SENSOR_ST;
extern uint8_t LUX_SENSOR_ST;
extern uint8_t TEMP_WARN;
extern uint8_t LUX_WARN;
extern uint8_t SOCK_ST;
extern uint8_t Counter;
extern sig_atomic_t flag;
extern uint8_t LogKillSafe;
extern uint8_t RunningThreads;

/************** Global Defines *********************/
#define SOCK_ON 			(1)
#define SOCK_OFF 			(0)


#define Temp_Sig 			(0XF0)
#define Lux_Sig 			(0xF1)

#define Celsius				(1)
#define Fahrenheit			(2)
#define Kelvin				(3)



#define TimerInterval		(1000)
#define RetryPeriod			(2000)
#define CountThresh		(RetryPeriod / TimerInterval)

#define Alive_Testing_Interval		(2)

#define I2C_BUS			(char *)"/dev/i2c-2"
#define DEBUG_PRINTF 		(1)

#define MSG_STRING  				"Unknown Thread!"


#define LOG_LINUX					(0X01)
#define LINUX_LOG					(0x02)
#define TERMINAL_LOG    			(0x03)


#define LOGGING_ALIVE 				(0b00000001)
#define SOCKET_ALIVE 				(0b00000010)
#define TEMP_ALIVE 					(0b00000100)
#define LUX_ALIVE 					(0b00001000)


#define Temperature_Signal			(0xF0)
#define Lux_Signal					(0xF1)

#define Celsius						(1)
#define Fahrenheit					(2)
#define Kelvin						(3)

#define PORT 						(8009)

#define time_high 					(0x02)
#define time_med 					(0x01)
#define time_low 					(0x00)

#define gain 						(0x10)


/********************** Enum defines ****************************/
typedef enum
{
	Main = 1,Logging = 2,Socket = 3,Temp_Log = 4,Lux =5
} Sources;

typedef struct Pthread_ArgsStruct
{
    char LogFile_Path[100];
}Pthread_ArgsStruct;

typedef struct ThreadStruct
{
	uint8_t Source;
	uint8_t Dest;
	char LogLevel[150];
	char Msg[150];
} ThreadStruct;

typedef enum{
	MAIN_QUEUE_E=0,
	LOGGING_QUEUE_E=1,
	SOCKET_QUEUE_E=2,
	TEMP_QUEUE_E=3,
	LUX_QUEUE_E=4
}QUEUE_ENUM;

#define Reading_Flag		0xFF


typedef enum{gpio53=0,gpio54=1,gpio55=2,gpio56=3}PINS;

/********************** Array defines *****************************/
char* LED_Dir_Path[]={"/sys/class/gpio/gpio53/direction",
					  "/sys/class/gpio/gpio54/direction",
					  "/sys/class/gpio/gpio55/direction",
					  "/sys/class/gpio/gpio56/direction"};

char* LED_Val_Path[]={"/sys/class/gpio/gpio53/value",
					  "/sys/class/gpio/gpio54/value",
					  "/sys/class/gpio/gpio55/value",
					  "/sys/class/gpio/gpio56/value"};

char* QueueValues[]={"/my_queue_main_q","/my_queue_log_q","/my_queue_server_q","/my_queue_temp_q","/my_queue_lux_q"};
char* SourceStrings[]={"Main Thread","Logging Thread","Socket Thread","Temp Thread","Lux Thread"};

///********************** function prototypes *****************************/
void SendToThreadQ(uint8_t Src, uint8_t Dst, char* Log, char* Message);
void Log_error(uint8_t Src, char* Err_Msg, int errnum, uint8_t SendToLogging);
void gpio_on_off(uint8_t pin,uint8_t state);

extern uint8_t LogKillSafe,RunningThreads,Lux_Error_Retry,LUX_SENSOR_ST;
extern uint8_t IF_RETRY,Present_Temp_Sensor_Output;
extern uint8_t lux_t_retry, lux_state_sensor;
extern uint8_t temp_t_retry, temp_state_sensor;

double GetTime();

#endif
