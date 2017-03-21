# Pickle / UART1 switch mode

https://docs.onion.io/omega2-docs/omega2p.html

Pickle Ports
```
PGC UART1_RXD GPIO#46
PGD UART1_TXD GPIO#45
VPP GPIO#17
```
Pin Muxing
```
0x10000060	25:24 UART1_MODE 	UART1 GPIO mode
		3: SW_R, SW_T
		2: PWM ch0/ch1
		1: GPIO
		0: UART-Lite #1

```
GPIO Mode
```
io -4a 0x10000060 0xfcffffff
io -4o 0x10000060 0x01000000
```
UART1 Mode
```
io -4a 0x10000060 0xfcffffff
```

