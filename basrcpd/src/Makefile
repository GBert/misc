#
# Makefile for the basrcpd project for BananaPi
# C 2018 Rainer Müller 
#
# This Makefile is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

EXE = srcpd
CONFDIR = /usr/local/etc
INSTDIR = /usr/local/bin

SRC = canbus.c clientservice.c config-srcpd.c ddl.c ddl_maerklin.c ddl_mfx.c ddl_nmra.c io.c loopback.c \
	mcs-gateway.c netservice.c srcp-command.c srcpd.c srcp-descr.c srcp-error.c srcp-fb.c srcp-ga.c \
	srcp-gl.c srcp-gm.c srcp-info.c srcp-power.c srcp-server.c srcp-session.c \
	srcp-sm.c srcp-time.c syslogmessage.c toolbox.c platform.c a20hw.c
OBJ = $(patsubst %.c,obj/%.o,$(SRC))
DEP = $(OBJ:.o=.d)

CC = gcc
CHK = cppcheck --enable=all

DEFS = -D_REENTRANT -DSYSCONFDIR=\"$(CONFDIR)\" -DHAVE_CONFIG_H 
INCS = -I/usr/include/libxml2 -I.
CFLAGS	= -g -O2 -pthread -Wall -MD $(DEFS) $(INCS) -std=gnu11
LDFLAGS	= -Wl,-Map=$(EXE).map -pthread 
LIBS =  -lxml2 

# modifications for special environment
-include MakeModifications

.PHONY:	all check install clean
all:	$(EXE)

$(EXE): $(OBJ)
	@echo **Link
	@$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)
	@echo Fertig.

$(DEP): | obj

obj:
	@mkdir -p obj

obj/%.o: %.c
	@echo **Compile $<
	@$(CC) -c $(CFLAGS) $< -o $@


check:
	@echo **Checking
	@$(CHK) . 2> cppcheck.txt


install:
	@echo **Installing
	@chown root:root $(EXE)
	@chmod 4711 $(EXE)
	@echo **PROG to $(INSTDIR)
	@cp -p $(EXE) $(INSTDIR)/. 
	@echo **CONF to $(CONFDIR)
	@cp -u $(EXE).conf $(CONFDIR)/. 


clean:
	@echo "**Clean"
	@rm -f $(OBJ) $(DEP) $(EXE) *~ *.bak $(EXE).map cppcheck.txt
	@rmdir obj


-include $(DEP)
