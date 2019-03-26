#include "../Includes/UnitTestLog.h"

int logger_task(FILE* ptr,char* buffer,int value)
{
	if(ptr==NULL)
		return FilePtrErr;
	switch(STRUCT.source_id)
	{
		case MAIN_TASK:
			if(strcmp(buffer,STRUCT.random_string))
				return StrCmpErr;
			return Success;
			break;

		case LIGHT_TASK:
			switch(STRUCT.data)
			{
				case VALUE:
					if(STRUCT.value == value)
						return Success;
					return ValCmpErr;
					break;

				case STRING:
					if(strcmp(buffer,STRUCT.random_string))
						return StrCmpErr;
					return Success;
					break;

				default: return DataValErr;
			}

		case TEMP_TASK:
			switch(STRUCT.data)
			{
				case VALUE:
					if(STRUCT.value == value)
						return Success;
					return ValCmpErr;
					break;

				case STRING:
					if(strcmp(buffer,STRUCT.random_string))
						return StrCmpErr;
					return Success;
					break;

				default: return DataValErr;
			}

		case SOCKET_TASK:
			if(strcmp(buffer,STRUCT.random_string))
				return StrCmpErr;
			return Success;
			break;
			
			default: return SourceIdErr;
	}
}

void InitializeStruct(int source_id, int data, char* random_string, int value)
{
	STRUCT.source_id=source_id;
	STRUCT.data=data;
	strcpy(STRUCT.random_string,random_string);
	STRUCT.value=value;
}
