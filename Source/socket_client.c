/*
*@File:socket_client.c
*@Description_File: C File for Light Sensor
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/26/2018
*/

#include "socket_client.h"

//Variables declared
uint8_t Resultant_status = 0;

int flag = 0;

typedef struct
{
  char string_written[150];
  int num;
}my_struct_to_be_passed;



my_struct_to_be_passed data1, data2;
my_struct_to_be_passed *p1 = &data1;
my_struct_to_be_passed *p2 = &data2;

char IP_Addressed_User[30]="10.0.0.227";


void gpio_on_off_client(uint8_t pin,uint8_t state)
{
	FILE *fp1;

	if(pin<4 && state<2)
	{
		fp1 = fopen(LED_Dir_Path[pin],"w");
		if(fp1==NULL) printf("Error in led\n");
		else
		printf("\n Success file open");
	}
	else
	{
		printf("\n Error in pin or state\n");
		return ;
	}

	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%d",pin);
	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%s","out");
	fclose(fp1);

	fp1 = 	fopen(LED_Val_Path[pin],"w");
	if(fp1==NULL) printf("Error in led\n");
	else
	printf("\n Success file open");

	fseek(fp1,0,SEEK_SET);
	fprintf(fp1,"%d",state);

	fclose(fp1);

}



int main(int argc, char *argv[])
{
  printf("[INFO]Socket IPC \n");
  char buff;
  //At the time of socket start ,display the time stamp on local machine
	clock_gettime(CLOCK_REALTIME, &timespec_struct);
  printf("[INFO] You have 4 options: 1) C = Celsius 2) F = Fehrenheit 3) K = Kelvin 4) L=lux \n");
  scanf("%s",&buff);
  if(buff=='C' || buff=='F' | buff=='K' | buff=='L')
  {
  	// if(buff=='C' || buff=='F' | buff=='K')
  	// {
  	// 	Resultant_status = Temperature_Status;
  	// }
  	// else
  	// {
  	// 	Resultant_status = Lux_Status;
  	// }
  	printf("[INFO]Inside loop 2");
	if(argc > 0)
	{
    printf("[INFO]Inside the loop");
    //check for celsius
    printf("[INFO]Success in getting arguments from user\n");
		if(buff=='C')
		{
      printf("[INFO]Now Temperature will be in Celsius\n");
			Resultant_status = Temperature_Status;
			strcpy(p1->string_written, "Temperature");
			  p1->num = 1;
		}
    //check for fehrenheit
		else if(buff=='F')
		{
      printf("[INFO]Now Temperature will be in Fehrenheit\n");
			Resultant_status = Temperature_Status;
			strcpy(p1->string_written, "Temperature");
      p1->num = 2;

		}
    //check for kelvin
		else if(buff=='K')
		{
      printf("[INFO]Now Temperature will be in Kelvin\n");
      Resultant_status = Temperature_Status;

			strcpy(p1->string_written, "Temperature");
      		  p1->num = 3;
		}
		else if(buff=='L')
		{
      printf("[INFO]Request is of Lux measurement\n");
      Resultant_status = Temperature_Status;

			strcpy(p1->string_written, "Lux");
      		  p1->num = 4;
		}

	}
}
	else
	{
	    Resultant_status = Temperature_Status;
	    strcpy(p1->string_written, "[INFO]Temperature");
	    printf("[INFO]By default in Celsius\n");
      		  p1->num = 1;
	}


	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd < 0)
	{
		printf("[ERROR]Socket Creation Failed\n");
    	exit (1);
	}

	struct timeval time1;
	time1.tv_sec = 2;
	time1.tv_usec = 0;
  client.sin_family = AF_INET;
  client.sin_port = htons(PORT);

	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &time1, sizeof(struct timeval)))
	{
		printf("[ERROR]Set sock opt failed\n");
	}

	if(inet_pton(AF_INET, IP_Addressed_User, &client.sin_addr)<=0)
	{
	  printf("[ERROR]Address not supported \n");
	}

	if(connect(socket_fd, (struct sockaddr *)&client, sizeof(client)) < 0)
	{
		printf("[ERROR]Socket Connection Failed\n");
	}

	gettimeofday(&current_time, NULL);
	printf("[INFO]Time Value is %lu %lu\n", current_time.tv_sec, current_time.tv_usec);
  	MEASURED_OUT = write(socket_fd,p1,sizeof(my_struct_to_be_passed));

	if (MEASURED_OUT < 0)
	{
		printf("[ERROR]write failed\n");
    exit (1);
	}

	gettimeofday(&current_time, NULL);
	MEASURED_IN = read(socket_fd,p2,sizeof(my_struct_to_be_passed));
	if(MEASURED_IN < 0)
	{
		printf("[ERROR]Read Failed\n");
		exit (1);
	}

	else if(MEASURED_IN == 0)
  {
    printf("[ERROR]Read Failed 2\n");
  }

  else if(MEASURED_IN == sizeof(my_struct_to_be_passed))
	{
		if(Resultant_status == Temperature_Status)
		{
		      printf("[INFO]value of P2 is %d",p2->num);
			if(p2->num == Temperature_OK)
			{

			printf("[ERROR]Temperature is OK\n");
			}
			else if(p2->num == Temperature_HIGH)
			{
			//	gpio_on_off_client(gpio56,1);
				printf("[ERROR]It is hot\n");
			}
			else if(p2->num == Temperature_LOW)
			{
				//gpio_on_off_client(gpio56,1);
				printf("[ERROR]It's too cold\n");
			}
			else
			{
			printf("[ERROR]Temperature state couldn't be determined\n")	;
			}
		}
		else if(Resultant_status == Lux_Status)
		{
				printf("[INFO]p2 is %d",p2->num);
				if(p2->num == Day_Time)
				{
				printf("[INFO]Daytime\n");
				}
				else if(p2->num == Night_Time)
				{
				printf("[INFO]Nighttime\n");
				}
				else
				{
				printf("[ERROR]Could not be determined\n");
				}
		}
		else
    {
      printf("[ERROR]Nothing could be determined\n");
    }
	}

	close(socket_fd);
	return 0;
}
