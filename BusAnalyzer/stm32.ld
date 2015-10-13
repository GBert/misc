/* Define memory mapping */
MEMORY
{
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
  rom (rx) : ORIGIN = 0x08000000, LENGTH = 64K
}

OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

PROVIDE(_estack = _sdata + LENGTH(ram));

/* These force the linker to search for particular symbols from
 * the start of the link process and thus ensure the user's
 * overrides are picked up
 */
SECTIONS
{
    /* Align isr vector table at 0x0*/
    . = 0x0;
    .isr_vector : 
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >rom

    .text : {
    CREATE_OBJECT_SYMBOLS
        . = ALIGN(4);
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
        _sidata = .;
    } >rom
    
    .data : AT ( _etext ) 
    {
        . = ALIGN(4);
        _sdata = .; /* Provide start of data section*/
        *(.data)    
        *(.data.*)
        . = ALIGN(4);
        _edata = .; /* Provide end of data section */
            
    } >ram
    
    .bss : 
    {
        . = ALIGN(4);
        _sbss = .; /* Provide start of bss section*/
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .; /* Provide end of bss section */
    } >ram
}

