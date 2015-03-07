Release Notes for PICkit 2 Command Line Interface
Linux Kernel 2.6 binary

PK2CMD.EXE        v1.20
Device File       v1.55.00 (included)
PICkit 2 Firmware v2.32.00 (minimum required)   

8 January, 2009

Special thanks to the following individuals for their critical contributions
to the development of this software:

Jeff Post, Xiaofan Chen, and Alain Gibaud.

-----------------------------------------------------------------------------
Table of Contents
-----------------------------------------------------------------------------
1.  Tool Definition
2.  Device Support List
3.  Operating System Support List
4.  What's New or Updated
5.  Known Problems
6.  Important Notes
7.  Installation
8.  Upgrading the PICkit 2 Operating System
9.  Command Line Options
10. Command Line Examples
11. Return Codes
12. Customer Support

----------------------------------------------------------------------------
1. Tool Definition
----------------------------------------------------------------------------
PICkit 2 Command Line Interface (PK2CMD) is a 32-bit command-line interface
to the PICkit 2 device programmer.

The PK2CMD executable requires the "PK2DeviceFile.dat" file for execution.  
As root (using 'su' or 'sudo'), copy the file pk2cmd to /usr/local/bin and
the file PK2DeviceFile.dat to /usr/share/pk2.

Your user PATH environment variable should include /usr/share/pk2. If using
bash, edit the file .bashrc to include:
  PATH=$PATH:/usr/share/pk2
  export PATH

Other command line interfaces should have a similar mechanism for setting
your PATH variable.

Optionally, the path to the device file may be explicitly specified on the
command line using option -b (-B).

Please read the file 'usbhotplug.txt' before installing pk2cmd. It explains
how to set up USB udev or hotplug rules so pk2cmd can be run in user mode.
You must also have libusb (http://libusb.sourceforge.net) installed on your
system.  (See the Installation section for more information)

This Readme is targeted to users of the command-line interface. Refer to the
"PICkit 2 User's Guide" for more information about the PICkit 2 Programmer.

----------------------------------------------------------------------------
2. Device Support List
----------------------------------------------------------------------------
Device support is dependent on the device file version installed
with the PICkit 2 device programmer.  For the versions listed above
device support is listed below.

When selecting a part using the "P" option (section 9), use the 
full name from the list below.

+ Indicates parts that require 4.75V minimum VDD for programming.
  PICkit 2 may not be able to generate sufficiently high VDD,
  so an external 5.0v power supply may be required.

# indicates Midrange parts that support low Vdd programming

Baseline Devices
----------------
PIC10F200       PIC10F202       PIC10F204       PIC10F206
PIC10F220       PIC10F222
PIC12F508       PIC12F509       PIC12F510	PIC12F519 
PIC16F505       PIC16F506       PIC16F526 
PIC16F54        PIC16F57        PIC16F59


Midrange/Standard Devices
----------------
>> All 'LF' versions of devices are supported
PIC12F609       PIC12HV609 	
PIC12F615       PIC12HV615      
PIC12F629       PIC12F635#      PIC12F675       PIC12F683#
PIC16F610       PIC16HV610      PIC16F616       PIC16HV616
PIC16F627       PIC16F628       PIC16F639 
PIC16F627A      PIC16F628A      PIC16F648A
PIC16F630       PIC16F631       PIC16F636#      PIC16F676
PIC16F677       PIC16F684#      PIC16F685#      PIC16F687#
PIC16F688#      PIC16F689#      PIC16F690#      
PIC16F72+
PIC16F73+       PIC16F74+       PIC16F76+       PIC16F77+
PIC16F716 
PIC16F737+      PIC16F747+      PIC16F767+      PIC16F777+
PIC16F785       PIC16HV785      
PIC16F84A       PIC16F87#       PIC16F88#
PIC16F818#      PIC16F819# 
PIC16F870       PIC16F871       PIC16F872       
PIC16F873       PIC16F874       PIC16F876       PIC16F877 
PIC16F873A      PIC16F874A      PIC16F876A      PIC16F877A
PIC16F882#
PIC16F883#      PIC16F884#      PIC16F886#      PIC16F887#
PIC16F913#      PIC16F914#      PIC16F916#      PIC16F917#
PIC16F946#

Midrange/1.8V Min Devices
----------------
PIC16F722       PIC16LF722 
PIC16F723       PIC16LF723      PIC16F724       PIC16LF724 
PIC16F726       PIC16LF726      PIC16F727       PIC16LF727 

PIC16F1933      PIC16F1934      PIC16F1936      PIC16F1937
PIC16F1938      PIC16F1939
PIC16LF1933     PIC16LF1934     PIC16LF1936     PIC16LF1937
PIC16LF1938     PIC16LF1939


PIC18F Devices
--------------
>> All 'LF' versions of devices are supported
PIC18F242       PIC18F252       PIC18F442       PIC18F452
PIC18F248       PIC18F258       PIC18F448       PIC18F458
PIC18F1220      PIC18F1320      PIC18F2220  
PIC18F1230      PIC18F1330      PIC18F1330-ICD    
PIC18F2221      PIC18F2320      PIC18F2321      PIC18F2331      
PIC18F2410      PIC18F2420      PIC18F2423      PIC18F2431
PIC18F2450      PIC18F2455      PIC18F2458      PIC18F2480
PIC18F2510      PIC18F2515      PIC18F2520      PIC18F2523  
PIC18F2525      PIC18F2550      PIC18F2553      PIC18F2580
PIC18F2585	
PIC18F2610      PIC18F2620      PIC18F2680      PIC18F2682  
PIC18F2685 
PIC18F4220      PIC18F4221      PIC18F4320      PIC18F4321  
PIC18F4331      PIC18F4410      PIC18F4420      PIC18F4423      
PIC18F4431      PIC18F4450      PIC18F4455      PIC18F4458
PIC18F4480      
PIC18F4510      PIC18F4515      PIC18F4520      PIC18F4523 
PIC18F4525      PIC18F4550      PIC18F4553      PIC18F4580
PIC18F4585
PIC18F4610      PIC18F4620      PIC18F4680      PIC18F4682  
PIC18F4685      PIC18F6310      PIC18F6390      PIC18F6393 
PIC18F6410      PIC18F6490      PIC18F6493      PIC18F6520
PIC18F6525      PIC18F6527      
PIC18F6585      PIC18F6620      PIC18F6621      PIC18F6622
PIC18F6627      PIC18F6628      PIC18F6680      PIC18F6720
PIC18F6722      PIC18F6723 
PIC18F8310      PIC18F8390      PIC18F8393      PIC18F8410
PIC18F8490      PIC18F8493 
PIC18F8520      PIC18F8525      PIC18F8527      PIC18F8585 
PIC18F8620      PIC18F8621      PIC18F8622      PIC18F8627
PIC18F8628
PIC18F8680      PIC18F8720      PIC18F8722	PIC18F8723


PIC18F_J_ Devices
-----------------
PIC18F24J10     PIC18LF24J10     
PIC18F24J11     PIC18LF24J11    PIC18F24J50     PIC18LF24J50
PIC18F25J10     PIC18LF25J10        
PIC18F25J11     PIC18LF25J11    PIC18F25J50     PIC18LF25J50
PIC18F26J11     PIC18LF26J11    PIC18F26J50     PIC18LF26J50
PIC18F44J10     PIC18LF44J10
PIC18F44J11     PIC18LF44J11    PIC18F44J50     PIC18LF44J50
PIC18F45J10     PIC18LF45J10
PIC18F45J11     PIC18LF45J11    PIC18F45J50     PIC18LF45J50
PIC18F46J11     PIC18LF46J11    PIC18F46J50     PIC18LF46J50
PIC18F63J11     PIC18F63J90     PIC18F64J11     PIC18F64J90
PIC18F65J10     PIC18F65J11     PIC18F65J15     PIC18F65J50
PIC18F65J90  
PIC18F66J10     PIC18F66J11     PIC18F66J15     PIC18F66J16 
PIC18F66J50     PIC18F66J55     PIC18F66J60     PIC18F66J65    
PIC18F66J90
PIC18F67J10     PIC18F67J11     PIC18F67J50     PIC18F67J60 
PIC18F67J90
PIC18F83J11     PIC18F83J90     PIC18F84J11     PIC18F84J90 
PIC18F85J10     PIC18F85J11     PIC18F85J15     PIC18F85J50
PIC18F85J90 
PIC18F86J10     PIC18F86J11     PIC18F86J15     PIC18F86J16 
PIC18F86J50     PIC18F86J55     PIC18F86J60     PIC18F86J65 
PIC18F86J90    
PIC18F87J10     PIC18F87J11     PIC18F87J50     PIC18F87J60
PIC18F87J90
PIC18F96J60     PIC18F96J65     PIC18F97J60 


PIC18F_K_ Devices
-----------------
PIC18F13K22     PIC18LF13K22    PIC18F14K22     PIC18LF14K22
PIC18F13K50     PIC18LF13K50    PIC18F14K50     PIC18LF14K50 
PIC18F14K50-ICD 
PIC18F23K20	PIC18F24K20     PIC18F25K20     PIC18F26K20 
PIC18F43K20     PIC18F44K20     PIC18F45K20     PIC18F46K20  


PIC24 Devices
-------------
PIC24FJ16GA002       PIC24FJ16GA004 
PIC24FJ32GA002       PIC24FJ32GA004 
PIC24FJ48GA002       PIC24FJ48GA004 
PIC24FJ64GA002       PIC24FJ64GA004 
PIC24FJ64GA006       PIC24FJ64GA008       PIC24FJ64GA010 
PIC24FJ96GA006       PIC24FJ96GA008       PIC24FJ96GA010 
PIC24FJ128GA006      PIC24FJ128GA008      PIC24FJ128GA010 
PIC24FJ128GA106      PIC24FJ128GA108      PIC24FJ128GA110 
PIC24FJ192GA106      PIC24FJ192GA108      PIC24FJ192GA110 
PIC24FJ256GA106      PIC24FJ256GA108      PIC24FJ256GA110 

PIC24FJ64GB106       PIC24FJ64GB108       PIC24FJ64GB110 
PIC24FJ128GB106      PIC24FJ128GB108      PIC24FJ128GB110 
PIC24FJ192GB106      PIC24FJ192GB108      PIC24FJ192GB110 
PIC24FJ256GB106      PIC24FJ256GB108      PIC24FJ256GB110 

PIC24HJ12GP201       PIC24HJ12GP202 
PIC24HJ16GP304      
PIC24HJ32GP202       PIC24HJ32GP204 
PIC24HJ32GP302       PIC24HJ32GP304
PIC24HJ64GP202       PIC24HJ64GP204
PIC24HJ64GP206       PIC24HJ64GP210       
PIC24HJ64GP502
PIC24HJ64GP504       PIC24HJ64GP506       PIC24HJ64GP510 
PIC24HJ128GP202      PIC24HJ128GP204
PIC24HJ128GP206      PIC24HJ128GP210      
PIC24HJ128GP306      PIC24HJ128GP310      
PIC24HJ128GP502      PIC24HJ128GP504
PIC24HJ128GP506      PIC24HJ128GP510 
PIC24HJ256GP206      PIC24HJ256GP210      PIC24HJ256GP610 


dsPIC33 Devices
---------------
dsPIC33FJ06GS101     dsPIC33FJ06GS102     dsPIC33FJ06GS202
dsPIC33FJ16GS402     dsPIC33FJ16GS404
dsPIC33FJ16GS502     dsPIC33FJ16GS504

dsPIC33FJ12GP201     dsPIC33FJ12GP202 
dsPIC33FJ16GP304    
dsPIC33FJ32GP202     dsPIC33FJ32GP204 
dsPIC33FJ32GP302     dsPIC33FJ32GP304 
dsPIC33FJ64GP202     dsPIC33FJ64GP204 
dsPIC33FJ64GP206     dsPIC33FJ64GP306     dsPIC33FJ64GP310 
dsPIC33FJ64GP706     dsPIC33FJ64GP708     dsPIC33FJ64GP710 
dsPIC33FJ64GP802     dsPIC33FJ64GP804 
dsPIC33FJ128GP202    dsPIC33FJ128GP204 
dsPIC33FJ128GP206    dsPIC33FJ128GP306    dsPIC33FJ128GP310 
dsPIC33FJ128GP706    dsPIC33FJ128GP708    dsPIC33FJ128GP710 
dsPIC33FJ256GP506    dsPIC33FJ256GP510    dsPIC33FJ256GP710 
dsPIC33FJ128GP802    dsPIC33FJ128GP804 

dsPIC33FJ12MC201     dsPIC33FJ12MC202 
dsPIC33FJ16MC304    
dsPIC33FJ32MC202     dsPIC33FJ32MC204 
dsPIC33FJ32MC302     dsPIC33FJ32MC304 
dsPIC33FJ64MC202     dsPIC33FJ64MC204 
dsPIC33FJ64MC506     dsPIC33FJ64MC508     dsPIC33FJ64MC510 
dsPIC33FJ64MC706     dsPIC33FJ64MC710    
dsPIC33FJ64MC802     dsPIC33FJ64MC804 
dsPIC33FJ128MC202    dsPIC33FJ128MC204  
dsPIC33FJ128MC506    dsPIC33FJ128MC510    dsPIC33FJ128MC706 
dsPIC33FJ128MC708    dsPIC33FJ128MC710 
dsPIC33FJ256MC510    dsPIC33FJ256MC710 
dsPIC33FJ128MC802    dsPIC33FJ128MC804 


dsPIC30 Devices
---------------
dsPIC30F2010         dsPIC30F2011         dsPIC30F2012
dsPIC30F3010         dsPIC30F3011         dsPIC30F3012
dsPIC30F3013         dsPIC30F3014 
dsPIC30F4011         dsPIC30F4012         dsPIC30F4013
dsPIC30F5011^        dsPIC30F5013^        dsPIC30F5015
dsPIC30F5016 
dsPIC30F6010A        dsPIC30F6011A        dsPIC30F6012A
dsPIC30F6013A        dsPIC30F6014A        dsPIC30F6015

^ These 2 devices are not supported for low VDD programming.


dsPIC30 SMPS Devices
--------------------
dsPIC30F1010
dsPIC30F2020         dsPIC30F2023


PIC32 Devices
--------------------
PIC32MX320F032H      PIC32MX320F064H      PIC32MX320F128L 
PIC32MX320F128H 
PIC32MX340F128H      PIC32MX340F128L
PIC32MX340F256H 
PIC32MX360F256L      PIC32MX360F512L 
PIC32MX420F032H
PIC32MX440F128L      PIC32MX440F128H
PIC32MX440F256H      PIC32MX440F512H
PIC32MX460F256L      PIC32MX460F512L 




KEELOQ HCS Devices
------------------
HCS200     HCS201     HCS300     HCS301      HCS320 
HCS360     HCS361     HCS362 

	HCSxxx File -> Import HEx Notes:
		The first line only may be imported from SQTP
                *.NUM files generated by the KEELOQ tool in 
                MPLAB.

        Connections for HCS devices
        ---------------------------------------
        PICkit 2 Pin             HCS Device Pin
        (2) Vdd                  8
        (3) GND                  5
        (5) PGC                 /3            HCS20x, 320
                                \3 -or- 4     HCS30x, 36x
        (4) PGD                  6
        (1) VPP                  2            HCS360, 361 only


MCP250xx CAN Devices
--------------------
MCP25020       MCP25025 
MCP25050       MCP25055 

!!IMPORTANT!! - MCP250xx devices are OTP and can only be
                programmed once.

        Connections for MCP250xx devices
        ---------------------------------------
        PICkit 2 Pin             MCP Device Pin (DIP)
	(1) Vpp                  11 Vpp
        (2) Vdd                  14 VDD
		- The MCP device MUST be powered from PICkit 2!
        (3) GND                  7 Vss
        (4) PGD                  5 DATA
        (5) PGC                  6 CLOCK



Serial EEPROM Devices
---------------------
NOTE: Other supported voltage grades are listed in parentheses
      next to the device.  Select the "LC" part number to program
      these other voltage grades.

11LC010 (AA) 
11LC020 (AA) 
11LC040 (AA) 
11LC080 (AA) 
11LC160 (AA) 

24LC00   (AA)(C)                  25LC010A (AA) 
24LC01B  (AA)                     25LC020A (AA) 
24LC02B  (AA)                     25LC040A (AA) 
24LC04B  (AA)                     25LC080A (AA) 
24LC08B  (AA)                     25LC080B (AA) 
24LC16B  (AA)                     25LC160A (AA) 
24LC32A  (AA)                     25LC160B (AA) 
24LC64   (AA)(FC)                 25LC320A (AA)
24LC128  (AA)(FC)                 25LC640A (AA) 
24LC256  (AA)(FC)                 25LC128  (AA) 
24LC512  (AA)(FC)                 25LC256  (AA) 
24LC1025 (AA)(FC)                 25LC512  (AA) 
                                  25LC1024 (AA) 


93LC46A/B/C  (AA)(-C) 
93LC56A/B/C  (AA)(-C) 
93LC66A/B/C  (AA)(-C) 
93LC76A/B/C  (AA)(-C) 
93LC86A/B/C  (AA)(-C) 

        Connections for 11LC devices
        ---------------------------------------
        PICkit 2 Pin             11LC Device Pin (DIP)
        (2) Vdd !                8 Vcc
        (3) GND                  4 Vss
        (6) AUX                  5 SCIO

	! 11LC devices may not program properly below 3.6V VDD.
          This is a limitation of the PICkit 2 AUX IO pin.


        Connections for 24LC devices
        ---------------------------------------
        PICkit 2 Pin             24LC Device Pin (DIP)
        (2) Vdd !                8 Vcc
        (3) GND                  4 Vss
        (5) PGC                  6 SCL (driven as push-pull)
        (6) AUX                  5 SDA (requires pullup)
                                 7 WP - disabled (GND)
                                 1, 2, 3 Ax pins
                                    Connect to Vdd or GND per
                                    datasheet and to set address

	! 24LC devices may not program properly below 3.6V VDD.
          This is a limitation of the PICkit 2 AUX IO pin.


        Connections for 25LC devices
        ---------------------------------------
        PICkit 2 Pin             25LC Device Pin (DIP)
        (1) VPP                  1 nCS
        (2) Vdd                  8 Vcc
        (3) GND                  4 Vss
        (4) PGD                  2 SO
        (5) PGC                  6 SCK
        (6) AUX                  5 SI
                                 7 nHOLD - disabled (Vdd)
                                 3 nWP - disabled (Vdd)


        Connections for 93LC devices
        ---------------------------------------
        PICkit 2 Pin             93LC Device Pin (DIP)
        (1) VPP                  1 CS
        (2) Vdd                  8 Vcc
        (3) GND                  5 Vss
        (4) PGD                  4 DO
        (5) PGC                  2 CLK
        (6) AUX                  3 DI
                                 7 PE - enabled (Vdd)
                                 6 'C' Device ORG 
                                    Set to select word size



----------------------------------------------------------------------------
3. Operating System Support List (Tested)
----------------------------------------------------------------------------
The pk2cmd binary for kernel 2.6 was built on CentOS 4.7 (RHEL 4.7). It has
been tested on the following systems:
CentOS 4.7
CentOS 5.1
Ubuntu 6.06
Ubuntu 7.10
Ubuntu 8.04
Ubuntu 8.10
Fedora Core 6
Fedora 9.

It is recommended that you build a binary from the source code.  The source
code is downloadable from the Microchip PICkit 2 webpage, and includes build
and installation instructions:
www.microchip.com/pickit2

----------------------------------------------------------------------------
4. What's New or Updated
----------------------------------------------------------------------------
>>> Version 1.20 <<<
NEW FEATURES:
- Faster PIC24, dsPIC33 programming
    PIC24 and dsPIC33 devices now use a Programming Executive (aka Enhanced
    ICSP) for faster programming.  The -Q option may be used to disable use
    of the PE, and revert programming to basic ICSP.  The PIC24H/dsPIC33
    Device ID corruption workaround is implemeted when using the PE.
    ** NOTE: Using the PE may fail on certain programming ports of 44-pin
             devices, especially PGC3/PGD3.  If an error is frequently 
             encountered, try another port or use -Q to disable the PE.

-?P New Help Command
    The help command -?P will list all supported devices in the current
    device file, along with the device's family.  Optionally, a string
    may be specified to search for devices beginning with the string.
    For example, -?PPIC16 will list all devices supported by the current
    device file beginning with "PIC16".

-P Command with Auto-Detect
    The -P command can now be used for auto detection of most microcontroller
    devices.  Note that not all supported parts can be auto detected.
    See the online command help (-p?) for more information.  A new return
    code AUTODETECT_FAILED may be returned when using auto detection.
    (See section 11.)
    ** NOTE: For integration with GUIs or other software, to avoid damage 
             to parts by VPP voltage levels above that rated for the part
             the recommended method for auto detection is:
             A) Get and cache a list of supported devices and families with
                -?P
             B) Get and cache a list of auto detectable families and IDs
                with -PF
             c) Use A and B to determine the family ID for a given part.
                Auto detect this part using -PFn where n is the family ID.

-J New Command
    The new -J option will display the percentage operation complete instead
    of a rotating slash for Write, Verify, Read, and Blank Check operations.

-L New Command
    The -L option allows the ICSP programming clock rate to be changed 
    (slowed).  This can increase programming times but allow programming to
    work on heavily loaded ICSP signals or with longer cables that are
    otherwise intermittent or fail.  Using -L is similar to disabling the
    "Fast Programming" option in the PICkit 2 Programmer GUI software.

-B New Command
    The -B command allows the path to the device file PK2DeviceFile.dat
    to be specified on the command line.  See -B? for more information.

-I shows device name
    The -I command now displays the device name associated with the read
    Device ID, and warns if it is different from the selected device.

- Binary file import / export
    For serial EEPROM devices only, binary format files may be loaded with
    -F for programming or read to with -GF.  See the online help for these
    commands, -F? and -G?.

- Faster Writes of PIC18F6xJxx and PIC18F8xJxx devices.

BUG FIXES:
- Fixed an issue that could cause verify or read lockups with certain USB
  chipsets, that often affected Apple MacOS machines.
- PIC32 PE (Programming Executive) updated to fix programming problems with
  some devices.
- A bug in previous versions caused serial EEPROM devices to not be verified
  during programming with -M.  Serial EEPROMs are now properly verified
  on a -M command.
- When no -T command is specified, PK2CMD will always exit with VDD off if
  a -P command is present.
- When no -R command is specified, PK2CMD will always exit with MCLR on if
  a -P command is present.
- An issue with displaying 14 character or longer Unit IDs was corrected.
  Unit ID length is now limited to 14 characters with -N.
- Baseline device checksums are now computed correctly (matching MPLAB) when
  Code Protect is enabled.
- PK2CMD now supports Device Files with more than 500 parts properly.
- Fixed an issue with -GF file names being created with an extraneous space
  at the end in some cases.


>>> Version 1.12 <<<<
Bug fix for "-s" option allows simultaneous operation of multiple instances
of PK2CMD and multiple PICkit 2 units.

Changes to "-s" option:
     See online help "pk2cmd -s?"

Bug fix for PIC32 programming with blank boot flash.

Requires OS firmware v2.32.00 or later


>>> Version 1.10 <<<<
-N option for assigning a Unit ID string to a PICkit 2 unit.

Support for multiple PICkit 2 units with 1 PC:
-S option to list all connected PICkit 2 units, or to select a particular
   unit for use by the Unit ID.

-M or -MP commands will only verify the programmed portions of Program
   Memory to reduce total programming time.  (This does not apply to
   verification with -Y or -YP)

PIC18 J-Series and PIC24FJ devices have updated handling of Configuration
   words to match MPLAB IDE and PICkit 2 Programmer v2.5x

New support for PIC32, 11LCxxx, and MCP250xx devices.

Now allows spacing between command options and parameters.
Ex: -pPIC16F887 and -p PIC16F887 are both valid.
For -GP and -GE option types, spaces are not allowed in the address ranges.
EX:
  Supported:     -G P 0-F, -GP0-F, -G P0-F, -GP 0-F
  Not Supported: -G P 0 - F, -GP0 -F, -GP0- F, or other combinations with
                                                  spaces in the range

----------------------------------------------------------------------
5. Known Problems
----------------------------------------------------------------------

When using option "-s" to list PICkit 2 units, it will display units
with firmware older than v2.30.00 as being in the bootloader with
a Unit ID of <bootloader>.
 
Use "-s#" to accurately list units with older firmware distinctly from
units in the bootloader.

The Programming Executive (PE) support for PIC24 and dsPIC33 devices may not
work with some programming ports (PGx3 usually) on some 44-Pin devices.  If
errors are frequently encountered, try using a different programming port or
using -Q to disable the PE.  Parts that may have this issue include but are
not limited to:
PIC24HJ16GP304, PIC24HJ32GP204
dsPIC33FJ16GP304, dsPIC33FJ32GP204, dsPIC33FJ16MC304, dsPIC33FJ32MC204

----------------------------------------------------------------------------
6. Important Notes
----------------------------------------------------------------------------
-------------------------
Note on -R and -T options
-------------------------

Four different cases requiring different uses of -R and -T to run after
programming:

1) MCU is externally powered, and MCLR pin is configured as IO.
      Switches needed to run after programming: NONE
2) MCU is externally powered, and MCLR pin is configured as MCLR
      Switches needed to run after programming: -R
3) MCU needs to be powered from PICkit 2, MCLR configured as IO.
      Switches needed to run after programming : -T
4) MCU needs to be powered from PICkit 2, MCLR configured as MCLR.
      Switches needed to run after programming: -R -T

----------------------------------------------------------------------------
7. Installation
----------------------------------------------------------------------------
It is recommended that you build a binary from the source code.  The source
code is downloadable from the Microchip PICkit 2 webpage, and includes build
and installation instructions:
www.microchip.com/pickit2

After building run "make install" as root (using 'su' or 'sudo'). See below
for manual installation.

The method to run pk2cmd as a normal user (not root) depends on your
particular Linux distribution. Please consult your Linux distribution
documentation or use the support option as detailed in Section 12 of this
documentation if you can not get it to work.

Please read the file 'usbhotplug.txt' before installing pk2cmd. It explains
how to set up USB udev or hotplug rules so pk2cmd can be run in user mode.

You will need libusb runtime to run pk2cmd.  Typical linux distros will have
libusb-0.1 package installed already, so no additonal installation is
needed. If not, use your linux distro's package management software to
install it (typically named libusb).

If you are running a very old Linux version which only includes an older
version of libusb (older than 0.1.10), then it is better you install
libusb-0.1 from the website below. In this case, you will have both the
runtime library and the development package installed in the same time.

You can download libusb-0.1 from http://libusb.sourceforge.net. See the
README and INSTALL files for instructions on how to install libusb on your
system.

The following describes manual installation, including for using a pre-built
binary:

As root (using 'su' or 'sudo'), copy the file pk2cmd to /usr/local/bin and
the file PK2DeviceFile.dat to /usr/share/pk2.

Your user PATH environment variable should include /usr/share/pk2. If using
bash, edit the file .bashrc to include:
  PATH=$PATH:/usr/share/pk2
  export PATH

Other command line interfaces should have a similar mechanism for setting
your PATH variable.

If you continue to have problems with pk2cmd not finding PK2DeviceFile.dat,
try copying the dat file to /usr/local/bin manually as a last resort.

Optionally, the path to the device file may be explicitly specified on the
command line using option -b (-B).

----------------------------------------------------------------------------
8. Upgrading the PICkit 2 Operating System
----------------------------------------------------------------------------
If the executable detects an incompatible OS, an updated operating system
may be downloaded using the -D command line option.  The latest OS firmware
can be downloaded from the PICkit 2 webpage at www.microchip.com/pickit2

----------------------------------------------------------------------------
9. Command Line Options
----------------------------------------------------------------------------
The following commands are available in the command line interface.

                          PICkit 2 COMMAND LINE HELP
Options              Description                              Default
----------------------------------------------------------------------------
A<value>             Set Vdd voltage                          Device Specific
B<path>              Specify the path to PK2DeviceFile.dat    Searches PATH
                                                              and calling dir
C                    Blank Check Device                       No Blank Check
D<file>              OS Download                              None
E                    Erase Flash Device                       Do Not Erase
F<file>              Hex File Selection                       None
G<Type><range/path>  Read functions                           None
                     Type F: = read into hex file,
                             path = full file path,
                             range is not used
                     Types P,E,I,C: = ouput read of Program,
                             EEPROM, ID and/or Configuration
                             Memory to the screen. P and E
                             must be followed by an address
                             range in the form of x-y where
                             x is the start address and y is
                             the end address both in hex,
                             path is not used
                             (Serial EEPROM memory is 'P')
H<value>             Delay before Exit                        Exit immediately
                         K = Wait on keypress before exit
                         1 to 9 = Wait <value> seconds
                                  before exit
I                    Display Device ID & silicon revision     Do Not Display
J<newlines>          Display operation percent complete       Rotating slash
                         N = Each update on newline
K                    Display Hex File Checksum                Do Not Display
L<rate>              Set programming speed                    Fastest
                     <rate> is a value of 1-16, with 1 being
                     the fastest.
M<memory region>     Program Device                           Do Not Program
                     memory regions:
                         P = Program memory
                         E = EEPROM
                         I = ID memory
                         C = Configuration memory
                         If no region is entered, the entire
                         device will be erased & programmed.
                         If a region is entered, no erase
                         is performed and only the given 
                         region is programmed.
                         All programmed regions are verified.
			 (serial EEPROM memory is 'P')
N<string>            Assign Unit ID string to first found     None
                     PICkit 2 unit.  String is limited to 14
                     characters maximum.  May not be used
                     with other options.
                     Example: -NLab1B
P<part>              Part Selection. Example: -PPIC16f887     (Required)
P                    Auto-Detect in all detectable families
PF                   List auto-detectable part families
PF<id>               Auto-Detect only within the given part
                     family, using the ID listed with -PF
                     Example: -PF2
Q                    Disable PE for PIC24/dsPIC33 devices     Use PE
R                    Release /MCLR after operations           Assert /MCLR
S<string/#>          Use the PICkit 2 with the given Unit ID  First found unit
                     string.  Useful when multiple PICkit 2
                     units are connected.
                     Example: -SLab1B
                     If no <string> is entered, then the
                     Unit IDs of all connected units will be
                     displayed.  In this case, all other
                     options are ignored. -S# will list units
                     with their firmware versions.
                     See help -s? for more info.
T                    Power Target after operations            Vdd off
U<value>             Program OSCCAL memory, where:            Do Not Program
                      <value> is a hexidecimal number
                      representing the OSCCAL value to be
                      programmed. This may only be used in
                      conjunction with a programming 
                      operation.
V<value>             Vpp override                             Device Specific
W                    Externally power target                  Power from Pk2
X                    Use VPP first Program Entry Method       VDD first
Y<memory region>     Verify Device                            Do Not Verify
                         P = Program memory
                         E = EEPROM
                         I = ID memory
                         C = Configuration memory
                         If no region is entered, the entire
                         device will be verified.
                         (Serial EEPROM memory is 'P')
Z                    Preserve EEData on Program               Do Not Preserve
?                    Help Screen                              Not Shown

     Each option must be immediately preceeded by a switch, Which can
     be either a dash <-> or a slash </> and options must be separated
     by a single space.

     Example:   PK2CMD /PPIC16F887 /Fc:\mycode /M
                               or
                PK2CMD -PPIC16F887 -Fc:\mycode -M

     Any option immediately followed by a question mark will invoke
     a more detailed description of how to use that option.

     Commands and their parameters are not case sensitive. Commands will
     be processed according to command order of precedence, not the order
     in which they appear on the command line. 
	Precedence:
                -?      (first)
                -B
		-S
                -D	
		-N
                -P
                -A -F -J -L -Q -V -W -X -Z
                -C
                -U
                -E
                -M
                -Y
                -G
                -I -K   
		-R -T
		-H      (last)
		

     The program will return an exit code upon completion which will
     indicate either successful completion, or describe the reason for
     failure. To view the list of exit codes and their descriptions,
     type -?E on the command line.

     type -?V on the command line for version information.

     type -?L on the command line for license information.

     type -?P on the command line for a listing of supported devices.
     type -?P<string> to search for and display a list of supported devices
                      beginning with <string>.

----------------------------------------------------------------------------
10. Command Line Examples
----------------------------------------------------------------------------
COMMAND LINE
   OPERATION PERFORMED
-----------------------------------------------------------------------------
PK2CMD /?
   Help screen displayed.

PK2CMD /?E
   All eXit (error) codes displayed.

PK2CMD /?V
   Display version information for the PK2CMD executable, PK2DeviceFile.dat,
   and the OS firmware of the connected PICkit 2 programmer.

PK2CMD /M?
   Help for the /M option displayed.

PK2CMD /DPK2V022000.hex
   Firmware file PK2V022000.hex downloaded to PICkit 2

PK2CMD /PPIC16F887 /FC:\PROJECT\PROGRAM.HEX /MP
   PICkit 2 set to a PIC16F887. Vdd and Vpp set to the device's
   default value.  File PROGRAM.HEX is loaded. The device's 
   program memory is programmed, with blank values 
   in the locations not specified by PROGRAM.HEX.  NOTE that
   the program memory is NOT erased first.

PK2CMD /PPIC16F887 /FC:\PROJECT\PROGRAM.HEX /M -A3.3
   PICkit 2 set to a PIC16F887. Vdd is set to 3.3 Volts and 
   Vpp set to the device's default value.  File PROGRAM.HEX 
   is loaded. The entire device is programmed, with blank values 
   in the locations not specified by PROGRAM.HEX.  NOTE that
   the entire device is erased before programming.

PK2CMD /PdsPIC30F6014A /FC:\PROJECT\HEXCODE.HEX /Y
   PICkit 2 set to a dsPIC30F6014A. Voltages are set
   to the device's default values. The device is verified 
   against the contents of HEXCODE.HEX.
   NOTE that any programming operation -M<> verifies the
   data programmed automatically.

PK2CMD /PdsPIC30F6014A /FC:\PROJECT\HEXCODE.HEX /M /Y
   PICkit 2 set to a dsPIC30F6014A. The file HEXCODE.HEX 
   is loaded. Voltages are set to the device's default 
   values and the device is programmed.
   After programming, the device is verified against the 
   contents of HEXCODE.HEX.

   Note: The Program (/M) operation implicitly performs a
   Verify when it completes the programming portion of the 
   operation. Specifying a Verify (/Y) in the above manner 
   effectively causes two Verify operations to be performed.

PK2CMD /P18F4220 /FC:\PROJECT\APPCODE.HEX /Z /M
   PICkit 2 is set to a P18F4220. The file APPCODE.HEX is 
   loaded. Voltages are set to the device's default values. 
   All of the device's memory is programmed, with blank values
   in the locations not specified by APPCODE.HEX. 
   The /z switch specifies PICkit 2 to preserve EEData.

PK2CMD /P18F4220 /FC:\PROJECT\APPCODE.HEX  /E /ME
   PICkit 2 is set to a P18F4220. The file APPCODE.HEX is 
   loaded. Voltages are set to the device's default values.
   All of the device's memory is erased before the device's
   Program Memory is programmed, with blank values in the 
   locations not specified by APPCODE.HEX. 

PK2CMD /P24LC128 /GP0-FF
   PICkit 2 is set to a 24LC128 serial EEPROM. Voltages are
   set to the device's default values. Device memory from 
   location 0x00 to 0xFF is displayed.

PK2CMD /P24LC128 /GFC:\PROJECT\EEVALUES.HEX
   PICkit 2 is set to a 24LC128 serial EEPROM. Voltages are
   set to the device's default values. The entire Device 
   memory is read and saved in the hex file EEVALUES.HEX
   at the given path.


PK2CMD /SLab1A /P24LC128 /GP0-FF
   The PICkit 2 with Unit ID "Lab1A" is set to a 24LC128
   serial EEPROM. Voltages are set to the device's default
   values. Device memory from location 0x00 to 0xFF is
   displayed from the serial EEPROM connected to this
   particular PICkit 2 unit.

Notes
-----

If downloading firmware, all other commands are ignored.
Firmware cannot be downloaded if more than 1 PICkit 2 unit
is connected to the PC.

If assigning a Unit ID using -N, all other commands are
ignored.

If displaying all available PICkit 2 units (-S), all other
commands are ignored.

Use -S with a string parameter, to execute the operations
on one of several PICkit 2 units with the Unit ID matching
the string paramter. 

A device (-P) must be specified for any PICkit 2 operation,
unless an auto detect method is used.

It is necessary to load a hex file every time a device
is programmed.  

All values not specified in the hex file are set to blank
(erased) values.

Some Midrange, PIC18F, and dsPIC30F parts require a row-erase
procedure to erase the chip at Vdd < 4.5 Volts.  This will be
done automatically if the selected device supports this function
and the Vdd setpoint or detected target Vdd is below 4.5V.

A warning will be issued for devices that only support bulk erases
at Vdd >= 4.5 Volts if an erase is attempted at a lower voltage.

The PICkit 2 VDD and VPP voltages can be calibrated using the 
PICkit 2 Programmer GUI application.  The calibration will
remain valid when used with PK2CMD on the same USB port.

----------------------------------------------------------------------------
11. Return Codes
----------------------------------------------------------------------------
Value   Code                    Notes
-----   ----                    -----
0       OPSUCCESS		Returned if all selected operations complete
                                successfully.
5       VOLTAGE_ERROR           A Vdd and/or Vpp voltage error was detected.
                                This could be due to PICkit 2 being 
                                improperly connected to a part, incorrect
                                part selection, or interference from other
                                circuitry on the target board.
7       OPFAILURE               Returned if an operation fails and a more
                                specific error does not exist.
10      NO_PROGRAMMER		The PK2CMD executable is unable to find a
                                connected PICkit 2 programmer.
11      WRONG_OS                Returned if the OS firmware must be updated
                                before being used with this version of
                                PK2CMD.
15      FILE_OPEN_ERROR         Returned if a file specified for reading to
                                (-gf...) cannot be opened for writing.
24      DEVICEFILE_ERROR        The PK2CMD executable cannot find the device
                                file PK2DeviceFile.dat or the device file
                                may be corrupted.
28      UPDGRADE_ERROR          Returned when an OS firmware upgade (-d...)
                                fails.
34      PGMVFY_ERROR            Returned if a program or verify operation
                                fails.
36      INVALID_CMDLINE_ARG	A syntax error in a command line argument
                                was detected, an invalid combination of 
                                operations was entered, an invalid value was
                                entered, or a memory region was selected
                                that is not supported by the current device.
37      INVALID_HEXFILE		Error opening or loading a specified hex
                                file (-f...).
39      AUTODETECT_FAILED       A part autodetect operation failed to find
                                a known part.

----------------------------------------------------------------------------
12. Customer Support
----------------------------------------------------------------------------

This unsupported software is provided for the convenience of Microchip
customers and solely for use with Microchip products.  This software and all
accompanying documentation is provided "as is" and may be downloaded and 
used at your own risk.

Microchip does not offer any support related to the use of this software or
information.

You may be able to obtain assistance from other users of the software on the
Microchip web forums at:
http://forum.microchip.com

Suggested forums for posting about this software are the following:

[Development Tools] "Linux, Open Source Projects and Other Topics"
http://forum.microchip.com/tt.aspx?forumid=182

[Development Tools] "Programmers (MPLAB PM3, PICSTART Plus, PICkit 2)"
http://forum.microchip.com/tt.aspx?forumid=15

Additional help may be obtained from the Pickit Developers mailing list:
pickit-devel@googlegroups.com
