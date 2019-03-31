#include "../Includes/LuxThread.h"
#include "../Includes/main.h"
//#include "POSIX_Qs.h"

//pthread mutex lock used for locking unlocking of data
pthread_mutex_t lock;

uint8_t Lux_Warning = 0x02;

//Function For Base register write
uint8_t lux_common_write(uint8_t* buffedesired_value,int buffer_bytes)
{
  //Go and write to a particular location, store value in temp var
   if(buffedesired_value==NULL)
    return AllocError;
 
  return Success;
}

//Function for base register read
uint8_t lux_common_read(uint8_t *buffedesired_value,int buffer_bytes)
{      //Go and read from the particular location
   if(buffedesired_value==NULL)
    return AllocError;
  if(sizeof(buffedesired_value)<SIZE_BUFF)
    return Error;
  return Success;
}

uint8_t write_pointer(uint8_t* x)
{
    //int lux = lux_common_write(x,1);      //Check if the temp value is returned success by the base reg Write_Register_Pointer
    //if not equal to 0 return fail
   uint8_t buffer_bytes = 1;
   uint8_t lux = lux_common_write(x,buffer_bytes);
   return lux;
}

//Lux read
uint8_t lux_read_reg(uint8_t* x,uint8_t bytes)
{
    uint8_t lux = lux_common_read(x,bytes);
    return lux;
}
//While sending bytes in write send 3 bytyes, lower 8, higher 8 and base address
uint8_t lux_write_reg1(uint8_t* x,uint8_t bytes)
{
   //if return not equal to 0 the return fail
    int lux = lux_common_write(x,bytes);
    if(lux!=0)
    {
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
      return 1;
    }
    else
    {            //if equal to 0 then return success
      return 0;
    }

}
uint8_t Word_Data_Register (uint8_t x)
{
  int temp = ((x & 0x0F0) | 0x0A);
  if(!temp)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
uint8_t Command_Write_Register(uint8_t x)
{
  int temp =  ((x & 0x0F) | 0x80);
  if(!temp)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
// //Common Custom function for all registers check for writing data to register
// uint8_t lux_write_register(uint8_t register_addr, uint8_t desired_val)
// {
// 		uint8_t lux_data_write = 0;
//     int a = 1;
// 		lux_data_write = Command_Write_Register(register_addr);
//     //Defined Register Address
//     int temp = ((register_addr ==0x07) || (register_addr == 0x09) || (register_addr == 0x09) || (register_addr == 0x0B));
//     if(temp)
//     {
// 			printf("[Error_Lux]Register Address wrong\n");
// 			return 1;
// 		}
//     //writing to specific register
// 		if(lux_write_reg(&lux_data_write))
// 		{
// 			printf("Lux Write Fail\n");
// 			return 1;
// 		}
//     //Writing value to that specific register
// 		if(lux_write_reg1(&desired_val,a))
// 		{
// 				Error_Data(Lux, "Desired Value writing failed\n", errno, Log_Native);
// 				return 1;
// 		}
// 		return 0;
// }

// //Common fucntion for reading register values
// uint8_t lux_read_register(uint8_t register_addr, uint8_t* desired_val)
// {
//   uint8_t lux_data_write = 0;
// 	lux_data_write = Command_Write_Register(register_addr);
//   if(lux_write_reg(&lux_data_write))
//     {
//       printf("Checking address failed where written", errno, Log_Native);
//       return 1;
//     }
//   //reading value from the register written above
//   if(lux_read_reg(desired_val,1))
//     {
//       Error_Data(Lux, "Lux Value read failed\n", errno, Log_Native);
//       return 1;
//     }
//       return 0;
// }
