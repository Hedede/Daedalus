/*
 * Copyright (C) 2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_FileSystem_SourceBuffer
#define Daedalus_FileSystem_SourceBuffer
#include <daedalus/common/types.h>
#include <daedalus/io/ReadFile.h>
namespace daedalus {
/*!
 * Wraps access to source files
 */
class SourceBuffer {
public:
	SourceBuffer(ReadFile& file)
		: buffer(nullptr), length(0)
	{
		if (!file.isOpen())
			return;
		
		length = file.size();

		buffer = new char[length + 1];

		diff_t res = file.read(buffer, length);

		if (res < 0)
			free();

		// Sentinel
		buffer[length+1] = 0;
	}

	~SourceBuffer()
	{
	}

	char const* begin()
	{
		return buffer;
	}

	char const* end()
	{
		return buffer + length;
	}

	size_t size()
	{
		return length;
	}
private:
	void free()
	{
		if (buffer)
			delete[] buffer;
		buffer = 0;
		length = 0;
	}

	char* buffer;
	size_t length;
};
} // namespace daedalus
#endif//Daedalus_FileSystem_SourceBuffer
