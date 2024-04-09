#include <iostream>
#include <windows.h>

int main() {
    DWORD targetProcessId = 10451; // proc id

    // Open the target process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open target process." << std::endl;
        return 1;
    }

    // Get the address of the exported function in the injected DLL
    HMODULE hRemoteDLL = GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_PROCESS, L"RemoteDLL.dll", hProcess);
    if (hRemoteDLL == NULL) {
        std::cerr << "Failed to get handle to RemoteDLL.dll in target process." << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Get the function pointer to RemoteFunction
    FARPROC remoteFunctionAddr = GetProcAddress(hRemoteDLL, "RemoteFunction");
    if (remoteFunctionAddr == NULL) {
        std::cerr << "Failed to get address of RemoteFunction in RemoteDLL.dll." << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Create a remote thread in the target process to call RemoteFunction
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remoteFunctionAddr, NULL, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Failed to create remote thread." << std::endl;
        CloseHandle(hRemoteDLL);
        CloseHandle(hProcess);
        return 1;
    }

    // Wait for the remote thread to finish
    WaitForSingleObject(hThread, INFINITE);

    // Clean up
    CloseHandle(hThread);
    CloseHandle(hRemoteDLL);
    CloseHandle(hProcess);

    return 0;
}
