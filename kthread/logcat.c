//***********************************************************************/
//    Author                    : Erwin
//    Original Date             : 29th May, 2014
//    Module Name               : logcat.c
//    Module Funciton           : 
//                                This module contains the logcat daemon thread declaration code.the logcat daemon thread
//                                is one of the kernel level threads and will print the log messages from other threads to the console.
//
//    Last modified Author      : 
//    Last modified Date        : 
//    Last modified Content     : 
//                                1. 
//                                2.
//    Lines number              :
//***********************************************************************/

#ifndef __TYPES_H__
#include "types.h"
#endif

#ifndef __STDAFX_H__
#include "..\INCLUDE\StdAfx.h"
#endif

#include "..\include\debug.h"

#ifndef __CONSOLE_H__
#include "..\include\console.h"
#endif

#ifndef __KAPI_H__
#include "..\include\kapi.h"
#endif


DWORD TAEntry(LPVOID pData)
{
	int i = 0;
	int Result = -1;

	while (TRUE)
	{
		i++;
		//DebugManager.Log(&DebugManager, "TA", "Thread A is logging...");
		Logk("TA", "TA is logging...");
		KernelThreadManager.Sleep((__COMMON_OBJECT*)&KernelThreadManager,200);
	}

}

DWORD TBEntry(LPVOID pData)
{
	int i = 0;

	int Result = -1;

	while (TRUE)
	{
		//DebugManager.Log(&DebugManager, "TB", "Thread B is logging...");
		Logk("TB", "Thread B is logging...");
		KernelThreadManager.Sleep((__COMMON_OBJECT*)&KernelThreadManager,500);
	}
}

DWORD TDEntry(LPVOID pData)
{
	int i = 0;
	int j = 0;

	int Result = -1;

	while (TRUE)
	{
		i++;
		//DebugManager.Log(&DebugManager, "KTD", "Kernel Thread D is logging...");
		Logk("TD", "Thread D is logging...");
		KernelThreadManager.Sleep((__COMMON_OBJECT*)&KernelThreadManager,200);
	}

}

DWORD TCEntry(LPVOID pData)
{
	int i = 0;
	int j = 0;

	int Result = -1;

	while (TRUE)
	{
		i++;
		//DebugManager.Log(&DebugManager, "KTC", "Kernel Thread C is logging...");
		Logk("TC", "Thread C is logging...");
		KernelThreadManager.Sleep((__COMMON_OBJECT*)&KernelThreadManager,500);
	}

}

DWORD LogcatDaemon(LPVOID pData)
{
	char buf[256] = {'0'};
	while(TRUE)
	{
		DebugManager.Logcat(&DebugManager, buf, 0);	
		if(buf[0] != '0')
		{
			if(Console.bInitialized && Console.bLLInitialized)
			{
				Console.PrintLine(buf);
			}
		}
		KernelThreadManager.Sleep(&KernelThreadManager, 500);
	}

}