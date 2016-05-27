CFLAGS = -I./include
CC = sdcc -mmcs51
CYCFX2PROG = cycfx2prog

c2prog.ihx: c2prog.rel uout.rel uin.rel timing_fx2_48mhz.rel

%.ihx: %.rel
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.rel: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $<

clean:
	for f in c2prog uout uin timing_fx2_48mhz; do \
		for e in asm ihx lnk lst map mem rel rst sym; do \
			rm -f $$f.$$e; \
		done; \
	done



.PHONY:run
run: c2prog.ihx
	$(CYCFX2PROG) prg:c2prog.ihx run

#	$(CYCFX2PROG) prg:$(BASENAME).ihx run delay:10 dbulk:6,-512,5
#	sleep 1
#	$(CYCFX2PROG) dbulk:6,-512
