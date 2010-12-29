# Copyright (c) Chris Rohlf 2011
#
# QueMod Plugin Template

CC=clang
LDFLAGS=-shared
CFLAGS=-pipe -ggdb -Wall -I/opt/quemod -fPIC

OBJECTS=template.o

template: $(OBJECTS)
	@$(CC) $(OBJECTS) $(CCFLAGS) -o template.qm $(LDFLAGS)

install:
	@cp -p template.qm /opt/quemod/plugins/
	@echo " "
	@echo " Installed!"
	@echo " "

uninstall:
	@rm /opt/quemod/plugins/template.qm

clean:
	@echo " "
	rm -f *~
	rm -f ../core core
	rm -f *.o
	rm -f plugins/*.o
	rm -f cscope.out
	rm -f template.qm

	@echo " "
	@echo "All Clean!"
	@echo " "
