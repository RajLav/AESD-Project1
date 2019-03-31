/*
*@File:temp_task.h
*@Description_File: Header File for Temperature Sensor
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/23/2018
*/

//Includes
#include "main.h"

//Macros Defined
#define Threshold_upper_Limit 			(30)
#define Threshold_lower_Limit 			(10)
#define Address_I2C        			(0x48)
#define Base_Temperature    			(0x00)
#define Above_Threshold       		 	(0x01)
#define Below_Threshold        			(0x02)
#define TLow_register            		(0x02)
#define THigh_register        			(0x03)
#define Base_Config_register        		(0x01)
#define Configuration_Register_Default_1        (0x6080)
#define Configuration_Register_Default_2        (0x60A0)
#define SM_MODE_ON        			(0x6180)
#define Fault_Bits_Read        			(0x7880)
#define EM_Mode_ON      			(0x6090)
#define CR_MODE_ON        			(0x6040)
#define Retry_Mode_ON				(10)
#define Temp_No_Retry				(0)
#define data_register_read			(0x00)


//Variables Defined

sig_atomic_t flag;
uint8_t LogKillSafe,AliveThreads,IF_RETRY,Present_Temp_Sensor_Output;

int File_Descriptor;

//Function Prototypes
uint8_t base_reg_write(uint8_t* buffer_value,int buffer_bytes);		// Common base register for writing to Thigh,Tlow and config.
void * TempThread(void * args);						// Individual temp queue is made and communicated with all the remaning threads. Also checks for kill command from USR1 or USR2.If socket client asks for temperature in any unit, this thread returns that specific unit (C,F,K)
uint8_t base_reg_read(uint8_t *buffer_value,int buffer_bytes);		// Common base register for reading Thigh,Tlow and config.
uint8_t write_reg_ptr(uint8_t* x);					 
uint8_t temp_read_reg(uint8_t* x);
uint8_t temp_write_reg(uint8_t* x);
uint8_t main_write_register(uint8_t register_addr, uint16_t desired_val); //Register addr must be between(0x01-0x03).Common base funciton for all registers (writing purpose), sends a buffer to the register through file descriptor.
uint8_t main_read_register(uint8_t register_addr, uint8_t* desired_val); //Register addr must be between(0x01-0x03).Common base funciton for all registers (writing purpose), sends a buffer to the register through file descriptor. Reads whole buffer through file-descriptor and prints two 8 bit numbers.(LSB,MSB)
uint8_t all_registers_check(void);					// Thigh and Tlow are written and read according to threshold set.
uint8_t config_register_temperature(void);				// Bits of config reg are written and read for purposes like shutdown mode, fault bits - em mode and cr(conversion rate) mode
uint8_t get_temp(float *t_data);					// Calculations are done in order to get a temp value in float in C,K or F from adc. Also checks id measured temo is above or below threshold.
uint8_t temp_initial_sensor(void);					// Starts temp sensor by openening I2C bus and checks slave addresses of I2C. 
uint8_t BIST_Temp_Check(void);						// BIST is performed.
