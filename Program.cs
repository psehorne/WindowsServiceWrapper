/*
 * Created by SharpDevelop.
 * User: Paul
 * Date: 4/16/2009
 * Time: 3:43 PM
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
 
//#include <windows.h>
//#include <stdio.h>
//#include "myservice.h"

//#define SLEEPTIME 5000

/*
OpenSCManager()
Library: Use Advapi32.lib.
*/

// forward declarations
//int DeleteService();
//int AddService(LPCTSTR lpszBinaryPathName);
//int Usage();

//SC_HANDLE schSCManager;
//SC_HANDLE schService;

using System;

namespace MyService
{
	class Program
	{
		define SLEEPTIME 5000
		BYTE args = 0;
		int DeleteService();
		int AddService(LPCTSTR lpszBinaryPathName);
		int Usage();

		SC_HANDLE schSCManager;
		SC_HANDLE schService;

	while (argc > ++args) {
		if (argv[args][0]=='-') {
			switch(toupper(argv[args][1])) {
			case 'D':
				return(DeleteService());
				break;
			case 'A':
				if (argc != 3) {
					return(Usage());
				}
				return(AddService(argv[++args]));
				break;
			default:
				return(Usage());
				break;
			} // switch
		} // if 
	} // while 
	return(Usage());
} // main

// function
int AddService(LPCTSTR lpszBinaryPathName) {
	SERVICE_DESCRIPTION ServiceDescription;
	int rc;

	schSCManager = OpenSCManager(
		MACHINENAME,
		SERVICES_ACTIVE_DATABASE,
		SC_MANAGER_ALL_ACCESS);

	if (schSCManager == NULL) {
        printf("Open SCM FAILED"); 
		Sleep(SLEEPTIME);
		return(rc=1);
	}
 

	schService = CreateService( 
        schSCManager,              // SCManager database 
        MYSERVICE,              // name of service 
		MYSERVICE_DISPLAY,
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_DEMAND_START,      // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        lpszBinaryPathName,        // service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 
 
    if (schService == NULL) {
        printf("CreateService FAILED\n"); 
		rc = 1;
    } else {
        printf("CreateService SUCCESS.\n"); 
		strcpy(ServiceDescription.lpDescription,MYSERVICE_DESCRIPTION);
		ChangeServiceConfig2(schService,
			SERVICE_CONFIG_DESCRIPTION,
			&ServiceDescription);
		rc=0;
	}
 
    CloseServiceHandle(schService); 
	Sleep(SLEEPTIME);
	return(rc);

} 

// function
int DeleteService() {
	int rc;

	schSCManager = OpenSCManager(
		MACHINENAME,
		SERVICES_ACTIVE_DATABASE,
		SC_MANAGER_ALL_ACCESS);

	schService = OpenService( 
		schSCManager,    // SCManager database 
		MYSERVICE,       // name of service 
		DELETE);         // only need DELETE access 
	
	if (schService == NULL) {
		printf("OpenService FAILED\n"); 
		Sleep(SLEEPTIME);
		return(rc=1);
	} else {
		printf("OpenService SUCCESS.\n"); 
	}
	
 	if (! DeleteService(schService) ) {
		printf("DeleteServicef FAILED"); 
		return (rc=1);
	} else {
		printf("DeleteService SUCCESS\n"); 
		rc=0;
	}
 	
	CloseServiceHandle(schService);
	Sleep(SLEEPTIME);
	return(rc);
}

	
// function
int Usage() {
	printf("\n\nOne of two parameters required:\n");
	printf("\t -d  (to delete service)\n");
	printf("\t -a FullyQualifiedPathnameToExecutable (to add service)\n");
	Sleep(SLEEPTIME);
	return(0);
	}
}
