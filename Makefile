all: daedalus daedalusc

daedalus-utils:
	@ $(MAKE) -C 'source/daedalus-utils' $(target)

daedalus: daedalus-utils
	@ $(MAKE) -C 'source/daedalus' $(target)

daedalusc:
	@ $(MAKE) -C 'source/daedalusc' $(target)
