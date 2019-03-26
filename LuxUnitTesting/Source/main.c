#include "UnitTestLux.c"

int pass,fail;

int main(void)
{
	pass=0;
	fail=0;
	printf("\t\t==================================\n");
	printf("\t\t||   Lux Meter Unit Testing     ||\n");
	printf("\t\t==================================\n");

//	CASES THAT GIVE IDEAL OUTPUTS 
	if(write_command_register(0x80))fail+=1;
	else pass+=1;
	if(write_control_reg(0,0x80,0x03))fail+=1;
	else pass+=1;
	if(write_timing_register(0,0x81))fail+=1;
	else pass+=1;
	if(write_timing_register_interrupt(0,0x86,0x00))fail+=1;
	else pass+=1;
	if(read_identification_register(0,0x0A))fail+=1;
	else pass+=1;
	if(threshold_write(0, &array,0x82,0x83,0x84,0x85))fail+=1;
	else pass+=1;
	if(threshold_read(0, &array,0x82,0x83,0x84,0x85))fail+=1;
	else pass+=1;
	if(data0_read(0,0x8C,0x8D))fail+=1;
	else pass+=1;
	if(data1_read(0,0x8E,0x8F))fail+=1;
	else pass+=1;
	if(get_lux(0,2.0,0.25))fail+=1;
	else pass+=1;
	if(light_sensor_setup(0x39))fail+=1;
	else pass+=1;
// CASES THAT GIVE ERRORS
	if(write_command_register(0x81))pass+=1;
	else fail+=1;
	if(write_control_reg(0,0x81,0x03))pass+=1;
	else fail+=1;
	if(write_timing_register(0,0x82))pass+=1;
	else fail+=1;
	if(write_timing_register_interrupt(0,0x86,0x02))pass+=1;
	else fail+=1;
	if(read_identification_register(0,0x0B))pass+=1;
	else fail+=1;
	if(threshold_write(0, &array,0x83,0x83,0x84,0x85))pass+=1;
	else fail+=1;
	if(threshold_read(0, &array,0x83,0x83,0x84,0x85))pass+=1;
	else fail+=1;
	if(data0_read(0,0x8C,0x8E))pass+=1;
	else fail+=1;
	if(data1_read(0,0x8F,0x8F))pass+=1;
	else fail+=1;
	if(get_lux(0,-2.0,0.25))pass+=1;
	else fail+=1;
	if(light_sensor_setup(0x30))pass+=1;
	else fail+=1;

	printf("-------------------------------------------------------------------------\n");
	printf("Total Test Cases:  %d || Passed Test Cases:  %d || Failed Test Cases:  %d\n",(pass+fail),pass,fail);
	printf("-------------------------------------------------------------------------\n");

}
