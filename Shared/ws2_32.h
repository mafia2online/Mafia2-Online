#ifndef __MYWINSOCK2_H
	#define __MYWINSOCK2_H
	#pragma push_macro("_WINSOCKAPI_")
	#undef _WINSOCKAPI_
	#include <winsock2.h>
	#pragma pop_macro("_WINSOCKAPI_")
#endif