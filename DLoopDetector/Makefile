DEPS=$(wildcard *.h)

LIBNAME=DLoopDetector
PREFIX=/usr/local

INSTALL_LIB=$(PREFIX)/lib
INSTALL_INCLUDE=$(PREFIX)/include

all: 
	@echo "Nothing to make in DLoopDetector. Just type make install."

clean:


install:
	mkdir -p $(INSTALL_INCLUDE)/$(LIBNAME) && \
	cp *.h $(INSTALL_INCLUDE)/$(LIBNAME)/

uninstall: 
	rm $(INSTALL_INCLUDE)/$(LIBNAME)/* && \
	rmdir $(INSTALL_INCLUDE)/$(LIBNAME)

