# Microsoft Developer Studio Project File - Name="master" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=master - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "master.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "master.mak" CFG="master - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "master - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "master - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "master - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MASTER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I ".\include" /I ".\arch\x86" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MASTER_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 libcmt.lib /nologo /base:"0x110000" /entry:"__OS_Entry" /dll /map /machine:I386 /ALIGN:16
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "master - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MASTER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MASTER_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /base:"0x110000" /entry:"?__init@@YAXXZ" /dll /debug /machine:I386 /nodefaultlib /pdbtype:sept /ALIGN:16
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "master - Win32 Release"
# Name "master - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "drivers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\drivers\x86\com.c
# End Source File
# Begin Source File

SOURCE=.\drivers\x86\IDEBASE.C
# End Source File
# Begin Source File

SOURCE=.\drivers\x86\IDEHD.C
# End Source File
# Begin Source File

SOURCE=.\drivers\x86\KEYBRD.C
# End Source File
# Begin Source File

SOURCE=.\drivers\x86\MOUSE.C
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib\stdio.c
# End Source File
# Begin Source File

SOURCE=.\lib\string.c
# End Source File
# End Group
# Begin Group "kthread"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\kthread\idle.c
# End Source File
# Begin Source File

SOURCE=.\kthread\logcat.c
# End Source File
# End Group
# Begin Group "arch"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\arch\x86\ARCH_X86.C
# End Source File
# Begin Source File

SOURCE=.\arch\x86\BIOS.C
# End Source File
# Begin Source File

SOURCE=.\arch\x86\HELLOCN.C
# End Source File
# End Group
# Begin Group "fs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fs\FAT32.C
# End Source File
# Begin Source File

SOURCE=.\fs\FAT322.C
# End Source File
# Begin Source File

SOURCE=.\fs\FATMGR.C
# End Source File
# Begin Source File

SOURCE=.\fs\FATMGR2.C
# End Source File
# Begin Source File

SOURCE=.\fs\FATSTR.C
# End Source File
# Begin Source File

SOURCE=.\fs\fsstr.c
# End Source File
# Begin Source File

SOURCE=.\fs\NTFS.C
# End Source File
# Begin Source File

SOURCE=.\fs\NTFS2.C
# End Source File
# Begin Source File

SOURCE=.\fs\NTFSDRV.C
# End Source File
# End Group
# Begin Group "shell"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\shell\EXTCMD.C
# End Source File
# Begin Source File

SOURCE=.\shell\FDISK.C
# End Source File
# Begin Source File

SOURCE=.\shell\FDISK2.C
# End Source File
# Begin Source File

SOURCE=.\shell\FIBONACCI.C
# End Source File
# Begin Source File

SOURCE=.\shell\FS.C
# End Source File
# Begin Source File

SOURCE=.\shell\HEDIT.C
# End Source File
# Begin Source File

SOURCE=.\shell\HYPERTRM.C
# End Source File
# Begin Source File

SOURCE=.\shell\IOCTRL_S.C
# End Source File
# Begin Source File

SOURCE=.\shell\SHELL.C
# End Source File
# Begin Source File

SOURCE=.\shell\SHELL1.C
# End Source File
# Begin Source File

SOURCE=.\shell\STAT_S.C
# End Source File
# Begin Source File

SOURCE=.\shell\SYSD_S.C
# End Source File
# End Group
# Begin Group "osentry"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\osentry\DRVENTRY.C
# End Source File
# Begin Source File

SOURCE=.\osentry\mem_scat.c
# End Source File
# Begin Source File

SOURCE=.\osentry\OS_ENTRY.C
# End Source File
# End Group
# Begin Group "user"

# PROP Default_Filter ""
# End Group
# Begin Group "kernel"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=.\kernel\COMQUEUE.C
# End Source File
# Begin Source File

SOURCE=.\kernel\console.c
# End Source File
# Begin Source File

SOURCE=.\kernel\debug.c
# End Source File
# Begin Source File

SOURCE=.\kernel\DEVMGR.C
# End Source File
# Begin Source File

SOURCE=.\kernel\DIM.C
# End Source File
# Begin Source File

SOURCE=.\kernel\HEAP.C
# End Source File
# Begin Source File

SOURCE=.\kernel\IOMGR.C
# End Source File
# Begin Source File

SOURCE=.\kernel\IOMGR2.C
# End Source File
# Begin Source File

SOURCE=.\kernel\KAPI.C
# End Source File
# Begin Source File

SOURCE=.\kernel\KERMOD.C
# End Source File
# Begin Source File

SOURCE=.\kernel\KMEMMGR.C
# End Source File
# Begin Source File

SOURCE=.\kernel\KTMGR.C
# End Source File
# Begin Source File

SOURCE=.\kernel\KTMGR2.C
# End Source File
# Begin Source File

SOURCE=.\kernel\MEM_FBL.C
# End Source File
# Begin Source File

SOURCE=.\kernel\MEMMGR.C
# End Source File
# Begin Source File

SOURCE=.\kernel\MODMGR.C
# End Source File
# Begin Source File

SOURCE=.\kernel\OBJMGR.C
# End Source File
# Begin Source File

SOURCE=.\kernel\OBJQUEUE.C
# End Source File
# Begin Source File

SOURCE=.\kernel\PAGEIDX.C
# End Source File
# Begin Source File

SOURCE=.\kernel\PCI_DRV.C
# End Source File
# Begin Source File

SOURCE=.\kernel\PERF.C
# End Source File
# Begin Source File

SOURCE=.\kernel\STATCPU.C
# End Source File
# Begin Source File

SOURCE=.\kernel\SYNOBJ.C
# End Source File
# Begin Source File

SOURCE=.\kernel\SYSCALL.C
# End Source File
# Begin Source File

SOURCE=.\kernel\SYSTEM.C
# End Source File
# Begin Source File

SOURCE=.\kernel\TYPES.C
# End Source File
# Begin Source File

SOURCE=.\kernel\VMM.C
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "include"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\INCLUDE\BUFFMGR.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\COMMOBJ.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\COMQUEUE.H
# End Source File
# Begin Source File

SOURCE=.\include\console.h
# End Source File
# Begin Source File

SOURCE=.\include\debug.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\DEVMGR.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\dim.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\GLOBVAR.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\HEAP.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\HELLOCN.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\iomgr.h
# End Source File
# Begin Source File

SOURCE=.\include\KAPI.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\KMEMMGR.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\KTMGR.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\KTMSG.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\MAILBOX.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\memmgr.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\MODMGR.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\OBJQUEUE.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\PAGEIDX.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\PCI_DRV.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\PERF.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\RINGBUFF.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\STATCPU.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\SYSCALL.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\SYSTEM.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\TYPES.H
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\VMM.H
# End Source File
# End Group
# Begin Group "drv_hdr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\drivers\x86\com.h
# End Source File
# Begin Source File

SOURCE=.\drivers\x86\IDEBASE.H
# End Source File
# Begin Source File

SOURCE=.\drivers\x86\IDEHD.H
# End Source File
# Begin Source File

SOURCE=.\drivers\x86\KEYBRD.H
# End Source File
# Begin Source File

SOURCE=.\drivers\x86\MOUSE.H
# End Source File
# End Group
# Begin Group "arch_hdr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\arch\x86\ARCH.H
# End Source File
# Begin Source File

SOURCE=.\arch\x86\BIOS.H
# End Source File
# Begin Source File

SOURCE=.\arch\x86\SYN_MECH.H
# End Source File
# End Group
# Begin Group "kt_hdr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\kthread\idle.h
# End Source File
# Begin Source File

SOURCE=.\kthread\logcat.h
# End Source File
# End Group
# Begin Group "fs_hdr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fs\FAT32.H
# End Source File
# Begin Source File

SOURCE=.\fs\fsstr.h
# End Source File
# Begin Source File

SOURCE=.\fs\NTFS.H
# End Source File
# End Group
# Begin Group "lib_hdr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib\stdio.h
# End Source File
# Begin Source File

SOURCE=.\LIB\string.h
# End Source File
# End Group
# Begin Group "shell_hdr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\shell\EXTCMD.H
# End Source File
# Begin Source File

SOURCE=.\shell\FDISK.H
# End Source File
# Begin Source File

SOURCE=.\shell\FIBONACCI.H
# End Source File
# Begin Source File

SOURCE=.\shell\FS.H
# End Source File
# Begin Source File

SOURCE=.\shell\HEDIT.H
# End Source File
# Begin Source File

SOURCE=.\shell\HYPERTRM.H
# End Source File
# Begin Source File

SOURCE=.\shell\IOCTRL_S.H
# End Source File
# Begin Source File

SOURCE=.\shell\SHELL.H
# End Source File
# Begin Source File

SOURCE=.\shell\STAT_S.H
# End Source File
# Begin Source File

SOURCE=.\shell\SYSD_S.H
# End Source File
# End Group
# Begin Group "config"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\config\config.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
