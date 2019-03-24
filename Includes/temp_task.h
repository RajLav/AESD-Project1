/*
*@File:temp_task.h
*@Description_File: Header File for Temperature Sensor
*@Author:Raj Lavingia
*@Date: 03/23/2018
*/

//Libraries inclided
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
//float c,k,f; //for temperature variations
//int buffer_value; //buffer in which value is put
int ret_ioctl; //return for ret_ioctl
int ret_write_register; //return for write rgeister
int ret_write_lower_register; //return for write lower reg
int temp3;
unsigned char MSB, LSB;


int temp_sensor_init(); //Used for temperature sensor initialisation
float final_read_temperature(int unit);
int all_temprg_rd_wr();
uint16_t read_temp_config_register();
int write_config_register_default();
int write_config_reg_conv_rate(uint8_t value );
int write_config_reg_em(uint8_t value );
int write_config_reg_on_off(uint8_t value );

int write_lower_register(int reg, uint16_t value );
void write_register(uint8_t value);
uint16_t read_high_register(int reg);
int write_high_register(int reg, uint16_t value );
uint16_t read_lower_register(int reg);
