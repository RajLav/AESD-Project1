/*
*@File:LoggingThread.c
*@Description_File: C File for main functionLog Thread
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/23/2018
*/

uint8_t log_init(char* user_path)
{
	FILE *file;
	file = fopen(user_path, "w");

	if(file == NULL)
	{
		printf("[ERROR]Error opening file! \n");
		exit(1);
	}

	fprintf(file,"[%lf] [INFO_LOG]: Successfully created logfile! TID: %ld PID: %ld\n\n", GetCurrentTime(), syscall(SYS_gettid),syscall(SYS_getpid));
	printf("[%lf] [INFO_LOG]: Successfully created logfile! TID: %ld PID: %ld\n\n", GetCurrentTime(), syscall(SYS_gettid),syscall(SYS_getpid));

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
		line = "[%lf][INFO] %s(%s): %s::		 Source: '%d'\n\n";
		fprintf(file, line,GetCurrentTime(),ThreadString[StringToSend->Dest], StringToSend->LogLevel, StringToSend->Msg, StringToSend->Source);
		printf(line, GetCurrentTime(),ThreadString[StringToSend->Dest], StringToSend->LogLevel, StringToSend->Msg, StringToSend->Source);
	}
	else
	{
		line = "[%lf][ERROR] %s(%s): %s::		 Source: '%d'\n\n";
		fprintf(file, line,GetCurrentTime(),ThreadString[6],StringToSend->LogLevel, StringToSend->Msg,  StringToSend->Source);
		printf(line, GetCurrentTime(),ThreadString[6], StringToSend->LogLevel, StringToSend->Msg,  StringToSend->Source);

	}



	fflush(file);
	fclose(file);
	return 0;
}
