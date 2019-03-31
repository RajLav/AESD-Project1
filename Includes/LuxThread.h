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

#define THresholdLOW					0x02
#define THresholdLOWHIGH					0x03
#define THresholdHIGHLOW					0x04
#define THresholdHIGHHIGH				0x05
#define Control_Register_start			0x03
#define Interrupt_Register_Control			0x1F

#define Lux_Set_Gain_High(x)			0x14

//Variables declared
int File_Descripter_LUX;
sig_atomic_t flag;
uint8_t LogKillSafe,AliveThreads,Lux_Error_Retry,Lux_Sensor_State;

//Function Prototypes
uint8_t lux_common_write(uint8_t* buffedesired_value,int buffer_bytes);
uint8_t lux_common_read(uint8_t *buffedesired_value,int buffer_bytes);
uint8_t write_pointer(uint8_t* x);
uint8_t lux_read_reg(uint8_t* x,uint8_t bytes);
uint8_t lux_write_reg1(uint8_t* x,uint8_t bytes);
uint8_t lux_write_reg(uint8_t* x);
uint8_t Word_Data_Register (uint8_t x);
uint8_t Command_Write_Register(uint8_t x);
uint8_t lux_write_register(uint8_t register_addr, uint8_t desired_val);
uint8_t lux_read_register(uint8_t register_addr, uint8_t* desired_val);
uint8_t custom_test_lux_config(void);
uint8_t get_lux(float *lux_final_value);
uint8_t day_night(float *tem);
uint8_t lux_initial_sensor(void);
uint8_t LuxThread_Init(void);
