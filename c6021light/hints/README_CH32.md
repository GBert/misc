Working with STM32 CLone CH32F103
=================================


OpenOCD
-------

```
--- ch32f1x.cfg	2021-03-01 12:49:43.252004689 +0100
+++ stm32f1x.cfg	2021-01-08 19:14:27.257735640 +0100
@@ -31,7 +31,7 @@
       set _CPUTAPID 0x3ba00477
    } {
       # this is the SW-DP tap id not the jtag tap id
-      set _CPUTAPID 0x2ba01477
+      set _CPUTAPID 0x1ba01477
    }
 }
```

```
openocd -d0  -f stlink-v2.cfg -f ch32f1x.cfg -c "program stm32fxx-diagnostics-v1.5.bin verify reset exit 0x08000000"
```
 
