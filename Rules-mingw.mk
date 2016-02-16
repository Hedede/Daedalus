#
# Copyright (C) 2015  hedede <haddayn@gmail.com>
#
# License LGPLv3 or later:
# GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
# This is free software: you are free to change and redistribute it.
# There is NO WARRANTY, to the extent permitted by law.

CXX=x86_64-w64-mingw32-g++

ifeq ($(Executable),true)
	OutputName = $(ProjectName).exe
else
	OutputName = lib$(ProjectName).dll
	CreateSymlinks = false
endif
