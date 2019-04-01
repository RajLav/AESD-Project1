/*
*@File:LuxThread.c
*@Description_File: C File for Light Sensor
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/26/2018
*/

#include "LuxThread.h"
#include "main.h"

//pthread mutex lock used for locking unlocking of data
pthread_mutex_t lock;

uint8_t Lux_Warning = 0x02;

//Function For Base register write
uint8_t lux_common_write(uint8_t* buffedesired_value,int buffer_bytes)
{
  //Go and write to a particular location, store value in temp var
    int lux=write(File_Descripter_LUX, buffedesired_value, buffer_bytes);
    //Check if Lux is equla to bytes read or not
    if(lux!=buffer_bytes)
    {            //if not then return fail
      Error_Data(Lux, "[ERROR]Bytes do not match!\n", errno, LOG_LINUX);
      return 1;
    }
    else
    {            //else return pass
      return 0;
    }
}

//Function for base register read
uint8_t lux_common_read(uint8_t *buffedesired_value,int buffer_bytes)
{      //Go and read from the particular location
    int lux =read(File_Descripter_LUX, buffedesired_value, buffer_bytes);
    //check if temp is equal to bytes read
    if(lux!=buffer_bytes)
    {            //if not then return fail
      Error_Data(Lux, "[ERROR]Bytes do not match!\n", errno, LOG_LINUX);
      return 1;
    }
    else
    {            //if true, then write the value which is read and return pass
			printf("[INFO]Read value is %d \n",*buffedesired_value);
      return 0;
    }
}

uint8_t write_pointer(uint8_t* x)
{
    int lux = lux_common_write(x,1);      //Check if the temp value is returned success by the base reg Write_Register_Pointer
    //if not equal to 0 return fail
    if(lux!=0)
    {
      Error_Data(Lux, "[ERROR]Retrun !=0 !\n", errno, LOG_LINUX);
      return 1;
    }
    else
    {
      return 0;
    }
}

//Lux read
uint8_t lux_read_reg(uint8_t* x,uint8_t bytes)
{
    uint8_t lux = lux_common_read(x,bytes);
    if(lux!=0)      //if equal to anything else then return 1
    {
       Error_Data(Lux, "[ERROR]Retrun !=0 !\n", errno, LOG_LINUX);
      return 1;
    }
    else      //if equal to 0 then return success
    {
      return 0;
    }
}
//While sending bytes in write send 3 bytyes, lower 8, higher 8 and base address
uint8_t lux_write_reg1(uint8_t* x,uint8_t bytes)
{
   //if return not equal to 0 the return fail
    int lux = lux_common_write(x,bytes);
    if(lux!=0)
    {
      Error_Data(Lux, "[ERROR]Retrun !=0 !\n", errno, LOG_LINUX);
      return 1;
    }
    else
    {            //if equal to 0 then return success
      return 0;
    }

}
//While sending bytes in write send 3 bytyes, lower 8, higher 8 and base address
uint8_t lux_write_reg(uint8_t* x)
{
       //if return not equal to 0 the return fail
    int lux = lux_common_write(x,1);
    if(lux!=0)
    {
      Error_Data(Lux, "[ERROR]Retrun !=0 !\n", errno, LOG_LINUX);
      return 1;
    }
    else
    {            //if equal to 0 then return success
      return 0;
    }

}

//Common Custom function for all registers check for writing data to register
uint8_t lux_write_register(uint8_t register_addr, uint8_t desired_val)
{
		uint8_t lux_data_write = 0;
    int a = 1;
		lux_data_write = Lux_Command_Word_Data(register_addr);
    //Defined Register Address
    int temp = ((register_addr ==0x07) || (register_addr == 0x08) || (register_addr == 0x09) || (register_addr == 0x0B));
    if(temp)
    {
			printf("[Error_Lux]Register Address wrong\n");
			Error_Data(Lux, "[ERROR]Register Address wrong!\n", errno, LOG_LINUX);
			return 1;
		}
    //writing to specific register
		if(lux_write_reg(&lux_data_write))
		{
			printf("[ERROR]Lux Write Fail\n");
			Error_Data(Lux, "[ERROR]RRegister Address wrong!\n", errno, LOG_LINUX);
			return 1;
		}
    //Writing value to that specific register
		if(lux_write_reg1(&desired_val,a))
		{
				Error_Data(Lux, "[ERROR]Desired Value writing failed\n", errno, LOG_LINUX);
				return 1;
		}
		return 0;
}

//Common fucntion for reading register values
uint8_t lux_read_register(uint8_t register_addr, uint8_t* desired_val)
{
  uint8_t lux_data_write = 0;
	lux_data_write = Lux_Command_Word_Data(register_addr);
  if(lux_write_reg(&lux_data_write))
    {
      printf("[ERROR]Checking address failed where written", errno, LOG_LINUX);
      Error_Data(Lux, "[ERROR]Checking address failed where written\n", errno, LOG_LINUX);
      return 1;
    }
  //reading value from the register written above
  if(lux_read_reg(desired_val,1))
    {
      Error_Data(Lux, "[ERROR]Lux Value read failed\n", errno, LOG_LINUX);
      return 1;
    }
      return 0;
}


uint8_t custom_test_lux_config(void)
{
    uint8_t read_value_stored;
		char Text_In_Log_File[220];

    //Base register control, 0x03
		if(lux_write_register(Control_Register_Address, Control_Register_start))
		{
    	printf("[Error_Lux]Control register Write Failed\n");
			return 1;
		}

    //Control Register reads 0x03, if not control register failed to start and lux cant be taken
		if(lux_read_register(Control_Register_Address, &read_value_stored))
		{
				Error_Data(Lux, "[ERROR]Control register 0x03 failed to read", ENOMSG, LOG_LINUX);
				return 1;
		}

    Sent_Queue(Lux, Logging, "[INFO]", "Control Register Success\n");

    //Function to set gain high and integration time in timing register
		if(lux_write_register(Timing_Register_Address, Lux_Set_Gain_High(2)))
		{
				printf("[Error_Lux]Gain not set\n");
				return 1;
		}
		// Reading back Timing Register for specific gain and time
		if(lux_read_register(Timing_Register_Address, &read_value_stored))
		{
				Error_Data(Lux, "[ERROR]Read: Timing_Register_Address", ENOMSG, LOG_LINUX);
				return 1;
		}
    Sent_Queue(Lux, Logging, "[INFO]", "Gain and Integration Time Success\n");

    //Interrupt control register write functuon
		if(lux_write_register(Interrupt_Control_register, Interrupt_Register_Control))
		{
				printf("[Error_Lux]Interrupt control register fail write\n");
				return 1;
		}

		if(lux_read_register(Interrupt_Control_register, &read_value_stored))
		{
				Error_Data(Lux, "[ERROR]Interrupt register not read successfully", ENOMSG, LOG_LINUX);
				return 1;
		}
    Sent_Queue(Lux, Logging, "[INFO]", "Interrupt Control Register Success\n");

  		static uint8_t lux_data_write[2] = {0};
  		lux_data_write[0] = Word_Data_Register (THresholdLOW);
  		if(lux_write_reg(&lux_data_write[0]))
  		{
  				printf("[Error_Lux]Tlow write address fail \n");
  				return 1;
  		}
  		lux_data_write[0] = 0x05;
  		lux_data_write[1] = 0x0A;
  		if(lux_write_reg1(&lux_data_write[0], 2))
  		{
  				printf("[Error_Lux]Write desired value failed\n");
  				return 1;
  		}

		lux_data_write[0] = Word_Data_Register (THresholdLOW);
		if(lux_write_reg(&lux_data_write[0]))
		{
        printf("[Error_Lux]Write desired value failed from read end\n");
				return 1;
		}
		if(lux_read_reg(&lux_data_write[0], 2))
		{
				Error_Data(Lux, "[ERROR]Read Tlow Fail", errno, LOG_LINUX);
				return 1;
		}
    Sent_Queue(Lux, Logging, "[INFO]", "\nInterrupt Threshold TLow Pass\n");

		lux_data_write[0] = Word_Data_Register (THresholdHIGHLOW);
		if(lux_write_reg(&lux_data_write[0]))
		{
				printf("[Error_Lux]Error for write register for Thuigh\n");
				return 1;
		}
		lux_data_write[0] = 0x0F;
		lux_data_write[1] = 0xF0;
		if(lux_write_reg1(&lux_data_write[0], 2))
		{
      printf("[Error_Lux]Error for write value for Thuigh\n");
			return 1;
		}
		lux_data_write[0] = Word_Data_Register (THresholdHIGHLOW);
		if(lux_write_reg(&lux_data_write[0]))
		{
      printf("[Error_Lux]Error for write register for Thuigh from read end\n");
			return 1;
		}
		if(lux_read_reg(&lux_data_write[0], 2))
		{
				Error_Data(Lux, "[ERROR]Thigh read failed", errno, LOG_LINUX);
				return 1;
		}
    Sent_Queue(Lux, Logging, "INFO", "\nInterrupt Threshold THigh Pass\n");

    //Read read register
		if(lux_read_register(Read_Register_ID, &read_value_stored))
		{
				Error_Data(Lux, "[ERROR]Reading failed from register\n", ENOMSG, LOG_LINUX);
				return 1;
		}
    Sent_Queue(Lux, Logging, "INFO", "\nID Register Test Succeeded\n");

		return 0;
}

//
// uint8_t get_lux(float *lux_final_value)
// {
//
// 		float value_CH0 ,value_CH1,ratio= 0;
// 	  uint8_t lux_data_write[2] = {0};
//
//     // Powering ON the Sensor by writing 0x03 to Control Register
//     if(lux_write_register(0x00, 0x03))
//     {
//         //Log_error(Lux, "Write: Lux_Control_Reg", ENOMSG, LOGGING_AND_LOCAL);
//         return 1;
//     }
//
//     // Setting High Gain and High Integration Time
//     if(lux_write_register(0x01, Lux_Set_Gain_High(2)))
//     {
//         //Log_error(Lux, "Write: Lux_Timing_Reg", ENOMSG, LOGGING_AND_LOCAL);
//         return 1;
//     }
//     lux_data_write[0] = Word_Data_Register (Data0_Lower_Bits);
//     int temp4 = (lux_write_reg(&lux_data_write[0]));
//     if(temp4)
//   	{
//   		printf("Data 0 Not written\n");
//   		return 1;
//     }
//     temp4 = (lux_read_reg(&lux_data_write[0], 2));
//     if(temp4)
//     {
//     	Error_Data(Lux, "Data 0 not read", errno, LOG_LINUX);
//     	return 1;
//     }
//     value_CH0 = (float)((lux_data_write[1] << 8) | lux_data_write[0]);
//
// 		lux_data_write[0] = Word_Data_Register (Data1_Lower_Bits);
// 		if(lux_write_reg(&lux_data_write[0]))
//   	{
//       printf("Data 1 Not written\n");
//     	return 1;
//   	}
//     if(lux_read_reg(&lux_data_write[0], 2))
//     {
//       Error_Data(Lux, "Data 1 not read", errno, LOG_LINUX);
//     	return 1;
//     }
//
// 		value_CH1 = (float)((lux_data_write[1] << 8) | lux_data_write[0]);
//
// 		ratio = (value_CH1 / value_CH0);
//     printf(" ratio %f %f \n",value_CH1, value_CH0);
//
//     //Whole calculation is from datasheet
// 		if((ratio > 0) && (ratio <= 0.5))
//     {
//       *lux_final_value = (0.0304 * value_CH0) - (0.062 * value_CH0 * pow(ratio, 1.4));
//     }
//
// 		else if((ratio > 0.5) && (ratio <= 0.61))
//     {
//       *lux_final_value = (0.0224 * value_CH0) - (0.031 * value_CH1);
//     }
//
// 		else if((ratio > 0.61) && (ratio <= 0.80))
//     {
//       *lux_final_value = (0.0128 * value_CH0) - (0.0153 * value_CH1);
//     }
//     else if((ratio > 0.80) && (ratio <= 1.30))
//     {
//       *lux_final_value = (0.00146 * value_CH0) - (0.00112 * value_CH1);
//     }
// 		else
//     {
//       *lux_final_value = 0;
//     }
//     printf(" lux value is %f \n",*lux_final_value);
//     return 0;
//
// }
//
// uint8_t day_night(float *tem)
// {
//   get_lux(tem);
//   if(*tem <= 100)
//   {
//     printf("\n Its Noght Time");
//   	Lux_Warning = 0x01;
//   }
//   else if(*tem >=300)
//   {
//     printf("\n Its Day Time");
//     Lux_Warning = 0x02;
//   }
//   return 0;
// }

uint8_t get_lux(float *l_data)
{
		static float lux_ch0 = 0;
		static float lux_ch1 = 0;
		static float ratio = 0;

		// Using Word Mode
		static uint8_t lux_data_write[2] = {0};

		// The following two steps - repeatedly turning on power, and setting gain & timing, aren't
		// absolutely necessary - but sometimes the sensors gives out 0 lux reading without these

		// Powering ON the Sensor by writing 0x03 to Control Register
		if(lux_write_register(0x00, 0x03))
		{
				Error_Data(Lux, "[ERROR]Return value != 0\n", ENOMSG, LOG_LINUX);
				return 1;
		}

		// Setting High Gain and High Integration Time
		if(lux_write_register(0x01, Lux_Set_Gain_High(Lux_High_Integration_Time)))
		{
				Error_Data(Lux, "[ERROR]Return value != 0\n", ENOMSG, LOG_LINUX);
				return 1;
		}

		// Reading Ch0
		lux_data_write[0] = Lux_Command_Word_Data(Lux_Data0_Low);
		if(write_pointer(&lux_data_write[0]))
		{
				Error_Data(Lux, "[ERROR]Return value != 0\n", ENOMSG, LOG_LINUX);
				return 1;
		}
		if(lux_read_reg(&lux_data_write[0], 2))
		{
				Error_Data(Lux, "[ERROR]Return value != 0\n", ENOMSG, LOG_LINUX);
				return 1;
		}
		lux_ch0 = (float)((lux_data_write[1] << 8) | lux_data_write[0]);

		// Reading Ch1
		lux_data_write[0] = Lux_Command_Word_Data(Lux_Data1_Low);
		if(write_pointer(&lux_data_write[0]))
		{
				Error_Data(Lux, "[ERROR]Return value != 0\n", ENOMSG, LOG_LINUX);
				return 1;
		}
		if(lux_read_reg(&lux_data_write[0], 2))
		{
				Error_Data(Lux, "[ERROR]Return value != 0\n", ENOMSG, LOG_LINUX);
				return 1;
		}
		lux_ch1 = (float)((lux_data_write[1] << 8) | lux_data_write[0]);

		ratio = (lux_ch1 / lux_ch0);
    printf("[INFO]ratio is %f %f",lux_ch1,lux_ch0);
		// Calculation is Based on Datasheet
		if((ratio > 0) && (ratio <= 0.5))		*l_data = (0.0304 * lux_ch0) - (0.062 * lux_ch0 * pow(ratio, 1.4));
		else if((ratio > 0.5) && (ratio <= 0.61))		*l_data = (0.0224 * lux_ch0) - (0.031 * lux_ch1);
		else if((ratio > 0.61) && (ratio <= 0.80))		*l_data = (0.0128 * lux_ch0) - (0.0153 * lux_ch1);
		else if((ratio > 0.80) && (ratio <= 1.30))		*l_data = (0.00146 * lux_ch0) - (0.00112 * lux_ch1);
		else		*l_data = 0;
    printf("[INFO]ratio is %f ",*l_data);

    // Checking whether it's day or night
		if(*l_data <= 100)
    {
      printf("inside Day loop ************************\n");
      LUX_WARN = 1;
    }
		else
    {
      printf("inside Day loop2 ************************\n");

      LUX_WARN = 2;

    }
		return 0;
}

uint8_t lux_initial_sensor(void)
{
    //Open I2C bus for Lux Sensor
		File_Descripter_LUX = open(I2C_BUS, O_RDWR);
		if(File_Descripter_LUX == -1)
		{
				Error_Data(Lux, "[ERROR]open(): I2C Bus", errno, LOG_LINUX);
				return 1;
		}
    //ioctl is used for multiple buses connected
		if(ioctl(File_Descripter_LUX, I2C_SLAVE, 0x39) == -1)
		{
				Error_Data(Lux, "[ERROR]ioctl(): I2C Bus", errno, LOG_LINUX);
				return 1;
		}
		return 0;
}

uint8_t LuxThread_Init(void)
{
		char Text[60];

		sprintf(Text, "[INFO]Lux Thread successfully created with TID: %ld", syscall(SYS_gettid));
		Sent_Queue(Lux, Logging, "[INFO]", Text);
    sprintf(Text, "[INFO]Lux Thread with PID: %ld", syscall(SYS_getpid));
    Sent_Queue(Lux, Logging, "[INFO]", Text);
		pthread_mutex_lock(&lock);

		if(lux_initial_sensor() != 0)
    {
      printf("[INFO]Lux not initialised properly");
      Error_Data(Lux, "[Lux_Error]Not initialised lux sensor", ENOMSG, LOG_LINUX);
      pthread_mutex_unlock(&lock);
      return 1;
    }
    else
		{
      printf("[INFO]Lux Sensor ON\n");
      Sent_Queue(Lux, Logging, "[INFO]", "\nLux Sensor Initiliazed Successfully\n");
	  }

		if(custom_test_lux_config() != 0)
    {
      printf("\n[INFO]BIST not proper");
      Error_Data(Lux, "[Error_Lux]BIST Not Proper Initialised", ENOMSG, LOG_LINUX);
      pthread_mutex_unlock(&lock);
      return 1;
    }

		else
		{
      printf("[INFO]Bist is proper for LUX\n");
      Sent_Queue(Lux, Logging, "[INFO]", "Lux Sensor BST Pass\n");
		}

		pthread_mutex_unlock(&lock);
		Sent_Queue(Lux, Logging, "[INFO]", "Normal Lux Queue started\n");

		return 0;
}
