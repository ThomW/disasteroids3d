# Microsoft Developer Studio Project File - Name="lesson12" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=lesson12 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Disasteroids3d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Disasteroids3d.mak" CFG="lesson12 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lesson12 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "lesson12 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lesson12 - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib dsound.lib dinput.lib winmm.lib kernel32.lib user32.lib gdi32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /profile /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "lesson12 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W4 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib glaux.lib dsound.lib dinput.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "lesson12 - Win32 Release"
# Name "lesson12 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3dgame.cpp
# End Source File
# Begin Source File

SOURCE=.\3dglasses.cpp
# End Source File
# Begin Source File

SOURCE=.\command.cpp
# End Source File
# Begin Source File

SOURCE=.\commands.cpp
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\cvars.cpp
# End Source File
# Begin Source File

SOURCE=.\diutil.cpp
# End Source File
# Begin Source File

SOURCE=.\dsutil.cpp
# End Source File
# Begin Source File

SOURCE=.\menus.cpp
# End Source File
# Begin Source File

SOURCE=.\mp3control.cpp
# End Source File
# Begin Source File

SOURCE=.\renderflags.cpp
# End Source File
# Begin Source File

SOURCE=.\timer.cpp
# End Source File
# Begin Source File

SOURCE=.\tokenize.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\3dglasses.h
# End Source File
# Begin Source File

SOURCE=.\command.h
# End Source File
# Begin Source File

SOURCE=.\commands.h
# End Source File
# Begin Source File

SOURCE=.\console.h
# End Source File
# Begin Source File

SOURCE=.\cvars.h
# End Source File
# Begin Source File

SOURCE=.\diutil.h
# End Source File
# Begin Source File

SOURCE=.\dsutil.h
# End Source File
# Begin Source File

SOURCE=.\game.h
# End Source File
# Begin Source File

SOURCE=.\glext.h
# End Source File
# Begin Source File

SOURCE=.\LookupTables.h
# End Source File
# Begin Source File

SOURCE=.\menus.h
# End Source File
# Begin Source File

SOURCE=.\modeldata.h
# End Source File
# Begin Source File

SOURCE=.\mp3control.h
# End Source File
# Begin Source File

SOURCE=.\renderflags.h
# End Source File
# Begin Source File

SOURCE=.\timer.h
# End Source File
# Begin Source File

SOURCE=.\tokenize.h
# End Source File
# End Group
# Begin Group "Resource File"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\Res\blast.bmp
# End Source File
# Begin Source File

SOURCE=".\Res\disasteroids 3d.ico"
# End Source File
# Begin Source File

SOURCE=.\Disasteroids.rc
# End Source File
# Begin Source File

SOURCE=.\Res\explode1.wav
# End Source File
# Begin Source File

SOURCE=.\Res\explode2.wav
# End Source File
# Begin Source File

SOURCE=.\Res\explode3.wav
# End Source File
# Begin Source File

SOURCE=.\Res\extraship.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\fire.wav
# End Source File
# Begin Source File

SOURCE=.\Res\font.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\life.wav
# End Source File
# Begin Source File

SOURCE=.\Res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\lsaucer.wav
# End Source File
# Begin Source File

SOURCE=.\Res\menubeep.wav
# End Source File
# Begin Source File

SOURCE=.\Res\rock.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\saucer.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sfire.wav
# End Source File
# Begin Source File

SOURCE=.\Res\shield.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ship.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\smoke.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\space_ll.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\space_lr.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\space_ul.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\space_ur.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ssaucer.wav
# End Source File
# Begin Source File

SOURCE=.\Res\Star.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\thrust.wav
# End Source File
# Begin Source File

SOURCE=.\Res\thumphi.wav
# End Source File
# Begin Source File

SOURCE=.\Res\thumplo.wav
# End Source File
# End Group
# Begin Group "Info"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Group
# End Target
# End Project
