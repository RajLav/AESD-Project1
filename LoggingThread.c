/*
*@File:LoggingThread.c
*@Description_File: C File for main functionLog Thread
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/23/2018
*/

#include "main.h"
#include "LoggingThread.h"

uint8_t LogKillSafe,RunningThreads,Lux_Error_Retry,LUX_SENSOR_ST;
uint8_t IF_RETRY,Present_Temp_Sensor_Output;
uint8_t lux_t_retry, lux_state_sensor;

uint8_t log_init(char* user_path)
{
	FILE *file;
	file = fopen(user_path, "w");

	if(file == NULL)
	{
		printf("[ERROR]Error opening file!\n");
		exit(1);
	}

	fprintf(file,"[%lf] [INFO_LOG]: Successfully created logfile! TID: %ld PID: %ld\n", GetTime(), syscall(SYS_gettid),syscall(SYS_getpid));
	printf("[%lf] [INFO_LOG]: Successfully created logfile! TID: %ld PID: %ld\n", GetTime(), syscall(SYS_gettid),syscall(SYS_getpid));
	printf("[INFO]The Created Threads have the following index values:\n");
	printf("[INFO]Main = 1,Logging = 2,Socket = 3,Temp = 4,Lux =5\n");
	fprintf(file,"[INFO]The Created Threads have the following index values:\n");
	fprintf(file,"[INFO]Main = 1,Logging = 2,Socket = 3,Temp = 4,Lux =5\n");
  fclose(file);
	return 0;
}


uint8_t log_file(char* user_path, ThreadStruct* StringToSend)
{
	FILE *file;
	file = fopen(user_path, "a");
	if(file == NULL)
	{
		printf("[ERROR]File could not be opend\n");
	}

	char* SourceString;

	if((StringToSend->Source)<6)
		SourceString = ThreadString[StringToSend->Source];
	else
		SourceString = ThreadString[6];


	char* line;
	if((StringToSend->Dest)<6)
	{
		line = "[%lf][INFO] %s(%s): %s::		 Source: '%d'\n";
		fprintf(file, line,GetTime(),ThreadString[StringToSend->Dest], StringToSend->LogLevel, StringToSend->Msg, StringToSend->Source);
		printf(line, GetTime(),ThreadString[StringToSend->Dest], StringToSend->LogLevel, StringToSend->Msg, StringToSend->Source);
	}
	else
	{
		line = "[%lf][ERROR] %s(%s): %s::		 Source: '%d'\n";
		fprintf(file, line,GetTime(),ThreadString[6],StringToSend->LogLevel, StringToSend->Msg,  StringToSend->Source);
		printf(line, GetTime(),ThreadString[6], StringToSend->LogLevel, StringToSend->Msg,  StringToSend->Source);

	}

	fclose(file);
	return 0;
}
