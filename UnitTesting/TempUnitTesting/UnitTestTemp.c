//Include temperature task header file from Include Folder
#include "temp_task.h"


char convert_data[2] = {0};
uint8_t default_config_byte_one = 0x60;
uint8_t default_config_byte_two = 0xA0;
int buffer_value;

void write_register(uint8_t value)
{
  ret_write_register = write(buffer_value, &value, sizeof(value));
//  printf("\nValue of ret_write_register: %d",ret_write_register);
  if(ret_write_register <0)
  {
    //check if valid value
    printf("\nError is there\n");
  }

}

int write_lower_register(int reg, uint16_t value, int buffregval)
{
  write_register(reg);
  uint8_t fd = value >>8;
  int value1 = value & 0x00FF;
  uint8_t bufftemp[3] = {0x02, fd, value1};
  int bufftemp1 = write(buffer_value, &bufftemp, sizeof(bufftemp));
  if ((bufftemp1) < 0)
  {
    printf("\nError is there\n");
    return -1;
  }
  return 0;
}

uint16_t read_lower_register(int reg)
{
  uint16_t value;
  uint8_t v[2]={0};
  write_register(reg);
  int temp1 = read(buffer_value,v,sizeof(v));
  if (temp1 <0)
  {
    printf("\nError is there\n");
    return -1;
  }
  printf("T-low register value is: %x %x\n", v[0], v[1]);
  return value;
}

//
// int write_high_register(int reg, uint16_t value )
// {
//   write_register(reg);
//   uint8_t fd = value >>8;
//   int value1 = value & 0x00FF;
//   uint8_t bufftemp[3] = {0x01, fd, value1};
//   int bufftemp1 = write(buffer_value, &bufftemp, sizeof(bufftemp));
//   if ((bufftemp1) < 0)
//   {
//     printf("\nwrite Error is there\n");
//     return -1;
//   }
//   return 0;
// }

uint16_t read_high_register(int reg)
{
  uint16_t value;
  uint8_t v[2]={0};
  write_register(reg);
  int temp1 = read(buffer_value,v,sizeof(v));
  if (temp1 <0)
  {
    printf("\nError is there\n");
    return -1;
  }
  printf("T-High register value is: %x %x\n", v[0], v[1]);
  return value;
}

int write_config_reg_on_off(uint8_t value )
{
  int fd1;
  write_register(1);
  if(value == 0)
  {
    int a=write(fd1, &default_config_byte_one, 1);
    if(a<0)
    {
      printf("\nError is there\n");
      return -1;
    }
  }
  else if(value == 1)
  {
    printf("\n Inside the loop of value 1\n");
  //  default_config_byte_one = 0x60;
  //  printf("\n Default config 1 value is first time%x ",default_config_byte_one);
    uint8_t y = (default_config_byte_one | 1);
    //uint8_t tmp = 0x61;
  //  printf("\n %x ",y);
    uint8_t bufftemp4[3] = {0x01,0xA0,0x61};

    write(fd1, &bufftemp4,3);
    // if(b==-5)
    // {
    //   printf("\nError is there\n");
    //   return -1;
    // }
  }
  else
  {
      printf("\nError is there due to wrong input\n");
      return -1;
  }



return 0;
}
int write_config_reg_on_off1(uint16_t value )
{
  write_register(0x01);
  uint8_t fd = value >>8;
  int value1 = value & 0x00FF;
  uint8_t bufftemp[3] = {0x01, fd, value1};
  int bufftemp1 = write(buffer_value, &bufftemp, sizeof(bufftemp));
  if ((bufftemp1) < 0)
  {
    printf("\nError is there\n");
    return -1;
  }
  return 0;
}

uint16_t read_high_register_onoff()
{
  uint16_t value;
  uint8_t v[2]={0};
  write_register(0x01);
  uint16_t raj = 0;int fd6;
  int temp1 = read(fd6,&raj,sizeof(raj));

  printf("T-High register value is: %x \n", raj);
  return value;
}


int write_high_register(int reg, uint16_t value )
{
  write_register(reg);
  uint8_t fd6 = value >>8;
  int value1 = value & 0x00FF;
  uint8_t bufftemp[3] = {0x01, fd6, value1};
  int bufftemp1 = write(buffer_value, &bufftemp, sizeof(bufftemp));
  if ((bufftemp1) < 0)
  {
    printf("\nwrite Error is there\n");
    return -1;
  }
  return 0;
}


float final_read_temperature(int temperature_unit)
{

  write_register(0);
  int temp2 = read(buffer_value,&convert_data,sizeof(convert_data));
  if (temp2 < 0)
  {
    printf("Error is there  : %s\n", strerror(errno));
    return -300;
  }
  else
  {

    temp3 = ((convert_data[0] << 8) | convert_data[1]) >> 4;

    if(temperature_unit == celsius)
    {
      printf("Temperature in Celsius is %f \n",temp3*0.0625);
    }
    else if(temperature_unit == kelvin)
    {
      printf("Temperature in Kelvin is %f \n",(1.8 * (temp3*0.0625)) + 32);
    }
    else if(temperature_unit == fehrenheit)
    {
      printf("Temperature in Fehrenheit is %f \n", temp3*0.0625+ 273.15);
    }
  }
}

int temp_sensor_init()
{
//  //Open the file system call
  buffer_value = open(bus, O_RDWR);
  //check if error is there or not in opening a file
  if(buffer_value < 0)
  {
    printf("\nError is there in opening buffer_value\n");
    return -1;
  }
  //pushing
  ret_ioctl = ioctl(buffer_value, I2C_SLAVE, addrabc);
  //check if return of ioctl is valid or not
  if (ret_ioctl< 0)
  {
    printf("\nError is there\n");
    return -1;
  }

 }

int main()
{
//  write_high_register(0x02,0x1234);
  //int val=read_high_register(0x02);
//  write_config_reg_on_off(1);
  //write_config_reg_on_off1(1);
   temp_sensor_init();
   //write_config_reg_on_off1(0xA061);
   //read_high_register_onoff();
//   write_high_register(0x01,0xA061);
//
  //read_high_register(0x01);
//write_config_reg(0xA061);
//read_config_reg();
 final_read_temperature(0);
}