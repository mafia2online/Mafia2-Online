// Copyright (C) 2016 by Mafia 2 Online Team

#pragma once

namespace Unicode {

/**
 * Converts the UTF-32 codepoint into an encoded UTF-8 byte sequence.
 *
 * @param codepoint       The UTF-32 codepoint to convert.
 * @param[out] utf8Text  The array of 5 chars where to encode the text.
 * @return The count of the bytes used in conversion - can be used to advance pointer - zero otherwise.
 */
unsigned ConvertUTF32toUTF8(const unsigned codepoint, char *utf8Text);

/**
 * Helper used to find start of the next UTF8 character in the string.
 *
 * @warning @a currentCharacter MUST point to the start of valid UTF8 sequence
 *          otherwise behaviour of this method is undefined.
 *
 * @param[in] string           The string in which to find the next UTF8 character.
 * @param[in] stringLength     The length of the string.
 * @param[in] currentCharacter The current character position from which to find next character.
 * @return 0 in case the given string is empty, start position of next UTF8 character in @a string
 *         in case next UTF8 character has been found, @a currentCharacter in case there are no more characters
 *         in the string.
 */
size_t FindNextUTF8CharacterStart(const char *const string, const size_t stringLength, const size_t currentCharacter);


/**
 * Helper used to find start of the previous UTF8 character in the string.
 *
 * @warning @a currentCharacter MUST point to the start of valid UTF8 sequence
 *          otherwise behaviour of this method is undefined.
 *
 * @param[in] string           The string in which to find the next UTF8 character.
 * @param[in] stringLength     The length of the string.
 * @param[in] currentCharacter The current character position from which to find previous character.
 * @return 0 in case the given string is empty, start position of previous UTF8 character in @a string
 *         in case previous UTF8 character has been found, @a currentCharacter in case there are no more characters
 *         in the string before @a currentCharacter.
 */
size_t FindPreviousUTF8CharacterStart(const char *const string, const size_t stringLength, const size_t currentCharacter);

unsigned UTF16CharacterToUTF32(const unsigned character);

unsigned GetUTF8CodePoint(const char* const string, unsigned& advance, const unsigned index = 0);
unsigned GetUTF8Length	(const char* const string);

};

/* eof */
