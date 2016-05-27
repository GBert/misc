SRCDIR  =.
OBJDIR  = .
HEXDIR = .

LIBDIR  =  
# ------------------------------------------------------
# Target and Source
TARGET = $(HEXDIR)/blinky

#C_SRC = $(SRCDIR)/blinky.c
C_SRC = $(wildcard  $(SRCDIR)/*.c)
ASM_SRC = 

C_SRC_FILE = $(notdir $(C_SRC))
C_OBJ_FILE = $(C_SRC_FILE:%.c=%.c.rel)

#OBJ = $(OBJDIR)/helloworld.c.rel      
OBJ = $(addprefix $(OBJDIR)/, $(C_OBJ_FILE)) 

# ------------------------------------------------------
# Usually SDCC's small memory model is the best choice.  If
# you run out of internal RAM, you will need to declare
# variables as "xdata", or switch to largeer model

# Memory Model (small, medium, large, huge)
MODEL  = small
# ------------------------------------------------------
# Memory Layout
# PRG Size = 4K Bytes
CODE_SIZE = --code-loc 0x0800 --code-size 8192
# INT-MEM Size = 256 Bytes
IRAM_SIZE = --idata-loc 0x0000  --iram-size 256
# EXT-MEM Size = 4K Bytes
XRAM_SIZE = --xram-loc 0x0000 --xram-size 4096



# ------------------------------------------------------
# SDCC

CC = sdcc
AS = sdas8051

MCU_MODEL = mcs51

#LIBS    = 
#LIBPATH = -L $(LIBDIR)

#DEBUG = --debug
AFLAGS = -l -s
CFLAGS = -I./inc -I$(INCDIR) -m$(MCU_MODEL) --model-$(MODEL) --out-fmt-ihx --no-xinit-opt $(DEBUG)
LFLAGS = $(LIBPATH) $(LIBS) -m$(MCU_MODEL) --model-$(MODEL) $(CODE_SIZE) $(IRAM_SIZE) $(XRAM_SIZE) --out-fmt-ihx  $(DEBUG)

# ------------------------------------------------------
# S = @

.PHONY: all clean

all: $(TARGET).hex


$(HEXDIR)/%.hex: $(OBJDIR)/%.ihx
	$(S) packihx $^ > $@

$(OBJDIR)/%.ihx: $(OBJ)
	$(S) $(CC) -o $@ $(LFLAGS) $^

$(OBJDIR)/%.c.rel: $(SRCDIR)/%.c
	$(S) $(CC) -o $@ $(CFLAGS) -c $^

$(OBJDIR)/%.asm.rel: $(SRCDIR)/%.asm
	$(S) $(AS) $(AFLAGS) -o $@ $^ 

clean:
	$(S) rm -rf *.lst *.lk *.hex *.asm *.rel *.rst *.sym *.map *.mem

