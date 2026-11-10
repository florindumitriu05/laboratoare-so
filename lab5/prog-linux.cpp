#include <iostream>
#include <unistd.h>
#include <vector>

bool isPrime(int n) {
    if (n < 2)
        return false;

    for (int d = 2; d*d <= n; d++)
        if (n%d == 0)
            return false;

    return true;
}

int main() {
    const int nProc = 10;
    const int nPrimes = 10000;

    pid_t childPids[nProc];
    int childPipes[nProc][2];

    for (int i = 0; i < nProc; i++) {
        if (pipe(childPipes[i]) != 0) {
            std::cerr << "Could not create child pipe!\n";
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nProc; i++) {
        pid_t forkPid = fork();
        if (forkPid > 0) {
            // Parent
            childPids[i] = forkPid;

            int a = nPrimes/nProc*i,
                b = nPrimes/nProc*(i+1);
            write(childPipes[i][1], &a, sizeof(a));
            write(childPipes[i][1], &b, sizeof(b));
        } else if (forkPid == 0) {
            // Child
            int a, b;
            read(childPipes[i][0], &a, sizeof(a));
            read(childPipes[i][0], &b, sizeof(b));
            close(childPipes[i][0]);

            std::vector<int> primes;
            for (int j = a; j <= b; j++)
                if (isPrime(j))
                    primes.push_back(j);

            int count = primes.size();
            write(childPipes[i][1], &count, sizeof(count));
            write(childPipes[i][1], primes.data(), count*sizeof(int));
            close(childPipes[i][1]);

            exit(EXIT_SUCCESS);
        } else {
            std::cerr << "Could not fork!\n";
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nProc; i++) {
        std::cout << "Procesul " << i+1 << "\n";
        close(childPipes[i][1]);
        int count;
        read(childPipes[i][0], &count, sizeof(count));

        int primes[count];
        read(childPipes[i][0], primes, sizeof(int)*count);
        
        close(childPipes[i][0]);
        for (int prime : primes) {
            std::cout << prime << " ";
        }
        std::cout << "\n";
    }

    return 0;
}