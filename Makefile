all: daedalus daedalusc

daedalus:
	@ $(MAKE) -C 'source/daedalus' $(target)

daedalusc:
	@ $(MAKE) -C 'source/daedalusc' $(target)

ddump:
	@ $(MAKE) -C 'source/ddump' $(target)
