#include "UnitTestTemp.c"

int pass,fail;
uint8_t array[2]={0};

int main(void)
{
	pass=0;
  fail=0;

  printf("\t\t==================================\n");
  printf("\t\t||   Temperature Unit Testing   ||\n");
  printf("\t\t==================================\n");

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(write_lower_register(BUFF_SIZE))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(read_lower_register(array))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(write_high_register(BUFF_SIZE+1))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(read_high_register(array))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(configuration_register_write(BUFF_SIZE-1))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(configuration_register_read(array))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(configuration_register_fault_bits(BUFF_SIZE-1))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(configuration_register_read_faults_bits(array))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(configuration_register_EM_bits(BUFF_SIZE-1,0x0040))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(configuration_register_read_EM(BUFF_SIZE-1,array))fail+=1;
  else pass+=1;

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE),ErrorStates[write_lower_register(BUFF_SIZE)]);
  if(final_read_temperature(0,0x55,0x51,85.312500))fail+=1;
  else pass+=1;

  /*** CASES WITH ERROR ***/

  printf("Error:%d::%s\n",write_lower_register(BUFF_SIZE+1),ErrorStates[write_lower_register(BUFF_SIZE+1)]);
  if(write_lower_register(BUFF_SIZE+1))pass+=1; //Buffer Incorrect Size
  else fail+=1;
  
  printf("Error:%d::%s\n",read_lower_register(NULL),ErrorStates[read_lower_register(NULL)]);
  if(read_lower_register(NULL))pass+=1;         //Array Null Pointer
  else fail+=1;

  printf("Error:%d::%s\n",write_high_register(BUFF_SIZE),ErrorStates[write_high_register(BUFF_SIZE)]);
  if(write_high_register(BUFF_SIZE))pass+=1;  //Buffer Incorrect Size
  else fail+=1;
  
  printf("Error:%d::%s\n",read_high_register(NULL),ErrorStates[read_high_register(NULL)]);
  if(read_high_register(NULL))pass+=1;        //Array Null Pointer
  else fail+=1;

  printf("Error:%d::%s\n",configuration_register_write(BUFF_SIZE-2),ErrorStates[configuration_register_write(BUFF_SIZE-2)]);
  if(configuration_register_write(BUFF_SIZE-2))pass+=1;       //Buffer Incorrect Size
  else fail+=1;

  printf("Error:%d::%s\n",configuration_register_read(NULL),ErrorStates[configuration_register_read(NULL)]);
  if(configuration_register_read(NULL))pass+=1;               //Array Null Pointer
  else fail+=1;

  printf("Error:%d::%s\n",configuration_register_fault_bits(BUFF_SIZE-2),ErrorStates[configuration_register_fault_bits(BUFF_SIZE-2)]);
  if(configuration_register_fault_bits(BUFF_SIZE-2))pass+=1;  //Buffer Incorrect Size
  else fail+=1;

  printf("Error:%d::%s\n",configuration_register_read_faults_bits(NULL),ErrorStates[configuration_register_read_faults_bits(NULL)]);
  if(configuration_register_read_faults_bits(NULL))pass+=1;   //Array Null Pointer
  else fail+=1;

  printf("Error:%d::%s\n",configuration_register_EM_bits(BUFF_SIZE-2,0x0040),ErrorStates[configuration_register_EM_bits(BUFF_SIZE-2,0x0040)]);
  if(configuration_register_EM_bits(BUFF_SIZE-2,0x0040))pass+=1;  //Buffer Incorrect Size
  else fail+=1;

  printf("Error:%d::%s\n",configuration_register_read_EM(BUFF_SIZE-2,NULL),ErrorStates[configuration_register_read_EM(BUFF_SIZE-2,NULL)]);
  if(configuration_register_read_EM(BUFF_SIZE-2,NULL))pass+=1; //Buffer Incorrect Size
  else fail+=1;
  
  printf("Error:%d::%s\n",final_read_temperature(5,0x55,0x51,85.312500),ErrorStates[final_read_temperature(5,0x55,0x51,85.312500)]);
  if(final_read_temperature(5,0x55,0x51,85.312500))pass+=1; //Incorrect Temp Unit
  else fail+=1;

  printf("-------------------------------------------------------------------------\n");
  printf("Total Test Cases:  %d || Passed Test Cases:  %d || Failed Test Cases:  %d\n",(pass+fail),pass,fail);
  printf("-------------------------------------------------------------------------\n");


  return 0;
}