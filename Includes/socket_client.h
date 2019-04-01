/*
*@File:socket_client.h
*@Description_File: Header File for socket client Sensor
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/26/2018
*/
#ifndef SOCKET_CLIENT_H_
#define SOCKET_CLIENT_H_

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
#define PORT (8009)

//Check if temperatures status is what
#define Temperature_OK			(0)
#define Temperature_HIGH		(1)
#define Temperature_LOW			(2)

//Check lights statsu
#define Night_Time				(1)
#define Day_Time				(2)

//Status of Temp or Lux by changing this particular Flag
#define Temperature_Status			(1)
#define Lux_Status					(2)

// Variables
int socket_fd, MEASURED_IN, MEASURED_OUT;
struct sockaddr_in client;


typedef enum{gpio53=0,gpio54=1,gpio55=2,gpio56=3}PINS;


/********************** Array defines *****************************/
char* LED_Dir_Path[]={"/sys/class/gpio/gpio53/direction",
					  "/sys/class/gpio/gpio54/direction",
					  "/sys/class/gpio/gpio55/direction",
					  "/sys/class/gpio/gpio56/direction"};

char* LED_Val_Path[]={"/sys/class/gpio/gpio53/value",
					  "/sys/class/gpio/gpio54/value",
					  "/sys/class/gpio/gpio55/value",
					  "/sys/class/gpio/gpio56/value"};


#endif