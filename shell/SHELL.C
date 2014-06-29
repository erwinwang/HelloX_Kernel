//***********************************************************************/
//    Author                    : Garry
//    Original Date             : May,27 2004
//    Module Name               : shell.cpp
//    Module Funciton           : 
//                                This module countains shell procedures.
//    Last modified Author      :
//    Last modified Date        :
//    Last modified Content     :
//                                1.
//                                2.
//    Lines number              :
//***********************************************************************/
#ifndef __STDAFX_H__
#include "..\INCLUDE\STDAFX.H"
#endif

#ifndef __KAPI_H__
#include "..\INCLUDE\KAPI.H"
#endif

#include "shell.h"
#include "IOCTRL_S.H"
#include "SYSD_S.H"
#include "extcmd.h"

//*******************
#include "..\include\debug.h"

#if defined(__I386__)
#ifndef __BIOS_H__
#include "..\arch\x86\BIOS.H"
#endif
#endif

#include "..\INCLUDE\MODMGR.H"
#include "..\include\console.h"
#include "..\lib\stdio.h"

//*******************
#include "../include/debug.h"
//*******************

//Host name array of the system.
#define MAX_HOSTNAME_LEN  16
CHAR    HostName[MAX_HOSTNAME_LEN] = {0};

//Shell thread's handle.
__KERNEL_THREAD_OBJECT*  g_lpShellThread = NULL;

//Command buffer and it's pointer.
static CHAR        CmdBuffer[MAX_BUFFER_LEN] = {0};
static WORD        BufferPtr = 0;  //Pointing to the first free byte of the CmdBuffer.

//
//The following function form the command parameter object link from the command
//line string.
//
__CMD_PARA_OBJ* FormParameterObj(LPSTR pszCmd)
{
	__CMD_PARA_OBJ*     pObjBuffer = NULL;    //Local variables.
	__CMD_PARA_OBJ*     pBasePtr   = NULL;
	__CMD_PARA_OBJ*     pTmpObj    = NULL;
	DWORD               dwCounter  = 0x0000;
	DWORD               index      = 0x0000;

	if(NULL == pszCmd)    //Parameter check.
		return NULL;

	pObjBuffer = (__CMD_PARA_OBJ*)KMemAlloc(sizeof(__CMD_PARA_OBJ),KMEM_SIZE_TYPE_ANY);
	if(NULL == pObjBuffer)
	{
		goto __TERMINAL;
	}

	pBasePtr = pObjBuffer;
	memzero(pBasePtr,sizeof(__CMD_PARA_OBJ));

	while(*pszCmd)
	{
		if(' ' == *pszCmd)
		{
			pszCmd ++;
			continue; 
		}                                 //Filter the space.

		if(('-' == *pszCmd) || ('/' == *pszCmd))
		{
			pszCmd ++;
			pObjBuffer->byFunctionLabel = *pszCmd;
			pszCmd ++;                    //Skip the function label byte.
			continue;
		}
		else
		{
			/*while((' ' != *pszCmd) && *pszCmd)  //To find the first parameter.
			{
			pszCmd ++;
			}
			if(!*pszCmd)
			break;
			while(' ' == *pszCmd)    //Filter the space.
			pszCmd ++;

			if(!*pszCmd)
			break;*/
			index = 0x0000;
			while(('-' != *pszCmd) && ('/' != *pszCmd) && *pszCmd)
			{
				while((' ' != *pszCmd) && (*pszCmd) && (dwCounter <= CMD_PARAMETER_LEN))
				{
					pObjBuffer->Parameter[index][dwCounter] = *pszCmd;
					pszCmd ++;
					dwCounter ++;
				}
				pObjBuffer->Parameter[index][dwCounter] = 0x00;  //Set the terminal flag.
				index ++;               //Ready to copy the next parameter to parameter object.
				dwCounter = 0;

				if(!*pszCmd)
					break;
				while(' ' != *pszCmd)
					pszCmd ++;          //Skip the no space characters if the parameter's length
				//is longer than the const CMD_PARAMETER_LEN.
				while(' ' == *pszCmd)
					pszCmd ++;          //Skip the space character.
			}

			pTmpObj = pObjBuffer;       //Update the current parameter object.
			pObjBuffer = (__CMD_PARA_OBJ*)NextParaAddr(pTmpObj,index);
			pTmpObj->byParameterNum = (BYTE)(index);
			if(!*pszCmd)
				break;
			pTmpObj->pNext = pObjBuffer;
		}
	}

__TERMINAL:
	return pBasePtr;
}

//
//Releases the parameter object created by FormParameterObj routine.
//
VOID ReleaseParameterObj(__CMD_PARA_OBJ* lpParamObj)
{
	if(NULL == lpParamObj)  //Parameter check.
		return;

	KMemFree((LPVOID)lpParamObj,KMEM_SIZE_TYPE_ANY,0);  //Release the memory.
	return;
}

//The following handlers are moved to shell1.cpp.
extern VOID VerHandler(LPSTR);          //Handles the version command.
extern VOID MemHandler(LPSTR);          //Handles the memory command.
extern VOID SysInfoHandler(LPSTR);      //Handles the sysinfo command.
extern VOID HlpHandler(LPSTR);
extern VOID LoadappHandler(LPSTR);
extern VOID GUIHandler(LPSTR);          //Handler for GUI command,resides in
//shell2.cpp file.

static VOID CpuHandler(LPSTR);
static VOID SptHandler(LPSTR);
static VOID ClsHandler(LPSTR);
static VOID RunTimeHandler(LPSTR);
static VOID SysNameHandler(LPSTR);
static VOID IoCtrlApp(LPSTR);
static VOID SysDiagApp(LPSTR);
static VOID Reboot(LPSTR);
static VOID Poweroff(LPSTR);
static VOID ComDebug(LPSTR);
static VOID DebugHandler(LPSTR);
static VOID AudioHandler(LPSTR);

//Internal command handler array.
__CMD_OBJ  CmdObj[] = {
	{"version"  ,    VerHandler},
	{"memory"   ,    MemHandler},
	{"sysinfo"  ,    SysInfoHandler},
	{"sysname"  ,    SysNameHandler},
	{"help"     ,    HlpHandler},
	{"cpuinfo"  ,    CpuHandler},
	{"support"  ,    SptHandler},
	{"runtime"  ,    RunTimeHandler},
	{"ioctrl"   ,    IoCtrlApp},
	{"sysdiag"  ,    SysDiagApp},
	{"loadapp"  ,    LoadappHandler},
	{"gui"      ,    GUIHandler},
	{"reboot"   ,    Reboot},
	{"poff"     ,    Poweroff},
	{"comdebug" ,    ComDebug},
	{"cls"      ,    ClsHandler},
	//You can add your specific command and it's handler here.
	//{'yourcmd',    CmdHandler},
	{"debug"    ,    DebugHandler},
	{"playaudio",    AudioHandler},

	//The last element of this array must be NULL.
	{NULL       ,    NULL}
};

//Com interface debugging application.
static VOID ComDebug(LPSTR pstr)
{
	HANDLE hCom1 = NULL;
	CHAR   *pData = "Hello China V1.76";  //Data to write to COM1 interface.
	DWORD  dwWriteSize = 0;
	CHAR   buff[16];

	//Try to open COM1 interface to debugging.
	hCom1 = IOManager.CreateFile(
		(__COMMON_OBJECT*)&IOManager,
		"\\\\.\\COM1",
		0,
		0,
		NULL);
	if(NULL == hCom1)
	{
		PrintLine("ComDebug : Can not open COM1 interface.");
		goto __TERMINAL;
	}
	else
	{
		PrintLine("ComDebug: Open COM1 interface successfully.");
		if(IOManager.WriteFile((__COMMON_OBJECT*)&IOManager,
			hCom1,
			strlen(pData),
			pData,
			&dwWriteSize))
		{
			PrintLine("Write data to COM1 interface successfully.");
		}
		else
		{
			PrintLine("Can not write data to COM1 interface.");
		}
		PrintLine("ComDebug: Try to read data from COM interface...");
		if(!IOManager.ReadFile((__COMMON_OBJECT*)&IOManager,
			hCom1,
			1,
			(LPVOID)&buff[0],
			NULL))
		{
			PrintLine("Can not read COM interface.");
		}
		else
		{
			PrintLine("Read COM interface sucessfully.");
		}
	}

__TERMINAL:
	if(NULL != hCom1)
	{
		IOManager.CloseFile((__COMMON_OBJECT*)&IOManager,
			hCom1);
	}
	return;
}

//
//sysname handler.
//This handler changes the system name,and save it to system config database.
//
static VOID SaveSysName(LPSTR pstr)
{
}

static VOID SysNameHandler(LPSTR pszSysName)
{
	__CMD_PARA_OBJ*    pCmdObj = NULL;

	pCmdObj = FormParameterObj(pszSysName);
	if(NULL == pCmdObj)
	{
		PrintLine("Not enough system resource to interpret the command.");
		goto __TERMINAL;
	}
	if((0 == pCmdObj->byParameterNum) || (0 == pCmdObj->Parameter[0][0]))
	{
		PrintLine("Invalid command parameter.");
		goto __TERMINAL;
	}

	if(StrLen(pCmdObj->Parameter[0]) >= MAX_HOSTNAME_LEN)
	{
		PrintLine("System name must not exceed 16 bytes.");
		goto __TERMINAL;
	}

	SaveSysName(pCmdObj->Parameter[0]);
	StrCpy(pCmdObj->Parameter[0],&HostName[0]);
__TERMINAL:
	if(NULL != pCmdObj)
	{
		ReleaseParameterObj(pCmdObj);
	}
	return;
}

//Handler for 'ioctrl' command.
VOID IoCtrlApp(LPSTR pstr)
{
	__KERNEL_THREAD_OBJECT*    lpIoCtrlThread    = NULL;

	lpIoCtrlThread = KernelThreadManager.CreateKernelThread(
		(__COMMON_OBJECT*)&KernelThreadManager,
		0,
		KERNEL_THREAD_STATUS_READY,
		PRIORITY_LEVEL_NORMAL,
		IoCtrlStart,
		NULL,
		NULL,
		"IO CTRL");
	if(NULL == lpIoCtrlThread)    //Can not create the IO control thread.
	{
		PrintLine("Can not create IO control thread.");
		return;
	}

	DeviceInputManager.SetFocusThread((__COMMON_OBJECT*)&DeviceInputManager,
		(__COMMON_OBJECT*)lpIoCtrlThread);    //Set the current focus to IO control
	//application.

	lpIoCtrlThread->WaitForThisObject((__COMMON_OBJECT*)lpIoCtrlThread);  //Block the shell
	//thread until
	//the IO control
	//application end.
	KernelThreadManager.DestroyKernelThread((__COMMON_OBJECT*)&KernelThreadManager,
		(__COMMON_OBJECT*)lpIoCtrlThread);  //Destroy the thread object.

}

//
//System diag application's shell start code.
//
VOID SysDiagApp(LPSTR pstr)
{
	__KERNEL_THREAD_OBJECT*        lpSysDiagThread    = NULL;

	lpSysDiagThread = KernelThreadManager.CreateKernelThread(
		(__COMMON_OBJECT*)&KernelThreadManager,
		0,
		KERNEL_THREAD_STATUS_READY,
		PRIORITY_LEVEL_NORMAL,
		SysDiagStart,
		NULL,
		NULL,
		"SYS DIAG");
	if(NULL == lpSysDiagThread)    //Can not create the kernel thread.
	{
		PrintLine("Can not start system diag application,please retry again.");
		return;
	}

	DeviceInputManager.SetFocusThread((__COMMON_OBJECT*)&DeviceInputManager,
		(__COMMON_OBJECT*)lpSysDiagThread);

	lpSysDiagThread->WaitForThisObject((__COMMON_OBJECT*)lpSysDiagThread);
	KernelThreadManager.DestroyKernelThread((__COMMON_OBJECT*)&KernelThreadManager,
		(__COMMON_OBJECT*)lpSysDiagThread);  //Destroy the kernel thread object.
}

//Entry point of reboot.
VOID Reboot(LPSTR pstr)
{
	ClsHandler(NULL); //Clear screen first.
#ifdef __I386__
	BIOSReboot();
#endif
}

//Entry point of poweroff.
VOID Poweroff(LPSTR pstr)
{
#ifdef __I386__
	BIOSPoweroff();
#endif
}

//Handler for 'runtime' command.
VOID RunTimeHandler(LPSTR pstr)
{
	CHAR  Buffer[192];
	DWORD week = 0,day = 0,hour = 0,minute = 0,second = 0;

	second = System.GetSysTick(NULL);  //Get system tick counter.
	//Convert to second.
	second *= SYSTEM_TIME_SLICE;
	second /= 1000;

	if(second >= 60)  //Use minute.
	{
		minute = second / 60;
		second = second % 60;
	}
	if(minute >= 60) //use hour.
	{
		hour   = minute / 60;
		minute = minute % 60;
	}
	if(hour >= 24) //Use day.
	{
		day  = hour / 24;
		hour = hour % 24;
	}
	if(day >= 7) //Use week.
	{
		week = day / 7;
		day  = day % 7;
	}

	//Show out the result.
	sprintf(Buffer,"System has running %d week(s), %d day(s), %d hour(s), %d minute(s), %d second(s).",
		week,day,hour,minute,second);
	PrintLine(Buffer);
}

//Handler for 'cls' command.
VOID ClsHandler(LPSTR pstr)
{
	ClearScreen();
}

//Handler for 'cpu' command.
VOID CpuHandler(LPSTR pstr)
{
	GotoHome();
	ChangeLine();
	PrintStr("Cpu Handler called.");
}

//Handler for 'support' command.
VOID SptHandler(LPSTR pstr)
{
	LPSTR strSupportInfo1 = "    For any technical support,send E-Mail to:";
	LPSTR strSupportInfo2 = "    garryxin@yahoo.com.cn.";
	LPSTR strSupportInfo3 = "    or join the QQ group : 38467832";

	PrintLine(strSupportInfo1);
	PrintLine(strSupportInfo2);
	PrintLine(strSupportInfo3);
	return;
}

//Default handler if no proper handler is located.
VOID  DefaultHandler(LPSTR pstr)
{
	LPSTR strPrompt = "You entered incorrect command name.";
	GotoHome();
	ChangeLine();
	PrintStr(strPrompt);
	return;
}

//*********************************
// For log service
//Author :	Erwin
//Email  :	erwin.wang@qq.com
//Date	 :  9th June, 2014
//********************************
VOID DebugHandler(LPSTR pstr)
{
	char buf[256] = {'0'};
	int count = 0;
	while(TRUE)
	{
		count++;
		Sleep(1000);
		DebugManager.Logcat(&DebugManager, buf, 0);
		if(buf[0] != '0')
		{
			PrintLine(buf);
		}
		if(count == 10)break;
	}
}
//******************************
// Audio test
//
//********************************

unsigned long wavstart = 0;
unsigned long wavlen = 0;
unsigned long frequence = 0;
unsigned int samplerate = 0;
unsigned int channels = 0;
unsigned int code = 0;
unsigned int samplebits = 0;
unsigned char pcmbuf[1000] = {0};
int readflag = 0;
int end = 0;
int loop = 0;

#define NumberOfTimes1 100
#define NumberOfTimes2 100
WORD Port = 0x210;
void Testsb()
{
	int Cnt1 = 0;
	int Cnt2 = 0;
	int Found = 0;
	char buf[32] = {0};

	Cnt1=NumberOfTimes1;

	while((Port<=0x260)&&!Found)
	{
		/*outportb(Port+0x6,1);
		outportb(Port+0x6,0);*/
		__outb(1, Port+0x6);
		__outb(0, Port+0x6);

		Cnt2=NumberOfTimes2;
		//while((Cnt2>0)&&(inportb(Port+0xE)<128))
		//	--Cnt2;
		while((Cnt2>0)&&(__inb(Port+0xE)<128))
			--Cnt2;
		/*if((Cnt2==0)||(inportb(Port+0xA)!=0xAA))*/
		if((Cnt2==0)||(__inb(Port+0xA)!=0xAA))
		{
			--Cnt1;
			if(Cnt1==0)
			{
				Cnt1=NumberOfTimes1;
				Port=Port+0x10;
			}
		}
		else
			Found=1;
	}
	if(!Found)
	{
		PrintLine("Nobase port found!\n");
		PrintLine("\nDSP not reseted!\n");
	}
	else
	{
		sprintf(buf, "DSP PORT:0x%x.", Port);
		PrintLine(buf);
	}
}

//
struct wavhead
{
	char riffid[4];
	long cksize;
	char fcctype[4];
	char fmtid[4];
	long ck16;
	unsigned fortag;
	unsigned nchan;
	long nsamps;
	long navgbgs;
	unsigned nblocka;
	unsigned wbpsam;
	char dataid[4];
	long datalen;
}wfhead;

DWORD NewTimer(LPVOID pstr)
{
	static unsigned long i = 0;
	__asm cli;
	if (end == 1) return 0;

	if(i < wavlen)
	{
		i++;
		if(i > 1000000000)
		{
			i = 0;
		}
		__outb(0x10, Port + 0xC);
		while(__inb(Port +0xC));

		__outb(pcmbuf[i % 1000], Port + 0xC);
		if(i % 1000 == 0)
		{
			readflag = 1;
		}
	}
	else
	{
		i = 0;
		if(loop == 1)
		{
			end = 0;
		}
		if(loop == 0)
		{
			end = 1;
		}
	}
	__asm sti;
}

void playwav(void)
{
	HANDLE file = NULL;
	DWORD dwReaded = 0;
	long num = 0;
	unsigned char buf[1000] = {0};
	int i = 0;
	char printbuf[64] = {0};
	//buf = (unsigned char *)KMemAlloc(4*1024, KMEM_4K_START_ADDRESS);
	
	file = CreateFile("C:\\win.wav", FILE_ACCESS_READ, 0, 0);
	if (!file)
	{
		PrintLine("Can not open file!");
	}
	ReadFile(file, 1000, buf, &dwReaded);
	for (i = 1; i <= 1000; i++)
	{
		if(buf[i-1] == 'd' && buf[i] == 'a'
			&& buf[i+1] == 't' && buf[i+2] == 'a')
			break;
	}
	wavstart = (unsigned long)(i + 7);
	memcpy(&wavlen, buf+i+3, 4);
	SetFilePointer(file, 20, 0, FILE_FROM_BEGIN);
	ReadFile(file, sizeof(short), &code, &dwReaded);
	SetFilePointer(file, 22, 0, FILE_FROM_BEGIN);
	ReadFile(file, sizeof(short), &channels, &dwReaded);
	SetFilePointer(file, 24, 0, FILE_FROM_BEGIN);
	ReadFile(file, sizeof(int), &frequence, &dwReaded);
	memcpy(&samplerate, &frequence, 2);
	SetFilePointer(file, 34, 0, FILE_FROM_BEGIN);
	ReadFile(file, sizeof(short), &samplebits, &dwReaded);

	if(code != 1 || channels != 1
		|| samplebits != 8)
	{
		sprintf(printbuf, "code:%d channels:%d samplebits:%d",
			code, channels, samplebits);
		PrintLine(printbuf);
		PrintLine("Can not play this file!");
		goto __TERMINAL;
	}

	SetFilePointer(file, wavstart, 0, FILE_FROM_BEGIN);

	//
	// Key code.
	//
	SetTimer(21, 1193186/44100, NewTimer, NULL, TIMER_FLAGS_ALWAYS);

	while(TRUE)
	{
		if(readflag == TRUE)
		{
			if(ReadFile(file, 1000, pcmbuf, &dwReaded) == 0)
			{
				SetFilePointer(file, wavstart, 0, FILE_FROM_BEGIN);
				ReadFile(file, 1000, pcmbuf, &dwReaded);
			}
			readflag = FALSE;
		}
		if(end == 1)
		{
			break;
		}
	}
__TERMINAL:
	if(file)
	{
		CloseFile(file);
	}
	
}
void AudioHandler(LPSTR pstr)
{
	//char wavfile = "C:\\win.wav";
	Testsb();
	if(Port <= 0x260)
	{
		loop = 1;
		playwav();
	}
}

//Command analyzing routine,it analyzes user's input and search
//command array to find a proper handler,then call it.
//Default handler will be called if no proper command handler is
//located.
static VOID  DoCommand()
{
	DWORD wIndex = 0x0000;
	BOOL bResult = FALSE;        //If find the correct command object,then
	//This flag set to TRUE.
	CHAR tmpBuffer[36];
	DWORD dwIndex = 0;           //Used for 'for' loop.
	__KERNEL_THREAD_OBJECT* hKernelThread = NULL;

	CmdBuffer[BufferPtr] = 0x00; //Prepare the command string.
	BufferPtr = 0;

	while((' ' != CmdBuffer[wIndex]) && CmdBuffer[wIndex] && (wIndex < 32))
	{
		tmpBuffer[wIndex] = CmdBuffer[wIndex];
		wIndex ++;
	}
	tmpBuffer[wIndex] = 0;

	dwIndex = 0;
	//for(dwIndex = 0;dwIndex < CMD_OBJ_NUM;dwIndex ++)
	while(CmdObj[dwIndex].CmdStr)
	{
		if(StrCmp(&tmpBuffer[0],CmdObj[dwIndex].CmdStr))
		{
			CmdObj[dwIndex].CmdHandler(&CmdBuffer[wIndex]);  //Call the command handler.
			bResult = TRUE;      //Set the flag.
			break;
		}
		dwIndex ++;
	}
	if(bResult)
	{
		goto __END;
	}

	dwIndex = 0;  //Now,should search external command array.
	while(ExtCmdArray[dwIndex].lpszCmdName)
	{
		if(StrCmp(&tmpBuffer[0],ExtCmdArray[dwIndex].lpszCmdName))  //Found.
		{
			hKernelThread = KernelThreadManager.CreateKernelThread(
				(__COMMON_OBJECT*)&KernelThreadManager,
				0,
				KERNEL_THREAD_STATUS_READY,
				PRIORITY_LEVEL_NORMAL,
				ExtCmdArray[dwIndex].ExtCmdHandler,
				(LPVOID)&CmdBuffer[wIndex],
				NULL,
				NULL);
			if(!ExtCmdArray[dwIndex].bBackground)  //Should wait.
			{
				DeviceInputManager.SetFocusThread((__COMMON_OBJECT*)&DeviceInputManager,
					(__COMMON_OBJECT*)hKernelThread);  //Give the current input focus to this thread.
				hKernelThread->WaitForThisObject((__COMMON_OBJECT*)hKernelThread);
				KernelThreadManager.DestroyKernelThread(
					(__COMMON_OBJECT*)&KernelThreadManager,
					(__COMMON_OBJECT*)hKernelThread);  //Destroy it.
				//Set focus thread to shell.
				DeviceInputManager.SetFocusThread((__COMMON_OBJECT*)&DeviceInputManager,
					NULL);
			}
			bResult = TRUE;
			goto __END;
		}
		dwIndex ++;
	}

	if(!bResult)
	{
		DefaultHandler(NULL);        //Call the default command handler.
	}

__END:
	return;
}

//Print out command prompt.
static VOID  PrintPrompt()
{
	LPSTR pszSysName = "[system-view]";
	if(HostName[0])
	{
		PrintLine(&HostName[0]);
	}
	else
	{
		PrintLine(pszSysName);
	}
	return;
}

//Shell thread's event handler.
static BOOL EventHandler(WORD wCommand,WORD wParam,DWORD dwParam)
{
	WORD wr = 0x0700;
	BYTE bt = 0x00;

	switch(wCommand)
	{
	case MSG_KEY_DOWN:
		bt = (BYTE)(dwParam);
		if(VK_RETURN == bt)
		{
			if(BufferPtr)
			{
				DoCommand();
			}
			PrintPrompt();
			break;
		}
		if(VK_BACKSPACE == bt)
		{
			if(0 != BufferPtr)
			{
				GotoPrev();
				BufferPtr --;
			}
			break;
		}
		else
		{
			if(MAX_BUFFER_LEN - 1 > BufferPtr)
			{
				CmdBuffer[BufferPtr] = bt;
				BufferPtr ++;
				wr += (BYTE)(dwParam);
				PrintCh(wr);
			}
		}
		break;
	case KERNEL_MESSAGE_TIMER:
	default:
		break;
	}
	return 0;
}

//
//Entry point of the text mode shell.
//
DWORD ShellEntryPoint(LPVOID pData)
{
	__KERNEL_THREAD_MESSAGE KernelThreadMessage;

	//Print out version and author information.
	//GotoHome();
	//ChangeLine();
	//PrintStr(VERSION_INFO);
	GotoHome();
	ChangeLine();

	PrintPrompt();
	while(TRUE)
	{
		if(GetMessage(&KernelThreadMessage))
		{
			if(KERNEL_MESSAGE_TERMINAL == KernelThreadMessage.wCommand)
			{
				break;
			}
			DispatchMessage(&KernelThreadMessage,EventHandler);
		}
	}

	//When reach here,it means the shell thread will terminate.We will reboot
	//the system in current version's implementation,since there is no interact
	//mechanism between user and computer in case of no shell.
	//NOTE:System clean up operations should be put here if necessary.
#ifdef __I386__
	BIOSReboot();
#endif
	return 0;
}
