LIBS=cs2_parse mr_ipc mr_ipchl mr_cs2ms2 config io_fkts uci cs2 srcp


.PHONY: $(LIBS)

all: $(LIBS)

$(TARGET): $(OBJS)
#	cp $@  $(LIB_PATH)
	cp config.h $(INCLUDE_PATH)

cs2_parse:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

mr_ipc: mr_cs2ms2 cs2
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

mr_ipchl: mr_ipc
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

mr_cs2ms2: cs2
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

config:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

io_fkts:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

uci:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

cs2:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

srcp:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

clean:
	make -C cs2_parse clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C mr_can clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C mr_ipc clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C config clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
