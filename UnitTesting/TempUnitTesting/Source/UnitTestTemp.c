
//Include temperature task header file from Include Folder
#include "../Includes/UnitTestTemp.h"

int buffer_value;
uint8_t v[2]={0};

int write_lower_register(int BuffSize ) //
{
  if(BuffSize!=BUFF_SIZE)
    return Error;
  return Success;
}

uint16_t read_lower_register( uint8_t *v) //
{
  if(v==NULL)
    return AllocError;
  if(sizeof(v)<SIZE_BUFF)
    return Error;
  return Success;
}

int write_high_register(int BuffSize) //
{
  if(BuffSize<(BUFF_SIZE+1))
    return Error;
  return Success;
}


uint16_t read_high_register(uint8_t *v) //
{
  if(v==NULL)
    return AllocError;
  if(sizeof(v)<SIZE_BUFF)
    return Error;
  return Success;
}


int configuration_register_write(int BuffSize) //
{
   if(BuffSize!=(BUFF_SIZE-1))
    return Error;
  return Success;
}


uint16_t configuration_register_read(uint8_t *v) //
{
  if(v==NULL)
    return AllocError;
  if(sizeof(v)<SIZE_BUFF)
    return Error;
  return Success;
}

int configuration_register_fault_bits(int BuffSize) //
{
  if(BuffSize!=(BUFF_SIZE-1))
    return Error;
  return Success;
}


uint16_t configuration_register_read_faults_bits(uint8_t *v) //
{
  if(v==NULL)
    return AllocError;
  if(sizeof(v)<(BUFF_SIZE-1))
    return Error;
  return Success;
  //return value;
}

int configuration_register_EM_bits(int BuffSize,uint16_t value) //
{
  if(value!=0x0040)
    return Error;
  if(BuffSize!=(BUFF_SIZE-1))
    return Error;
  return Success;
}

uint16_t configuration_register_read_EM(int BuffSize,uint8_t *v) //
{
  if(v==NULL)
    return AllocError;
 if(BuffSize!=(BUFF_SIZE-1))
    return Error;
  if(sizeof(v)<(BUFF_SIZE-1))
    return Error;
  return Success;
}

int final_read_temperature(int temperature_unit,int MSB,int LSB,float calculated)
{
    int c = ((MSB << 8) | LSB) >> 4; 
    float temp;
    switch(temperature_unit)
    {
      case celsius:
        temp = c*0.0625;
        if(temp == calculated)
        return Success;
      return Error;
        break;

      case fehrenheit:
        temp = (1.8 * c) + 32;
        if(temp == calculated)
        return Success;
      return Error;
        break;

      case kelvin:
         temp = c + 273.15;
         if(temp == calculated)
        return Success;
      return Error;
        break;

        default: return TempNotInUnits;
        break;
    }

   
    
}


