#
# Copyright (C) 2015  hedede <haddayn@gmail.com>
#
# License LGPLv3 or later:
# GNU Lesser GPL version 3 <http://gnu.org/licenses/lgpl-3.0.html>
# This is free software: you are free to change and redistribute it.
# There is NO WARRANTY, to the extent permitted by law.

CXXFLAGS += -fPIC

ifeq ($(Executable),true)
	OutputName = $(ProjectName)
else
	OutputShortName = lib$(ProjectName).so
	OutputName = $(OutputShortName).$(Version)
	CreateSymlinks = true
endif
