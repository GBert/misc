OBJ_CAN2UDP = can2udp.o
OBJ_CAN2LAN = can2lan.o
CAN2UDP_BIN = can2udp
CAN2LAN_BIN = can2lan

PROGS = can2udp can2lan

all:	${PROGS}

can2udp: $(OBJ_CAN2UDP)
	$(CC) $(CFLAGS) -o $@ $(OBJ_CAN2UDP) $(LDFLAGS)

can2lan: $(OBJ_CAN2LAN)
	$(CC) $(CFLAGS) -o $@ $(OBJ_CAN2LAN) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -rf $(CAN2UDP_BIN) $(CAN2LAN_BIN) $(OBJ_CAN2UDP) $(OBJ_CAN2LAN)
