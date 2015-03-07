@echo OFF

:begin

pk2cmd -ppic10f200 -fC:\Walter\PICKit2\Testing\PIC10F200,204 testfile.HEX -m

if not errorlevel 37 goto err36
echo ERRRORLEVEL 37
goto end

:err36
if not errorlevel 36 goto err34
echo ERRRORLEVEL 36
goto end

:err34
if not errorlevel 34 goto err28
echo ERRRORLEVEL 34
goto end

:err28
if not errorlevel 28 goto err24
echo ERRRORLEVEL 28
goto end

:err24
if not errorlevel 24 goto err15
echo ERRRORLEVEL 24
goto end

:err15
if not errorlevel 15 goto err11
echo ERRRORLEVEL 15
goto end

:err11
if not errorlevel 11 goto err10
echo ERRRORLEVEL 11
goto end

:err10
if not errorlevel 10 goto err7
echo ERRRORLEVEL 10
goto end

:err7
if not errorlevel 7 goto err5
echo ERRRORLEVEL 7
goto end

:err5
if not errorlevel 5 goto err0
echo ERRRORLEVEL 5
goto end

:err0
echo ERRRORLEVEL 0

:end