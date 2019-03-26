#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <complex.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <float.h>

int array[4]={0};

#define time_high	 0x02  //for 402ms
#define time_med 	 0x01  //for 101ms
#define time_low	 0x00  //for 13ms
#define gain 		 0x10 //for maximum gain

#define COMMAND_REG		 0x80
#define CONTROL_REG		 0x03
#define TIMING_REG		 0x81
#define	TIMING_REG_INT	 0x86
#define	ID_REG			 0x0A
#define	TEMP0_LOW		 0x8C
#define TEMP0_HIGH		 0x8D
#define	TEMP1_LOW		 0x8E
#define TEMP1_HIGH		 0x8F
#define	SLAVE_ADDR		 0x39

#define TEMP0 			 0X82
#define TEMP1 			 0X83
#define TEMP2 			 0X84
#define TEMP3 			 0X85

