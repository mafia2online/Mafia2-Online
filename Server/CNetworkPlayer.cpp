/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Server
* File       : CNetworkPlayer.cpp
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#include	"StdInc.h"

extern	CCore				* pCore;

// temp lol
unsigned int playerColors[] = 
{
	0xE59338FF, 0xEEDC2DFF, 0xD8C762FF, 0x3FE65CFF, 0xFF8C13FF, 0xC715FFFF, 0x20B2AAFF, 0xDC143CFF,
	0x6495EDFF, 0xF0E68CFF, 0x778899FF, 0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF,
	0x4949A0FF, 0x148B8BFF, 0x14ff7fFF, 0x556b2fFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF,
	0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF, 0x4949A0FF, 0x148b8bFF, 0x14ff7fFF, 0x556b2fFF,
	0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 0xBD34DAFF, 0x247C1BFF, 0x0C8E5DFF,
	0x635B03FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF,
	0x54137DFF, 0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF,
	0x3793FAFF, 0x90239DFF, 0xE9AB2FFF, 0xAF2FF3FF, 0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF,
	0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 0x4B8987FF, 0x491B9EFF,
	0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF,
	0x2A51E2FF, 0xE3AC12FF, 0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF,
	0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 0xC1F7ECFF, 0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF,

	0xE59338FF, 0xEEDC2DFF, 0xD8C762FF, 0x3FE65CFF, 0xFF8C13FF, 0xC715FFFF, 0x20B2AAFF, 0xDC143CFF,
	0x6495EDFF, 0xF0E68CFF, 0x778899FF, 0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF,
	0x4949A0FF, 0x148B8BFF, 0x14ff7fFF, 0x556b2fFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF,
	0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF, 0x4949A0FF, 0x148b8bFF, 0x14ff7fFF, 0x556b2fFF,
	0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 0xBD34DAFF, 0x247C1BFF, 0x0C8E5DFF,
	0x635B03FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF,
	0x54137DFF, 0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF,
	0x3793FAFF, 0x90239DFF, 0xE9AB2FFF, 0xAF2FF3FF, 0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF,
	0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 0x4B8987FF, 0x491B9EFF,
	0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF,
	0x2A51E2FF, 0xE3AC12FF, 0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF,
	0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 0xC1F7ECFF, 0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF,

	0xE59338FF, 0xEEDC2DFF, 0xD8C762FF, 0x3FE65CFF, 0xFF8C13FF, 0xC715FFFF, 0x20B2AAFF, 0xDC143CFF,
	0x6495EDFF, 0xF0E68CFF, 0x778899FF, 0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF,
	0x4949A0FF, 0x148B8BFF, 0x14ff7fFF, 0x556b2fFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF,
	0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF, 0x4949A0FF, 0x148b8bFF, 0x14ff7fFF, 0x556b2fFF,
	0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 0xBD34DAFF, 0x247C1BFF, 0x0C8E5DFF,
	0x635B03FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF,
	0x54137DFF, 0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF,
	0x3793FAFF, 0x90239DFF, 0xE9AB2FFF, 0xAF2FF3FF, 0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF,
	0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 0x4B8987FF, 0x491B9EFF,
	0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF,
	0x2A51E2FF, 0xE3AC12FF, 0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF,
	0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 0xC1F7ECFF, 0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF,

	0xE59338FF, 0xEEDC2DFF, 0xD8C762FF, 0x3FE65CFF, 0xFF8C13FF, 0xC715FFFF, 0x20B2AAFF, 0xDC143CFF,
	0x6495EDFF, 0xF0E68CFF, 0x778899FF, 0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF,
	0x4949A0FF, 0x148B8BFF, 0x14ff7fFF, 0x556b2fFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF,
	0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF, 0x4949A0FF, 0x148b8bFF, 0x14ff7fFF, 0x556b2fFF,
	0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 0xBD34DAFF, 0x247C1BFF, 0x0C8E5DFF,
	0x635B03FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF,
	0x54137DFF, 0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF,
	0x3793FAFF, 0x90239DFF, 0xE9AB2FFF, 0xAF2FF3FF, 0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF,
	0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 0x4B8987FF, 0x491B9EFF,
	0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF,
	0x2A51E2FF, 0xE3AC12FF, 0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF,
	0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 0xC1F7ECFF, 0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF,

	0xE59338FF, 0xEEDC2DFF, 0xD8C762FF, 0x3FE65CFF, 0xFF8C13FF, 0xC715FFFF, 0x20B2AAFF, 0xDC143CFF,
	0x6495EDFF, 0xF0E68CFF, 0x778899FF, 0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF,
	0x4949A0FF, 0x148B8BFF, 0x14ff7fFF, 0x556b2fFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF,
	0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF, 0x4949A0FF, 0x148b8bFF, 0x14ff7fFF, 0x556b2fFF,
	0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 0xBD34DAFF, 0x247C1BFF, 0x0C8E5DFF,
	0x635B03FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF,
	0x54137DFF, 0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF,
	0x3793FAFF, 0x90239DFF, 0xE9AB2FFF, 0xAF2FF3FF, 0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF,
	0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 0x4B8987FF, 0x491B9EFF,
	0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF,
	0x2A51E2FF, 0xE3AC12FF, 0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF,
	0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 0xC1F7ECFF, 0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF,

	0xE59338FF, 0xEEDC2DFF, 0xD8C762FF, 0x3FE65CFF, 0xFF8C13FF, 0xC715FFFF, 0x20B2AAFF, 0xDC143CFF,
	0x6495EDFF, 0xF0E68CFF, 0x778899FF, 0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF,
	0x4949A0FF, 0x148B8BFF, 0x14ff7fFF, 0x556b2fFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF,
	0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF, 0x4949A0FF, 0x148b8bFF, 0x14ff7fFF, 0x556b2fFF,
	0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 0xBD34DAFF, 0x247C1BFF, 0x0C8E5DFF,
	0x635B03FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF,
	0x54137DFF, 0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF,
	0x3793FAFF, 0x90239DFF, 0xE9AB2FFF, 0xAF2FF3FF, 0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF,
	0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 0x4B8987FF, 0x491B9EFF,
	0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF,
	0x2A51E2FF, 0xE3AC12FF, 0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF,
	0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 0xC1F7ECFF, 0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF,

	0xE59338FF, 0xEEDC2DFF, 0xD8C762FF, 0x3FE65CFF, 0xFF8C13FF, 0xC715FFFF, 0x20B2AAFF, 0xDC143CFF,
	0x6495EDFF, 0xF0E68CFF, 0x778899FF, 0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF,
	0x4949A0FF, 0x148B8BFF, 0x14ff7fFF, 0x556b2fFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF,
	0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF, 0x4949A0FF, 0x148b8bFF, 0x14ff7fFF, 0x556b2fFF,
	0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 0xBD34DAFF, 0x247C1BFF, 0x0C8E5DFF,
	0x635B03FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF,
	0x54137DFF, 0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF,
	0x3793FAFF, 0x90239DFF, 0xE9AB2FFF, 0xAF2FF3FF, 0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF,
	0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 0x4B8987FF, 0x491B9EFF,
	0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF,
	0x2A51E2FF, 0xE3AC12FF, 0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF,
	0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 0xC1F7ECFF, 0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF,

	0xE59338FF, 0xEEDC2DFF, 0xD8C762FF, 0x3FE65CFF, 0xFF8C13FF, 0xC715FFFF, 0x20B2AAFF, 0xDC143CFF,
	0x6495EDFF, 0xF0E68CFF, 0x778899FF, 0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF,
	0x4949A0FF, 0x148B8BFF, 0x14ff7fFF, 0x556b2fFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF,
	0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF, 0x4949A0FF, 0x148b8bFF, 0x14ff7fFF, 0x556b2fFF,
	0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 0xBD34DAFF, 0x247C1BFF, 0x0C8E5DFF,
	0x635B03FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF,
	0x54137DFF, 0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF,
	0x3793FAFF, 0x90239DFF, 0xE9AB2FFF, 0xAF2FF3FF, 0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF,
	0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 0x4B8987FF, 0x491B9EFF,
	0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF,
	0x2A51E2FF, 0xE3AC12FF, 0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF,
	0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 0xC1F7ECFF, 0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF,

	0xE59338FF, 0xEEDC2DFF, 0xD8C762FF, 0x3FE65CFF, 0xFF8C13FF, 0xC715FFFF, 0x20B2AAFF, 0xDC143CFF,
	0x6495EDFF, 0xF0E68CFF, 0x778899FF, 0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF,
	0x4949A0FF, 0x148B8BFF, 0x14ff7fFF, 0x556b2fFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF,
	0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF, 0x4949A0FF, 0x148b8bFF, 0x14ff7fFF, 0x556b2fFF,
	0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 0xBD34DAFF, 0x247C1BFF, 0x0C8E5DFF,
	0x635B03FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF,
	0x54137DFF, 0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF,
	0x3793FAFF, 0x90239DFF, 0xE9AB2FFF, 0xAF2FF3FF, 0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF,
	0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 0x4B8987FF, 0x491B9EFF,
	0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF,
	0x2A51E2FF, 0xE3AC12FF, 0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF,
	0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 0xC1F7ECFF, 0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF,

	0xE59338FF, 0xEEDC2DFF, 0xD8C762FF, 0x3FE65CFF, 0xFF8C13FF, 0xC715FFFF, 0x20B2AAFF, 0xDC143CFF,
	0x6495EDFF, 0xF0E68CFF, 0x778899FF, 0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF,
	0x4949A0FF, 0x148B8BFF, 0x14ff7fFF, 0x556b2fFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF,
	0xF4A460FF, 0xEE82EEFF, 0xFFD720FF, 0x8b4513FF, 0x4949A0FF, 0x148b8bFF, 0x14ff7fFF, 0x556b2fFF,
	0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 0xBD34DAFF, 0x247C1BFF, 0x0C8E5DFF,
	0x635B03FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF,
	0x54137DFF, 0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF,
	0x3793FAFF, 0x90239DFF, 0xE9AB2FFF, 0xAF2FF3FF, 0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF,
	0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 0x4B8987FF, 0x491B9EFF,
	0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF,
	0x2A51E2FF, 0xE3AC12FF, 0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF,
	0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 0xC1F7ECFF, 0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF
};

CNetworkPlayer::CNetworkPlayer( void )
{
	// Reset variables
	m_playerId = INVALID_ENTITY_ID;
	m_uiColour = 0xFFFFFFFF;
	m_onFootSync.m_dwSelectedWeapon = 1;
	m_onFootSync.m_fHealth = 720.0f;
	m_onFootSync.m_uiModelIndex = 0;
	m_bAiming = false;
	m_bShooting = false;
	m_State = PLAYERSTATE_UNKNOWN;
	m_ulLastPingTime = 0;

	// 
	SetVehicle( NULL );
	SetSeat( INVALID_ENTITY_ID );
}

CNetworkPlayer::~CNetworkPlayer( void )
{

}

void CNetworkPlayer::SetId( EntityId playerId )
{
	// Set the player id
	m_playerId = playerId;

	// Set the new player colour
	m_uiColour = playerColors[ playerId ];
}

void CNetworkPlayer::ChangeNick( const char * szNick )
{
	// Call the event
	CSquirrelArguments pArguments;
	pArguments.push( m_playerId );
	pArguments.push( szNick );
	pArguments.push( GetNick() );
	pCore->GetEvents()->Call( "onPlayerChangeNick", &pArguments );

	// Set the new nick
	SetNick( szNick );
}

void CNetworkPlayer::SetColour( unsigned int uiColour )
{
	// Is this not the same colour?
	if( uiColour != m_uiColour )
	{
		// Set the new player colour
		m_uiColour = uiColour;

		// Construct new bitstream
		RakNet::BitStream pBitStream;

		// Write the playerid
		pBitStream.WriteCompressed( m_playerId );

		// Write the colour
		pBitStream.Write( uiColour );

		// Send it to all clients
		pCore->GetNetworkModule()->Call( RPC_SETPLAYERCOLOUR, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
	}
}

unsigned short CNetworkPlayer::GetPing( void )
{
	return (unsigned short)pCore->GetNetworkModule()->GetPlayerPing( m_playerId );
}

void CNetworkPlayer::SetModel( unsigned int uiModel )
{
	// Is this not the same model?
	if( uiModel != GetModel() )
	{
		// Construct new bitstream
		RakNet::BitStream pBitStream;

		// Write the model id
		pBitStream.WriteCompressed( uiModel );

		// Send it to the player
		pCore->GetNetworkModule()->Call( RPC_SETPLAYERMODEL, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, m_playerId, false );
	}
}

unsigned int CNetworkPlayer::GetModel( void )
{
	return m_onFootSync.m_uiModelIndex;
}

void CNetworkPlayer::SetHealth( float fHealth )
{
	// Is this not the same health?
	if( fHealth != GetHealth() )
	{
		// Construct new bitstream
		RakNet::BitStream pBitStream;

		// Write the health
		pBitStream.Write( fHealth );

		// Send it to the player
		pCore->GetNetworkModule()->Call( RPC_SETPLAYERHEALTH, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, m_playerId, false );
	}
}

float CNetworkPlayer::GetHealth( void )
{
	return m_onFootSync.m_fHealth;
}

void CNetworkPlayer::GiveWeapon( int iWeapon, int iAmmo )
{
	// Construct new bitstream
	RakNet::BitStream pBitStream;

	// Write the weapon
	pBitStream.WriteCompressed( iWeapon );

	// Write the ammo
	pBitStream.WriteCompressed( iAmmo );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_GIVEPLAYERWEAPON, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, m_playerId, false );
}

void CNetworkPlayer::RemoveWeapon( int iWeapon, int iAmmo )
{
	// Construct new bitstream
	RakNet::BitStream pBitStream;

	// Write the weapon
	pBitStream.WriteCompressed( iWeapon );

	// Write the ammo
	pBitStream.WriteCompressed( iAmmo );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_REMOVEPLAYERWEAPON, &pBitStream, HIGH_PRIORITY, RELIABLE_ORDERED, m_playerId, false );
}

int CNetworkPlayer::GetWeapon( void )
{
	return (int)m_onFootSync.m_dwSelectedWeapon;
}

void CNetworkPlayer::SetPosition ( CVector3 vecPosition )
{
	// Construct new bitstream
	RakNet::BitStream bitStream;

	// Write the position
	bitStream.Write ( vecPosition );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_SETPLAYERPOS, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, m_playerId, false );
}

void CNetworkPlayer::GetPosition ( CVector3 * vecPosition )
{
	// Are we in a vehicle?
	if( IsInVehicle() )
	{
		// Copy the vehicle position
		m_pVehicle->GetPosition( vecPosition );
	}
	else
	{
		// Copy the last synced position
		memcpy( vecPosition, &m_onFootSync.m_vecPosition, sizeof(CVector3) );
	}
}

void CNetworkPlayer::SetRotation ( CVector3 vecRotation )
{
	// Construct new bitstream
	RakNet::BitStream bitStream;

	// Write the rotation
	bitStream.Write ( vecRotation );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_SETPLAYERROT, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, m_playerId, false );
}

void CNetworkPlayer::GetRotation ( CVector3 * vecRotation )
{
	// Are we in a vehicle?
	if( IsInVehicle() )
	{
		// Copy the vehicle position
		m_pVehicle->GetRotation ( vecRotation );
	}
	else
	{
		// Copy the last synced position
		memcpy( vecRotation, &m_onFootSync.m_vecRotation, sizeof(CVector3) );
	}
}

void CNetworkPlayer::AddForPlayer( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player id
	bitStream.WriteCompressed( m_playerId );

	// Write the player name
	bitStream.Write( RakNet::RakString( GetNick() ) );

	// Write the player colour
	bitStream.WriteCompressed( m_uiColour );

	// Write the player model
	bitStream.WriteCompressed( m_onFootSync.m_uiModelIndex );

	// Send it to the player
	pCore->GetNetworkModule()->Call( RPC_NEW_PLAYER, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );
}

void CNetworkPlayer::AddForWorld( void )
{
	// Loop all the players
	for (EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		// Is this not this player and is active?
		if( i != m_playerId && pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Add this player for this player
			AddForPlayer( i );
		}
	}
}

void CNetworkPlayer::RemoveForPlayer( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player id
	bitStream.WriteCompressed( m_playerId );

	// Send it to the client
	pCore->GetNetworkModule()->Call( RPC_REMOVE_PLAYER, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );
}

void CNetworkPlayer::RemoveForWorld( void )
{
	// Loop all the players
	for (EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		// Is this not this player and is active?
		if( i != m_playerId && pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Remove this player for this player
			RemoveForPlayer( i );
		}
	}
}

void CNetworkPlayer::KillForPlayer( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player id
	bitStream.WriteCompressed( m_playerId );

	// Send it to the client
	pCore->GetNetworkModule()->Call( RPC_PLAYER_DEATH, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );
}

void CNetworkPlayer::KillForWorld( void )
{
	// Mark as dead
	SetDead( true );

	// Loop all the players
	for (EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		// Is this not this player and is active?
		if( i != m_playerId && pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Kill this player for this player
			KillForPlayer( i );
		}
	}
}

void CNetworkPlayer::SpawnForPlayer( EntityId playerId )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player id
	bitStream.WriteCompressed( m_playerId );

	// Send it to the client
	pCore->GetNetworkModule()->Call( RPC_PLAYER_SPAWN, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, playerId, false );
}

void CNetworkPlayer::SpawnForWorld( void )
{
	// Mark as not dead
	SetDead( false );

	// Is the player in a vehicle?
	if( IsInVehicle() )
	{
		// Reset the vehicle seat occupant
		m_pVehicle->SetOccupant( m_iSeat, NULL );

		// Decrease the occupants count
		m_pVehicle->m_uiTotalOccupants--;

		// Reset the vehicle
		SetVehicle( NULL );

		// Reset the seat
		SetSeat( INVALID_ENTITY_ID );
	}

	// Loop all the players
	for (EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		// Is this not this player and is active?
		if( i != m_playerId && pCore->GetPlayerManager()->IsActive( i ) )
		{
			// Spawn this player for this player
			SpawnForPlayer( i );
		}
	}
}

void CNetworkPlayer::StoreOnFootSync( OnFootSync * onFootSync )
{
	// Has the player changed weapon?
	if( onFootSync->m_dwSelectedWeapon != m_onFootSync.m_dwSelectedWeapon )
	{
		// Call the event
		CSquirrelArguments pArguments;
		pArguments.push( m_playerId );
		pArguments.push( (int)onFootSync->m_dwSelectedWeapon );
		pArguments.push( (int)m_onFootSync.m_dwSelectedWeapon );
		pCore->GetEvents()->Call( "onPlayerChangeWeapon", &pArguments );
	}

	// Has the player changed health?
	if( onFootSync->m_fHealth != m_onFootSync.m_fHealth )
	{
		// Call the event
		CSquirrelArguments pArguments;
		pArguments.push( m_playerId );
		pArguments.push( onFootSync->m_fHealth );
		pArguments.push( m_onFootSync.m_fHealth );
		pCore->GetEvents()->Call( "onPlayerChangeHealth", &pArguments );
	}

	// Copy the sync data
	memcpy( &m_onFootSync, onFootSync, sizeof(OnFootSync) );

	// Send the on foot sync
	SendOnFootSync();

	// Send a ping back to the player
	Ping ();
}

void CNetworkPlayer::StoreInVehicleSync( InVehicleSync * inVehicleSync )
{
	// Copy the sync data
	memcpy( &m_inVehicleSync, inVehicleSync, sizeof(InVehicleSync) );

	// Send the vehicle sync
	SendInVehicleSync();

	// Send a ping back to the player
	Ping ();
}

void CNetworkPlayer::StorePassengerSync( InPassengerSync * passengerSync )
{
	// Has the player changed health?
	if( m_onFootSync.m_fHealth != passengerSync->m_fHealth )
	{
		// Call the event
		CSquirrelArguments pArguments;
		pArguments.push( m_playerId );
		pArguments.push( passengerSync->m_fHealth );
		pArguments.push( m_onFootSync.m_fHealth );
		pCore->GetEvents()->Call( "onPlayerChangeHealth", &pArguments );

		// Store the new health
		m_onFootSync.m_fHealth = passengerSync->m_fHealth;
	}

	// Copy the sync data
	memcpy( &m_passengerSync, passengerSync, sizeof(InPassengerSync) );

	// Send the passenger sync
	SendPassengerSync();

	// Are we not driving?
	if ( !m_pVehicle || m_iSeat != 0 )
	{
		// Send a ping back to the player
		Ping ();
	}
}

void CNetworkPlayer::SendOnFootSync( void )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player id
	bitStream.WriteCompressed( m_playerId );

	// Write the player ping
	bitStream.WriteCompressed( GetPing() );

	// Write the sync data
	bitStream.Write( (char *)&m_onFootSync, sizeof(OnFootSync) );

	// Send it to other clients
	pCore->GetNetworkModule()->Call( RPC_PLAYER_SYNC, &bitStream, LOW_PRIORITY, UNRELIABLE_SEQUENCED, m_playerId, true );
}

void CNetworkPlayer::SendInVehicleSync( void )
{
	// Is the vehicle invalid?
	if ( !m_pVehicle )
		return;

	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player id
	bitStream.WriteCompressed( m_playerId );

	// Write the vehicle id
	bitStream.WriteCompressed( m_pVehicle->GetId() );

	// Write the player ping
	bitStream.WriteCompressed( GetPing() );

	// Write the passenger sync structure
	bitStream.Write( (char *)&m_inVehicleSync, sizeof(InVehicleSync) );

	// Send it to other clients
	pCore->GetNetworkModule()->Call( RPC_VEHICLE_SYNC, &bitStream, LOW_PRIORITY, UNRELIABLE_SEQUENCED, m_playerId, true );
}

void CNetworkPlayer::SendPassengerSync( void )
{
	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player id
	bitStream.WriteCompressed( m_playerId );

	// Write the player ping
	bitStream.WriteCompressed( GetPing() );

	// Write the passenger sync structure
	bitStream.Write( (char *)&m_passengerSync, sizeof(InPassengerSync) );

	// Send it to other clients
	pCore->GetNetworkModule()->Call( RPC_PASSENGER_SYNC, &bitStream, LOW_PRIORITY, UNRELIABLE_SEQUENCED, m_playerId, true );
}

void CNetworkPlayer::Pulse( void )
{
	// Should we ping this player?
	if ( (SharedUtility::GetTime() - m_ulLastPingTime) > 1000 )
	{
		// Ping the player
		pCore->GetNetworkModule()->GetRakPeer()->Ping ( pCore->GetNetworkModule()->GetRakPeer()->GetSystemAddressFromIndex ( m_playerId ) );

		// Set the last ping time
		m_ulLastPingTime = SharedUtility::GetTime ();
	}

	/*
	// Do we need to sync the datastore?
	if ( ((SharedUtility::GetTime() - m_ulLastDatastoreSyncTime) > 1000) && m_bSyncDatastore )
	{
		//
		unsigned int uiCount = 0;

		// Construct a new bitstream
		RakNet::BitStream bitStream;

		// Loop over all objects to sync to the client
		for ( std::list< const char * >::iterator iter = m_datastoreSync.begin(); iter != m_datastoreSync.end(); iter ++ )
		{
			// Should we sync this item?
			if ( m_pDatastore->ShouldSync ( *iter ) )
			{
				// Increment the object count
				uiCount ++;
			}

			// Remove the current item from the list
			//iter = m_datastoreSync.erase ( iter );
		}

		// Write the object count to the bitstream
		bitStream.WriteCompressed ( uiCount );

		// Loop over all objects to sync to the client
		for ( std::list< const char * >::iterator iter = m_datastoreSync.begin(); iter != m_datastoreSync.end(); iter ++ )
		{
			// Should we sync this item?
			if ( m_pDatastore->ShouldSync ( *iter ) )
			{
				
			}

			// Remove the current item from the list
			iter = m_datastoreSync.erase ( iter );
		}
	}
	*/
}

void CNetworkPlayer::Kick( void )
{
	// Kick the player from the server
	pCore->GetNetworkModule()->Call( RPC_KICKPLAYER, NULL, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, m_playerId, false );
}

void CNetworkPlayer::Ping ( void )
{
	// Get our current ping
	unsigned short usPing = GetPing ();

	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player ping
	bitStream.WriteCompressed ( usPing );

	// Send the bitstream to this client
	pCore->GetNetworkModule()->Call ( RPC_PLAYERPING, &bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, m_playerId, false );
}

void CNetworkPlayer::HandleVehicleEnter( EntityId vehicleId, EntityId seat )
{
	// Get a pointer to the vehicle
	CNetworkVehicle * pNetworkVehicle = pCore->GetVehicleManager()->Get( vehicleId );

	// Is the vehicle invalid?
	if( !pNetworkVehicle )
		return;

	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Get the current occupant of that seat (if any)
	CNetworkPlayer * pOccupant = pNetworkVehicle->GetOccupant ( seat );

	// Do we already have an occupant in this seat?
	if ( pOccupant )
	{
		// Let the client know we can't handle this
		bitStream.Write0 ();

		// Write the current occupant id
		bitStream.WriteCompressed ( pOccupant->GetId () );

		// Write the vehicle id
		bitStream.WriteCompressed ( vehicleId );

		// Write the seat id
		bitStream.WriteCompressed ( seat );

		// Send this packet back to the client
		pCore->GetNetworkModule()->Call ( RPC_ENTER_VEHICLE, &bitStream, IMMEDIATE_PRIORITY, RELIABLE_SEQUENCED, m_playerId, false );

		return;
	}

	// Set the vehicle
	SetVehicle( pNetworkVehicle );

	// Set the seat
	SetSeat( seat );

	// Pass it to the vehicle
	pNetworkVehicle->HandlePlayerEnter( this, seat );

	// Write the enter result
	bitStream.Write1 ();

	// Write the player id
	bitStream.WriteCompressed( m_playerId );

	// Write the vehicle id
	bitStream.WriteCompressed( vehicleId );

	// Write the seat
	bitStream.WriteCompressed( seat );

	// Send it to other clients
	pCore->GetNetworkModule()->Call( RPC_ENTER_VEHICLE, &bitStream, HIGH_PRIORITY, RELIABLE_SEQUENCED, m_playerId, true );
}

void CNetworkPlayer::HandleVehicleExit( EntityId vehicleId, EntityId seat, bool bQuickly )
{
	// Get a pointer to the vehicle
	CNetworkVehicle * pNetworkVehicle = pCore->GetVehicleManager()->Get( vehicleId );

	// Is the vehicle invalid?
	if( !pNetworkVehicle )
		return;

	// Reset the vehicle
	SetVehicle( NULL );

	// Reset the seat
	SetSeat( INVALID_ENTITY_ID );

	// Pass it to the vehicle
	pNetworkVehicle->HandlePlayerExit( this, seat );

	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player id
	bitStream.WriteCompressed( m_playerId );

	// Write if we're exiting quickly
	bQuickly ? bitStream.Write1() : bitStream.Write0();

	// Send it to other clients
	pCore->GetNetworkModule()->Call( RPC_EXIT_VEHICLE, &bitStream, HIGH_PRIORITY, RELIABLE_SEQUENCED, m_playerId, true );
}

void CNetworkPlayer::HandleVehicleEnterDone( void )
{
	// Are we not in a vehicle?
	if ( !m_pVehicle )
		return;

	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the player id
	bitStream.WriteCompressed( m_playerId );

	// Send it to other clients
	pCore->GetNetworkModule()->Call( RPC_ENTER_VEHICLE_DONE, &bitStream, HIGH_PRIORITY, RELIABLE_SEQUENCED, m_playerId, true );
}

void CNetworkPlayer::StartSyncVehicle( CNetworkVehicle * pNetworkVehicle )
{
	// Is the vehicle invalid?
	/*if( !pNetworkVehicle )
		return;

	// Are we already syncing this vehicle?
	if( IsSyncingVehicle( pNetworkVehicle ) )
		return;

	// Add this vehicle to the syncer list
	m_syncingVehicles.push_back( pNetworkVehicle );

	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the vehicle id
	bitStream.WriteCompressed( pNetworkVehicle->GetId() );

	// Send it the the client
	pCore->GetNetworkModule()->Call( RPC_PLAYERSYNCVEHICLE, &bitStream, MEDIUM_PRIORITY, RELIABLE, m_playerId, false );*/
}

void CNetworkPlayer::StopSyncVehicle( CNetworkVehicle * pNetworkVehicle )
{
	// Is the vehicle invalid?
	/*if( !pNetworkVehicle )
		return;

	// Are we not syncing this vehicle?
	if( !IsSyncingVehicle( pNetworkVehicle ) )
		return;

	// Remove this vehicle from our syncing
	m_syncingVehicles.remove( pNetworkVehicle );

	// Construct a new bitstream
	RakNet::BitStream bitStream;

	// Write the vehicle id
	bitStream.WriteCompressed( pNetworkVehicle->GetId() );

	// Send it the the client
	pCore->GetNetworkModule()->Call( RPC_PLAYERSTOPSYNCVEHICLE, &bitStream, MEDIUM_PRIORITY, RELIABLE, m_playerId, false );*/
}

bool CNetworkPlayer::IsSyncingVehicle( CNetworkVehicle * pNetworkVehicle )
{
	// Loop over every vehicle we're syncing
	/*for( std::list< CNetworkVehicle* >::iterator iter = m_syncingVehicles.begin(); iter != m_syncingVehicles.end(); iter++ )
	{
		// Is this the vehicle we're looking for?
		if( (*iter) == pNetworkVehicle )
			return true;
	}*/

	return false;
}

void CNetworkPlayer::HandlePlayerQuit( void )
{
	// Loop over all vehicles this player is syncing
	for( std::list< CNetworkVehicle* >::iterator iter = m_syncingVehicles.begin(); iter != m_syncingVehicles.end(); iter++ )
	{
		// Get a pointer to the vehicle
		CNetworkVehicle * pNetworkVehicle = *iter;

		// Is the current vehicle valid?
		if( pNetworkVehicle )
		{
			// Reset the last syncer
			pNetworkVehicle->SetLastSyncer( NULL );
		}
	}

	// Clear the syncing list
	m_syncingVehicles.clear();
}

bool CNetworkPlayer::PutInVehicle ( CNetworkVehicle * pVehicle, EntityId seatId )
{
	// Is the vehicle invalid?
	if ( !pVehicle )
		return false;

	// Is the seat invalid?
	if ( seatId < 0 || seatId > MAX_SEATS )
		return false;

	// Send bitstream to all players
	RakNet::BitStream bitStream;
	bitStream.WriteCompressed ( m_playerId );
	bitStream.WriteCompressed ( pVehicle->GetId () );
	bitStream.WriteCompressed ( seatId );
	pCore->GetNetworkModule()->Call ( RPC_PUTINVEHICLE, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
	return true;
}

void CNetworkPlayer::RemoveFromVehicle ( void )
{
	// Send bitstream to all players
	RakNet::BitStream bitStream;
	bitStream.WriteCompressed ( m_playerId );
	pCore->GetNetworkModule()->Call ( RPC_REMOVEFROMVEHICLE, &bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, INVALID_ENTITY_ID, true );
}

void CNetworkPlayer::GiveMoney ( int iMoney )
{
	// Send bitstream to player
	RakNet::BitStream pBitStream;
	pBitStream.Write( iMoney );
	pCore->GetNetworkModule()->Call( RPC_GIVEPLAYERMONEY, &pBitStream, HIGH_PRIORITY, RELIABLE, m_playerId, false );
}

void CNetworkPlayer::TakeMoney ( int iMoney )
{
	// Send bitstream to player
	RakNet::BitStream pBitStream;
	pBitStream.Write( iMoney );
	pCore->GetNetworkModule()->Call( RPC_TAKEPLAYERMONEY, &pBitStream, HIGH_PRIORITY, RELIABLE, m_playerId, false );
}

int CNetworkPlayer::GetMoney ( void )
{
	// todo
	return 0;
}