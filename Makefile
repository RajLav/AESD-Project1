main: main.c
	arm-linux-gcc -o Final main.c -lpthread -lrt -D_BSD_SOURCE -lm -I.

client: socket_client.c
	gcc -o client socket_client.c -I.

bbg: TempThread.c
	arm-linux-gcc -o temp TempThread.c -lpthread -lrt -D_BSD_SOURCE -lm -I.


bbg2: LuxThread.c
	arm-linux-gcc -o Lux LuxThread.c -lpthread -lrt -D_BSD_SOURCE -lm -I.

