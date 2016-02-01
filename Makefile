all: daedalus daedalusc

daedalus:
	@ $(MAKE) -C 'source/daedalus' $(TARGET)

daedalusc:
	@ $(MAKE) -C 'source/daedalusc' $(TARGET)
