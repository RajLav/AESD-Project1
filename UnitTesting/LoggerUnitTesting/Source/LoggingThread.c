

#include "../Includes/LoggingThread.h"
#include "../Includes/main.h"
#include "../Includes/UnitTestLog.h"
//#include "POSIX_Qs.h"


double GetTime()
{
	struct timeval time;

	gettimeofday(&time, 0);

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

	if((StringToSend->Src)<6)
		SourceString = ThreadString[StringToSend->Src];
	else
	{
		SourceString = ThreadString[6];
		error=ValCmpErr;
	}


	char* line;
	if((StringToSend->Dst)<6)
	{
		line = "[%lf] %s(%s): %s\n		L-> Source: '%s'\n\n";
		fprintf(file, line,GetTime(),ThreadString[StringToSend->Dst], StringToSend->log_level, StringToSend->Message, SourceString);
		printf(line, GetTime(),ThreadString[StringToSend->Dst], StringToSend->log_level, StringToSend->Message, SourceString);
	}
	else
	{
		line = "[%lf] %s(%s): %s\n		L-> Source: '%s'\n\n";
		fprintf(file, line,GetTime(),ThreadString[6],StringToSend->log_level, StringToSend->Message, SourceString);
		printf(line, GetTime(),ThreadString[6], StringToSend->log_level, StringToSend->Message, SourceString);
		error=ValCmpErr;
	}

	fflush(file);
	fclose(file);

	return error;
}
