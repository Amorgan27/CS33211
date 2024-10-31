// Annika Morgan
// Computer Org
// 10/29/2024
// producer

#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>


int main()
{
    //define shared mem and semaphore names
    const char nameMem = "Shared Memory";
    const charsemaFull = "Full";
    const char available = "Available";
    const charmutexSem = "Mutex";

    const int SIZE = 2; // buffer size

    sem_t fill,ready, mutex;    //semaphore pointers
    int shared_memory_file_descriptor; //File descripter for shared mem
    intbuffer;    //buffer pointer
    int loop_count = 5;     //num of iterations

    // create and open shared mem 
    shared_memory_file_descriptor = shm_open(nameMem, O_CREAT | O_RDWR, 0666);

    // Set the size of the shared mem
    ftruncate(shared_memory_file_descriptor, sizeof(int));

    //map shared mem to address space
    buffer = (int )mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_file_descriptor, 0);

    //open semaphores
    fill = sem_open(semaFull, O_CREAT, 0666, 0);
    ready = sem_open(available, O_CREAT, 0666, 3);
    mutex = sem_open(mutexSem, O_CREAT, 0666, 1);


    std::cout << "\n"  //empty line
              << std::endl;
    while (loop_count--)
    {
        // wait until space is available
        sem_wait(ready);
        sleep(rand()%2+1); //wait to see if  available/wait on the mutex lock 
        sem_wait(mutex);   //wait for exclusive access

        // limit table size
        //check if buffer is not full
        if (buffer < SIZE)
        {
            (buffer)++; //added an item to the buffer
            std::cout << "Produced an item" << std::endl;
            sem_post(mutex);  // release exclusive access
            sem_post(fill); // send that semaphore is open

            //if buffer is full, print a message
            if (buffer == SIZE)
            {
                std::cout << "The producer table is full\n";
            }
        }
    }

    // Close and unlink semaphores
    sem_close(fill);
    sem_close(ready);
    sem_close(mutex);
    sem_unlink(semaFull);
    sem_unlink(available);
    sem_unlink(mutexSem);

    // close and unlink shared memory
    munmap(buffer, sizeof(int));
    close(shared_memory_file_descriptor);
    shm_unlink(nameMem);

    return 0;
}
