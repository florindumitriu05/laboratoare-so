#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "child") {
        int start, end;
        std::cin >> start >> end;

        std::vector<int> primes;
        for (int n = start; n <= end; ++n) {
            if (isPrime(n)) {
                primes.push_back(n);
            }
        }

        std::cout << primes.size() << "\n";
        for (int p : primes) {
            std::cout << p << " ";
        }
        std::cout << std::endl;

        return 0;
    }

    const int NUM_PROCESSES = 10;
    const int RANGE_SIZE = 1000;

    std::vector<HANDLE> processHandles;
    std::vector<HANDLE> readPipes;

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        HANDLE hReadStdin, hWriteStdin;
        HANDLE hReadStdout, hWriteStdout;
        SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

        if (!CreatePipe(&hReadStdin, &hWriteStdin, &sa, 0)) {
            std::cerr << "Eroare la crearea pipe-ului stdin!\n";
            return 1;
        }
        if (!CreatePipe(&hReadStdout, &hWriteStdout, &sa, 0)) {
            std::cerr << "Eroare la crearea pipe-ului stdout!\n";
            return 1;
        }

        SetHandleInformation(hWriteStdin, HANDLE_FLAG_INHERIT, 0);
        SetHandleInformation(hReadStdout, HANDLE_FLAG_INHERIT, 0);

        std::string cmd = std::string(argv[0]) + " child";
        std::vector<char> cmdBuffer(cmd.begin(), cmd.end());
        cmdBuffer.push_back('\0');

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdInput = hReadStdin;
        si.hStdOutput = hWriteStdout;
        si.hStdError = hWriteStdout;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA(NULL, cmdBuffer.data(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            std::cerr << "Eroare la crearea procesului " << i + 1 << "\n";
            return 1;
        }

        CloseHandle(hReadStdin);
        CloseHandle(hWriteStdout);

        
        int start = i * RANGE_SIZE + 1;
        int end = (i + 1) * RANGE_SIZE;
        if (start == 1) start = 2;

        DWORD written;
        std::string intervalStr = std::to_string(start) + " " + std::to_string(end) + "\n";
        WriteFile(hWriteStdin, intervalStr.c_str(), intervalStr.size(), &written, NULL);
        CloseHandle(hWriteStdin);

        processHandles.push_back(pi.hProcess);
        readPipes.push_back(hReadStdout);
        CloseHandle(pi.hThread);
    }

    WaitForMultipleObjects(processHandles.size(), processHandles.data(), TRUE, INFINITE);

    for (int i = 0; i < NUM_PROCESSES; ++i) {
        char buffer[4096];
        DWORD bytesRead;
        std::string output = "";

        while (ReadFile(readPipes[i], buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            output += buffer;
        }

        size_t newlinePos = output.find('\n');
        int count = 0;
        std::string primesList = "";
        if (newlinePos != std::string::npos) {
            count = std::stoi(output.substr(0, newlinePos));
            primesList = output.substr(newlinePos + 1);
        }

        int start_range = i * RANGE_SIZE + 1;
        int end_range = (i + 1) * RANGE_SIZE;
        if (start_range == 1) start_range = 2;

        std::cout << "Procesul " << i + 1 << " (interval " << start_range << " - " << end_range << "):\n  -> ";
        if (count == 0) {
            std::cout << "Niciun număr prim găsit.";
        }
        else {
            std::cout << primesList;
        }
        std::cout << "\n\n";

        CloseHandle(readPipes[i]);
        CloseHandle(processHandles[i]);
    }

    return 0;
}