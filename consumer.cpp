// Annika Morgan
// Computer Org
// 10/29/2024
// Consumer

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/mman.h>
#include "shared.hpp"

int main(int argc, char *argv[]) {
    sleep(1); // make sure the producer runs first by sleeping for one second

    //open the shared memory
    int shmShared = shm_open(shmPath, O_RDWR, S_IRUSR | S_IWUSR);
    if(shmShared == -1) {
        std::cerr << "Waiting for shared memory to be created..." << std::endl;
        sleep(1); // wait again before retrying
    }

    //set size of mem
    ftruncate(shmShared, sizeof(int));

    // map the shared mem to the address space of the process
    sharedData *consumer = (sharedData*)(mmap(nullptr, sizeof(sharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shmShared, 0));
    if (consumer == MAP_FAILED) {
        std::cerr << "ERROR IN MAPPING SHARED MEMORY: " << strerror(errno) << std::endl;
        close(shmShared);
        return 1;
    }

    // initializing semaphores and variables for buffer.
    consumer->empty = sem_open("/empty_semaphore", 0);
    consumer->full = sem_open("/full_semaphore", 0);

    if (consumer->empty == SEM_FAILED || consumer->full == SEM_FAILED) {
        std::cerr << "Error opening semaphores: " << strerror(errno) << std::endl;
        munmap(consumer, sizeof(sharedData));
        close(shmShared);
        return 1;
    }

    // consume a number of items
    for (int i = 0; i < 2; ++i) {
        sleep(1);
        sem_wait(consumer->full);    // wait until the buffer is not empty
        sem_wait(consumer->mutex);  // locking the critical section
        std::cout << " " << std::endl;
        std::cout << "Consumed : " << i << std::endl;

        consumer->out = (consumer->out + 1) % maxItems;  // update index
        sem_post(consumer->empty); // signal empty slot
        sem_post(consumer->mutex); // unlock the critical section
    }

    // unmap the shared memory
    if (munmap(consumer, sizeof(sharedData)) == -1) {
         std::cerr << "Error unmapping shared memory: " << strerror(errno) << std::endl;
    }

    // cleanup
    close(shmShared);    //close the shared memory object
    shm_unlink(shmPath);    // Remove the shared memory object
    return 0;
}
