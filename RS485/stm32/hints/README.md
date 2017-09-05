Problems using BluePill
=======================

```
% st-flash write generic_boot20_pc13.bin 0x8000000
st-flash 1.4.0-6-g7b7a5c1
2017-09-05T10:19:53 INFO src/common.c: Loading device parameters....
2017-09-05T10:19:53 INFO src/common.c: Device connected is: F1 Medium-density device, id 0x20036410
2017-09-05T10:19:53 INFO src/common.c: SRAM size: 0x5000 bytes (20 KiB), Flash: 0 bytes (0 KiB) in pages of 1024 bytes
Unknown memory region
```
```
% st-flash write generic_boot20_pc13.bin 0x8000000
st-flash 1.4.0-6-g7b7a5c1
2017-09-05T10:25:10 INFO src/common.c: Loading device parameters....
2017-09-05T10:25:10 WARN src/common.c: unknown chip id! 0xe0042000
```
Use OpenOCD to disable lock and erase the whole flash (fixes bost issues):
```
# push reset button and release it after openocd started
openocd -d0  -f stlink-v2.cfg -f stm32f1x.cfg -f ocd-unlock.cfg
```

Prices (Aliexpress)
-------------------

```
STLinkV2(clone)	1,50 Euro
Blue-Pill	1,40 Euro
```

