all: daedalus daedalusc

daedalus-utils:
	@ $(MAKE) -C 'source/daedalus-utils' $(TARGET)

daedalus: daedalus-utils
	@ $(MAKE) -C 'source/daedalus' $(TARGET)

daedalusc:
	@ $(MAKE) -C 'source/daedalusc' $(TARGET)
