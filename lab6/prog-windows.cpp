#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>

struct SharedData {
    int counter;
};

#define SHM_NAME "Local\\MySharedMemoryExample"
#define SEM_NAME "Local\\MySemaphoreExample"

void run_worker(int id) {
    HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, SHM_NAME);
    if (hMapFile == NULL) {
        std::cerr << "Worker " << id << ": Nu am putut deschide memoria!" << std::endl;
        return;
    }

    SharedData* data = (SharedData*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData));

    HANDLE hSem = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SEM_NAME);
    if (hSem == NULL) {
        std::cerr << "Worker " << id << ": Nu am putut deschide semaforul!" << std::endl;
        return;
    }

    srand(GetTickCount() ^ GetCurrentProcessId());
    bool done = false;

    while (!done) {
        int before, after;
        WaitForSingleObject(hSem, INFINITE);

        before = data->counter;

        int coin_flip = rand() % 2;

        if (data->counter < 1000)
            data->counter += coin_flip;
        else
            done = true;

        after = data->counter;
        ReleaseSemaphore(hSem, 1, NULL);
        printf("Child(%d) updates counter: %d -> %d\n", GetCurrentProcessId()%2, before, after);
    }

    UnmapViewOfFile(data);
    CloseHandle(hMapFile);
    CloseHandle(hSem);
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "worker") {
        run_worker(std::stoi(argv[2]));
        return 0;
    }

    std::cout << "--- Controllerul pornește ---" << std::endl;

    HANDLE hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedData), SHM_NAME);
    if (hMapFile == NULL) {
        std::cerr << "Eroare la creare memoria partajată: " << GetLastError() << std::endl;
        return 1;
    }

    SharedData* data = (SharedData*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData));
    data->counter = 0;

    HANDLE hSem = CreateSemaphoreA(NULL, 1, 1, SEM_NAME);
    if (hSem == NULL) {
        std::cerr << "Eroare la creare semafor: " << GetLastError() << std::endl;
        return 1;
    }

    GetModuleFileNameA(NULL, cmdLine, MAX_PATH);

    std::string cmd1 = std::string(cmdLine) + " worker 1";
    std::string cmd2 = std::string(cmdLine) + " worker 2";

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi1, pi2;

    std::cout << "Lansare Proces 1 și Proces 2..." << std::endl;

    if (!CreateProcessA(NULL, (LPSTR)cmd1.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi1)) {
        std::cerr << "Eroare la pornirea procesului 1" << std::endl;
    }
    if (!CreateProcessA(NULL, (LPSTR)cmd2.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi2)) {
        std::cerr << "Eroare la pornirea procesului 2" << std::endl;
    }

    // 4. Așteptăm ca procesele să termine
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    // 5. Curățenie
    std::cout << "\nFinal: Numărul din memorie este: " << data->counter << std::endl;

    UnmapViewOfFile(data);
    CloseHandle(hMapFile);
    CloseHandle(hSem);
    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    std::cout << "Program finalizat." << std::endl;
    return 0;
}

