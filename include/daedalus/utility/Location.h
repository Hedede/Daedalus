/*
 * Copyright (C) 2016 Hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_Location
#define Daedalus_Location
#include <daedalus/io/SourceBuffer.h>
namespace daedalus {
std::pair<size_t,size_t> countLines(SourceBuffer& buf, ptrdiff_t offset)
{
	auto pos  = std::begin(buf);
	auto end  = pos + offset;
	auto last = pos;

	size_t row = 1;

	while (pos++ < end) {
		if (*pos == '\n') {
			last = pos;
			++row;
		}
	}

	auto column = end - last + 1;

	return {row, column};
}
} // namespace daedalus
#endif//Daedalus_Location
