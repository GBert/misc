Roco WLAN Maus / Gleisbox
=========================


Loks 
----

WLAN Maus Senden

```
21:27:28.545 ->UDP    len 0x000f ID 0x0040 e9 f1 00 03 00 01 4c 4f 4b 31 00   
21:27:28.724 ->UDP    len 0x000f ID 0x0040 e9 f1 00 03 00 01 4c 4f 4b 31 00
```

WLAN Maus Empfangen
```
0f 00 -- 40 00 -- e9 f1 -- 00 03 -- 00  -- 04   -- 4c 4f 4b 31       -- 66
10 00 -- 40 00 -- ea f1 -- 00 47 -- 01  -- 04   -- 42 52 31 38 32    -- 72
10 00 -- 40 00 -- ea f1 -- 12 67 -- 02  -- 04   -- 53 43 48 4f 4d    -- 32
11 00 -- 40 00 -- eb f1 -- 00 0a -- 03  -- 04   -- 41 42 43 44 45 46 -- 10
LEN   -- CMD1  -- CMD2  -- ADDR  -- IDX -- ANZ  -- NAME als ASCII    -- XOR
``` 

https://www.open4me.de/index.php/2017/06/zz21-wlan-maus-lok-bibliothek-befehle/#more-848

