/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Launcher
* File       : StdInc.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

// O/S includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <ShlObj.h>

// Shared
#include <Threading/CThread.h>
#include <SharedUtility.h>
#include <md5/md5.h>
#include <Network/CHttpClient.h>

// Project
#include "CUpdate.h"