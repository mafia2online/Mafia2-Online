// Copyright (C) 2016 by Mafia 2 Online Team

#include <cstring>
#include <assert.h>

#include "Types.h"
#include "Unicode.h"

namespace Unicode {

unsigned UTF16CharacterToUTF32(const unsigned character)
{
	unsigned short utf16_char = static_cast<unsigned short>(character & 0xffff);
	if ((utf16_char >= 0xD800) && (utf16_char <= 0xDBFF)) {
		unsigned short second_surrogate = static_cast<unsigned short>((character >> 16) & 0xffff);
		if ((second_surrogate >= 0xDC00) && (second_surrogate <= 0xDFFF)) {
			return static_cast<unsigned>(((utf16_char - 0xD800) << 10) + (second_surrogate - 0xDC00) + 0x0010000);
		}
	}
	return character;
}

size_t FindNextUTF8CharacterStart(const char *const string, const size_t stringLength, const size_t currentCharacter)
{
	if (stringLength == currentCharacter) {
		return currentCharacter;
	}

	if (stringLength ==  0) {
		return 0;
	}

	unsigned advance = 0;
	const unsigned cp = Unicode::GetUTF8CodePoint(string, advance, currentCharacter);
	if (NotNil(cp) && (cp != 0)) {
		return currentCharacter + static_cast<size_t>(advance);
	}
	return currentCharacter;
}

size_t FindPreviousUTF8CharacterStart(const char *const string, const size_t stringLength, const size_t currentCharacter)
{
	if (currentCharacter == 0 || stringLength ==  0) {
		return 0;
	}

	size_t previousCharacterStart = 0;

	unsigned advance = 0;
	for (size_t i = 0; i < currentCharacter; i += advance) {
		const unsigned cp = Unicode::GetUTF8CodePoint(string, advance, i);
		if (cp == 0 || IsNil(cp))
			break;

		previousCharacterStart = i;
	}
	return previousCharacterStart;
}

unsigned ConvertUTF32toUTF8(const unsigned codepoint, char *utf8Text)
{
	if (codepoint <= 0x7F) {
		utf8Text[0] = static_cast<char> ( codepoint );
		utf8Text[1] = '\0';
		return 1;
	} else if (codepoint <= 0x7FF) {
		utf8Text[0] = 0xC0 | static_cast<char>  ((codepoint >> 6) & 0x1F);
		utf8Text[1] = 0x80 | static_cast<char> (codepoint & 0x3F);
		utf8Text[2] = '\0';
		return 2;
	} else if (codepoint <= 0xFFFF) {
		utf8Text[0] = 0xE0 | static_cast<char>  ((codepoint >> 12) & 0x0F);
		utf8Text[1] = 0x80 | static_cast<char>  ((codepoint >> 6) & 0x3F);
		utf8Text[2] = 0x80 | static_cast<char>  (codepoint & 0x3F);
		utf8Text[3] = '\0';
		return 3;
	} else if (codepoint <= 0x10FFFF) {
		utf8Text[0] = 0xF0 | static_cast<char> ((codepoint >> 18) & 0x0F);
		utf8Text[1] = 0x80 | static_cast<char> ((codepoint >> 12) & 0x3F);
		utf8Text[2] = 0x80 | static_cast<char> ((codepoint >> 6) & 0x3F);
		utf8Text[3] = 0x80 | static_cast<char> (codepoint & 0x3F);
		utf8Text[4] = '\0';
		return 4;
	}
	return 0;
}

unsigned GetUTF8CodePoint(const char* const string, unsigned& advance, const unsigned index)
{
	const uint8 first = static_cast<uint8>(string[index]);
	if (first < 0x80) {
		advance = 1;
		return first;
	}

	// Two octets used for character.
	// Pattern: 110xxxxx 10xxxxxx
	// First code point:  U+0080
	// Second code point: U+07FF
	if ((first >> 5) == 6) {
		const uint8 second = static_cast<uint8>(string[index + 1]);

		if (((second >> 6) & 2) != 2) {
			return Nil();
		}

		advance = 2;
		return static_cast<unsigned>((first & 0x1F) << 6) + (second & 0x3F);
	}

	// Three octets used for character.
	// Pattern: 1110xxxx 10xxxxxx 10xxxxxx
	// First code point:  U+0800
	// Second code point: U+FFFF
	if ((first >> 4) == 14)  {
		const uint8 second = static_cast<uint8>(string[index + 1]);
		const uint8 third  = static_cast<uint8>(string[index + 2]);

		if (((second >> 6) & 2) != 2) {
			return Nil();
		}

		if (((third >> 6) & 2) != 2) {
			return Nil();
		}

		advance = 3;
		return static_cast<unsigned>((first & 0xF) << 12) + ((second & 0x3F) << 6) + (third & 0x3F);
	}

	// Four octets used for character.
	// Pattern: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	// First code point:  U+10000
	// Second code point: U+1FFFFF
	if ((first >> 3) == 30) {
		const uint8 second = static_cast<uint8>(string[index + 1]);
		const uint8 third  = static_cast<uint8>(string[index + 2]);
		const uint8 forth  = static_cast<uint8>(string[index + 3]);

		if (((second >> 6) & 2) != 2) {
			return Nil();
		}

		if (((third >> 6) & 2) != 2) {
			return Nil();
		}

		if (((forth >> 6) & 2) != 2) {
			return Nil();
		}

		advance = 4;
		return static_cast<unsigned>((first & 0x7) << 18) + ((second & 0x3F) << 12) + ((third & 0x3F) << 6) + (forth & 0x3F);
	}
	return Nil();
}

unsigned GetUTF8Length(const char* const string)
{
	unsigned length = 0;
	unsigned advance = 1;
	for (unsigned i = 0; i < strlen(string); i += advance) {
		if (IsNil(GetUTF8CodePoint(string, advance, i))) {
			break;
		}
		++ length;
	}
	return length;
}

}

/* eof */
