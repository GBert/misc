@echo off

set OCD_ROOT=C:\Programme\OpenOCD\0.2.0\

rem if "%1" == "" goto errorBinary

rem %OCD_ROOT%\bin\openocd.exe -d0 -f %OCD_ROOT%\interface\olimex-arm-usb-ocd.cfg -f %OCD_ROOT%\target\stm32.cfg
%OCD_ROOT%\bin\openocd.exe -d0 -f %OCD_ROOT%\interface\olimex-arm-usb-ocd.cfg -f %OCD_ROOT%\target\stm32.cfg

if errorlevel 1	goto errorDebug

goto end

:errorBinary
echo Error: No binary specified!
goto end

:errorDebug
echo Error: Flashing controller failed!
goto end

:end
