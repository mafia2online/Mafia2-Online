/*
www.sourceforge.net/projects/tinyxml
Original file by Yves Berquin.

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

/*
 * THIS FILE WAS ALTERED BY Tyge Løvset, 7. April 2005.
 */


#ifndef TIXML_USE_STL

#include "tinystr.h"

// Error value for find primitive
const _TiXmlString::size_type _TiXmlString::npos = static_cast< _TiXmlString::size_type >(-1);


// Null rep.
_TiXmlString::Rep _TiXmlString::nullrep_ = { 0, 0, { '\0' } };


void _TiXmlString::reserve (size_type cap)
{
	if (cap > capacity())
	{
		_TiXmlString tmp;
		tmp.init(length(), cap);
		memcpy(tmp.start(), data(), length());
		swap(tmp);
	}
}


_TiXmlString& _TiXmlString::assign(const char* str, size_type len)
{
	size_type cap = capacity();
	if (len > cap || cap > 3*(len + 8))
	{
		_TiXmlString tmp;
		tmp.init(len);
		memcpy(tmp.start(), str, len);
		swap(tmp);
	}
	else
	{
		memmove(start(), str, len);
		set_size(len);
	}
	return *this;
}


_TiXmlString& _TiXmlString::append(const char* str, size_type len)
{
	size_type newsize = length() + len;
	if (newsize > capacity())
	{
		reserve (newsize + capacity());
	}
	memmove(finish(), str, len);
	set_size(newsize);
	return *this;
}


_TiXmlString operator + (const _TiXmlString & a, const _TiXmlString & b)
{
	_TiXmlString tmp;
	tmp.reserve(a.length() + b.length());
	tmp += a;
	tmp += b;
	return tmp;
}

_TiXmlString operator + (const _TiXmlString & a, const char* b)
{
	_TiXmlString tmp;
	_TiXmlString::size_type b_len = static_cast<_TiXmlString::size_type>( strlen(b) );
	tmp.reserve(a.length() + b_len);
	tmp += a;
	tmp.append(b, b_len);
	return tmp;
}

_TiXmlString operator + (const char* a, const _TiXmlString & b)
{
	_TiXmlString tmp;
	_TiXmlString::size_type a_len = static_cast<_TiXmlString::size_type>( strlen(a) );
	tmp.reserve(a_len + b.length());
	tmp.append(a, a_len);
	tmp += b;
	return tmp;
}


#endif	// TIXML_USE_STL
