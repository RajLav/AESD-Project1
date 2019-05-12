
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

//#include "POSIX_Qs.h"
#include "main.h"
#include "TempThread.h"
#include "LuxThread.h"
#include "LoggingThread.h"
#include "SocketThread.h"

#define my_queue_main					"/my_queue_main_q"
#define my_queue_log				"/my_queue_log_q"
#define my_queue_server				"/my_queue_server_q"
#define my_queue_temp					"/my_queue_temp_q"
#define my_queue_lux					"/my_queue_lux_q"

uint8_t LogKillSafe,RunningThreads,Lux_Error_Retry,LUX_SENSOR_ST;
uint8_t IF_RETRY,Present_Temp_Sensor_Output;
uint8_t lux_t_retry, lux_state_sensor;
uint8_t temp_t_retry, temp_state_sensor;

sig_atomic_t flag;

void * Log_Queue_Thread(void * args)
{
	struct Pthread_ArgsStruct *Arguments = args;
	log_init(Arguments->LogFile_Path);
	mqd_t MY_QUEUE;
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(ThreadStruct);
	attr.mq_curmsgs = 0;
	MY_QUEUE = mq_open(my_queue_log, O_CREAT | O_RDONLY | O_CLOEXEC, 0666, &attr);
	if(MY_QUEUE == (mqd_t) -1)
	{
		Error_Data(Logging, "Logging Queue not openend\n", errno, TERMINAL_LOG);
	}

	ThreadStruct MsgRecv;
	while(1)
	{
		if(mq_receive(MY_QUEUE, &MsgRecv, sizeof(ThreadStruct), NULL) == -1)
		{
			Error_Data(Logging, "Log Receive Failed\n", errno, TERMINAL_LOG);
		}
		else
		{
			log_file(Arguments->LogFile_Path, &MsgRecv);
			RunningThreads |= LOGGING_ALIVE;
		}

			if((flag == SIGUSR1) || (flag == SIGUSR2))
			{
				printf("[%lf] Killed Log\n", GetTime());
				RunningThreads &= ~LOGGING_ALIVE;
				mq_unlink(my_queue_log);
				printf("[%lf] Logging Thread: Logging Thread has terminated successfully and will now exit\n", GetTime());
				return 0;
			}
	}

}

void Sent_Queue(uint8_t Src, uint8_t Dst, char* Log, char* Message)
{
	ThreadStruct StructSent =
	{
		.Source = Src,
		.Dest = Dst
	};
	strcpy(StructSent.LogLevel, Log);
	strcpy(StructSent.Msg, Message);

	mqd_t MY_QUEUE;

	char *destination_Address;
	if(StructSent.Dest == 1)
			destination_Address = my_queue_main;
		else if(StructSent.Dest == 2)
			destination_Address = my_queue_log;
		else if(StructSent.Dest == 3)
			destination_Address = my_queue_server;
		else if(StructSent.Dest == 4)
			destination_Address = my_queue_temp;
		else if(StructSent.Dest == 5)
			destination_Address = my_queue_lux;
		else
		{
			StructSent.Dest = Logging;
			destination_Address = my_queue_log;
			char *text1 = "[ERROR]destination_Address Error!\n";
			strcpy(StructSent.LogLevel, text1);
		}


	MY_QUEUE = mq_open(destination_Address, O_WRONLY | O_CLOEXEC);

	char ErrMsg[250];

	if(MY_QUEUE == (mqd_t) -1)
	{
		printf("\nError is there in openeing sent queue\n");
	}

	mq_send(MY_QUEUE, &StructSent, sizeof(ThreadStruct), 0);
	mq_close(MY_QUEUE);
}

void Error_Data(uint8_t Src, char* Err_Msg, int errnum, uint8_t LogVal)
{
	char Error_Log[150];
	char* SourceString;
	sprintf(Error_Log, "%s: \n", Err_Msg, Error_Log);

	if(Src == 0)
		SourceString = "Main Thread";
	else if(Src ==1)
		SourceString = "Logging Thread";
	else if(Src ==2)
		SourceString = "Socket Thread";
	else if(Src == 3)
		SourceString = "Temp Thread";
	else if(Src == 4)
		SourceString = "Lux Thread";
	else
		SourceString = "Unknown Thread";

	if(LogVal ==  LOG_LINUX)
	{
			printf("[%lf][ERROR]'%s' => %s \n", GetTime(), SourceString, Error_Log);
			Sent_Queue(Src, Logging, "ERROR\n", Error_Log);
	}

}


void * Temperature_Thread(void * args)
{
    uint8_t temp1;
    printf("success\n");
    bist();
    mqd_t MY_QUEUE;
    struct mq_attr attr;
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(ThreadStruct);
    attr.mq_curmsgs = 0;
    MY_QUEUE = mq_open(my_queue_temp, O_CREAT | O_RDONLY | O_NONBLOCK | O_CLOEXEC, 0666, &attr);
    if(MY_QUEUE == (mqd_t) -1)
    {
			printf("[Error]In openeing Temperature thread Queue\n");
		}

    uint8_t Temperature_Unit = Celsius;
    ThreadStruct MsgRecv;
		float Temperature_C = 0;
    static char Text_Log_Printed[150];
    char Temperature_Text[150];
		
    while(1)
    {
        RunningThreads |= TEMP_ALIVE;
        if((flag == Reading_Flag))
        {
          flag = 0;
          temp1 = get_temp(&Temperature_C);
          if(temp1)
            {
              Error_Data(Temp_Log, "Error while Reading Temperature\n", ENOMSG, LOG_LINUX);
              temp_t_retry = Retry_Mode_ON;
            }
            else
              {
                float Temperature_F = (Temperature_C * 1.8) + 32;
                float Temperature_K = Temperature_C + 273.15;
    //            if(Temperature_C > 40.000000 || Temperature_C < 5.000000)
                      //  	gpio_on_off(gpio54,1);

                        if(Temperature_Unit == Celsius)
                        {
                        	printf("Temperature :%f-C\n", Temperature_C);
                          sprintf(Temperature_Text, "Temperature :%f-C", Temperature_C);
													Sent_Queue(Temp_Log, Logging, "INFO", Temperature_Text);
                        }
                        else if(Temperature_Unit == Fahrenheit)
                         {
                         	printf("Temperature :%f-F\n", Temperature_F);
                          sprintf(Temperature_Text, "Temperature :%f-F", Temperature_F);
													Sent_Queue(Temp_Log, Logging, "INFO", Temperature_Text);
                         }
                        else if(Temperature_Unit == Kelvin)
                        {
														printf("Temperature :%f-K\n", Temperature_K);
                            sprintf(Temperature_Text,"Temperature :%f-K", Temperature_K);
														Sent_Queue(Temp_Log, Logging, "INFO", Temperature_Text);
                        }

                        int temp1 = mq_receive(MY_QUEUE, &MsgRecv, sizeof(ThreadStruct), NULL);
                        if(temp1 != -1)
                        {
                                if(temp1 == sizeof(ThreadStruct))
                                {
                                        if(strcmp("TC",MsgRecv.Msg) == 0)
                                        {
                                                Temperature_Unit = Celsius;
                                                sprintf(Temperature_Text, "Temperature :%f-C\n", Temperature_C);
                                        }
                                        else if(strcmp("TF",MsgRecv.Msg) == 0)
                                        {
                                                Temperature_Unit = Fahrenheit;
                                                sprintf(Temperature_Text, "Temperature :%f-F", Temperature_F);
                                        }
                                        else if(strcmp("TK",MsgRecv.Msg) == 0)
                                        {
                                                Temperature_Unit = Kelvin;
                                                sprintf(Temperature_Text, "Temperature :%f-K", Temperature_K);
                                        }
                                        Sent_Queue(Temp_Log, Socket, "INFO", Temperature_Text);
                                }
                                else
									Error_Data(Temp_Log, Text_Log_Printed, ENOMSG, LOG_LINUX);

                        }
                }
        }


        else if((flag == SIGUSR1) || (flag == SIGUSR2))
        {
          printf("\n[ERROR]Terminating Temperature Thread");
					if(mq_unlink(my_queue_temp) == 0)
          {
						printf("\nSuccessfully unlinked Temp queue");
          }

            char TempTxt[150];
            if(flag == SIGUSR1)
            {
							printf("\n[ERROR]USR1 Received=>%d", flag);
            }
            else if(flag == SIGUSR2)
            {
              printf("\n[ERROR]USR2 Received=>%d", flag);
            }
            RunningThreads &= ~TEMP_ALIVE;
						printf("\nTemp Thread terminated successfully");
            return 0;
        }
    }
}


void * LuxThread(void * args)
{

	uint8_t resp;
	resp = LuxThread_Init();
	if(resp)
	{
		Error_Data(Lux, "[ERROR]Error Initializing Lux Sensor\n", ENOMSG, LOG_LINUX);
		lux_t_retry = Retry_Score;
	}
	else
	{
		lux_t_retry = 0;
		printf("\n===============================================LUX INIT DONE");
	}


	mqd_t MY_QUEUE;
	struct mq_attr attr;
	attr.mq_flags = O_NONBLOCK;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(ThreadStruct);
	attr.mq_curmsgs = 0;

	MY_QUEUE = mq_open(my_queue_lux, O_CREAT | O_RDONLY | O_NONBLOCK | O_CLOEXEC, 0666, &attr);
	if(MY_QUEUE == (mqd_t) -1)
	{
		Error_Data(Lux, "Opening lUX queue error", errno, LOG_LINUX);
	}

	ThreadStruct MsgRecv;
	float Lux_Value = 0;
	static char Text_In_Log_File[150];	char Lux_Text[150];
	while(1)
	{
		RunningThreads |= LUX_ALIVE;

		  if(flag == Reading_Flag)
{
				printf("\n======================== LUX IF PASSED");
				flag = 0;
				resp = get_lux(&Lux_Value);
				if(resp)
				{
						Error_Data(Lux, "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[]]]]]]]]]]]]]]]]]]]]]]]]]]]]]Error while Reading Lux\n", ENOMSG, LOG_LINUX);
						lux_t_retry = 10;
				}
				else
				{
						sprintf(Lux_Text, "Lux :%f", Lux_Value);

            Sent_Queue(Lux, Logging, "INFO", Lux_Text);
												int resp = mq_receive(MY_QUEUE, &MsgRecv, sizeof(ThreadStruct), NULL);
												if(resp != -1)
												{
														if(resp == sizeof(ThreadStruct))
														{


																if(strcmp("LX",MsgRecv.Msg) == 0)
																{


																		sprintf(Lux_Text, "Lux is *%f*", Lux_Value);

																}

																Sent_Queue(Lux, Socket, "INFO", Lux_Text);
														}
														else
														{
																sprintf(Text_In_Log_File, "[ERROR]Issue with received bytes!");

														}
												}
										}
}

			else if((flag == SIGUSR1) || (flag == SIGUSR2))
		{

			printf("[ERROR]Terminating Lux Thread\n");

			if(mq_unlink(my_queue_lux) == 0)
			{
				printf("Successfully unlinked Lux queue!\n");
			}

			char luxTxt[150];
			if(flag == SIGUSR1)
			{
				printf( "\n[ERROR]USR1 Received =>%d\n", flag);
			}
			else if(flag == SIGUSR2)
			{
				printf( "\n[ERROR]USR2 Received =>%d\n", flag);
			}

			RunningThreads &= ~LUX_ALIVE;
			printf("\nLux Thread terminated successfully");
			return 0;
		}
	}
}


void * SocketThread(void * args)
{

	if(Server_initialisation())
	{
		Error_Data(Socket, "[ERROR]Socket Init Failed\n", ENOMSG, LOG_LINUX);
		return 0;
	}
	else		Sent_Queue(Socket, Logging, "[INFO]", "Socket Initialization Success\n");


	mqd_t MY_QUEUE;


	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(ThreadStruct);
	attr.mq_curmsgs = 0;


	MY_QUEUE = mq_open(my_queue_server, O_CREAT | O_RDONLY | O_CLOEXEC, 0666, &attr);
	if(MY_QUEUE == (mqd_t) -1)
	{
		Error_Data(Socket, "Queue socket openig error\n", errno, LOG_LINUX);
	}

	ThreadStruct MsgRecv;

	while(1)
	{

		while((flag != SIGUSR1) && (flag != SIGUSR2))
		{

			SOCK_ST = SOCK_ON;

			RunningThreads |= SOCKET_ALIVE;

			custom_socket = accept(socket_fd, (struct sockaddr *)0, 0);

			if(custom_socket < 0)
			{
				Error_Data(Socket, "[ERROR]Failed to Accept Socket\n", errno, LOG_LINUX);
			}
			else
			{
				cust_sock = custom_socket;
			}

			   MEASURED_IN = read(custom_socket,p2,sizeof(my_struct_to_be_passed));
            if(MEASURED_IN < 0)
            {
                Error_Data(Socket, "[ERROR]Failed to Read Socket\n", errno, LOG_LINUX);
            }

            char Socket_Text[150];
            sprintf(loglevel_sock, "INFO");

            char Socket_Text_q[60];
            sprintf(loglevel_q, "INFO");

            if(strcmp("external", p2->string_written) == 0)
            {
              	printf("\n[INFO]Socket Thread Exiting");
								printf("\nUser Signal Passed - Terminating Socket Thread");
                if(mq_unlink(my_queue_server) == 0)
                {
									printf("\nSuccessfully unlinked Socket queue!");
                }
                	char TempTxt[150];
                if(flag == SIGUSR1)
                {
                  printf("\n[ERROR]USR1 - Received : %d", flag);
                }
                else
                {
									printf("\n[ERROR]USR2 - Received : %d", flag);
                }
								RunningThreads &= ~SOCKET_ALIVE;
								printf("\nSocket Thread terminated successfully");
                return 0;
            }
						else
            {
                if(strcmp("Temperature", p2->string_written) == 0)
                {
										req_warning =	TEMP_WARN_Req;
                    if(p2->num == Celsius)
                    {
                        sprintf(Socket_Text, "[INFO]Client Requested Temperature in C");
                        sprintf(Socket_Text_q, "TC");
                    }
                    else if(p2->num == Fahrenheit)
                    {
                        sprintf(Socket_Text, "[INFO]lient Requested Temperature in F");
                        sprintf(Socket_Text_q, "TF");
                    }
                    else if(p2->num == Kelvin)
                    {
                        sprintf(Socket_Text, "[INFO]Client Requested Temperature in K");
                        sprintf(Socket_Text_q, "TK");
                    }
                    else
                    {
                        sprintf(Socket_Text, "[INFO]Client Requested Temperature in Invalid Parameter - Sending in C");
                        sprintf(loglevel_sock, "WARNING");
                        sprintf(Socket_Text_q, "TC");
                    }

                    Sent_Queue(Socket, Temp_Log, loglevel_q, Socket_Text_q);
                }

                else if(strcmp("Lux", p2->string_written) == 0)
                {
										req_warning =	LUX_WARN_Req;
                    sprintf(Socket_Text, "Client Requested Lux");
                    sprintf(Socket_Text_q, "LX");
                    Sent_Queue(Socket, Lux, loglevel_q, Socket_Text_q);
                }

                else
                {
                    sprintf(Socket_Text, "Invalid Client Request");
                    sprintf(loglevel_sock, "ERROR");
                }

                Sent_Queue(Socket, Logging, loglevel_sock, Socket_Text);

                struct timespec tm;
                clock_gettime(CLOCK_REALTIME, &tm);
                tm.tv_sec += 2;

                int resp = mq_timedreceive(MY_QUEUE, &MsgRecv, sizeof(ThreadStruct), NULL, &tm);
								printf("Resp: %d\n", resp);
                if(resp == -1)
                {
                    Error_Data(Socket, "Receive Fail \n", errno, LOG_LINUX);
                    p1->num = 0;
                }
                else if(resp < sizeof(ThreadStruct))
                {
                    Error_Data(Socket, "Receive Fail\n", errno, LOG_LINUX);
                    p1->num = 0;
                }
                else if(resp == sizeof(ThreadStruct))
                {
                    sprintf(Socket_Text, "[INFO]Response from Queue: %s\n", MsgRecv.Msg);

                    strcpy(loglevel_sock, "INFO");

										if(req_warning ==TEMP_WARN_Req)	{printf("1st type : ");p1->num = TEMP_WARN;}
										else if(req_warning == LUX_WARN_Req)		{printf("2st type : %d",LUX_WARN);p1->num = LUX_WARN;}
										else
										{
											p1->num = 0;
										}
                    Sent_Queue(Socket, Logging, loglevel_sock, Socket_Text);
                }

                custom_socket = cust_sock;
                MEASURED_OUT = write(custom_socket,p1,sizeof(my_struct_to_be_passed));
                if (MEASURED_OUT < 0)
                {
                    Error_Data(Socket, "[ERROR]Failed to write socket!", errno, LOG_LINUX);
                }
                else
                {
                    sprintf(Socket_Text, "[INFO]Data sent Successfully to the Remote Client");
                    strcpy(loglevel_sock, "INFO");
                    Sent_Queue(Socket, Logging, loglevel_sock, Socket_Text);
                }
            }
        }
    }
}
