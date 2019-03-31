

#include "../Includes/LoggingThread.h"
#include "../Includes/main.h"
#include "../Includes/UnitTestLog.h"
//#include "POSIX_Qs.h"


double GetTime()
{
	/* Declaring structure for time */
	struct timeval time;

	/* Get current time and save it */
	gettimeofday(&time, 0);

	/* Combine the Secs with uSecs by typecasting
     * Long Int to Double and return as 1 Double value */
	return (double)(time.tv_sec)+(((double)(time.tv_usec))/1000000);
}

uint8_t log_init(char* user_path)
{
	FILE *file;
	file = fopen(user_path, "w");
	ErrorState error=Success;

	if(file == NULL)
	{
		printf("Error opening file! \n");
		error=FilePtrErr;
		//exit(1);
		return error;
	}
	
	fprintf(file,"[%lf] [INFO_LOG]: Successfully created logfile! TID: %ld\n\n", GetTime(), syscall(SYS_gettid));
	printf("[%lf] [INFO_LOG]: Successfully created logfile! TID: %ld\n\n", GetTime(), syscall(SYS_gettid));
	
	fflush(file);
	fclose(file);

	return error;

}


uint8_t log_file(char* user_path, ThreadStruct* StringToSend)
{
	FILE *file;
	file = fopen(user_path, "a");
	ErrorState error=Success;

	if(file == NULL)
	{
		printf("File could not be opend\n");
		error=FilePtrErr;
		return error;
	}

	char* SourceString;

	if((StringToSend->Source)<6)
		SourceString = ThreadString[StringToSend->Source];
	else
	{
		SourceString = ThreadString[6];
		error=ValCmpErr;
	}


	char* line;
	if((StringToSend->Dest)<6)
	{
		line = "[%lf] %s(%s): %s\n		L-> Source: '%s'\n\n";
		fprintf(file, line,GetTime(),ThreadString[StringToSend->Dest], StringToSend->LogLevel, StringToSend->Msg, SourceString);
		printf(line, GetTime(),ThreadString[StringToSend->Dest], StringToSend->LogLevel, StringToSend->Msg, SourceString);
	}
	else
	{
		line = "[%lf] %s(%s): %s\n		L-> Source: '%s'\n\n";
		fprintf(file, line,GetTime(),ThreadString[6],StringToSend->LogLevel, StringToSend->Msg, SourceString);
		printf(line, GetTime(),ThreadString[6], StringToSend->LogLevel, StringToSend->Msg, SourceString);
		error=ValCmpErr;
	}

	fflush(file);
	fclose(file);

	return error;
}
