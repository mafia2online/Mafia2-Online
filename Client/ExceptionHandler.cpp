// Copyright (C) 2016 by M2O Team

#include <Windows.h>
#include <stdio.h>
#include <Dbghelp.h>

#include "ExceptionHandler.h"

char minidumpsPath[MAX_PATH + 1] = { 0 };

/**
 * Should dump contain full memory dump? Use only for memory related debugging.
 *
 * Dumps are usually the same size as virtual memory (when tested +/-2GB)
 */
#define FULL_MEMORY_DUMP 0

/**
 * Should dump contain thread info?
 */
#define THREAD_INFO_DUMP 0

inline LONG GetFilterReturnCode(void)
{
#ifdef _DEBUG
	if (IsDebuggerPresent())
		return EXCEPTION_CONTINUE_SEARCH;
#endif
	return EXCEPTION_EXECUTE_HANDLER;
}

LONG WINAPI ExceptionFilter(PEXCEPTION_POINTERS exceptionInfo)
{
	const BOOL createFolderResult = CreateDirectory(minidumpsPath, NULL);
	DWORD lastError = GetLastError();
	if (!createFolderResult && (lastError != ERROR_ALREADY_EXISTS)) {
		char message[512] = { 0 };
		sprintf(message, "Multiplayer mod has crashed. Unfortunately due to some problem the creation of dumps folder failed, try running mod as administrator.\nIf it does not help please send bug report with screenshot of this dialog on http://bugs.mafia2-online.com\nError code: %X", lastError);
		MessageBox(NULL, message, "Mafia2-Online has crashed!", MB_ICONERROR);
		return GetFilterReturnCode();
	}

	char dumpFileName[32] = { 0 };

	SYSTEMTIME t;
	GetLocalTime(&t);
	sprintf(dumpFileName,"M2O_%02u_%02u_%04u_%02u_%02u_%02u", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond);

	char exceptionSavePath[MAX_PATH + 1];
	sprintf(exceptionSavePath, "%s\\%s.dmp", minidumpsPath, dumpFileName);

	const HANDLE file = CreateFile(exceptionSavePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	lastError = GetLastError();
	if (!file) {
		char message[512] = { 0 };
		sprintf(message, "Multiplayer mod has crashed. Unfortunately due to some problem the creation of dump file failed, try running mod as administrator.\nIf it does not help please send bug report with screenshot of this dialog on http://bugs.mafia2-online.com\nError code: %X", lastError);
		MessageBox(NULL, message, "Mafia2-Online has crashed!", MB_ICONERROR);
		return GetFilterReturnCode();
	}

	MINIDUMP_EXCEPTION_INFORMATION info;
	info.ThreadId = GetCurrentThreadId();
	info.ExceptionPointers = exceptionInfo;
	info.ClientPointers = FALSE;

	const unsigned minidumpType = MiniDumpNormal
#if FULL_MEMORY_DUMP
		| MiniDumpWithFullMemory
#endif
#if THREAD_INFO_DUMP
		| MiniDumpWithThreadInfo
#endif
		;

	const BOOL miniDumpWriteResult = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MINIDUMP_TYPE(minidumpType), &info, NULL, NULL);
	lastError = GetLastError();
	if (!miniDumpWriteResult) {
		char message[512] = { 0 };
		sprintf(message, "Multiplayer mod has crashed. Unfortunately due to some problem the write of dump file failed, try running mod as administrator.\nIf it does not help please send bug report with screenshot of this dialog on http://bugs.mafia2-online.com\nError code: %X", lastError);
		MessageBox(NULL, message, "Mafia2-Online has crashed!", MB_ICONERROR);

		CloseHandle(file);
		return GetFilterReturnCode();
	}

	CloseHandle(file);

	char message[512] = { 0 };
	sprintf(message, "Multiplayer mod has crashed. This is serious problem mostly caused by some bug in the code.\nPlease report it on http://bugs.mafia2-online.com and attach this file:\n%s", exceptionSavePath);
	MessageBox(NULL, message, "Mafia2-Online has crashed!", MB_ICONERROR);

	return GetFilterReturnCode();
}

bool ExceptionHandler::Install(const char *const path)
{
	if (!path || strlen(path) > MAX_PATH)
		return false;

	strncpy(minidumpsPath, path, MAX_PATH);

	ReapplyExceptionFilter();
	return true;
}

void ExceptionHandler::ReapplyExceptionFilter(void)
{
	SetUnhandledExceptionFilter(ExceptionFilter);
}