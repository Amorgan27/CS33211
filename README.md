# CS33211


My name is Annika Morgan is this a program that uses a producer and a consumer with a shared buffer. This program also uses semaphores to get into the critical section.

This code contains two files in the C++ programming language. The producer.cpp contains the code for the producer and the the second file consumer.cpp contains the code for the consumer.

To run the program you need to comiple it using three commands
$ gcc producer.c -pthread -lrt -o producer
$ gcc consumer.c -pthread -lrt -o consumer
$ ./producer & ./consumer &

My problems:
I was having trouble figuring out the shared memory and learning how to use semaphores to access the critical section.
