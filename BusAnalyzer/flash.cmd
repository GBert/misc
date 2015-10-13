@echo off

set OCD_ROOT=C:\Programme\OpenOCD\0.2.0\

if "%1" == "" goto errorBinary

rem %OCD_ROOT%\bin\openocd.exe -d0 -f %OCD_ROOT%\interface\olimex-arm-usb-ocd.cfg -f %OCD_ROOT%\target\stm32.cfg -c init -c "reset halt" -c "flash write_image erase %1.elf" -c "verify_image %1.elf" -c "reset run" -c shutdown
%OCD_ROOT%\bin\openocd.exe -d0 -f %OCD_ROOT%\interface\olimex-arm-usb-ocd.cfg -f %OCD_ROOT%\target\stm32.cfg -c init -c "reset halt" -c "flash write_image erase %1.elf" -c "verify_image %1.elf" -c "reset run" -c shutdown

if errorlevel 1	goto errorFlash

goto end

:errorBinary
echo Error: No binary specified!
goto end

:errorFlash
echo Error: Flashing controller failed!
goto end

:end
