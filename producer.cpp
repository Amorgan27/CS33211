// Annika Morgan
// Computer Org
// 10/29/2024
// producer

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/mman.h>
#include "shared.hpp"

int main(int argc, char* argv[]){

    // create/open the shared memory 
    int shmShared = shm_open(shmPath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shmShared == -1) {
        std::cerr << "Error creating shared memory: " << strerror(errno) << std::endl;
        return 1;
    }

    //set the size of the shared memory
    ftruncate(shmShared, sizeof(sharedData));

    // map the shared memory
    sharedData *producer = static_cast<sharedData*>(mmap(nullptr, sizeof(sharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shmShared, 0));
    if (producer == MAP_FAILED) {
        std::cerr << "Error mapping shared memory: " << strerror(errno) << std::endl;
        close(shmShared);
        return 1;
    }

    // initalizing
    producer->in = 0;
    producer->out = 0;

    // unlink any existing semaphores
    sem_unlink("/empty_semaphore");
    sem_unlink("/full_semaphore");
    sem_unlink("/mutex_semaphore");
   

    // create and initialize the semaphores
    producer->empty = sem_open("/empty_semaphore", O_CREAT, 0666, maxItems);
    producer->full = sem_open("/full_semaphore", O_CREAT, 0666, 0);
    producer->mutex = sem_open("/mutex_semaphore", O_CREAT, 0666, 1);


    if (producer->empty == SEM_FAILED || producer->full == SEM_FAILED) {
        std::cerr << "Error initializing semaphores: " << strerror(errno) << std::endl;
        munmap(producer, sizeof(sharedData));
        close(shmShared);
        return 1;
    }

    // producing items
    for (int i = 0; i < 2; ++i) {
        sleep(1);  // waiting to simulate work
        sem_wait(producer->empty); // wait until there is an empty slot
        sem_wait(producer->mutex); // lock the critical section
    
        //update the 'in' index and produce an item
        producer->in = (producer->in + 1) % maxItems; 
        std::cout << " " << std::endl;
        std::cout << "Produced : " << i << std::endl;


        sem_post(producer->mutex); // unlocking the critical section
        sem_post(producer->full);  // signal that new item is produced

    }

    // cleanup
    sem_unlink("/empty_semaphore");
    sem_unlink("/full_semaphore");
    sem_unlink("/mutex_semaphore");
    shm_unlink(shmPath);
    sem_close(producer->empty);
    sem_close(producer->full);
    sem_close(producer->mutex);

    munmap(producer, sizeof(sharedData));
    close(shmShared);

    return 0;
}
