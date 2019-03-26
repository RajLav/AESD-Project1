
/*
*@File:light_task.h
*@Description_File: Header File for light Sensor
*@Author:Raj Lavingia
*@Date: 03/23/2018
*/
#include "light_task.h"


uint8_t write_command_register(int fd)
{
	uint8_t data;
	data = 0x80;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
}

uint8_t write_control_reg(int fd)
{
	uint8_t data;
	data = 0x80;
	write(fd, &data, 1);

	data =0x03;
	write(fd, &data, 1);

	return data;
}


uint8_t read_control_reg(int fd)
{
	uint8_t data;
	read(fd, &data, 1);
	printf("\ndata is %x",data);
}


uint8_t write_timing_register(int fd)
{
	uint8_t data;
	data = 0x81;
	write(fd, &data, 1);

	data =time_high|gain; //can be 12, 10,11
	write(fd, &data, 1);

	return data;
}


uint8_t read_timing_register(int fd)
{
	uint8_t data;
	read(fd, &data, 1);
	printf("\ntiming data is %x",data);
}


uint8_t write_timing_register_interrupt(int fd)
{
	uint8_t data;
	data = 0x86;
	write(fd, &data, 1);

	data =0x00; //can be 00,10
	write(fd, &data, 1);

	return data;
}


uint8_t read_timing_register_interrupt(int fd)
{
	uint8_t data;
	read(fd, &data, 1);
	printf("\ntiming data  interrupt is %x",data);
}


uint8_t read_identification_register(int fd)
{
	uint8_t data;
	data = 0x80 | 0x0A;
	write(fd, &data, 1);

	read(fd, &data, 1);
	printf("\ntiming data  identification is %x",data);
	return data;
}

int threshold_write(int fd, int *array)

{
	int temp = 0x82;
	write(fd, &temp,1) ;
	temp = array[0];
	write(fd, &temp, 1);
	temp = 0x83;
  write(fd, &temp, 1);
	temp = array[1];
	write(fd, &temp, 1);
	temp = 0x84;
	write(fd, &temp, 1);
	temp = array[2];
 	write(fd, &temp, 1);
	temp = 0x85;
	write(fd, &temp, 1);
	temp = array[3];
	write(fd, &temp, 1);
	return 0;

}

int threshold_read(int fd, int *array)

{

	int temp = 0x82;
	read(fd, &temp,1) ;
	temp = array[0];
	read(fd, &temp, 1);
	printf("\n array[0] is %x \n",array[0]);

	temp = 0x83;
  read(fd, &temp, 1);
	temp = array[1];
	read(fd, &temp, 1);
	printf("\n array[1] is %x \n",array[1]);
	temp = 0x84;
	read(fd, &temp, 1);
	temp = array[2];
 	read(fd, &temp, 1);
	printf("\n array[2] is %x \n",array[2]);
	temp = 0x85;
	read(fd, &temp, 1);
	temp = array[3];
	read(fd, &temp, 1);
	printf("\n array[3] is %x \n",array[3]);
	return 0;

}



uint16_t data0_read(int fd)
{
	int temp =  0x8C;
	write(fd, &temp, 1);
	uint8_t dlow0;
	read(fd, &dlow0, 1);
	temp =  0x8D;
  write(fd, &temp, 1);
	uint16_t dhigh0;
	read(fd, &dhigh0, 1);
	uint16_t final = dhigh0<<8 | dlow0;
	return final;
}

uint16_t data1_read(int fd)
{
	int temp =  0x8E;
	write(fd, &temp, 1);
	uint8_t dlow1;
  read(fd, &dlow1, 1);
	temp =  0x8F;
	write(fd, &temp, 1);
	uint16_t dhigh1;
	read(fd, &dhigh1, 1);
	uint16_t final = dhigh1<<8 | dlow1;
	return final;
}


float get_lux(int fd)
{
	float ch_0 = 0, ch_1 = 0;
	float adc,lux;

	if(write_control_reg(fd) < 0)
	{
          return -1;
        }
        if(write_timing_register(fd) < 0)
        {
	 return -1;
	}

	ch_0 = (float)data0_read(fd);
	ch_1 = (float)data1_read(fd);

	adc = ch_1/ch_0;

	if(adc>0 && adc <= 0.5)
	return lux = (0.0304 * ch_0) - (0.062 * ch_0 * pow(adc, 1.4));

	else if((adc>0.5) && (adc<=0.61))
	return lux = (0.0224 * ch_0) - (0.031 * ch_1);

    	else if((adc>0.61)&&(adc<=0.80))
        return lux = (0.0128 * ch_0) - (0.0153 * ch_1);

    	else if((adc>0.80) && (adc<=1.30))
        return lux = (0.00146 * ch_0) - (0.00112 * ch_1);

    	else if(adc > 1.30)
        return lux = 0;

return -1;
}

uint8_t light_sensor_setup()
{
	char filename[20] = "/dev/i2c-2";
	int file;
	file = open(filename, O_RDWR);
	if (file < 0)
	{
		perror("Error in file opening");
		exit(-1);
	}
	int slave_addr = 0x39;

	if (ioctl(file, I2C_SLAVE, slave_addr) < 0)
	{
		perror("Ioctl error");
		exit(-1);
	}
	return file;
}

int main()
{
int file;
light_sensor_setup();
int array[5] = {0,0,0,0};
time_t curtime;
time(&curtime);
write_control_reg(file);
read_control_reg(file);
write_timing_register(file);
read_timing_register(file);
write_timing_register_interrupt(file);
read_timing_register_interrupt(file);
read_identification_register(file);
threshold_write(file, array);
threshold_read(file, array);
float lumen = get_lux(file);
printf("Time stamp: %s",ctime(&curtime));
printf("Length of time stamp : %ld",strlen(ctime(&curtime)));
printf("The current lux is %f\n", lumen);

}
