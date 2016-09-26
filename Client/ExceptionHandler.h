// Copyright (C) 2016 by M2O Team

#pragma once

/**
 * Exception-handler class producing mini dump after application crash.
 */
class ExceptionHandler
{
public:

	/**
	 * Install exception handler.
	 *
	 * @param path The path where minidump will be saved in case of crash. (max 260 characters)
	 */
	static bool Install(const char *const path);

	/** Hacky way of reapplying exception filter to filter out most of the crashes used by client */
	static void ReapplyExceptionFilter();
};
