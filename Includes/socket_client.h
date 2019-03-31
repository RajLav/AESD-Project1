/*
*@File:socket_client.h
*@Description_File: Header File for socket client Sensor
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/26/2018
*/


//Includes are seperate because main.h cant be included for socket client
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <malloc.h>
#include <arpa/inet.h>

//Structures used for time
struct timespec timespec_struct;
struct timeval current_time;

//Port Defined for connection between client and server
#define PORT 8009


//Temperature in specific unit
#define tempc 25
#define temf  75
#define tempk 300

//Check if temperatures status is what
#define Temperature_OK					0x00
#define Temperature_HIGH		0x01
#define Temperature_LOW		0x02

//Check lights statsu
#define Night_Time			0x01
#define Day_Time				0x02

//Status of Temp or Lux by changing this particular Flag
#define Temperature_Status			1
#define Lux_Status				2


int socket_fd, info_in, info_out;
struct sockaddr_in client;
