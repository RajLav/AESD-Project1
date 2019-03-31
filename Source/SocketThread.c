/*
*@File:SocketThread.c
*@Description_File: C file for Socket Server
*@Author:Raj Lavingia and Yash Gupte
*@Date: 03/29/2018
*/
#include "SocketThread.h"

uint8_t Server_initialisation(void)
{
		char Message_Text[60];
		sprintf(Message_Text, "Socket Server Created with TID: %ld", syscall(SYS_gettid));
		Sent_Queue(Socket, Logging, "[INFO_SERVER]", Message_Text);
		sprintf(Message_Text, "Socket Server Created with PID: %ld", syscall(SYS_getpid));
		Sent_Queue(Socket, Logging, "[INFO_SERVER", Message_Text);
		struct sockaddr_in custom_server;

		//Socket Dexcriptor
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if(socket_fd < 0)
		{
			printf("Socket Creation Failed\n");
			exit (1);
		}

		//ports defined
		custom_server.sin_family = AF_INET;
		custom_server.sin_addr.s_addr = INADDR_ANY;
		custom_server.sin_port = htons(PORT);

		//Binding successful or not
		if(bind(socket_fd, (struct sockaddr *)&custom_server, sizeof(custom_server)) < 0)
		{
			printf("Binding Failed\n");
			exit (1);
		}

		//Listen to multiple clients at the same time
		if(listen(socket_fd, 6) < 0)
		{
			printf("Listening Failed\n");
			exit (1);
		}
		return 0;
}
