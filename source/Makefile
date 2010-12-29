## QueMod Makefile
## Chris.Rohlf@gmail.com 2010/2011

## Pass the -DDEBUG flag to CFLAGS to see
## verbose debug output from QueMod

#CC=gcc
CC=clang
LDFLAGS=-lnetfilter_queue -ldl -m32
CFLAGS=-pipe -ggdb -Wall -O2 #-DDEBUG

OBJECTS=quemod.o cksum.o misc.o parse_config.o nfq_core.o \
plugins.o stats.o vectors.o write_packets.o packets.o

quemod: $(OBJECTS)
	@$(CC) $(OBJECTS) $(CCFLAGS) -o quemod $(LDFLAGS)

install:
	@cp -p quemod /usr/bin/quemod
	@cp -p ../conf/quemod.conf /opt/quemod/quemod.conf
	@cp -p include/*.h /opt/quemod/quemod/
	@cp -p quemod ../bin/quemod
	@echo " "
	@echo " Installed!"
	@echo " "

uninstall:
	@rm /usr/bin/quemod

clean:
	@echo " "
	rm -f ../bin/quemod
	rm -f core *~ quemod
	rm -f *.o
	rm -f *.so

	@echo " "
	@echo "All Clean!"
	@echo " "
