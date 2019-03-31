#include "UnitTest.c"

int pass,fail;
uint8_t array[2]={0};

int main(void)
{
	pass=0;
  fail=0;
  uint8_t variable;

  uint8_t* bufferdesired = &variable; 

  printf("\t\t==================================\n");
  printf("\t\t||   Temperature Unit Testing   ||\n");
  printf("\t\t==================================\n");

  // 
  printf("Error:%d::%s\n",base_reg_write(bufferdesired,BUFF_SIZE),ErrorStates[base_reg_write(bufferdesired,BUFF_SIZE)]);
  if(base_reg_write(bufferdesired,BUFF_SIZE))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",base_reg_read(bufferdesired,BUFF_SIZE),ErrorStates[base_reg_read(bufferdesired,BUFF_SIZE)]);
  if(base_reg_read(bufferdesired,BUFF_SIZE))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_reg_ptr(bufferdesired),ErrorStates[write_reg_ptr(bufferdesired)]);
  if(write_reg_ptr(bufferdesired))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",temp_read_reg(bufferdesired),ErrorStates[temp_read_reg(bufferdesired)]);
  if(temp_read_reg(bufferdesired))fail+=1;
  else pass+=1; //THigh_register, final_threshold_high

  uint16_t final_threshold_high = Threshold_upper_Limit << 8;
  printf("Error:%d::%s\n",main_write_register(THigh_register, final_threshold_high),ErrorStates[main_write_register(THigh_register, final_threshold_high)]);
  if(main_write_register(THigh_register, final_threshold_high))fail+=1;
  else pass+=1; //main_read_registe

  static uint8_t Register_buffer_storage[2];
  printf("Error:%d::%s\n",main_read_register(THigh_register, &Register_buffer_storage[0]),ErrorStates[main_read_register(THigh_register, &Register_buffer_storage[0])]);
  if(main_read_register(THigh_register,&Register_buffer_storage[0]))fail+=1;
  else pass+=1;

  //uint16_t final_threshold_high = Threshold_upper_Limit << 8;
  printf("Error:%d::%s\n",all_registers_check(),ErrorStates[all_registers_check()]);
  if(all_registers_check())fail+=1;
  else pass+=1; //final_read_temperature

  printf("Error:%d::%s\n",final_read_temperature(celsius,0x55,0x51,85.312500),ErrorStates[final_read_temperature(celsius,0x55,0x51,85.31250)]);
  if(final_read_temperature(celsius,0x55,0x51,85.31250))fail+=1;
  else pass+=1;

  /*** CASES WITH ERROR ***/

  printf("Error:%d::%s\n",base_reg_write(NULL,BUFF_SIZE),ErrorStates[base_reg_write(NULL,BUFF_SIZE)]);
  if(base_reg_write(NULL,BUFF_SIZE))pass+=1;
  else fail+=1;

  printf("Error:%d::%s\n",base_reg_read(NULL,BUFF_SIZE),ErrorStates[base_reg_read(NULL,BUFF_SIZE)]);
  if(base_reg_read(NULL,BUFF_SIZE))pass+=1;
  else fail+=1;

  printf("Error:%d::%s\n",write_reg_ptr(NULL),ErrorStates[write_reg_ptr(NULL)]);
  if(write_reg_ptr(NULL))pass+=1;
  else fail+=1;

  printf("Error:%d::%s\n",temp_read_reg(NULL),ErrorStates[temp_read_reg(NULL)]);
  if(temp_read_reg(NULL))pass+=1;
  else fail+=1;

 // final_threshold_high = 1 << 8;
  printf("Error:%d::%s\n",main_write_register(0x45, final_threshold_high),ErrorStates[main_write_register(0x45, final_threshold_high)]);
  if(main_write_register(0x45, final_threshold_high))pass+=1;
  else fail+=1;

  printf("Error:%d::%s\n",final_read_temperature(4,0x55,0x51,85.312500),ErrorStates[final_read_temperature(4,0x55,0x51,85.31250)]);
  if(final_read_temperature(4,0x55,0x51,85.31250))pass+=1;
  else fail+=1;

  printf("Error:%d::%s\n",final_read_temperature(celsius,0x55,0x51,30.0),ErrorStates[final_read_temperature(celsius,0x55,0x51,30.0)]);
  if(final_read_temperature(celsius,0x55,0x51,30.0))pass+=1;
  else fail+=1;

  printf("-------------------------------------------------------------------------\n");
  printf("Total Test Cases:  %d || Passed Test Cases:  %d || Failed Test Cases:  %d\n",(pass+fail),pass,fail);
  printf("-------------------------------------------------------------------------\n");


  return 0;
}