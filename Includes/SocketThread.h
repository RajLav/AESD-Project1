/*
*@File:LuxThread.c
*@Description_File: Header file for Socket Server
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/29/2018
*/
#ifndef SOCKETTHREAD_H_
#define SOCKETTHREAD_H_

#include "main.h"
#define TEMP_WARN_Req				(1)
#define LUX_WARN_Req				(2)
uint8_t TEMP_WARN;
uint8_t LUX_WARN;
uint8_t req_warning = 0;

uint8_t SOCK_ST;


typedef struct
{
  char string_written[150];
  int num;
}my_struct_to_be_passed;

my_struct_to_be_passed data1, data2;
my_struct_to_be_passed *p1 = &data1;
my_struct_to_be_passed *p2 = &data2;


int socket_fd, custom_socket, cust_sock, MEASURED_IN, MEASURED_OUT;
char loglevel_sock[30], loglevel_q[30];

pthread_mutex_t lock;

sig_atomic_t flag;
uint8_t LogKillSafe;
uint8_t RunningThreads;

void * SocketThread(void * args);
uint8_t Server_initialisation(void);
uint8_t Server_Killed_Externally(void);


#endif
