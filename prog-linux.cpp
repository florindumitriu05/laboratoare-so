#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sys/mman.h> // shm_open
#include <sys/wait.h>
#include <fcntl.h> // pentru constante O_*
#include <unistd.h> // ftruncate
#include <semaphore.h>

#define SHM_NAME "/counter_shm"

struct shared_data_t {
    int counter;
    sem_t semaphore;
};

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1) {
        std::cerr << "Eroare la crearea memoriei comune\n";
        exit(EXIT_FAILURE);
    }
    
    shared_data_t* shared_data;

    if (ftruncate(shm_fd, sizeof(*shared_data)) == -1)  {
        std::cerr << "Eroare la setarea marimii memoriei comune\n";
        exit(EXIT_FAILURE);
    }

    shared_data = (shared_data_t*)mmap(NULL, sizeof(*shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);

    if (shared_data == MAP_FAILED) {
        std::cerr << "Eroare la mmap!\n";
        exit(EXIT_FAILURE);
    }

    if (sem_init(&shared_data->semaphore, 1, 1) == -1) {
        std::cerr << "Eroare la sem_init!\n";
        exit(EXIT_FAILURE);
    }

    shared_data->counter = 0;
    
    pid_t childPids[2];
    for (int i = 0; i < 2; i++) {
        pid_t forkPid = fork();
        if (forkPid == 0) {
            // Child
            std::cout << "Child started!\n";
            srand(time(nullptr) ^ getpid());
            bool done = false;

            while (!done) {
                int coin_flip = random()%2;
                int before, after;

                sem_wait(&shared_data->semaphore);
                before = shared_data->counter;

                if (shared_data->counter < 1000) {
                    shared_data->counter += coin_flip;
                }
                else {
                    done = true;
                }
                
                after = shared_data->counter;
                sem_post(&shared_data->semaphore);
                printf("Child(%d) updates counter: %d -> %d\n", getpid()%2, before, after);
            }

            exit(EXIT_SUCCESS);
        } else if (forkPid > 0){
            childPids[i] = forkPid;
        } else {
            std::cerr << "Eroare la fork!\n";
            exit(EXIT_FAILURE);
        }
    }

    waitpid(childPids[0], nullptr, 0);
    waitpid(childPids[1], nullptr, 0);

    std::cout << "Final counter value: " << shared_data->counter << std::endl;

    sem_destroy(&shared_data->semaphore);
    munmap(shared_data, sizeof(*shared_data));
    shm_unlink(SHM_NAME);
    return 0;
}