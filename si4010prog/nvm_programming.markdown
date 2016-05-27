Write Non-Volatile Memory(NVM)
==============================
The Si4010 contains 8 KB of non-volatile, one time programmable, memory.
si4010prog is able to write firmware in the NVM, but does require some extra
dependencies:

- Silicon Labs Si4010 Burn Tools Package. This can be downloaded from the
  Silicon Labs web page.
- 6.5 Volt power supply connected to GPIO0
- EZ-USB FX2 based C2 interface (FT232 interface currently doesn't work)

**WARNING:** The process of writing NVM is irreversible. This also applies to
the device state and device flags.

**WARNING:** Be prepared to brick at least a few chips. Although you don't have
to you very probably will at some point.

Preparing burn file
-------------------
To burn a firmware file into the NVM a NVM burn file(NBF) must be prepared
first. This can be done using the *gui_composer.exe* utility from the Silicon
Labs Si4010 Burn Tools. For a detailed description on how to use this tool see
chapter 12 of AN674 - "Si4010 NVM Burning Tools and Flows".

In general you probably want to burn the firmware to an empty device and set
the right fuses to automatically start the firmware upon power on. To do this
use the following command:

    C:\Silabs\MCU\Si4010_NVMProgUtil\gui_composer.exe --nvm_burn_file=my_output.nbf --boot_hex=my_firmware.hex 0xe180 --state=user --bf_exe_user_boot

This command will take *my_firmware.hex* as its input and will output the file
*my_output.nbf*. It sets the device into 'user' state(--state=user) and enables
the automatic execution of the firmware upon power up(--bf\_exe\_user\_boot).
Setting the device into the 'user' state with the 'exe\_user\_boot' flag,
instead of the 'run' state, will still allows debugging of the application
through the C2 port.

**WARNING:** Changing device state and flags is irreversible. 

Burn NBF file
-------------
To burn the NBF file to the device, first extract the special burning firmware
from the NBF file. Open the NBF file in a text editor. Find the 'File 1'
section. This section contains Intel HEX encoded data. Copy the Intel HEX data
into a new file. For simplicity I'll refer to this file as burn.hex from here
on.  
TODO: Incorporate this step into si4010prog

Now enable the 3.3 Volt power supply to the SI4010. Once this power supply is
up bring up the 6.5 Volt power supply on GPIO0.

**Warning:** Make sure the 3.3 Volt power supply is enabled when the 6.5 Volt
power supply is enabled!

Now load the burn.hex file into Si4010 using si4010prog and run it:

    # si4010prog -d fx2:// reset prg:burn.hex run

The burning firmware will now burn your firmware into NVM memory. It will
communicate its state on the GPIO pins. The following states are possible:

    | GPIO2  | GPIO3 | Meaning
    |-----------------------------------------------
    |   1    |   1   | Busy
    |   0    |   1   | Burning Succesful
    |   1    |   0   | Burning Failed
    |   0    |   0   | Invalid(burning firmware not run?)

If the burning failed an error code can be retrieved from XRAM at address
0x3EE. The following status codes are defined:

> - 1 = XDATA Address Overflow  
>     The burner program moved out of the valid XDATA address range. This
>     should never happen.  
> - 2 = Bit Conflict  
>     The user was running the burner in strict mode and a 0 was scheduled to
>     be burned into a location that already had a 1.  
> - 3 = Exceeded Burn Threshold  
>     This error occurs when a given bit in the NVM has not successfully burned
>     after 10 attempts.  
> - 4 = Burn Outside Valid NVM Address  
>     The burner has been instructed to burn an NVM address which is outside
>     the valid NVM address range. Last 64 bytes of NVM are reserved for
>     Silicon Labs production test (PT). User code is not allowed to be burned
>     there.  
> *-- Silicon Labs, AN674; Table 12. "GUI Burner Error Codes"*

Finally don't forget to disable the 6.5 Volt before disabling the 3.3 Volt.

Patching NVM firmware
---------------------
The NVM allows setting single bits from 0 to 1. The block format used to store
a program in NVM also allows adding extra blocks after the initial burning.
This can be used to patch or even fully replace the currently burned firmware.

The current problem is that the gui_composer.exe doesn't offer an option to do
this. So if you want this you'll have to manually construct a NVM burning
firmware. In other words: Yes it is physically possible but actually doing it
is currently not supported.

Technical details
-----------------
Most of the NVM burning process is pretty well documented by Silicon Labs in
AN674. However the actual burning of the NVM bits is not documented. This is
what I found out about it so far.

A NBF file is just a INI like file with a Intel hex encoded program for the
Si4010 plus some extra information about how the file was generated. The Si4010
progam in the NBF is build up out of the *gui_burn.hex* file with the options /
firmware provided to gui_composer.exe appended.

gui_burn.hex is the actual program code that burns data into NVM and
communicates the status back to the programmer through GPIO pins/XRAM.
This program takes its input at XRAM address 0x500, and returns its error
code in address 0x3EE. The input data is build up as follows:

- 0x500-0x504: Booleans, If true set bit in 0xE003(PROT3_CTRL). If false
  nothing is done. bit mapping:

  - 0x500 => 0x80 (NVM_C2_PROT; NVM Disable)
  - 0x501 => 0x40 (MTP_C2_PROT; MTP Disable)
  - 0x502 => 0x20 (MEM_C2_PROT; RAM Clear)
  - 0x503 => 0x02 (USER_CONT; Exe User Boot)
  - 0x504 => 0x10 (BOOT_XO_ENA; XO Early Enable)

- 0x505: boolean, If true: set PROT0_CTRL.C2_OFF
- 0x506: TODO
- 0x507: bitset, Set PROT0_CTRL.NVM_BLOWN
- 0x508: boolean, If true: set PROT0_CTRL.NVM_WR_PROT
- 0x509: boolean, If true: set PROT0_CTRL.MTP_PROT
- 0x50A-..: Data, Data to write to NVM encoded in a list of blocks

The data at address 0x50A is encoded as follows:

    struct MemBlock {
        uint16_t wLen;
        uint16_t wNVMAddr;
        uint8_t  bData[wLen];
    }

The block list stops when wLen is equal to 0.

The gui_burn.hex program controls the NVM through a number of SFR registers.
These registers are not documented. Based on some guessing the following
registers and possible function has been identified:
    NVM_ADDRL = 0xd1
    NVM_ADDRH = 0xd2
    NVM_CTRL  = 0xd3
    NVM_CFG   = 0xd4
    NVM_DATA  = 0xd6
If this is correct and the meaning of the bits within the registers is unknown.
