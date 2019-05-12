/*
*@File:LuxThread.h
*@Description_File: Header File for Light Sensor
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/26/2018
*/

#include "main.h"

// Address of light sensor

#define Retry_Score			10


// Internal register addresses
#define Control_Register_Address						0x00
#define Timing_Register_Address						0x01
#define Interrupt_Control_register					0x06
#define Read_Register_ID							0x0A

#define Data0_Lower_Bits						0x0C
#define Data0_Higher_Bits						0x0D
#define Data1_Lower_Bits						0x0E
#define Data1_Higher_Bits						0x0F
#define Lux_Data0_Low						0x0C
#define Lux_Data0_High					0x0D
#define Lux_Data1_Low						0x0E
#define Lux_Data1_High					0x0F

#define THresholdLOW					0x02
#define THresholdLOWHIGH					0x03
#define THresholdHIGHLOW					0x04
#define THresholdHIGHHIGH				0x05
#define Control_Register_start			0x03
#define Interrupt_Register_Control			0x1F

// Defines and macros for using command register with high flexibility

#define Word_Command_Reg(x)		((x & 0X0F) | 0XA0)


#define Lux_Low_Integration_Time		(0)		// 13.7ms
#define Lux_Med_Integration_Time		(1)		// 101ms
#define Lux_High_Integration_Time		(2)		// 402ms

#define Lux_Set_Gain_High(x)				x |	0X10

//Variables declared
int File_Descripter_LUX;
sig_atomic_t flag;

//Function Prototypes
uint8_t lux_common_write(uint8_t* buffedesired_value,int buffer_bytes);
uint8_t lux_common_read(uint8_t *buffedesired_value,int buffer_bytes);
uint8_t write_pointer(uint8_t* x);
uint8_t lux_read_reg(uint8_t* x,uint8_t bytes);
uint8_t lux_write_reg1(uint8_t* x,uint8_t bytes);
uint8_t lux_write_reg(uint8_t* x);
uint8_t lux_write_register(uint8_t register_addr, uint8_t desired_val);
uint8_t lux_read_register(uint8_t register_addr, uint8_t* desired_val);
uint8_t custom_test_lux_config(void);
uint8_t get_lux(float *lux_final_value);
uint8_t day_night(float *tem);
uint8_t lux_initial_sensor(void);
uint8_t LuxThread_Init(void);
