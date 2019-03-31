/*
*@File:LoggingThread.c
*@Description_File: C File for main functionLog Thread
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/23/2018
*/

void log_init(char* user_path)
{
	FILE *file;
	file = fopen(user_path, "w");

	if(file == NULL)
	{
		printf("Error opening file! \n");
		exit(1);
	}

	fprintf(file,"[%lf] [INFO_LOG]: Successfully created logfile! TID: %ld\n\n", GetCurrentTime(), syscall(SYS_gettid));
	printf("[%lf] [INFO_LOG]: Successfully created logfile! TID: %ld\n\n", GetCurrentTime(), syscall(SYS_gettid));

	fflush(file);
	fclose(file);

}


void log_file(char* user_path, ThreadStruct* StringToSend)
{
	FILE *file;
	file = fopen(user_path, "a");
	if(file == NULL)
	{
		printf("File could not be opend\n");
	}

	char* SourceString;

	if((StringToSend->Source)<6)
		SourceString = ThreadString[StringToSend->Source];
	else
		SourceString = ThreadString[6];


	char* line;
	if((StringToSend->Dest)<6)
	{
		line = "[%lf] %s(%s): %s\n		L-> Source: '%s'\n\n";
		fprintf(file, line,GetCurrentTime(),ThreadString[StringToSend->Dest], StringToSend->LogLevel, StringToSend->Msg, SourceString);
		printf(line, GetCurrentTime(),ThreadString[StringToSend->Dest], StringToSend->LogLevel, StringToSend->Msg, SourceString);
	}
	else
	{
		line = "[%lf] %s(%s): %s\n		L-> Source: '%s'\n\n";
		fprintf(file, line,GetCurrentTime(),ThreadString[6],StringToSend->LogLevel, StringToSend->Msg, SourceString);
		printf(line, GetCurrentTime(),ThreadString[6], StringToSend->LogLevel, StringToSend->Msg, SourceString);

	}



	fflush(file);
	fclose(file);
}
