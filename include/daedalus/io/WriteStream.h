/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_IO_WriteStream
#define Daedalus_IO_WriteStream
#include <string>
#include <daedalus/common/types.h>

namespace daedalus {
namespace io {
class WriteStream {
public:
	virtual i32 put(char c) = 0;
	virtual i32 put(std::string str) = 0;
	virtual size_t position() const = 0;
};

} // namespace io
} // namespace daedalus
#endif//Daedalus_IO_WriteStream
