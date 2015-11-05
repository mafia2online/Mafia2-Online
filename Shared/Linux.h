/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Shared Library
* File       : Linux.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#ifndef _WIN32
#include <unistd.h>
#include <limits.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/time.h>
#include <inttypes.h>

#define	MAX_PATH					PATH_MAX
#define	Sleep(ms)					usleep((ms) * 1000)
#define	ExitProcess(code)			_exit(code)
#define	THREAD_PRIORITY_NORMAL		4
#define	stricmp						strcasecmp
#define closesocket					close

typedef bool 						BOOL;
typedef unsigned long				DWORD;
typedef unsigned short				WORD;
typedef char 						* PCHAR;
typedef	void						* PVOID;
typedef unsigned char				BYTE;

char * itoa( int value, char * str, int radix );
#endif