/*
 * Copyright (C) 2014-2015  absurdworlds
 * Copyright (C)      2016  Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_string
#define Daedalus_string
#include <algorithm>
#include <vector>
#include <string>
namespace daedalus {
namespace string {
inline std::string as_string(int value)
{
	return std::to_string(value);
}

inline std::string as_string(long value)
{
	return std::to_string(value);
}

inline std::string as_string(long long value)
{
	return std::to_string(value);
}

inline std::string as_string(unsigned value)
{
	return std::to_string(value);
}

inline std::string as_string(unsigned long value)
{
	return std::to_string(value);
}

inline std::string as_string(unsigned long long value)
{
	return std::to_string(value);
}

inline std::string as_string(float value)
{
 	return std::to_string(value);
}

inline std::string as_string(double value)
{
	return std::to_string(value);
}

inline std::string as_string(long double value)
{
	return std::to_string(value);
}

inline std::string as_string(char const* value)
{
	return std::string(value);
}

inline std::string as_string(std::string value)
{
	return value;
}

void tolower(std::string& str)
{
	std::transform(std::begin(str), std::end(str),
		       std::begin(str), ::tolower);
}

std::string compose(std::string const& fmt,
                    std::vector<std::string> const& args);

template<typename... Args>
std::string compose(std::string const& fmt, Args const&... args)
{
	std::vector<std::string> bits{as_string(args)...};

	return compose(fmt, bits);
}
} // namespace string
} // namespace daedalus
#endif//Daedalus_string
