//Include temperature task header file from Include Folder
#include "../Includes/UnitTestTemp.h"
#include "../Includes/TempThread.h"
#include "../Includes/main.h"


uint8_t buffer_value;
uint8_t v[2]={0};

uint8_t base_reg_write(uint8_t* bufferedesired,int buffer_bytes) //
{
  if(bufferedesired==NULL)
    return AllocError;
 
  return Success;
}

uint8_t base_reg_read(uint8_t* bufferedesired,int buffer_bytes) //
{
  if(bufferedesired==NULL)
    return AllocError;
  if(sizeof(bufferedesired)<SIZE_BUFF)
    return Error;
  return Success;
}

uint8_t write_reg_ptr(uint8_t* x) //
{
  uint8_t buffer_bytes = 1;
  uint8_t temp = base_reg_write(x,buffer_bytes);
  if(buffer_bytes!=BUFF_SIZE-1)
    return Error;
  return temp;
}


uint8_t temp_read_reg(uint8_t *x) //
{
  uint8_t buffer_bytes = 2 ;
  uint8_t temp = base_reg_read(x,2);
  if(buffer_bytes!=BUFF_SIZE)
    return Error;
  return temp;
}

uint8_t temp_write_reg(uint8_t *x) //
{
  uint8_t buffer_bytes = 3;
  uint8_t temp = base_reg_write(x,buffer_bytes);
  if(buffer_bytes!=BUFF_SIZE+1)
    return Error;
  return temp;
}

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
      return temp;
    }
  temp = (temp_write_reg(&buffer_array));
  if(temp)
    {
      printf("[Error]In writing to temp\n");
      return temp;
    }
    return Success;
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


