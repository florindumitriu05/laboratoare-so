#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        int sleepSeconds = std::stoi(argv[1]);
        std::cout << "[Child PID: " << GetCurrentProcessId() << "] Sleeping for " << sleepSeconds << " second(s)...\n";
        
        Sleep(sleepSeconds * 1000);
        
        std::cout << "[Child PID: " << GetCurrentProcessId() << "] Finished and exiting.\n";
        return 0;
    }

    std::cout << "[Parent PID: " << GetCurrentProcessId() << "] Starting 3 child processes...\n";

    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    std::vector<HANDLE> processHandles;

    for (int i = 1; i <= 3; ++i) {
        std::string cmdLine = std::string("\"") + path + "\" " + std::to_string(i);

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::vector<char> cmdBuffer(cmdLine.begin(), cmdLine.end());
        cmdBuffer.push_back('\0');

        if (!CreateProcessA(
            NULL,               // Application name (use command line)
            cmdBuffer.data(),   // Command line arguments
            NULL,               // Process handle not inheritable
            NULL,               // Thread handle not inheritable
            FALSE,              // Set handle inheritance to FALSE
            0,                  // No creation flags
            NULL,               // Use parent's environment block
            NULL,               // Use parent's starting directory
            &si,                // Pointer to STARTUPINFO
            &pi                 // Pointer to PROCESS_INFORMATION
        )) {
            std::cerr << "CreateProcess failed with error code: " << GetLastError() << "\n";
            for (HANDLE h : processHandles) CloseHandle(h);
            return 1;
        }

        CloseHandle(pi.hThread);

        processHandles.push_back(pi.hProcess);
        std::cout << "[Parent] Spawned child " << i << " (PID: " << pi.dwProcessId << ", will sleep " << i << "s)\n";
    }

    std::cout << "\n[Parent] Waiting for all child processes to complete using WaitForMultipleObjects...\n";

    WaitForMultipleObjects(
        static_cast<DWORD>(processHandles.size()), 
        processHandles.data(), 
        TRUE,               
        INFINITE            
    );

    std::cout << "[Parent] All child processes have completed. Cleaning up handles and exiting.\n";

    for (HANDLE h : processHandles) {
        CloseHandle(h);
    }

    return 0;
}
