/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client - Game
* File       : CModelManager.h
* Developers : h0pk1nz <bydlocoder@hotmail.com>
*
***************************************************************/

#pragma once

#include <vector>

#include "COffsets.h"
#include "CString.h"

#include "engine/CM2ModelManager.h"
#include "engine/CM2Entity.h"

#define MODELMGR_MAX			1000
#define SDS_LOAD_DIR_ADDR		0x18F6FDC
#define SDS_LOAD_DIR_CARS		"/sds/cars/"
#define SDS_LOAD_DIR_HCHAR		"/sds/hchar/"
#define SDS_LOAD_DIR_PLAYER		"/sds/player/"
#define SDS_LOAD_DIR_TRAFFIC	"/sds/traffic/"

class CModelManager
{

	friend class CM2ModelManager;

private:

	std::list < CM2ModelManager * > m_modelManagers;

protected:

	static char				* GetDir(void);
	static void				SetDir(const char * pszDirectory);

public:

	CModelManager(void);
	~CModelManager(void);

	CM2ModelManager			* Load(String strModelDirectory, String strModelName);
	CM2ModelManager			* Load(const char * szModelDirectory, const char * szModelName);

	bool					Free(const char * szModelName);
	bool					Free(CM2ModelManager * pModelManager);

	void					Clear(void);

	CM2ModelManager			* GetModelManagerByName(const char * szModelName);

};