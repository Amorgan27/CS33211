// Annika Morgan
// Com Org
// 11/1/24
// shared.hpp

#ifndef shared_h
#define shared_h

#include <pthread.h>
#include <semaphore.h>

#define maxItems 2  //define max items in the buffer

// structure to hold the shared variables
struct sharedData{
    sem_t *empty; // semaphore to indicate empty slots in buffer
    sem_t *full; // semaphore to indicate full slots in the buffer
    sem_t *mutex; // semaphore to protect the critical section
    int table[maxItems]; // buffer to hold items
    int in, out; // indices for the producer and consumer to access the buffer
};
// Path for the shared memory 
const char* shmPath = "/unique_Memory";


#endif 
