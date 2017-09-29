RS422 R&uuml;chmelder
=====================

Ziel: einfach (leichter Nachbau), preiswert und robust / kompatibel

Vorteile:
- diffenrentieller Bus -> St&ouml;rsicherheit
- einfach und g&uuml;nstig im Aufbau
- jeder RS232 kann durch einen Transceiver (z.B. SN75179) auf RS422 umger&uuml;stet werden

Ideensammlung:
--------------
- Ersatz des S88 Busses durch RS422
- ASCII Protokoll SLCAN -> M*rklin CAN bzw. XnTCP
- gerichtet wie S88 mit Richtung auf - zur Zentrale - und ab - von der Zentrale weg -
- Aufbau mit RJ45 incl. Stromversorgung (Inline Power &uuml;ber nicht benutzte Adern)
- Adernpaare wie Fastethernet 12 36 / "PoE" &uuml;ber restlich Adern
- Umsetzung auf Ethernet (can2udp bzw. XnTCPd)

Platform
--------
STM32 Blue-Pill
SN75179
Router A5-V11 - f&uuml;r LAN
libopencm3

Aber auch andere Paltformen - bevorzugt MCUs mit 2xUART







