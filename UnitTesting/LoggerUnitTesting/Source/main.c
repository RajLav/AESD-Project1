#include "UnitTestLog.c"
 #include "../Includes/main.h"
 #include "../Includes/LoggingThread.h"
 #include "LoggingThread.c"

int pass,fail;
char* path = "LOG_FILE";

int main(void)
{
	FILE* fptr=fopen("unittest.txt","a");

  	printf("\t\t==================================\n");
  	printf("\t\t||   Logger Unit Testing        ||\n");
  	printf("\t\t==================================\n");

  	//printf(log_init(fptr);
  	printf("Error:%d::%s\n",log_init(path),ErrorStates[log_init(path)]);
	if(log_init(path))fail+=1;
	else pass+=1;

	// Proper => exepcted error 0
	 InitializeStruct(1,2,"check","check2");
	 printf("Error:%d::%s\n",log_file(path,&STRUCT),ErrorStates[log_file(path,&STRUCT)]);
	 if(log_file(path,&STRUCT))fail+=1;
	 else pass+=1;

	 /*** ERROR CASES ***/

	 printf("Error:%d::%s\n",log_init(NULL),ErrorStates[log_init(NULL)]);
	if(log_init(NULL))pass+=1;
	else fail+=1;

	 InitializeStruct(1,2,"check","check2");
	 printf("Error:%d::%s\n",log_file(NULL,&STRUCT),ErrorStates[log_file(NULL,&STRUCT)]);
	 if(log_file(NULL,&STRUCT))pass+=1;
	 else fail+=1;

	printf("-------------------------------------------------------------------------\n");
  	printf("Total Test Cases:  %d || Passed Test Cases:  %d || Failed Test Cases:  %d\n",(pass+fail),pass,fail);
  	printf("-------------------------------------------------------------------------\n");

	

	return 0;
}
