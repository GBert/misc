LIBS=cs2_parse mr_can mr_ipc config io_fkts


.PHONY: $(LIBS)

all: $(LIBS)

$(TARGET): $(OBJS)
#	cp $@  $(LIB_PATH)
	cp config.h $(INCLUDE_PATH)

cs2_parse:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

mr_can:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

mr_ipc:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

config:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

io_fkts:
	make -C $@  CFLAGS="$(CFLAGS)"  INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)

clean:
	make -C cs2_parse clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C mr_can clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C mr_ipc clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
	make -C config clean INCLUDE_PATH=../$(INCLUDE_PATH) LIB_PATH=../$(LIB_PATH)
