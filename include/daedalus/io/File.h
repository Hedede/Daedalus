/*
 * Copyright (C) 2014  absurdworlds
 * Copyright (C) 2015-2016 hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#ifndef Daedalus_File
#define Daedalus_File
#include <string>
#include <memory>
#include <daedalus/common/types.h>
namespace daedalus {
/*!
 * Base class for file streams
 */
class File {
public:
	//! Flags specifying how to treat the file
	enum Mode {
		//! Open file for reading
		Read     = 1,
		//! Open file for writing
		Write    = 1 << 1,
		//! Append mode - seek to the end of file before each write
		Append   = 1 << 2,
		//! Clear the file contents
		Truncate = 1 << 3
	};

	File(std::string const& path, Mode mode);
	virtual ~File();

	File(File&& other);
	File& operator = (File&& other);

	/*!
	 * Check if file is open
	 */
	bool isOpen() const;

	/*!
	 * Read specified number of bytes from file to buffer
	 */
	diff_t read(void* buffer, diff_t count);

	/*!
	 * Write specified number of bytes to file from buffer.
	 */
	diff_t write(void const* buffer, diff_t count);

	enum class SeekMode {
		Set,
		Offset,
		Reverse,
	};
	/*!
	 * Set pointer position
	 */
	diff_t seek(diff_t count, SeekMode mode);

	/*!
	 * Get pointer position
	 */
	diff_t tell() const;

	/*!
	 * Get size of file in bytes
	 */
	size_t size() const;

	/*!
	 * Get full path to file
	 */
	std::string const& path() const;

private:
	/*!
	 * Close the file
	 */
	void close();

	std::string path_;
	size_t size_;

	class Details;
	std::unique_ptr<Details> details;
};
} // namespace daedalus
#endif//Daedalus_File
