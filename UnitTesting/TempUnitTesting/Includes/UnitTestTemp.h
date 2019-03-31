#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdint.h>
#include <mqueue.h>

#define celsius 0
#define kelvin 1
#define fehrenheit 2
#define bus  (char*)"/dev/i2c-2" //path in bbg
#define addrabc (0x48)
#define	BUFF_SIZE	(0x02)
#define	SIZE_BUFF	sizeof(uint16_t)
//float c,k,f; //for temperature variations
//int buffer_value; //buffer in which value is put
int ret_ioctl; //return for ret_ioctl
int ret_write_register; //return for write rgeister
int ret_write_lower_register; //return for write lower reg
int temp3;
unsigned char MSB, LSB;

typedef enum{Success=0,Error=1,AddressError=2,AllocError=3,TempNotInUnits=4}ErrorCodes;
char* ErrorStates[]={"Success\n","Error\n","Address Error\n","Allocation Error\n","Temperature Unit Error\n"};