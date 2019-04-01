/*Name : Raj Lavingia and Yash Gupte
Credits : Nikhil Divekar, POorn Mehta, Akash, Khalid Awadhi
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

pthread_mutex_t lock, lock_var;
sig_atomic_t flag = 0;
uint8_t sync_lux_temp = 0;
uint8_t LogKillSafe = 3;
uint8_t RunningThreads = 0x00;

uint8_t Counter = 0;
uint8_t IF_RETRY = 0;
uint8_t IF_RETRY_LUX = 0;
uint8_t TEMP_SENSOR_ST = SENSOR_OFF;
uint8_t LUX_SENSOR_ST = SENSOR_OFF;

uint8_t SOCK_ST = SOCK_OFF;

double GetTime()
{
	struct timeval time;

	gettimeofday(&time, 0);

	return (double)(time.tv_sec)+(((double)(time.tv_usec))/1000000);
}

/* References :  Poorna Mehta */

uint8_t Server_Killed_Externally(void)
{

    typedef struct
    {
      char str[150];
      int num;
    }t_strct;

    t_strct t_strct1;
    t_strct *pt_strct1 = &t_strct1;

    strcpy(pt_strct1->str, "external");
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

    t_out = write(temp_sock,pt_strct1,sizeof(t_strct));
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
		printf("\n Success file open");
	}
	else
	{
		printf("\n Error in pin or state\n");
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
	printf("\n Success file open");

	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%d",state);

	fclose(fp1);

}


void signal_handler(int value)
{
	if(value == SIGVTALRM)
	{
			if(SOCK_ST == SOCK_ON)
			{
				RunningThreads |= SOCKET_ALIVE;
			}
			Counter += 1;
			if(Counter == CountThresh)
			{
					Counter = 0;
					if(IF_RETRY > 0)
					{
							IF_RETRY =IF_RETRY - 1;
							if(close(File_Descriptor))
							Sent_Queue(Main, Logging, "[INFO]", "\nRetrying for temperature\n");
							if(temp_initial_sensor()) printf("Sensor not initialiased\n");
							else
							{
								IF_RETRY = 0;
								TEMP_SENSOR_ST = SOCK_ON;
								Sent_Queue(Main, Logging, "[INFO]", "\nTemperature sensor is ON\n");
							}
					}

					if(IF_RETRY_LUX > 0)
					{
							IF_RETRY_LUX -= 1;
							if(close(File_Descripter_LUX))
							Sent_Queue(Main, Logging, "[INFO]", "Retrying for Lux\n");
							if(LuxThread_Init())	printf("Sensor not initialiased\n");
							else
							{
								IF_RETRY_LUX = 0;
								LUX_SENSOR_ST = SENSOR_ON;
								Sent_Queue(Main, Logging, "INFO", "\nLux Sensor is ON Now\n");
							}
					}
			}
			if(sync_lux_temp == 0)
			{
				flag = Temp_Sig;
				sync_lux_temp = 1;
			}
			else
			{
				flag = Lux_Sig;
				sync_lux_temp = 0;
			}
	}
	else
	{
			flag = value;
			SOCK_ST = SOCK_OFF;
			if(Server_Killed_Externally())        printf("\nSocket killing failed\n");
	}
}

void timer_setup(void)
{

		struct sigaction sig_action_time;
		struct itimerval sig_action_time_timer;

		sig_action_time.sa_handler = &signal_handler;


		sig_action_time_timer.it_interval.tv_sec = 0;
		sig_action_time_timer.it_interval.tv_usec = 250000;


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

	printf("Initiated ===> PID: %d\n\n", getpid());


	sprintf(LogFilePathUsr, "./log.txt");

	strcpy(args.LogFile_Path, LogFilePathUsr);
	if( pthread_mutex_init(&lock, NULL) != 0 )
	{
		Error_Data(Main, "mutex init error", errno, TERMINAL_LOG);
		return -1;
	}

	if( pthread_mutex_init(&lock_var, NULL) != 0 )
	{
		Error_Data(Main, "mutex init error", errno, TERMINAL_LOG);
		return -1;
	}


	if(pthread_create(&Log_pThread, NULL, &Log_Queue_Thread, (void *)&args) != 0)
	{
		Error_Data(Main, "logging pthread error", errno, TERMINAL_LOG);
	}
	else
	{
		printf("[%lf] [MAIN]::Successfully created logging pthread!\n\n", GetTime());
	}
		sleep(2);
	if(pthread_create(&Socket_pThread, NULL, &SocketThread, NULL) != 0)
	{
		Error_Data(Main, "Socket pthread error", errno, LOG_LINUX);
	}
	else
	{
		printf("[%lf] [MAIN]::Successfully created socket pthread!\n\n", GetTime());
	}
		sleep(2);

	if(pthread_create(&Temp_pThread, NULL, &Temperature_Thread, NULL) != 0)
	{
		Error_Data(Main, "Temperature pthread error", errno, LOG_LINUX);
	}
	else
	{
		printf("[%lf] [MAIN]::Successfully created temperature pthread!\n\n", GetTime());
	}
		sleep(2);

	if(pthread_create(&Lux_pThread, NULL, &LuxThread, NULL) != 0)
	{
		Error_Data(Main, "Lux pthread error", errno, LOG_LINUX);
	}
	else
	{
		printf("[%lf] [MAIN]::Successfully created lux pthread!\n\n", GetTime());
	}
	 sleep(2);

		timer_setup();

	while( RunningThreads != 0 )
	{
		printf("\n Running Threads==>\n");
		uint8_t CurrentAlive = RunningThreads;
		RunningThreads = 0;

	printf("After lock\n");

		if(CurrentAlive & LOGGING_ALIVE)
		{
			Sent_Queue(Main, Logging, "INFO", "Logging pThread is alive");
			printf("[%lf] [MAIN]: Logging pThread is alive\n\n", GetTime());
		}
		else
		{
			Error_Data(Main, "Logging pThread dead", 42, TERMINAL_LOG);
		}

		if(CurrentAlive & SOCKET_ALIVE)
		{
			Sent_Queue(Main, Logging, "INFO", "Socket pThread is alive");
			printf("[%lf] [MAIN]: Socket pThread is alive\n\n", GetTime());
		}
		else
		{
			Error_Data(Main, "Socket pThread is dead", 42, TERMINAL_LOG);
		}
		if(CurrentAlive & TEMP_ALIVE)
		{
			Sent_Queue(Main, Logging, "INFO", "Temperature pThread is alive");
			printf("[%lf] [MAIN]: Temp pThread is alive\n\n", GetTime());
		}
		else
		{
			printf("Temp pThread is dead\n");
			Error_Data(Main, "Temp pThread is dead", 42, TERMINAL_LOG);
		}

		if(CurrentAlive & LUX_ALIVE)
		{
			Sent_Queue(Main, Logging, "INFO", "Lux pThread is alive");
			printf("[%lf] [MAIN]: Lux pThread is alive\n\n", GetTime());
		}
		else
		{
			Error_Data(Main, "Lux pThread is dead", 42, TERMINAL_LOG);
		}
		sleep(10);

}
	printf("[%lf] [MAIN]: All threads Exit\n\n", GetTime());

	 pthread_join(Log_pThread, NULL);
	 pthread_join(Socket_pThread, NULL);
	 pthread_join(Temp_pThread, NULL);
	 pthread_join(Lux_pThread, NULL);
}
