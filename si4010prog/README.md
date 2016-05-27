SI4010 Programmer/Debugger
--------------------------
si4010prog is a tool to program and debug the Silicon Labs SI4010 micro
controller.

C2 Bus Master
-------------
The si4010prog program requires some sort of C2 Bus master hardware to interact
with the C2 bus.

 * Cypress FX2: The best tested interface. See drivers/c2_fx2/README.md
 * Linux GPIO: Driver using Linux GPIO. See drivers/c2_gpio/README.md
 * Linux LPT: Deprecated, LPT port base bus master
 * FT232: C2 bus using the FT232(H) chip. See drivers/c2_ft232/README.md

See the drivers/ directory for more available bus masters, or run _si4010prog -d
help_ to get a full list of available bus masters.

Known Issues
------------
- Currently there is no way to check if the micro controller is running. 
- Issuing a halt command when micro controller already halted cause protocol
  break. This can only be fixed by doing a 'reset'.
- Timing of official FT232R(L) chips is broken

Usage Examples
--------------
- Identify device

        # si4010prog identify
        Device ID: 0x24; Revision ID: 0x02

- Load program into xram and run:

        # si4010prog reset prg:test.ihx run
        Resetting SI4010
        Programming CODE memory using "test.ihx"
        Resuming SI4010 MCU

- Halt running process, get program counter, and dump 16 byte at address 0x08

        # si4010prog halt getpc dram:0x8,16
        Halting SI4010 MCU
        Getting program counter:
        0x01f1
        Dumping 16 bytes of RAM at 0x08:
          0x0000 01daa1a2a3000000 0000000000000000                                      ................

NVM programming
---------------
See 'nvm_programming.markdown'
