#include "UnitTestLog.c"

int pass,fail;

int main(void)
{
	FILE* fptr=fopen("unittest.txt","a");

  	printf("\t\t==================================\n");
  	printf("\t\t||   Logger Unit Testing        ||\n");
  	printf("\t\t==================================\n");

	// Proper => exepcted error 0
	InitializeStruct(MAIN_TASK,VALUE,TaskArray[MAIN_TASK],23);
	printf("Error:%d::%s\n",logger_task(fptr,TaskArray[MAIN_TASK],23),ErrorStates[logger_task(fptr,TaskArray[MAIN_TASK],23)]);
	if(logger_task(fptr,TaskArray[MAIN_TASK],23))pass+=1;
	else pass+=1;

	// NULL Pointer =>expected error 1
	InitializeStruct(MAIN_TASK,VALUE,TaskArray[MAIN_TASK],23);
	printf("Error:%d::%s\n",logger_task(NULL,TaskArray[MAIN_TASK],23),ErrorStates[logger_task(NULL,TaskArray[MAIN_TASK],23)]);
	if(logger_task(NULL,TaskArray[MAIN_TASK],23))pass+=1;
	else fail+=1;

	// String is invalid =>expected error 2
	InitializeStruct(MAIN_TASK,VALUE,TaskArray[MAIN_TASK],23);
	printf("Error:%d::%s\n",logger_task(fptr,"Invalid String",23),ErrorStates[logger_task(fptr,"Invalid String",23)]);
	if(logger_task(fptr,"Invalid String",23))pass+=1;
	else fail+=1;

	// Value is invalid =>expected error 3
	InitializeStruct(LIGHT_TASK,VALUE,TaskArray[MAIN_TASK],23);
	printf("Error:%d::%s\n",logger_task(fptr,TaskArray[MAIN_TASK],404),ErrorStates[logger_task(fptr,TaskArray[MAIN_TASK],404)]);
	if(logger_task(fptr,TaskArray[MAIN_TASK],404))pass+=1;
	else fail+=1;

	// Value is invalid =>expected error 4
	InitializeStruct(LIGHT_TASK,VALUE+2,TaskArray[MAIN_TASK],23);
	printf("Error:%d::%s\n",logger_task(fptr,TaskArray[MAIN_TASK],23),ErrorStates[logger_task(fptr,TaskArray[MAIN_TASK],23)]);
	if(logger_task(fptr,TaskArray[MAIN_TASK],23))pass+=1;
	else fail+=1;

	// Task is invalid =>expected error 5
	InitializeStruct(LIGHT_TASK+3,VALUE,TaskArray[MAIN_TASK],23);
	printf("Error:%d::%s\n",logger_task(fptr,TaskArray[MAIN_TASK],23),ErrorStates[logger_task(fptr,TaskArray[MAIN_TASK],23)]);
	if(logger_task(fptr,TaskArray[MAIN_TASK],23))pass+=1;
	else fail+=1;

	printf("-------------------------------------------------------------------------\n");
  	printf("Total Test Cases:  %d || Passed Test Cases:  %d || Failed Test Cases:  %d\n",(pass+fail),pass,fail);
  	printf("-------------------------------------------------------------------------\n");

	

	return 0;
}