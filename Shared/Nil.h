// Copyright (C) 2016 by Mafia 2 Online Team

#pragma once

/**
 * Implementation of the Nil.
 *
 * @see Nil
 * @see IsNil
 * @see NotNil
 */

/** Implementation detail */
namespace Detail {
/**
 * Nil value definitions.
 */
//@{
template <typename TYPE>
struct NilValue;

template <>
struct NilValue<unsigned char>			{ static const unsigned char Value = static_cast<unsigned char>(-1); };

template <>
struct NilValue<unsigned short>			{ static const unsigned short Value = static_cast<unsigned short>(-1); };

template <>
struct NilValue<unsigned int>			{ static const unsigned int Value = static_cast<unsigned int>(-1); };

template <>
struct NilValue<unsigned long long>		{ static const unsigned long long Value = static_cast<unsigned long long>(-1); };
//@}
} /* namespace Detail */

/**
 * Helper generating the Nil for given type.
 *
 * Usage
 * @code
 * const unsigned value = Nil();
 * @endcode
 */
struct Nil
{
	template <typename TYPE>
	operator TYPE(void) { return Detail::NilValue<TYPE>::Value; }
};

/**
 * Check if given value is Nil.
 *
 * @param[in] value The value to check.
 * @return @c true if value is Nil @c false otherwise.
 */
template <typename TYPE>
bool IsNil(const TYPE value)
{
	return (value == Detail::NilValue<TYPE>::Value);
}

/**
 * Check if given value is not Nil.
 *
 * @param[in] value The value to check.
 * @return @c true if value is not Nil @c false otherwise.
 */
template <typename TYPE>
bool NotNil(const TYPE value)
{
	return (value != Detail::NilValue<TYPE>::Value);
}

/* EOF */
