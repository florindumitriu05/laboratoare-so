#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    std::cout << "Procesul principal porneste cele 3 procese...\n\n";

    std::vector<pid_t> pids;

    for (int i = 3; i >= 1; --i) {
        pid_t pid = fork();
        
        if (pid < 0) {
            std::cerr << "Eroare la fork!\n";
            return 1;
        } else if (pid == 0) {
            // Child
            std::string arg = std::to_string(i);

            std::cout << "Procesul " << i << " a inceput\n";
            sleep(i);
            std::cout << "Procesul " << i << " a terminat\n";
            
            exit(EXIT_SUCCESS);
        } else {
            // Parent
            pids.push_back(pid);
        }
    }

    for (pid_t pid : pids) {
        int status;
        waitpid(pid, &status, 0);
    }

    std::cout << "\nToate procesele s-au terminat!";
    return 0;
}