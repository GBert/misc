#
# Makefile für srcpd-Projekt für den BananaPi
#

EXE = srcpd
CONFDIR = /usr/local/etc

#SRC = clientservice.c config-srcpd.c dcc-address.c dccar.c ddl.c ddl_maerklin.c ddl_nmra.c ddl-s88.c \
#	hsi-88.c i2c-dev.c ib.c io.c li100-main.c loconet.c loopback.c m605x.c netservice.c \
#	portio.c selectrix.c srcp-command.c srcpd.c srcp-descr.c srcp-error.c srcp-fb.c srcp-ga.c \
#	srcp-gl.c srcp-gm.c srcp-info.c srcp-lock.c srcp-power.c srcp-server.c srcp-session.c \
#	srcp-sm.c srcp-time.c syslogmessage.c toolbox.c ttycygwin.c zimo.c
#
#rausgespart: can.c   

SRC = clientservice.c config-srcpd.c ddl.c ddl_maerklin.c ddl_mfx.c ddl_nmra.c io.c loopback.c \
	netservice.c srcp-command.c srcpd.c srcp-descr.c srcp-error.c srcp-fb.c srcp-ga.c \
	srcp-gl.c srcp-gm.c srcp-info.c srcp-lock.c srcp-power.c srcp-server.c srcp-session.c \
	srcp-sm.c srcp-time.c syslogmessage.c toolbox.c a20hw.c
OBJ = $(patsubst %.c,obj/%.o,$(SRC))
DEP = $(OBJ:.o=.d)

# CC = gcc

DEFS = -D_REENTRANT -DHAVE_CONFIG_H 
INCS = -I/usr/include/libxml2 -I.
CFLAGS	= -g -O2 -pthread -Wall -MD $(DEFS) $(INCS) -std=gnu11
LDFLAGS	= -Wl,-Map=$(EXE).map -pthread
LIBS =  -lxml2 

all:	$(EXE)

$(EXE): $(OBJ)
	@$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

$(DEP): | obj

obj:
	@mkdir -p obj

obj/%.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@


.PHONY:	clean
clean:
	@rm -f $(OBJ) $(DEP) $(EXE) *~ *.bak *.map
	@rmdir obj


-include $(DEP)
