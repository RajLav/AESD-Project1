#include "../Includes/UnitTestLux.h"


typedef enum{Success=0,Error=1,AddressError=2}ErrorCode;

uint8_t write_command_register(int data)
{
	if(data!=COMMAND_REG)
		return Error;
	return Success;
}

uint8_t write_control_reg(int fd,int command,int control)
{
	if(command != COMMAND_REG)
		return AddressError;
	if(control!= CONTROL_REG)
		return Error;
	return Success;
}


// uint8_t read_control_reg(int fd)
// {
// 	uint8_t data;
// 	read(fd, &data, 1);
// 	printf("\ndata is %x",data);
// }


uint8_t write_timing_register(int fd,int timing)
{
	if(timing != TIMING_REG)
		return AddressError;
	int data =time_high|gain; 

	if(data == 0x10 || data==0x11 || data==0x12)
		return Success;
	
	return Error;
}


// uint8_t read_timing_register(int fd)
// {
// 	uint8_t data;
// 	read(fd, &data, 1);
// 	printf("\ntiming data is %x",data);
// }


uint8_t write_timing_register_interrupt(int fd,int timing_int,int val)
{
	if(timing_int != TIMING_REG_INT)
		return AddressError;
	if(val == 0x00 || val == 0x10)
		return Success;
	return Error;
}


// uint8_t read_timing_register_interrupt(int fd)
// {
// 	uint8_t data;
// 	read(fd, &data, 1);
// 	printf("\ntiming data  interrupt is %x",data);
// }


uint8_t read_identification_register(int fd,int id)
{
	uint8_t data;
	if(id != ID_REG)
		return AddressError;
	return Success;
}

int threshold_write(int fd, int *array,int temp0,int temp1,int temp2,int temp3)
{
	int temp = temp0; 
	if(temp != TEMP0)return AddressError;
	temp = array[0];

	temp = temp1; 
	if(temp != TEMP1)return AddressError;
	temp = array[1];

	temp = temp2; 
	if(temp != TEMP2)return AddressError;
	temp = array[2];

	temp = temp3; 
	if(temp != TEMP3)return AddressError;
	temp = array[3];

	return Success;

}

int threshold_read(int fd, int *array,int temp0,int temp1,int temp2,int temp3)
{

	int temp = temp0; 
	if(temp != TEMP0)return AddressError;
	temp = array[0];

	temp = temp1; 
	if(temp != TEMP1)return AddressError;
	temp = array[1];

	temp = temp2; 
	if(temp != TEMP2)return AddressError;
	temp = array[2];

	temp = temp3; 
	if(temp != TEMP3)return AddressError;
	temp = array[3];

	return Success;

}


uint16_t data0_read(int fd,int temp0_low,int temp0_high)
{
	int temp = temp0_low;
	if(temp != TEMP0_LOW)
		return AddressError;
	temp = temp0_high;
 	if(temp != TEMP0_HIGH)
 		return AddressError;
	return Success;
}

uint16_t data1_read(int fd,int temp1_low,int temp1_high)
{
	int temp = temp1_low;
	if(temp != TEMP1_LOW)
		return AddressError;
	temp = temp1_high;
 	if(temp != TEMP1_HIGH)
 		return AddressError;
	return Success;
}


int get_lux(int fd,float ch_1,float ch_0)
{

	float adc,lux;
	adc = ch_1/ch_0;
	//printf("%f",adc);
	if(adc<0)
		return Error;
	return Success;
}

uint8_t light_sensor_setup(int slave_addr)	// Send Slave address
{
	int address = slave_addr;
	if(slave_addr != SLAVE_ADDR)
		return AddressError;
	return Success;
}

