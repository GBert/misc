TARGET=libmr_ipc.a
OBJS=create.o destroy.o init.o exit.o connect.o connect_if.o server.o server_if.o accept.o \
    send.o receive.o encode_can.o decode_can.o \
    cmd_set_null.o cmd_get_null.o \
    cmd_set_run.o cmd_get_run.o cmd_set_track_proto.o cmd_get_track_proto.o cmd_set_cfg_zheader.o \
    cmd_set_locomotive_dir.o cmd_set_locomotive_speed.o cmd_set_locomotive_fkt.o \
    cmd_get_locomotive_dir.o cmd_get_locomotive_speed.o cmd_get_locomotive_fkt.o \
    cmd_set_acc_pos.o cmd_set_query.o cmd_get_acc_pos.o cmd_set_member.o cmd_get_member.o \
    cmd_set_get_member.o cmd_set_get_locname.o cmd_get_get_locname.o  cmd_set_get_loclist.o \
    cmd_set_get_locinfo.o cmd_get_get_locinfo.o cmd_get_query.o cmd_get_cfg_zheader.o \
    cmd_set_cfg_header.o cmd_get_cfg_header.o cmd_set_cfg_data.o cmd_get_cfg_data.o \
    cmd_set_can_bootldr.o cmd_get_can_bootldr.o \
    cmd_set_status_request.o cmd_set_status_size.o cmd_set_status_data.o cmd_set_status_response.o cmd_get_status_request.o cmd_get_status_size.o cmd_get_status_data.o \
    cmd_set_messwert_request.o cmd_get_messwert_request.o cmd_get_set_config.o cmd_set_set_config.o cmd_get_config_response.o cmd_set_config_response.o cmd_set_messwert_response.o

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rvs $@ $+
	cp $@  $(LIB_PATH)
	cp mr_ipc.h $(INCLUDE_PATH)

create.o: create.c mr_ipc.h

destroy.o: destroy.c mr_ipc.h

init.o: init.c mr_ipc.h

exit.o: exit.c mr_ipc.h

clear.o: clear.c mr_ipc.h

connect.o: connect.c mr_ipc.h

connect_if.o: connect_if.c mr_ipc.h

server.o: server.c mr_ipc.h

server_if.o: server_if.c mr_ipc.h

accept.o: accept.c mr_ipc.h

send.o: send.c mr_ipc.h

receive.o: receive.c mr_ipc.h

encode_can.o: encode_can.c mr_ipc.h

decode_can.o: decode_can.c mr_ipc.h

cmd_set_null.o: cmd_set_null.c mr_ipc.h

cmd_set_run.o: cmd_set_run.c mr_ipc.h

cmd_set_track_proto.o: cmd_set_track_proto.c mr_ipc.h

cmd_set_locomotive_dir.o: cmd_set_locomotive_dir.c mr_ipc.h

cmd_set_locomotive_speed.o: cmd_set_locomotive_speed.c mr_ipc.h

cmd_set_locomotive_fkt.o: cmd_set_locomotive_fkt.c mr_ipc.h

cmd_set_acc_pos.o: cmd_set_acc_pos.c mr_ipc.h

cmd_set_get_member.o: cmd_set_get_member.c mr_ipc.h

cmd_set_get_locname.o: cmd_set_get_locname.c mr_ipc.h

cmd_set_get_locinfo.o: cmd_set_get_locinfo.c mr_ipc.h

cmd_set_get_loclist.o: cmd_set_get_loclist.c mr_ipc.h

cmd_set_query.o: cmd_set_query.c mr_ipc.h

cmd_set_member.o: cmd_set_member.c mr_ipc.h

cmd_set_cfg_header.o: cmd_set_cfg_header.c

cmd_set_cfg_zheader.o: cmd_set_cfg_zheader.c

cmd_set_cfg_data.o: cmd_set_cfg_data.c mr_ipc.h

cmd_set_status_request.o: cmd_set_status_request.c mr_ipc.h

cmd_set_status_data.o: cmd_set_status_data.c mr_ipc.h

cmd_set_status_response.o: cmd_set_status_response.c mr_ipc.h

cmd_get_null.o: cmd_get_null.c mr_ipc.h

cmd_get_run.o: cmd_get_run.c mr_ipc.h

cmd_get_track_proto.o: cmd_get_track_proto.c mr_ipc.h

cmd_get_locomotive_dir.o: cmd_get_locomotive_dir.c mr_ipc.h

cmd_get_locomotive_speed.o: cmd_get_locomotive_speed.c mr_ipc.h

cmd_get_locomotive_fkt.o: cmd_get_locomotive_fkt.c mr_ipc.h

cmd_get_acc_pos.o: cmd_get_acc_pos.c mr_ipc.h

cmd_get_member.o: cmd_get_member.c mr_ipc.h

cmd_get_get_locname.o: cmd_get_get_locname.c mr_ipc.h

cmd_get_get_locinfo.o: cmd_get_get_locinfo.c mr_ipc.h

cmd_get_query.o: cmd_get_query.c mr_ipc.h

cmd_get_cfg_header.o: cmd_get_cfg_header.c mr_ipc.h

cmd_get_cfg_zheader.o: cmd_get_cfg_zheader.c mr_ipc.h

cmd_get_cfg_data.o: cmd_get_cfg_data.c mr_ipc.h

cmd_set_can_bootldr.o: cmd_set_can_bootldr.c mr_ipc.h

cmd_get_can_bootldr.o: cmd_get_can_bootldr.c mr_ipc.h

cmd_get_status_request.o: cmd_get_status_request.c mr_ipc.h

cmd_get_status_size.o: cmd_get_status_size.c mr_ipc.h

cmd_get_status_data.o: cmd_get_status_data.c mr_ipc.h

cmd_set_messwert_request.o: cmd_set_messwert_request.c mr_ipc.h

cmd_get_messwert_request.o: cmd_get_messwert_request.c mr_ipc.h

cmd_set_set_config.o: cmd_set_set_config.c mr_ipc.h

cmd_get_set_config.o: cmd_get_set_config.c mr_ipc.h

cmd_set_config_response.o: cmd_set_config_response.c mr_ipc.h

cmd_get_config_response.o: cmd_get_config_response.c mr_ipc.h

cmd_set_messwert_response.o: cmd_set_messwert_response.c mr_ipc.h

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
	rm -f $(INCLUDE_PATH)/mr_ipc.h
	rm -f $(LIB_PATH)/$(TARGET)
