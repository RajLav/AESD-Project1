/*
*@File:SocketThread.h
*@Description_File: Header file for Socket Server
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/29/2018
*/
#ifndef SOCKETTHREAD_H_
#define SOCKETTHREAD_H_

#include "main.h"
typedef struct
{
  char string_written[150];
  int num;
}my_struct_to_be_passed;

my_struct_to_be_passed data1, data2;
my_struct_to_be_passed *p1 = &data1;
my_struct_to_be_passed *p2 = &data2;


int socket_fd, custom_socket, cust_sock, info_in, info_out;

pthread_mutex_t lock;
void * SocketThread(void * args);
uint8_t Server_initialisation(void);
uint8_t Server_Killed_Externally(void);


#endif
