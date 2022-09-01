#include <Windows.h>
#include <stdio.h>

typedef NTSTATUS(__stdcall *Rtl_AdjustPrivilege)(DWORD, BOOL, INT, PBOOL);
Rtl_AdjustPrivilege RtlAdjustPrivilege = nullptr;

enum Privilege : int
{
    SeDebugPrivilege = 20,
};

typedef NTSTATUS(NTAPI* Nt_SetInformationProcess)(HANDLE, ULONG, void*, ULONG);
Nt_SetInformationProcess NtSetInformationProcess = nullptr;

enum Process : int
{
    ProcessBreakOnTermination = 29,
};

void GetFunctions() {

    HMODULE ntdll = LoadLibraryA("ntdll.dll");
    RtlAdjustPrivilege = (Rtl_AdjustPrivilege)GetProcAddress(ntdll, "RtlAdjustPrivilege");
    NtSetInformationProcess = (Nt_SetInformationProcess)GetProcAddress(ntdll, "NtSetInformationProcess");
    if (!RtlAdjustPrivilege || !NtSetInformationProcess)
    {
        printf("Error: Failed to get imports from ntdll\n");
        Sleep(-1);
    }
}

ULONG ProcessInformation = 1;
BOOL FalsePtr = 0;

int main() 
{
    GetFunctions();

    RtlAdjustPrivilege(SeDebugPrivilege, 1, 0, &FalsePtr);
    NTSTATUS status = NtSetInformationProcess(GetCurrentProcess(), ProcessBreakOnTermination, &ProcessInformation, sizeof(ProcessInformation));

    if (status != 0)
        printf("Error: %#x\n", status);


    printf("Press any key to BSOD!\n");
    getchar();

    return 0;
}