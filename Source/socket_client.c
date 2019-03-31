#include "socket_client.h"


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

int main(int argc, char *argv[])
{
  printf("Socket IPC \n");
  char buff;
  //At the time of socket start ,display the time stamp on local machine
	clock_gettime(CLOCK_REALTIME, &timespec_struct);
  printf("\n You have 4 options: 1) C = Celsius 2) F = Fehrenheit 3) K = Kelvin 4) L=lux \n");
  scanf("%s",&buff);
  if(buff=='C' || buff=='F' | buff=='K' | buff=='L')
  {
  printf("\n Inside loop 2");
	if(argc > 0)
	{
    printf("\nInside the loop");
    //check for celsius
    printf("Success in getting arguments from user\n");
		if(buff=='C')
		{
      printf("Now Temperature will be in Celsius\n");
			Resultant_status = Temperature_Status;
			strcpy(p1->string_written, "Temperature");
			 p1->num = 1;
		}
    //check for fehrenheit
		else if(buff=='F')
		{
      printf("Now Temperature will be in Fehrenheit\n");
			Resultant_status = Temperature_Status;
			strcpy(p1->string_written, "Temperature");
			p1->num = 2;
		}
    //check for kelvin
		else if(buff=='K')
		{
      printf("Now Temperature will be in Kelvin\n");
			Resultant_status = Temperature_Status;
			strcpy(p1->string_written, "Temperature");
			p1->num = 3;
		}
		else if(buff=='L')
		{
      printf("Request is of Lux measurement\n");
			Resultant_status = Lux_Status;
			strcpy(p1->string_written, "Lux");
			p1->num = 1;
		}
		
	}
}
	else
	{
    Resultant_status = Temperature_Status;
    strcpy(p1->string_written, "Temperature");
    p1->num = 1;
    printf("By default in Celsius\n");
	}


	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd < 0)
	{
		printf("\nSocket Creation Failed\n");
    exit (1);
	}

	struct timeval time1;
	time1.tv_sec = 2;
	time1.tv_usec = 0;
  client.sin_family = AF_INET;
  client.sin_port = htons(PORT);

	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &time1, sizeof(struct timeval)))
	{
		printf("Set sock opt failed\n");
	}

	if(inet_pton(AF_INET, IP_Addressed_User, &client.sin_addr)<=0)
	{
	  printf("Address not supported \n");
	}

	if(connect(socket_fd, (struct sockaddr *)&client, sizeof(client)) < 0)
	{
		printf("\nSocket Connection Failed\n");
	}

	gettimeofday(&current_time, NULL);
	printf("Time Value is %lu %lu\n", current_time.tv_sec, current_time.tv_usec);
  info_out = write(socket_fd,p1,sizeof(my_struct_to_be_passed));

	if (info_out < 0)
	{
		printf("write failed\n");
    exit (1);
	}

	gettimeofday(&current_time, NULL);
	info_in = read(socket_fd,p2,sizeof(my_struct_to_be_passed));
	if(info_in < 0)
	{
		printf("\nRead Failed\n");
		exit (1);
	}

	else if(info_in == 0)
  {
    printf("Read Failed 2\n");
  }

  else if(info_in == sizeof(my_struct_to_be_passed))
	{
		if(Resultant_status == Temperature_Status)
		{
      printf("value of P2 is %d",p2->num);
			if(p2->num == Temperature_OK)
      {
        printf("\nTemperature is OK\n");
      }
			else if(p2->num == Temperature_HIGH)
      {
        printf("\nIt is hot\n");
      }
			else if(p2->num == Temperature_LOW)
      {
        printf("\nIt's too cold\n");
      }
			else
      {
        printf("\nTemperature state couldn't be determined\n")	;
      }
		}
		else if(Resultant_status == Lux_Status)
		{
      printf("p2 is %d",p2->num);
			if(p2->num == Day_Time)
      {
        printf("\n Daytime\n");
      }
			else if(p2->num == Night_Time)
      {
        printf("\n Nighttime\n");
      }
      else
      {
        printf("Could not be determined\n");
      }
		}
		else
    {
      printf("Nothing could be determined\n");
    }
	}

	close(socket_fd);
	return 0;
}
