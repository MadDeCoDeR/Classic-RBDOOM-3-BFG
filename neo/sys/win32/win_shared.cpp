/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#pragma hdrstop
#include "precompiled.h"

#include "win_local.h"
#include <lmerr.h>
#include <lmcons.h>
#include <lmwksta.h>
#include <errno.h>
#include <fcntl.h>
#include <direct.h>
#include <io.h>
#include <conio.h>
#undef StrCmpN
#undef StrCmpNI
#undef StrCmpI

// RB begin
#if !defined(__MINGW32__)
#include <comdef.h>
#include <comutil.h>
#include <Wbemidl.h>


// RB: no <atlbase.h> with Visual C++ 2010 Express
#if defined(USE_MFC_TOOLS)
#include <atlbase.h>
#else
#include "win_nanoafx.h"
#endif

#endif // #if !defined(__MINGW32__)
// RB end

#pragma comment (lib, "wbemuuid.lib")

#pragma warning(disable:4740)	// warning C4740: flow in or out of inline asm code suppresses global optimization

/*
================
Sys_Milliseconds
================
*/
int Sys_Milliseconds()
{
	static DWORD sys_timeBase = timeGetTime();
	return timeGetTime() - sys_timeBase;
}

/*
========================
Sys_Microseconds
========================
*/
uint64 Sys_Microseconds()
{
	static uint64 ticksPerMicrosecondTimes1024 = 0;
	
	if( ticksPerMicrosecondTimes1024 == 0 )
	{
		ticksPerMicrosecondTimes1024 = ( ( uint64 )Sys_ClockTicksPerSecond() << 10 ) / 1000000;
		assert( ticksPerMicrosecondTimes1024 > 0 );
	}
	
	return ( ( uint64 )( ( int64 )Sys_GetClockTicks() << 10 ) ) / ticksPerMicrosecondTimes1024;
}

/*
================
Sys_GetDriveFreeSpace
returns in megabytes
================
*/
int Sys_GetDriveFreeSpace( const char* path )
{
	DWORDLONG lpFreeBytesAvailable;
	DWORDLONG lpTotalNumberOfBytes;
	DWORDLONG lpTotalNumberOfFreeBytes;
	int ret = 26;
	//FIXME: see why this is failing on some machines
	if( ::GetDiskFreeSpaceEx( path, ( PULARGE_INTEGER )&lpFreeBytesAvailable, ( PULARGE_INTEGER )&lpTotalNumberOfBytes, ( PULARGE_INTEGER )&lpTotalNumberOfFreeBytes ) )
	{
		ret = ( double )( lpFreeBytesAvailable ) / ( 1024.0 * 1024.0 );
	}
	return ret;
}

/*
========================
Sys_GetDriveFreeSpaceInBytes
========================
*/
int64 Sys_GetDriveFreeSpaceInBytes( const char* path )
{
	DWORDLONG lpFreeBytesAvailable;
	DWORDLONG lpTotalNumberOfBytes;
	DWORDLONG lpTotalNumberOfFreeBytes;
	int64 ret = 1;
	//FIXME: see why this is failing on some machines
	if( ::GetDiskFreeSpaceEx( path, ( PULARGE_INTEGER )&lpFreeBytesAvailable, ( PULARGE_INTEGER )&lpTotalNumberOfBytes, ( PULARGE_INTEGER )&lpTotalNumberOfFreeBytes ) )
	{
		ret = lpFreeBytesAvailable;
	}
	return ret;
}

/*
================
Sys_GetCurrentMemoryStatus

	returns OS mem info
	all values are in kB except the memoryload
================
*/
void Sys_GetCurrentMemoryStatus( sysMemoryStats_t& stats )
{
	MEMORYSTATUSEX statex = {};
	unsigned __int64 work;
	
	statex.dwLength = sizeof( statex );
	GlobalMemoryStatusEx( &statex );
	
	memset( &stats, 0, sizeof( stats ) );
	
	stats.memoryLoad = statex.dwMemoryLoad;
	
	work = statex.ullTotalPhys >> 20;
	stats.totalPhysical = *( int* )&work;
	
	work = statex.ullAvailPhys >> 20;
	stats.availPhysical = *( int* )&work;
	
	work = statex.ullAvailPageFile >> 20;
	stats.availPageFile = *( int* )&work;
	
	work = statex.ullTotalPageFile >> 20;
	stats.totalPageFile = *( int* )&work;
	
	work = statex.ullTotalVirtual >> 20;
	stats.totalVirtual = *( int* )&work;
	
	work = statex.ullAvailVirtual >> 20;
	stats.availVirtual = *( int* )&work;
	
	work = statex.ullAvailExtendedVirtual >> 20;
	stats.availExtendedVirtual = *( int* )&work;
}

/*
================
Sys_LockMemory
================
*/
bool Sys_LockMemory( void* ptr, int bytes )
{
	return ( VirtualLock( ptr, ( SIZE_T )bytes ) != FALSE );
}

/*
================
Sys_UnlockMemory
================
*/
bool Sys_UnlockMemory( void* ptr, int bytes )
{
	return ( VirtualUnlock( ptr, ( SIZE_T )bytes ) != FALSE );
}

/*
================
Sys_SetPhysicalWorkMemory
================
*/
void Sys_SetPhysicalWorkMemory( int minBytes, int maxBytes )
{
	::SetProcessWorkingSetSize( GetCurrentProcess(), minBytes, maxBytes );
}

/*
================
Sys_GetCurrentUser
================
*/
char* Sys_GetCurrentUser()
{
	static char s_userName[1024];
	unsigned long size = sizeof( s_userName );
	
	
	if( !GetUserName( s_userName, &size ) )
	{
		strcpy( s_userName, "player" );
	}
	
	if( !s_userName[0] )
	{
		strcpy( s_userName, "player" );
	}
	
	return s_userName;
}

//GK: Begin

/*
================
Sys_Wcstrtombstr

	wcstombs is unreliable on Windows so instead it has to be done the Windows way in order to include the Sytem's ANSI code page
================
*/
int Sys_Wcstrtombstr(char* Dest, const wchar_t* Source, size_t size) {
	int wstrlen = size;
	int mbstrlen = WideCharToMultiByte(CP_ACP, NULL, Source, wstrlen, NULL, 0, NULL, 0);
	if (WideCharToMultiByte(CP_ACP, NULL, Source, wstrlen, Dest, mbstrlen, NULL, 0) > 0) {
		Dest[mbstrlen] = '\0';
		return mbstrlen + 1;
	}
	return -1;
}

int Sys_Mbstrtowcstr(wchar_t* Dest, const char* Source, size_t size)
{
	int strlen = size;
	int wstrlen = MultiByteToWideChar(CP_ACP, NULL, Source, strlen, NULL, 0);
	if (MultiByteToWideChar(CP_ACP, NULL, Source, strlen, Dest, wstrlen) > 0) {
		Dest[wstrlen] = '\0';
		return wstrlen + 1;
	}
	return -1;
}

/*
================
Sys_GetDateFormated

	returns Date String formated by user's System Setings (short format)
	
	NOTE: It might not work well if user has set short date format with letters
================
*/
int Sys_GetDateFormated(char* Dest, SYSTEMTIME* systime) {
	int bufferSize = GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, LOCALE_USE_CP_ACP | DATE_SHORTDATE, systime, NULL, NULL, 0, NULL);

	wchar_t wdate[256];

	if (GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, LOCALE_USE_CP_ACP | DATE_SHORTDATE, systime, NULL, wdate, bufferSize, NULL) > 0) {
		wdate[bufferSize] = '\0';
		return Sys_Wcstrtombstr(Dest, wdate, bufferSize + 1);
	}
	return -1;
}


/*
================
Sys_GetTimeFormated

	returns Time String formated by user's System Setings (short format and ignore am pm sufix)

	Unlike the Date, this one is not so prune to error since it uses only numbers
================
*/
int Sys_GetTimeFormated(char* Dest, SYSTEMTIME* systime) {
	int bufferSize = GetTimeFormatEx(LOCALE_NAME_USER_DEFAULT, LOCALE_USE_CP_ACP | TIME_NOSECONDS | TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER, systime, NULL, NULL, 0);

	wchar_t wtime[256];

	if (GetTimeFormatEx(LOCALE_NAME_USER_DEFAULT, LOCALE_USE_CP_ACP | TIME_NOSECONDS | TIME_FORCE24HOURFORMAT | TIME_NOTIMEMARKER, systime, NULL, wtime, bufferSize) > 0) {
		wtime[bufferSize] = '\0';
		return Sys_Wcstrtombstr(Dest, wtime, bufferSize + 1);
	}
	return -1;
}

/*
================
Sys_GetSystemFormatedTime

	returns DateTime String formated by user's System Setings (short format and ignore am pm sufix)

	This Combines the results of Sys_GetDateFormated and Sys_GetTimeFormated.
	It returns a multibyte string
================
*/
int Sys_GetSystemFormatedTime(char* Dest, ID_TIME_TYPE timeInt) {

	tm* time = localtime(&timeInt);
	SYSTEMTIME* systime = new SYSTEMTIME();
	systime->wYear = time->tm_year + 1900;
	systime->wMonth = time->tm_mon + 1;
	systime->wDay = time->tm_mday;
	systime->wHour = time->tm_hour;
	systime->wMinute = time->tm_min;
	systime->wSecond = time->tm_sec;
	systime->wDayOfWeek = time->tm_wday;
	char date[256];
	Sys_GetDateFormated(date, systime);
	char stime[256];
	Sys_GetTimeFormated(stime, systime);
	return sprintf(Dest, "%s %s\0", date, stime);
}

void Sys_ParseError(int errorCode, char* msgbuf, int bufsize)
{
	char tbuf[256];
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // Default language
		(LPTSTR)&tbuf,
		bufsize,
		NULL
	);
	sprintf(msgbuf, "%s\0", tbuf);
}

//GK: End