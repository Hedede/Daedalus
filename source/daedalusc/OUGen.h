/*
 * Copyright (C) 2016 Hedede <hededrk@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
#include <iostream>
#include <daedalus/ou/Parser.h>
namespace daedalus {
int ou_gen(char** argv)
{
	if (argv[1] == 0)
		return 1;

	ReadFile file(argv[1]);
	SourceBuffer buffer(file);
	ou::Parser parser(buffer);
	auto ou = parser.loadOutputUnits();

	std::cout << argv[1] << std::endl;
	for (auto& u : ou) {
		std::cout << u.name << " = " << u.subtitle << std::endl;
	}

	return 0;
}
} // namespace daedalus
