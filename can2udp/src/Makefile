OBJ_CAN2UDP = can2udp.o
OBJ_CAN2LAN = can2lan.o
BIN = can2udp can2lan

can2udp: $(OBJ_CAN2UDP)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ_CAN2UDP) $(LDFLAGS)

can2udp: $(OBJ_CAN2LAN)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ_CAN2LAN) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ)
