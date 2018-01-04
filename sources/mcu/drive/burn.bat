@echo off
SET AVR_BIN="E:\Dev\WinAVR-20100110\bin"
SET BIN_FILE=CatFeeder
SET COM_PORT=COM4

cd Release
REM "%AVR_BIN%"\avrdude.exe -p m328p -c stk500v1 -b 9600 -v -v -v -e -C "%AVR_BIN%\avrdude.conf" -P %COM_PORT% -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0x05:m
"%AVR_BIN%"\avrdude.exe -p m328p -c stk500v1 -b 9600 -v -v -v -e -C "%AVR_BIN%\avrdude.conf" -P %COM_PORT% -U flash:w:%BIN_FILE%.hex:i
cd ..

pause