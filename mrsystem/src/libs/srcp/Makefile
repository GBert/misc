DOKUDIR=srcp
DOKUPATH="../../doku/quellcode/$(DOKUDIR)"
DOKUWEBPATH="/var/www/Doku/mrsystem/quellcode/$(DOKUDIR)"
TARGET=libsrcp.a
OBJS=srcp_d_handshake.o srcp_d_get_cmd.o \
	srcp_e_answer_cmd.o srcp_e_answer_cmd_info.o srcp_e_answer_hndshake.o \
	srcp_v7_d_get.o srcp_v7_d_init.o srcp_v7_d_read.o srcp_v7_d_set.o srcp_v7_d_term.o srcp_v7_d_verify.o srcp_v7_d_wait.o srcp_v7_d_write.o srcp_v7_e_answer.o \
	srcp_v8_d_check.o srcp_v8_d_get.o srcp_v8_d_init.o srcp_v8_d_reset.o srcp_v8_d_set.o srcp_v8_d_term.o srcp_v8_d_verify.o srcp_v8_d_wait.o \
	srcp_parse.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp srcp.h $(INCLUDE_PATH)

srcp_d_handshake.o: srcp_d_handshake.c srcp.h srcp_parse.h

srcp_d_get_cmd.o: srcp_d_get_cmd.c srcp.h srcp_parse.h

srcp_e_answer_cmd.o: srcp_e_answer_cmd.c srcp.h srcp_parse.h

srcp_e_answer_cmd_info.o: srcp_e_answer_cmd_info.c srcp.h srcp_parse.h

srcp_e_answer_hndshake.o: srcp_e_answer_hndshake.c srcp.h srcp_parse.h

srcp_v7_d_get.o: srcp_v7_d_get.c srcp.h srcp_parse.h

srcp_v7_d_init.o: srcp_v7_d_init.c srcp.h srcp_parse.h

srcp_v7_d_read.o: srcp_v7_d_read.c srcp.h srcp_parse.h

srcp_v7_d_set.o: srcp_v7_d_set.c srcp.h srcp_parse.h

srcp_v7_d_term.o: srcp_v7_d_term.c srcp.h srcp_parse.h

srcp_v7_d_verify.o: srcp_v7_d_verify.c srcp.h srcp_parse.h

srcp_v7_d_wait.o: srcp_v7_d_wait.c srcp.h srcp_parse.h

srcp_v7_d_write.o: srcp_v7_d_write.c srcp.h srcp_parse.h

srcp_v7_e_answer.o: srcp_v7_e_answer.c srcp.h srcp_parse.h

srcp_v8_d_check.o: srcp_v8_d_check.c srcp.h srcp_parse.h

srcp_v8_d_get.o: srcp_v8_d_get.c srcp.h srcp_parse.h

srcp_v8_d_init.o: srcp_v8_d_init.c srcp.h srcp_parse.h

srcp_v8_d_reset.o: srcp_v8_d_reset.c srcp.h srcp_parse.h

srcp_v8_d_set.o: srcp_v8_d_set.c srcp.h srcp_parse.h

srcp_v8_d_term.o: srcp_v8_d_term.c srcp.h srcp_parse.h

srcp_v8_d_verify.o: srcp_v8_d_verify.c srcp.h srcp_parse.h

srcp_v8_d_wait.o: srcp_v8_d_wait.c srcp.h srcp_parse.h

srcp_parse.o: srcp_parse.c srcp_parse.h

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
	rm -f $(INCLUDE_PATH)/srcp.h
	rm -f $(LIB_PATH)/$(TARGET)

clean_doku:
	if [ -d $(DOKUPATH) ] ; then rm -r $(DOKUPATH) ; fi
