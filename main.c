/*Name : Raj Lavingia and Yash Gupte
Date : 03/31/2019
*/
#include "main.h"

/* Our includes */

#include "LoggingThread.h"
#include "SocketThread.h"
#include "TempThread.h"
#include "LuxThread.h"
#include "POSIX_Qs.c"
#include "LoggingThread.c"
#include "SocketThread.c"
#include "TempThread.c"
#include "LuxThread.c"
#include <sys/time.h>
#include <stdio.h>

/* Global Variables */
pthread_t Log_pThread, Socket_pThread, Temp_pThread, Lux_pThread;

uint8_t RunningThreads,Lux_Error_Retry;
uint8_t IF_RETRY,Present_Temp_Sensor_Output;
uint8_t lux_t_retry = 0;
uint8_t temp_t_retry = 0;
sig_atomic_t flag = 0;
uint8_t sync_lux_temp = 0;
uint8_t RunningThreads = 0x00;
uint8_t Counter = 0;
uint8_t IF_RETRY = 0;
uint8_t IF_RETRY_LUX = 0;
uint8_t SOCK_ST = SOCK_OFF;

//to get the time for every function
double GetTime()
{
	struct timeval time;
	gettimeofday(&time, 0);
	return (double)(time.tv_sec)+(((double)(time.tv_usec))/1000000);
}

uint8_t Server_Killed_Externally(void)
{
    typedef struct
    {
      char String_Written[150];
      int num;
    }server_structure_killed;

    server_structure_killed t_strct1;
    server_structure_killed *pt_strct1 = &t_strct1;
    strcpy(pt_strct1->String_Written, "external");
    pt_strct1->num = 1;

    int temp_sock, t_out;
    struct sockaddr_in t_client;
    temp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(temp_sock < 0)
    {
        return 1;
    }

    t_client.sin_family = AF_INET;

    if(inet_pton(AF_INET, "127.0.0.1", &t_client.sin_addr)<=0)
    {
			return 1;
    }

    t_client.sin_port = htons(PORT);

    if(connect(temp_sock, (struct sockaddr *)&t_client, sizeof(t_client)) < 0)
    {
      return 1;
    }

    t_out = write(temp_sock,pt_strct1,sizeof(server_structure_killed));
    if (t_out < 0)
    {
    return 1;
    }

    return 0;
}

void gpio_on_off(uint8_t pin,uint8_t state)
{
	FILE *fp1;

	if(pin<4 && state<2)
	{
		fp1 = fopen(LED_Dir_Path[pin],"w");
		if(fp1==NULL) printf("Error in led\n");
		else
		printf("Success file open\n");
	}
	else
	{
		printf("Error in pin or state\n");
		return ;
	}
	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%d",pin);
	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%s","out");
	fclose(fp1);
	fp1 = 	fopen(LED_Val_Path[pin],"w");
	if(fp1==NULL) printf("Error in led\n");
	else
	printf("Success file open\n");
	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%d",state);
	fclose(fp1);
}


void signal_handler(int value)
{
	if(value == SIGVTALRM)
	{
			flag = Reading_Flag;
			Counter += 1;
			if(Counter >= 2)
			{
					printf("===============================================IN COUNT\n");
					Counter = 0;
					if(temp_t_retry > 0)
					{
							temp_t_retry -= 1;
							if(close(File_Descriptor))		printf("Closing File Des for temp failed\n");
							if(temp_initial_sensor()) printf("**********************************************Retrying for Temp\n");
							else
							{
								temp_t_retry = 0;
								Sent_Queue(Main, Logging, "[INFO]", "Temperature sensor is ON\n");
							}
					}

					if(lux_t_retry > 0)
					{
							printf("===============================================IN RET LUX\n");
							lux_t_retry -= 1;
							if(close(File_Descripter_LUX))		printf("\nClosing File Des for lux failed");
							if(LuxThread_Init())	printf("**********************************************Retrying for Lux\n");
							else
							{
								lux_t_retry = 0;
								Sent_Queue(Main, Logging, "INFO", "Lux Sensor is ON Now\n");
							}
					}
			}

	}
	else
	{
			flag = value;
			SOCK_ST = SOCK_OFF;
			if(Server_Killed_Externally())        printf("****************************************************Socket killing failed\n");
			else	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Socket killing successful\n");
			Sent_Queue(Main, Logging, "INFO", "Killing Log\n");
	}
}

void timer_setup(void)
{

		struct sigaction sig_action_time;
		struct itimerval sig_action_time_timer;
		sig_action_time.sa_handler = &signal_handler;
		sig_action_time_timer.it_interval.tv_sec = 1;
		sig_action_time_timer.it_interval.tv_usec = 0;
		sig_action_time_timer.it_value.tv_sec = 2;
		sig_action_time_timer.it_value.tv_usec = 0;
		sigaction(SIGVTALRM, &sig_action_time, NULL);
		sigaction(SIGUSR1, &sig_action_time, NULL);
		sigaction(SIGUSR2, &sig_action_time, NULL);
		setitimer(ITIMER_VIRTUAL, &sig_action_time_timer,NULL);
}

int main(void)
{
	struct Pthread_ArgsStruct args;
	char LogFilePathUsr[100];
	printf("Initiated ===> PID: %d\n", getpid());
	sprintf(LogFilePathUsr, "./log.txt");
	strcpy(args.LogFile_Path, LogFilePathUsr);
	if(pthread_create(&Log_pThread, NULL, &Log_Queue_Thread, (void *)&args) != 0)
	{
		Error_Data(Main, "logging pthread error\n", errno, TERMINAL_LOG);
	}
	else
	{
		printf("[%lf] [MAIN]::Successfully created logging pthread!\n", GetTime());
	}
		sleep(2);
	if(pthread_create(&Socket_pThread, NULL, &SocketThread, NULL) != 0)
	{
		Error_Data(Main, "[MAIN]::Socket pthread error\n", errno, LOG_LINUX);
	}
	else
	{
		printf("[%lf] [MAIN]::Successfully created socket pthread!\n", GetTime());
	}
		sleep(2);

	if(pthread_create(&Temp_pThread, NULL, &Temperature_Thread, NULL) != 0)
	{
		Error_Data(Main, "Temperature pthread error\n", errno, LOG_LINUX);
	}
	else
	{
		printf("[%lf] [MAIN]::Successfully created temperature pthread!\n", GetTime());
	}
		sleep(2);

	if(pthread_create(&Lux_pThread, NULL, &LuxThread, NULL) != 0)
	{
		Error_Data(Main, "Lux pthread error", errno, LOG_LINUX);
	}
	else
	{
		printf("[%lf] [MAIN]::Successfully created lux pthread!\n", GetTime());
	}
	 sleep(2);

		timer_setup();

	while( RunningThreads != 0 )
	{
		printf("Running Threads==>\n");
		uint8_t CurrentAlive = RunningThreads;
	//	RunningThreads = 0;
		printf("After lock\n");
		if(CurrentAlive & LOGGING_ALIVE)
		{
			Sent_Queue(Main, Logging, "INFO", "Logging pThread is alive\n");
			printf("[%lf] [MAIN]: Logging pThread is alive\n", GetTime());
		}
		else
		{
			Error_Data(Main, "[Error]Logging pThread dead\n", 42, TERMINAL_LOG);
		}

		if(CurrentAlive & SOCKET_ALIVE)
		{
			Sent_Queue(Main, Logging, "[INFO]", "Socket pThread is alive\n");
			printf("[%lf] [MAIN]: Socket pThread is alive\n", GetTime());
		}
		else
		{
			Error_Data(Main, "[Error]Socket pThread is dead\n", 42, TERMINAL_LOG);
		}
		if(CurrentAlive & TEMP_ALIVE)
		{
			Sent_Queue(Main, Logging, "[INFO]", "Temperature pThread is alive\n");
			printf("[%lf] [MAIN]:: Temp pThread is alive\n", GetTime());
		}
		else
		{
			printf("Temp pThread is dead\n");
			Error_Data(Main, "[Error]Temp pThread is dead\n", 42, TERMINAL_LOG);
		}

		if(CurrentAlive & LUX_ALIVE)
		{
			Sent_Queue(Main, Logging, "[INFO]", "Lux pThread is alive\n");
			printf("[%lf] [MAIN]: Lux pThread is alive\n\n", GetTime());
		}
		else
		{
			Error_Data(Main, "Lux pThread is dead", 42, TERMINAL_LOG);
		}
		sleep(15);

}
	printf("[%lf] [MAIN]:: All threads Exit\n", GetTime());

	 pthread_join(Log_pThread, NULL);
	 printf("\n\n\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>LOGOGOSGSGO");
	 pthread_join(Temp_pThread, NULL);
	 printf("\n\n\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>TEMP");
	 pthread_join(Lux_pThread, NULL);
	 printf("\n\n\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>LUX");
	 pthread_join(Socket_pThread, NULL);
	 printf("\n\n\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>SOCKETTTTT");
}
