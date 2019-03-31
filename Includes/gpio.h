#include "main.h"

typedef enum{gpio53=0,gpio54=1,gpio55=2,gpio56=3}PINS;

char* LED_Dir_Path[]={"/sys/class/gpio/gpio53/direction",
					  "/sys/class/gpio/gpio54/direction",
					  "/sys/class/gpio/gpio55/direction",
					  "/sys/class/gpio/gpio56/direction"};

char* LED_Val_Path[]={"/sys/class/gpio/gpio53/value",
					  "/sys/class/gpio/gpio54/value",
					  "/sys/class/gpio/gpio55/value",
					  "/sys/class/gpio/gpio56/value"};