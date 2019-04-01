/*
*@File:main.h
*@Description_File: Header File for main function
*@Author:Raj Lavingia
*@Date: 03/23/2018
*/

/*Inclusion of Libraries for main.c*/
#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <math.h> //pow
#include <float.h>
#include <complex.h>
#include <time.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <malloc.h>


#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

typedef struct 
{
	uint8_t Src;
	uint8_t Dst;
	char log_level[150];
	char Message[150];
} ThreadStruct;


 char* ThreadString[]={" ","Main","Logging","Socket","Temperature","Lux","Unknown"};

 #endif
