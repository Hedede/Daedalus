/*
 * Copyright (C) 2014-2015  absurdworlds
 * Copyright (C) 2015       Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <daedalus/utility/string.h>
namespace daedalus {
namespace string {
static char const CompositionChar = '%';

std::string compose(
        std::string const& fmt,
       	std::vector<std::string> const& args)
{
	char const delim = CompositionChar;

	std::string result;
	result.reserve(fmt.size());

	size_t pos = 0;

	while (pos != std::string::npos) {
		size_t nextpos = fmt.find(delim, pos);
		
		result += fmt.substr(pos, nextpos - pos);
		if (nextpos == std::string::npos)
			break;

		char idx = fmt[++nextpos];
		if (!isdigit(idx)) {
			if (idx == delim) {
				result += delim;
				++nextpos;
			} else {
				result += delim;
			}
		} else {
			pos = nextpos;

			while (isdigit(fmt[nextpos]))
				++nextpos;

			size_t arg_no = stoull(fmt.substr(pos, nextpos - pos));

			if (arg_no < args.size())
				result += args[arg_no];
		}
		pos = nextpos;
	}

	return result;
}
} // namespace string
} // namespace daedalus
