#include <Windows.h>
#include <stdio.h>
#include "..\myservice.h"

// forward declaration
VOID  WINAPI MyServiceStart (DWORD argc, LPTSTR *argv); 
VOID  WINAPI MyServiceCtrlHandler (DWORD opcode); 
DWORD MyServiceInitialization (DWORD argc, LPTSTR *argv, DWORD *specificError); 
VOID  SvcDebugOut(LPSTR String, DWORD Status); 
 
void main( ) 
{ 
    SERVICE_TABLE_ENTRY   DispatchTable[] = 
	{ 
		{ MYSERVICE, MyServiceStart },
		{ NULL,      NULL           }
    }; 
 
    if (!StartServiceCtrlDispatcher( DispatchTable)) 
    { 
        SvcDebugOut(" [MY_SERVICE] StartServiceCtrlDispatcher error = %d\n", GetLastError()); 
    } 
} // main()
 
VOID SvcDebugOut(LPSTR String, DWORD Status) 
{ 
    CHAR  Buffer[1024]; 
    if (strlen(String) < 1000) 
    { 
        sprintf(Buffer, String, Status); 
        OutputDebugStringA(Buffer); 
    } 
} 


void WINAPI MyServiceStart (DWORD argc, LPTSTR *argv) 
{ 
    DWORD status; 
    DWORD specificError; 
 
    MyServiceStatus.dwServiceType        = SERVICE_WIN32_OWN_PROCESS; 
    MyServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    MyServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP; 
    MyServiceStatus.dwWin32ExitCode      = 0; 
    MyServiceStatus.dwServiceSpecificExitCode = 0; 
    MyServiceStatus.dwCheckPoint         = 0; 
    MyServiceStatus.dwWaitHint           = 0; 
 
    MyServiceStatusHandle = RegisterServiceCtrlHandler(MYSERVICE,MyServiceCtrlHandler); 
 
    if (MyServiceStatusHandle == (SERVICE_STATUS_HANDLE)0) 
    { 
        SvcDebugOut(" [MY_SERVICE] RegisterServiceCtrlHandler failed %d\n", GetLastError()); 
        return; 
    } 
 
    // Initialization code goes here. 
    status = MyServiceInitialization(argc,argv, &specificError); 
 
    // Handle error condition 
    if (status != NO_ERROR) 
    { 
        MyServiceStatus.dwCurrentState       = SERVICE_STOPPED; 
        MyServiceStatus.dwCheckPoint         = 0; 
        MyServiceStatus.dwWaitHint           = 0; 
        MyServiceStatus.dwWin32ExitCode      = status; 
        MyServiceStatus.dwServiceSpecificExitCode = specificError; 
 
        SetServiceStatus (MyServiceStatusHandle, &MyServiceStatus); 
        return; 
    } 
 
    // Initialization complete - report running status. 
    MyServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
    MyServiceStatus.dwCheckPoint         = 0; 
    MyServiceStatus.dwWaitHint           = 0; 
 
    if (!SetServiceStatus (MyServiceStatusHandle, &MyServiceStatus)) 
    { 
        status = GetLastError(); 
        SvcDebugOut(" [MY_SERVICE] SetServiceStatus error %ld\n",status); 
    } 
 
    // This is where the service does its work.
	while (1) {
		Sleep(60000); // milliseconds
	}
    SvcDebugOut(" [MY_SERVICE] Returning the Main Thread \n",0); 
 
    return; 
} 
 
// Stub initialization function. 
DWORD MyServiceInitialization(DWORD   argc, LPTSTR  *argv, 
    DWORD *specificError) 
{ 
    argv; 
    argc; 
    specificError; 
    return(0); 
} 


VOID WINAPI MyServiceCtrlHandler (DWORD Opcode) 
{ 
    DWORD status; 
 
    switch(Opcode) 
    { 
		
        case SERVICE_CONTROL_PAUSE: 
        // Do whatever it takes to pause here. 
            MyServiceStatus.dwCurrentState = SERVICE_PAUSED; 
            break; 
 
        case SERVICE_CONTROL_CONTINUE: 
        // Do whatever it takes to continue here. 
            MyServiceStatus.dwCurrentState = SERVICE_RUNNING; 
            break; 
		
 
        case SERVICE_CONTROL_STOP: 
        // Do whatever it takes to stop here. 
            MyServiceStatus.dwWin32ExitCode = 0; 
            MyServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            MyServiceStatus.dwCheckPoint    = 0; 
            MyServiceStatus.dwWaitHint      = 0; 
 
            if (!SetServiceStatus (MyServiceStatusHandle, 
                &MyServiceStatus))
            { 
                status = GetLastError(); 
                SvcDebugOut(" [MY_SERVICE] SetServiceStatus error %ld\n",status); 
            } 
 
            SvcDebugOut(" [MY_SERVICE] Leaving MyService \n",0); 
            return; 
 
        case SERVICE_CONTROL_INTERROGATE: 
        // Fall through to send current status. 
            break; 
 
        default: 
            SvcDebugOut(" [MY_SERVICE] Unrecognized opcode %ld\n", 
                Opcode); 
    } 
 
    // Send current status. 
    if (!SetServiceStatus (MyServiceStatusHandle,  &MyServiceStatus)) 
    { 
        status = GetLastError(); 
        SvcDebugOut(" [MYSERVICE] SetServiceStatus error %ld\n",status); 
    } 
    return; 
} 