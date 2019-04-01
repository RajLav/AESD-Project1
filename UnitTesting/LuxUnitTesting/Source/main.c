#include "UnitTest.c"

int pass,fail;
uint8_t array[2]={0};
uint8_t r_bytes=0x00;

int main(void)
{
	pass=0;
  fail=0;
  uint8_t variable;

  uint8_t* bufferdesired = &variable; 

  printf("\t\t==================================\n");
  printf("\t\t||   Lux Unit Testing           ||\n");
  printf("\t\t==================================\n");

  // 
  printf("Error:%d::%s\n",lux_common_write(bufferdesired,BUFF_SIZE),ErrorStates[lux_common_write(bufferdesired,BUFF_SIZE)]);
  if(lux_common_write(bufferdesired,BUFF_SIZE))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",lux_common_read(bufferdesired,BUFF_SIZE),ErrorStates[lux_common_read(bufferdesired,BUFF_SIZE)]);
  if(lux_common_read(bufferdesired,BUFF_SIZE))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_pointer(bufferdesired),ErrorStates[write_pointer(bufferdesired)]);
  if(write_pointer(bufferdesired))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",lux_read_reg(bufferdesired,2),ErrorStates[lux_read_reg(bufferdesired,2)]);
  if(lux_read_reg(bufferdesired,2))fail+=1;
  else pass+=1; //THigh_register, final_threshold_high

  printf("Error:%d::%s\n",lux_write_reg1(bufferdesired, r_bytes),ErrorStates[lux_write_reg1(bufferdesired, r_bytes)]);
  if(lux_write_reg1(bufferdesired, r_bytes))fail+=1;
  else pass+=1; //main_read_registe

  printf("Error:%d::%s\n",lux_write_reg(bufferdesired),ErrorStates[lux_write_reg(bufferdesired)]);
  if(lux_write_reg(bufferdesired))fail+=1;
  else pass+=1; //main_read_registe //Word_Data_Register


 //  ** CASES WITH ERROR **

 // 
  printf("Error:%d::%s\n",lux_common_write(NULL,BUFF_SIZE),ErrorStates[lux_common_write(NULL,BUFF_SIZE)]);
  if(lux_common_write(NULL,BUFF_SIZE))pass+=1;
  else fail+=1;

  printf("Error:%d::%s\n",lux_common_read(NULL,BUFF_SIZE),ErrorStates[lux_common_read(NULL,BUFF_SIZE)]);
  if(lux_common_read(NULL,BUFF_SIZE))pass+=1;
  else fail+=1;

  printf("Error:%d::%s\n",write_pointer(NULL),ErrorStates[write_pointer(NULL)]);
  if(write_pointer(NULL))pass+=1;
  else fail+=1;

  printf("Error:%d::%s\n",lux_read_reg(NULL,2),ErrorStates[lux_read_reg(NULL,2)]);
  if(lux_read_reg(NULL,2))pass+=1;
  else fail+=1;

  printf("Error:%d::%s\n",lux_write_reg1(NULL, r_bytes),ErrorStates[lux_write_reg1(NULL, r_bytes)]);
  if(lux_write_reg1(NULL, r_bytes))pass+=1;
  else fail+=1;

  printf("Error:%d::%s\n",lux_write_reg(NULL),ErrorStates[lux_write_reg(NULL)]);
  if(lux_write_reg(NULL))pass+=1;
  else fail+=1;//Word_Data_Register


  printf("-------------------------------------------------------------------------\n");
  printf("Total Test Cases:  %d || Passed Test Cases:  %d || Failed Test Cases:  %d\n",(pass+fail),pass,fail);
  printf("-------------------------------------------------------------------------\n");


  return 0;
}