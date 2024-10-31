// Annika Morgan
// Computer Org
// 10/29/2024
// Consumer

#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

void PrintSemaphoreValue(std::string nameMem, sem_t semaphore, int &value);
int main()
{
    const charnameMem = "Shared Memory";
    const char semaFull = "Full";
    const charavailable = "Available"; //for semaphores open functions
    const char mutexSem = "Mutex";

    const int SIZE = 2; // size of buffer

    sem_tfill, ready,mutex; // semaphore pointers
    int shared_memory_file_descriptor; //shared mem file discriptor
    int buffer; //pointer to the shared mem buffer
    int loop_count = 5; //number of iterations 

    // Make shared process
    shared_memory_file_descriptor = shm_open(nameMem, O_CREAT | O_RDWR, 0666);

    // Set the size of the mem
    ftruncate(shared_memory_file_descriptor, sizeof(int));

    //map shared mem
    buffer = (int)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_file_descriptor, 0);

    //open the semaphores
    fill = sem_open(semaFull, O_RDWR);

    ready = sem_open(available, O_RDWR);

    mutex = sem_open(mutexSem, O_RDWR);

    //variable that holds semaphore value
    int value = -1;
    while(loop_count--){
        sem_wait(fill);         //wait until buffer is not empty
        sem_wait(mutex);        //wait for exclusive access to buffer

        (* buffer)--;           //consume
        sem_post(mutex);        //release access
        std::cout << "consumed an item\n"; //take item out
        sem_post(ready); //post that semephore is now open
    }



    // Close and unlink semaphores Remove Shared mem
    sem_close(fill);
    sem_close(ready);
    sem_close(mutex);
    sem_unlink(semaFull); 
    sem_unlink(available);
    sem_unlink(mutexSem);

    /* close and unlink shared memory*/
    munmap(buffer, sizeof(int));
    close(shared_memory_file_descriptor);
    shm_unlink(nameMem);

    return 0;
}


// function to print the semaphore value
void PrintSemaphoreValue(std::string nameMem, sem_tsemaphore, int &value) {
    std::cout << nameMem << " value: " << sem_getvalue(semaphore, &value) << std::endl;
}
