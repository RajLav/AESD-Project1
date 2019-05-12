**#AESD Project 1   
#Author : Raj Lavingia    
#Term : Spring 19**  

**Project Description :**  

In this project, we used 2 sensors, Temperature Sensor and Lux Sensor and interfaced them with Beagle Bone Green.  
The sensors were interfaced through I2C mechanism.  
Both the sensors were running at the same time and on the same I2C bus.  

**Startup Test**  
Initially both the sensors are checked and if either of the sensors don't work then the startup test fails and then the tasks are not spawned further.

**Tasks :**  
There are total 4 tasks in this code.  
1) Temperature Task  
2) Lux Task  
3) Log Task  
4) Socket Task  

The data from all the tasks were given to their individual Queue and those Queues are talking to the log queue in order to log the messages in the given text file. Specific mutex locks and unlocks are kept to avoid bus contention between the 2 sensors.  

The program gives a "Alive" message every 10 seconds in order to tell that both the sensors are working perfectly. If any one of the sensors are removed or non functional in the middle then the other sensor keeps on working and the "Not alive" message gets displayed for the specific sensor which is removed. All these is getting logged into a log file.
All the registers were written and read back of both the sensors and verified if they are working correctly.

Whenever there is a request from the external client, ther server responds accordingly. Let's say the client asks for a temperature, then at that particular time the server talks with the temperature or lux queue and then the data which the server obtains from them is being transferred to the socket queue and the client gets its data. It also sends if it's a day time or night time or if the temperture is cold or hot according to the threshold values which are set by the user.  

When an external kill signal comes from USR1 or USR2 then all the threads gets terminated 1 by 1 and then at the end the main threads exits the program gracefully. PID is used in order to kill the runnning thread.

**Development Board :**  
Beagle Bone Green

**Language Used :**  
C Language 

**Platform**  
Linux

**Concepts Leaned**  
#Multi Threading  
#P thread create  
#Interfacing 2 sensors on single I2C  
#Timer Handling  
#External Request Handle  
#Fail Safe Mechanisms  






