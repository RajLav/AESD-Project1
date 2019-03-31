#include "../Includes/UnitTestLog.h"
#include "../Includes/LoggingThread.h"
#include "../Includes/main.h"


void InitializeStruct(int Source, int Dest, char* LogLevel, char* Msg)
{
	STRUCT.Source=Source;
	STRUCT.Dest=Dest;
	strcpy(STRUCT.LogLevel,LogLevel);
	strcpy(STRUCT.Msg,Msg);
}
