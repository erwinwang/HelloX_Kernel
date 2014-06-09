//***********************************************************************/
//    Author                    : Erwin
//	  Email						: erwin.wang@qq.com
//    Original Date             : 29th May, 2014
//    Module Name               : debug.c
//    Module Funciton           : 
//                                Designed for debug subsystem. The log functions 
//								  now have been implemented.
//
//    Last modified Author      :
//    Last modified Date        :
//    Last modified Content     :
//                                1.
//                                2.
//    Lines number              :
//***********************************************************************/

#ifndef __STDAFX_H__
#include "..\include\kapi.h"
#endif

#ifndef __DEBUG_H__
#include "..\include\debug.h"
#endif

#include "..\lib\stdio.h"
#include "..\lib\string.h"

static void Log(__DEBUG_MANAGER *pThis, char *tag, char *msg)
{
	__DEBUG_MANAGER *pDebugManager = pThis;

	__LOG_MESSAGE *pMsg			   = (void*)0;
	__KERNEL_THREAD_OBJECT *lpCurrentThread = NULL;

	int Result = -1;

	pMsg = (__LOG_MESSAGE *)KMemAlloc(sizeof(__LOG_MESSAGE),KMEM_SIZE_TYPE_ANY);

	lpCurrentThread = KernelThreadManager.lpCurrentKernelThread;


	//Set to default now
	pMsg->code = 0;
	pMsg->format = 0;
	pMsg->len = 0;
	pMsg->pid = 0;
	pMsg->tid = 0;
	pMsg->time = 0;

	//
	//*****XXX*******
	// FIXME
	//
	pMsg->name = lpCurrentThread->KernelThreadName;
	pMsg->msg = msg;
	pMsg->tag = tag;

	//Get the authority to visit the bufferqueue

	Result = pDebugManager->pMutexForBufferQueue->WaitForThisObject((__COMMON_OBJECT*)DebugManager.pMutexForBufferQueue);
	if (Result == OBJECT_WAIT_RESOURCE)
	{
		pDebugManager->pBufferQueue->Enqueue(
			pDebugManager->pBufferQueue,
			pMsg);
		pDebugManager->pMutexForBufferQueue->ReleaseMutex((__COMMON_OBJECT*)DebugManager.pMutexForBufferQueue);
	}

	KMemFree(pMsg, KMEM_SIZE_TYPE_ANY, 0);

	return;
}

static void Logcat(__DEBUG_MANAGER *pThis, char *buf, int len)
{
	__DEBUG_MANAGER *pDebugManager = pThis;

	__LOG_MESSAGE *pMsg = (void *)0;
	__LOG_MESSAGE *p = (void *)0;
	int Result = -1;
	DWORD dwPriority    =   PRIORITY_LEVEL_IDLE;

	//
	//KernelThreadManager.GetCurrentIRQL(KernelThreadManager.lpCurrentKernelThread);
	//
	dwPriority = KernelThreadManager.lpCurrentKernelThread->dwThreadPriority;


	pMsg = (__LOG_MESSAGE *)KMemAlloc(sizeof(__LOG_MESSAGE),KMEM_SIZE_TYPE_ANY);

	//
	// Get the authority to visit the kernel bufferqueue
	//
	Result = DebugManager.pMutexForKRNLBufferQueue->WaitForThisObject((__COMMON_OBJECT*)DebugManager.pMutexForKRNLBufferQueue);
	if (Result == OBJECT_WAIT_RESOURCE)
	{
		p = pDebugManager->pKRNLBufferQueue->Dequeue(pDebugManager->pKRNLBufferQueue);
		pDebugManager->pMutexForKRNLBufferQueue->ReleaseMutex((__COMMON_OBJECT*)DebugManager.pMutexForKRNLBufferQueue);
	}

	if(p == (void *)0)
	{
		//
		// Get the authority to visit the user bufferqueue
		//
		Result = DebugManager.pMutexForBufferQueue->WaitForThisObject((__COMMON_OBJECT*)DebugManager.pMutexForBufferQueue);
		if (Result == OBJECT_WAIT_RESOURCE)
		{
			p = pDebugManager->pBufferQueue->Dequeue(pDebugManager->pBufferQueue);
			pDebugManager->pMutexForBufferQueue->ReleaseMutex((__COMMON_OBJECT*)DebugManager.pMutexForBufferQueue);
		}
	}

	if(p != (void *)0)
	{
		*pMsg = *p;
		sprintf(buf, "tag:%s name:%s time:%d pid:%d tid:%d msg:%s", 
			pMsg->tag, pMsg->name, pMsg->time, 
			pMsg->pid, pMsg->tid, pMsg->msg);	
	}

	KMemFree(pMsg, KMEM_SIZE_TYPE_ANY, 0);

	return;	
}

static void Logk(__DEBUG_MANAGER *pThis, char *tag, char *msg)
{
	__DEBUG_MANAGER *pDebugManager = pThis;

	__LOG_MESSAGE *pMsg			   = (void*)0;
	__KERNEL_THREAD_OBJECT *lpCurrentThread = NULL;

	int Result = -1;

	pMsg = (__LOG_MESSAGE *)KMemAlloc(sizeof(__LOG_MESSAGE),KMEM_SIZE_TYPE_ANY);

	lpCurrentThread = KernelThreadManager.lpCurrentKernelThread;


	//Set to default now
	pMsg->code = 0;
	pMsg->format = 0;
	pMsg->len = 0;
	pMsg->pid = 0;
	pMsg->tid = lpCurrentThread->dwThreadID;
	pMsg->time = 0;

	//
	//*****XXX*******
	// FIXME
	//
	pMsg->name = lpCurrentThread->KernelThreadName;
	pMsg->msg = msg;
	pMsg->tag = tag;

	//Get the authority to visit the bufferqueue

	Result = pDebugManager->pMutexForKRNLBufferQueue->WaitForThisObject((__COMMON_OBJECT*)DebugManager.pMutexForKRNLBufferQueue);
	if (Result == OBJECT_WAIT_RESOURCE)
	{
		pDebugManager->pKRNLBufferQueue->Enqueue(
			pDebugManager->pKRNLBufferQueue,
			pMsg);
		pDebugManager->pMutexForKRNLBufferQueue->ReleaseMutex((__COMMON_OBJECT*)DebugManager.pMutexForKRNLBufferQueue);
	}

	KMemFree(pMsg, KMEM_SIZE_TYPE_ANY, 0);

	return;
}


static int Enqueue(__BUFFER_QUEUE *pThis, __LOG_MESSAGE *pMsg)
{

	__BUFFER_QUEUE	*pBufferQueue	=	pThis;
	int				saved			=	pBufferQueue->tail;
	DWORD			dwFlags			=	0;

	if(pBufferQueue->len >= 256) return -1;

	pBufferQueue->tail = (pBufferQueue->tail + 1) % 256;
	pBufferQueue->BufferQueue[pBufferQueue->tail] = *pMsg;
	pBufferQueue->len++;

	return saved;
}

static __LOG_MESSAGE *Dequeue(__BUFFER_QUEUE *pThis)
{
	__BUFFER_QUEUE *pBufferQueue = pThis;
	int saved = pBufferQueue->head;

	if(pBufferQueue->len <= 0) return (void *)0;


	pBufferQueue->head = (pBufferQueue->head + 1) % 256;
	pBufferQueue->len--;

	return &pBufferQueue->BufferQueue[saved];

}

static void Initialize(__DEBUG_MANAGER *pThis)
{
	__DEBUG_MANAGER	*pDebugManager				=		pThis;
	__BUFFER_QUEUE	*pBufferQueue				=		(void *)0;
	__BUFFER_QUEUE	*pKRNLBufferQueue			=		(void *)0;
	__MUTEX			*pMutexForBUfferQueue		=		(void *)0;
	__MUTEX			*pMutexForKRNLBUfferQueue	=		(void *)0;

	int	i = 0;

	//****************KERNEL BUFFERQUEUE*************
	// Intialize the sync object
	pMutexForKRNLBUfferQueue = (__MUTEX*)CreateMutex();
	if (!pMutexForKRNLBUfferQueue)return;

	pDebugManager->pMutexForKRNLBufferQueue = pMutexForKRNLBUfferQueue;

	// Intialize the kernel BufferQueue.
	pKRNLBufferQueue = (__BUFFER_QUEUE *)KMemAlloc(sizeof(__BUFFER_QUEUE),KMEM_SIZE_TYPE_ANY);
	if(!pKRNLBufferQueue)return;
	for(; i < 256; i++)
	{
		pKRNLBufferQueue->BufferQueue[i].code = 0;
		pKRNLBufferQueue->BufferQueue[i].format = 0;
		pKRNLBufferQueue->BufferQueue[i].len = 0;
		pKRNLBufferQueue->BufferQueue[i].name = (void *)0;
		pKRNLBufferQueue->BufferQueue[i].msg = (void *)0;
		pKRNLBufferQueue->BufferQueue[i].tag = (void *)0;
		pKRNLBufferQueue->BufferQueue[i].time = 0;
		pKRNLBufferQueue->BufferQueue[i].pid = 0;// get pid
		pKRNLBufferQueue->BufferQueue[i].tid = 0;// get tid
	}
	// FIXME
	//pBufferQueue->head = pBufferQueue ->tail = -1;
	pKRNLBufferQueue->head = pBufferQueue ->tail = 0;
	pKRNLBufferQueue->len = 0;

	pKRNLBufferQueue->Enqueue = Enqueue;
	pKRNLBufferQueue->Dequeue = Dequeue;

	pDebugManager->pKRNLBufferQueue = pKRNLBufferQueue;


	//**************USER BUFFERQUEUE**************

	// Intialize the sync object
	pMutexForBUfferQueue = (__MUTEX*)CreateMutex();
	if (!pMutexForBUfferQueue)return;

	pDebugManager->pMutexForBufferQueue = pMutexForBUfferQueue;

	// Intialize the user BufferQueue.
	pBufferQueue = (__BUFFER_QUEUE *)KMemAlloc(sizeof(__BUFFER_QUEUE),KMEM_SIZE_TYPE_ANY);
	if(!pBufferQueue)return;
	for(; i < 256; i++)
	{
		pBufferQueue->BufferQueue[i].code = 0;
		pBufferQueue->BufferQueue[i].format = 0;
		pBufferQueue->BufferQueue[i].len = 0;
		pBufferQueue->BufferQueue[i].name = (void *)0;
		pBufferQueue->BufferQueue[i].msg = (void *)0;
		pBufferQueue->BufferQueue[i].tag = (void *)0;
		pBufferQueue->BufferQueue[i].time = 0;
		pBufferQueue->BufferQueue[i].pid = 0;// get pid
		pBufferQueue->BufferQueue[i].tid = 0;// get tid
	}
	// FIXME
	//pBufferQueue->head = pBufferQueue ->tail = -1;
	pBufferQueue->head = pBufferQueue ->tail = 0;
	pBufferQueue->len = 0;

	pBufferQueue->Enqueue = Enqueue;
	pBufferQueue->Dequeue = Dequeue;

	pDebugManager->pBufferQueue = pBufferQueue;

	return;
}

static void Unintialize(__DEBUG_MANAGER *pThis)
{

	__DEBUG_MANAGER *DebugManager = pThis;
	// Uninitialize the sync object..

	// Release the user BufferQueue
	if(!DebugManager->pBufferQueue)
	{
		KMemFree(DebugManager->pBufferQueue, KMEM_SIZE_TYPE_ANY, sizeof(__BUFFER_QUEUE));
	}

	// Release the kernel BufferQueue
	if(!DebugManager->pKRNLBufferQueue)
	{
		KMemFree(DebugManager->pKRNLBufferQueue, KMEM_SIZE_TYPE_ANY, sizeof(__BUFFER_QUEUE));
	}

	if (!DebugManager->pMutexForBufferQueue)
	{
		ObjectManager.DestroyObject(&ObjectManager,(__COMMON_OBJECT*)DebugManager->pMutexForBufferQueue);
	}

	if (!DebugManager->pMutexForKRNLBufferQueue)
	{
		ObjectManager.DestroyObject(&ObjectManager,(__COMMON_OBJECT*)DebugManager->pMutexForKRNLBufferQueue);
	}

	return;
} 

__DEBUG_MANAGER DebugManager =
{
	// For user 
	(void *)0,
	NULL,       //MODIFIED BY GARRY: Please keep same structure in header file's definition,only warning in C will be generated if
	//align is not enforced and can lead fatal run time error.
	// For kernel
	(void *)0,
	NULL,

	Log,		//Log
	Logk,		//Logk
	Logcat,		//Logcat
	Initialize,
	Unintialize,
};