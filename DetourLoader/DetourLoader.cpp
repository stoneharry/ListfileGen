#include <iostream>
#include <Windows.h>
#include <string>
#include <unordered_set>
#include <fstream>
#include <detours/detours.h>
#include <detours/detver.h>

int main()
{
    CHAR szFullExe[MAX_PATH] = "\0";
    CHAR szDllPath[MAX_PATH] = "\0";
    CHAR szCommand[2048];
    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    PCHAR pszFileExe = NULL;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    szCommand[0] = L'\0';

    std::cout << "Input WoW.exe file name:\n";

    std::string fileName;
    std::cin >> fileName;
    if (fileName.length() > 0)
    {
        strcpy_s(szFullExe, fileName.c_str());
    }

    std::cout << "Input DLL file name:\n";
    std::string dllName;
    std::cin >> dllName;
    if (dllName.length() > 0)
    {
        strcpy_s(szDllPath, dllName.c_str());
    }

    std::cout << "Starting " << szFullExe << " injecting " << szDllPath << "\n";

    if (!DetourCreateProcessWithDllExA(szFullExe[0] ? szFullExe : NULL, szCommand,
        NULL, NULL, TRUE, dwFlags, NULL, NULL,
        &si, &pi, szDllPath, NULL)) {

        std::cout << "Failed to attach\n";
        return 9008;
    }
    else
    {
        std::cout << "created process\n";
    }

    ResumeThread(pi.hThread);

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD dwResult = 0;
    if (!GetExitCodeProcess(pi.hProcess, &dwResult)) {
        std::cout << "GetExitCodeProcess failed: " << GetLastError() << "\n";
        return 9008;
    }

    std::cout << "exiting with status " << dwResult << "\n";
}
