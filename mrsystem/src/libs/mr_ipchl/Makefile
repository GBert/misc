DOKUDIR=mr_ipchl
DOKUPATH="../../doku/quellcode/$(DOKUDIR)"
DOKUWEBPATH="/var/www/Doku/mrsystem/quellcode/$(DOKUDIR)"
TARGET=libmr_ipchl.a
OBJS=can_bootldr.o config_response.o config_value.o locname_request.o \
	locinfo_request.o member_response.o member_request.o \
	messwert_request.o status_response.o status_name_response.o \
	status_pos_response.o status_request.o cfg_header_request.o \
	cfg_data_request.o loclist_request.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp mr_ipchl.h $(INCLUDE_PATH)

can_ bootldr.o: can_bootldr.c mr_ipchl.h

config_response.o: config_response.c mr_ipchl.h

config_value.o: config_value.c mr_ipchl.h

locinfo_request.o: locinfo_request.c mr_ipchl.h

locname_request.o: locname_request.c mr_ipchl.h

loclist_request.o: loclist_request.c mr_ipchl.h

member_response.o: member_response.c mr_ipchl.h

member_request.o: member_request.c mr_ipchl.h

messwert_request.o: messwert_request.c mr_ipchl.h

status_response.o: status_response.c mr_ipchl.h

status_name_response.o: status_name_response.c mr_ipchl.h

status_pos_response.o: status_pos_response.c mr_ipchl.h

status_request.o: status_request.c mr_ipchl.h

cfg_header_request.o: cfg_header_request.c mr_ipchl.h

cfg_data_request.o: cfg_data_request.c mr_ipchl.h

doku:
	if [ ! -d $(DOKUPATH) ] ; then mkdir $(DOKUPATH) ; fi
	doxygen Doxyfile

install_web:
	if [ ! -d $(DOKUWEBPATH) ] ; then mkdir $(DOKUWEBPATH) ; fi
	if [ ! -d $(DOKUWEBPATH)/search ] ; then mkdir $(DOKUWEBPATH)/search ; fi
	find $(DOKUPATH) -type f -maxdepth 0 -exec install -m 644 -D "{}" $(DOKUWEBPATH) ";"
	find $(DOKUPATH)/search -type f -maxdepth 0 -exec install -m 644 -D "{}" $(DOKUWEBPATH)/search ";"

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/mr_ipchl.h
	rm -f $(LIB_PATH)/$(TARGET)

clean_doku:
	if [ -d $(DOKUPATH) ] ; then rm -r $(DOKUPATH) ; fi
