
/*
*@File:temp_task.h
*@Description_File: Header File for Temperature Sensor
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/23/2018
*/

//Includes
#include "main.h"

//Macros Defined
#define Threshold_upper_Limit (30)
#define Threshold_lower_Limit (10)
#define Address_I2C        0x48
#define Base_Temperature    0x00
#define Above_Threshold        0x01
#define Below_Threshold        0x02
#define TLow_register            0x02
#define THigh_register        0x03
#define Base_Config_register        0x01
#define Configuration_Register_Default_1        0x6080
#define Configuration_Register_Default_2        0x60A0
#define SM_MODE_ON        0x6180
#define SM_MODE_OFF        0x6080
#define Fault_Bits_Read        0x7880
#define EM_Mode_ON        0x6090
#define EM_Mode_OFF    0x6080
#define CR_MODE_ON        0x6040
#define Retry_Mode_ON			10
#define Temp_No_Retry					0
#define data_register_read			0x00


//Function Prototypes
uint8_t base_reg_write(uint8_t* buffer_value,int buffer_bytes);
void * TempThread(void * args);
uint8_t TempThread_Init(void);
uint8_t base_reg_read(uint8_t *buffer_value,int buffer_bytes);
uint8_t write_reg_ptr(uint8_t* x);
uint8_t temp_read_reg(uint8_t* x);
uint8_t temp_write_reg(uint8_t* x);
uint8_t main_write_register(uint8_t register_addr, uint16_t desired_val);
uint8_t main_read_register(uint8_t register_addr, uint8_t* desired_val);
uint8_t all_registers_check(void);
uint8_t config_register_temperature(void);
uint8_t get_temp(float *t_data);
uint8_t temp_initial_sensor(void);
uint8_t BIST_Temp_Check(void);


