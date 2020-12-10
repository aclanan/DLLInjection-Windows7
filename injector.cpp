#include <Windows.h>
#include <iostream>

int main(int argc, char* argv[]) {
	HANDLE processHandle;
	PVOID remoteBuffer;
	wchar_t dllPath[] = TEXT("C:\\Users\\User\\Desktop\\badfolder\\messagebox_x64.dll");

	printf("Injecting DLL to PID: %i\n", atoi(argv[1]));
	processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(atoi(argv[1])));
	remoteBuffer = VirtualAllocEx(processHandle, NULL, sizeof (dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
	BOOL wpm = WriteProcessMemory(processHandle, remoteBuffer, (LPVOID)dllPath, sizeof (dllPath) + 1, NULL);

	if (!wpm) {
		std::cout << "WriteProcessMemoryFailed" << wpm <<std::endl; 
		exit; 
	}
	

	PTHREAD_START_ROUTINE threatStartRoutineAddress = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	
	HANDLE hloadThread = CreateRemoteThread(processHandle, NULL, 0, threatStartRoutineAddress, remoteBuffer, 0, NULL);
	//RtlCreateUserThread(processHandle,NULL,false,0,0,0,threatStartRoutineAddress,remoteBuffer,NULL,NULL);
	//NtCreateThreadEx();
	
	WaitForSingleObject(hloadThread, INFINITE);

	std::cout << "DLL Path loacted at:" << std::hex << remoteBuffer << std::endl; 

	//std::cin.get();

	VirtualFreeEx(processHandle,remoteBuffer,sizeof(dllPath) + 1 ,MEM_RELEASE);
	std::cout << "Done" << std::endl; 
 
	return 0;
}


