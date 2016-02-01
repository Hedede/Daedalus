/*
 * Copyright (C) 2014  absurdworlds
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_ReadFile
#define Daedalus_ReadFile
#include <daedalus/io/File.h>
namespace daedalus {
/*!
 * Provides interface for reading files
 */
class ReadFile : private File {
public:
	ReadFile(std::string const& path)
		: File(path, File::Read)
	{
	}

	virtual ~ReadFile() = default;

	using File::isOpen;
	using File::read;
	using File::seek;
	using File::tell;
	using File::size;
	using File::path;
};
} // namespace daedalus
#endif//Daedalus_ReadFile
