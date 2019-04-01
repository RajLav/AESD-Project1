#include "../Includes/UnitTestLog.h"
#include "../Includes/LoggingThread.h"
#include "../Includes/main.h"


void InitializeStruct(int Source, int Dest, char* LogLevel, char* Msg)
{
	STRUCT.Src=Source;
	STRUCT.Dst=Dest;
	strcpy(STRUCT.log_level,LogLevel);
	strcpy(STRUCT.Message,Msg);
}
