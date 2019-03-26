/*
*@File:light_task.h
*@Description_File: Header File for light Sensor
*@Author:Raj Lavingia
*@Date: 03/23/2018
*/
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

#define time_high 0x02  //for 402ms
#define time_med 0x01  //for 101ms
#define time_low 0x00  //for 13ms

#define gain 0x10  //for maximum gain
//
// int control_register_write ( int fd);
//
// int control_register_read ( int fd);
//
// int timing_reg_wr ( int fd, int msg);
//
// int timing_reg_rd(int fd);
//
// int control_reg_int_wr(int fd, int msg);
//
// int control_reg_int_rd(int fd);
//
// int threshold_int_reg_wr(int fd, int *array);
//
// int threshold_int_reg_rd(int fd, int *array);
//
// int id_reg_rd(int fd);
//
// uint16_t data0_reg_rd(int fd);
//
// uint16_t data1_reg_rd(int fd);
//
// float get_lux(int fd);
//
//
// int light_init(void);
//
//
// int all_reg_rd_wr(int fd);
