/*
*@File:temp_task.c
*@Description_File: C File for Temperature Sensor
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/23/2018
*/
#include "TempThread.h"
#include "POSIX_Qs.h"
#include "main.h"

//pthread mutex lock used for locking unlocking of data
pthread_mutex_t lock;

uint8_t Warning_LED_Indicator = Base_Temperature;


//Function For Base register write
uint8_t base_reg_write(uint8_t* buffedesired_value,int buffer_bytes)
{
        //Go and write to a particular location, store value in temp var
    int temp=write(File_Descriptor, buffedesired_value, buffer_bytes);
        //Check if temp is equla to bytes read or not
    if(temp!=buffer_bytes)
    {
            //if not then return fail
      return 1;
    }
    else
    {
            //else return pass
      return 0;
    }
}

//Function for base register read
uint8_t base_reg_read(uint8_t *buffedesired_value,int buffer_bytes)
{
      //Go and read from the particular location
    int temp =read(File_Descriptor, buffedesired_value, buffer_bytes);
        //check if temp is equal to bytes read
    if(temp!=buffer_bytes)
    {
            //if not then return fail
      return 1;
    }
    else
    {
            //if true, then write the value which is read and return pass
            printf("\n Read value is %d \n",*buffedesired_value);
      return 0;
    }
}

uint8_t write_reg_ptr(uint8_t* x)
{
      //Check if the temp value is returned success by the base reg Write_Register_Pointer
    int temp = base_reg_write(x,1);
          //if not equal to 0 return fail
    if(temp!=0)
    {
            //if equal to 0 then let this function retrun 0
      return 1;
    }
    else
    {
      return 0;
    }
}

uint8_t temp_read_reg(uint8_t* x)
{
      //while reading , it will read 2 bytes not 3
    uint8_t temp = base_reg_read(x,2);
    if(temp!=0)
    {
      //if equal to anything else then return 1
      return 1;
    }
    else
    {
      //if equal to 0 then return success
      return 0;
    }
}
//While sending bytes in write send 3 bytyes, lower 8, higher 8 and base address
uint8_t temp_write_reg(uint8_t* x)
{
        //if return not equal to 0 the return fail
    int temp = base_reg_write(x,3);
    if(temp!=0)
    {
      return 1;
    }

    else
    {
            //if equal to 0 then return success
      return 0;
    }

}

//Common Custom function for all registers check for writing data to register
uint8_t main_write_register(uint8_t register_addr, uint16_t desired_val)
{
  //Sent_Queue(Temp, Logging, "[INFO]", "Main Write Register Pass in second attempt\n");
  //register addr is the respective base address and desired value is the value you want to write in that registe
    uint8_t buffer_array[3]={register_addr,desired_val>>8,desired_val & 0x00FF};
  int temp = ((register_addr < 0x01) || (register_addr > 0x03));
  if(temp)
    {
      printf("[Error] In register addressing\n");
      return 1;
    }
  temp = (write_reg_ptr(&register_addr));
  if(temp)
    {
         printf("[Error] In writing to register\n");
      return 1;
    }
  temp = (temp_write_reg(&buffer_array));
  if(temp)
    {
      printf("[Error]In writing to temp\n");
      return 1;
    }
    return 0;
}

//Common Custom function for all registers check for reading data to register
uint8_t main_read_register(uint8_t register_addr, uint8_t* desired_val)
{
  int temp = (write_reg_ptr(&register_addr));
  if(temp)
    {
      printf("[Error]In writing to register\n");
      return 1;
    }
  temp = (temp_read_reg(desired_val));
  if(temp)
    {
      printf("[Error]In reading from register");
      return 1;
    }
  //  Sent_Queue(Temp, Logging, "[INFO]", "Main Read Register Pass\n");
    return 0;
}

//Check all registers for write and read equality for thigh and tlow
uint8_t all_registers_check(void)
{
        static uint8_t Register_buffer_storage[2];
        uint16_t final_threshold_high = Threshold_upper_Limit << 8;
        int temp = (main_write_register(THigh_register, final_threshold_high));
        if(temp)
        {
          printf("[Error]Thigh not written \n");
          return 1;
        }
        temp=(main_read_register(THigh_register, &Register_buffer_storage[0]));
        if(temp)
        {
          printf("[Error]Thigh not read \n");
          return 1;
        }

        temp = (main_write_register(TLow_register, Threshold_lower_Limit << 8));
        if(temp)
        {
          printf("[Error]Tlow not written \n");
          return 1;
        }

        if(main_read_register(TLow_register, &Register_buffer_storage[0]))
        {
          printf("[Error]Tlow not read \n");
          return 1;
        }
  //
        return 0;
}

//Configuration register write and read values
uint8_t config_register_temperature(void)
{
        static uint8_t Register_buffer_storage[2];
        static char Text_Log_Printed[150];

        if(main_read_register(Base_Config_register, &Register_buffer_storage[0]))
        {
          printf("[Error]Config Register not written properly\n");
          return 1;
        }

        Sent_Queue(Temp, Logging, "[INFO]", "\nConfig Test Success\n");

        if(main_write_register(Base_Config_register, SM_MODE_ON))
        {
          printf("[Error]SM Mode On write\n");
          return 1;
        }

        if(main_read_register(Base_Config_register, &Register_buffer_storage[0]))
        {
          printf("[Error]SM Mode On read\n");
          return 1;
        }

        if(main_write_register(Base_Config_register, Fault_Bits_Read))
        {
          printf("[Error]Fault Bits write");
          return 1;
        }

              if(main_read_register(Base_Config_register, &Register_buffer_storage[0]))
        {
          printf("[Error]Fault Bits read");
          return 1;
        }

              if(main_write_register(Base_Config_register, EM_Mode_ON))
        {
          printf("[Error]EM On write");
          return 1;
        }

        if(main_read_register(Base_Config_register, &Register_buffer_storage[0]))
        {
          printf("[Error]EM On read");
          return 1;
        }

            if(main_write_register(Base_Config_register, CR_MODE_ON))
        {
          printf("[Error]CR On write");
          return 1;
        }

        if(main_read_register(Base_Config_register, &Register_buffer_storage[0]))
        {
          Error_Data(Temp, "Error in Reading Base register", ENOMSG, Log_Native);
            return 1;
        }

    //    Sent_Queue(Temp, Logging, "[INFO]", "\nAll Registers Test Succeeded\n");
        return 0;
}

//Function for calculating the final value of temperature using ADC
uint8_t get_temp(float *final_temp_data_stored)
{
        static uint8_t Register_buffer_storage[2];
        int temp = (main_read_register(data_register_read, &Register_buffer_storage[0]));
        if(temp)
        {
          Error_Data(Temp, "Register read fail\n", ENOMSG, Log_Native);
          return 1;
        }
        *final_temp_data_stored = ((Register_buffer_storage[0] << 8) | Register_buffer_storage[1]) >> 4;
        *final_temp_data_stored *= 0.0625;
        printf( "Generated Temp Value : %f",*final_temp_data_stored );
        //check for thresholds

        if(*final_temp_data_stored < Threshold_lower_Limit)
        {
          Sent_Queue(Temp, Logging, "[INFO]", "\nValue is below threshold\n");
          Warning_LED_Indicator = Below_Threshold;
        }
        else if(*final_temp_data_stored > Threshold_upper_Limit)
        {
          Sent_Queue(Temp, Logging, "[INFO]", "\nValue is Above threshold\n");
          Warning_LED_Indicator = Above_Threshold;
        }
        else
        {
          Sent_Queue(Temp, Logging, "[INFO]", "\nValue is equals threshold\n");
          Warning_LED_Indicator = Base_Temperature;
        }
                printf( "temp value : %f",*final_temp_data_stored );
              return 0;
}

//Function for starting thr Temperature sensor
uint8_t temp_initial_sensor(void)
{
        //I2c Bus opened // path is predefined
        File_Descriptor = open(I2C_BUS, O_RDWR);
        if(File_Descriptor <0)
        {
                Error_Data(Temp, "open(): I2C Bus", errno, Log_Native);
                return 1;
        }
        if(ioctl(File_Descriptor, I2C_SLAVE, Address_I2C) <0)
        {
                Error_Data(Temp, "ioctl(): I2C Bus", errno, Log_Native);
                return 1;
        }
      //  Sent_Queue(Temp, Logging, "[INFO]", "\nI2C initialised properly with temp sensor\n");
        return 0;
}

//Function for checking thr BST of Temperature
uint8_t BIST_Temp_Check(void)
{
    printf("\n In temp check function");
        char data_print[60];
        sprintf(data_print, "[INFO]Temperature Thread Created Successfully with TID: %ld\n", syscall(SYS_gettid));
    Sent_Queue(Temp, Logging, "[INFO]", data_print);
        sprintf(data_print, "[INFO]PID: %ld\n", syscall(SYS_getpid));
    Sent_Queue(Temp, Logging, "[INFO]", data_print);

        pthread_mutex_lock(&lock);

    int temp = temp_initial_sensor();
        if(temp != 0)
        {
      printf("\n Sensor not initilised");
            Error_Data(Temp, "[Error]Temperataure sensor not initialised \n", ENOMSG, Log_Native);
            pthread_mutex_unlock(&lock);
            return 1;
        }
    else
    {
      printf("\n in success loop");
      Sent_Queue(Temp, Logging, "INFO", "\nTemperature Sensor Initiliazed Successfully\n");
    }

        int temp1 = main_write_register(Base_Config_register, Configuration_Register_Default_1);
        if(temp1 != 0)
        {
      printf("\n reset successfullu not done");
            Error_Data(Temp, "[Error]Sensor Not reset properly\n", ENOMSG, Log_Native);
            pthread_mutex_unlock(&lock);
            return 1;
        }
    else
    {
      printf("\n reset successfullu done");
      Sent_Queue(Temp, Logging, "INFO", "\nReset Successfully\n");
    }

    int temp2 = all_registers_check();
        if(temp2 !=0)
        {
          Error_Data(Temp, "[Error]All registers not checked successfully\n", ENOMSG, Log_Native);
            pthread_mutex_unlock(&lock);
            return 1;
        }
        else
        {
      printf("Register OK\n");
      Sent_Queue(Temp, Logging, "[INFO]", "\nAll registers check Successfully\n");
        }

        int temp3 = config_register_temperature();
        if(temp3 != 0)
        {
            Error_Data(Temp, "[Error]BST not success\n", ENOMSG, Log_Native);
            pthread_mutex_unlock(&lock);
            return 1;
        }
        else
        {
      printf("Bist Pass\n");
            Sent_Queue(Temp, Logging, "[INFO]", "Built In Success\n");
        }

         pthread_mutex_unlock(&lock);
     printf("Normal function\n");
         // Sent_Queue(Temp, Logging, "[INFO]", "Normal thread of temp started\n");
    return 0;

}
void bist()
{
  uint8_t temp1;
  //will check for all registers and start the temp sensor
  temp1 = BIST_Temp_Check();
  printf("\n success value is %d",temp1);
  if(temp1)
  {
    printf("\n inital temp check not success");
    Error_Data(Temp, "Error while Initializing Temperature Sensor", ENOMSG, Log_Native);
    IF_RETRY = Retry_Mode_ON;
    Present_Temp_Sensor_Output = Sensor_Offline;
  }
  else
  {
    printf("\n inital temp check  success");
    IF_RETRY = 0;
    Present_Temp_Sensor_Output = Sensor_Online;
  }
}
