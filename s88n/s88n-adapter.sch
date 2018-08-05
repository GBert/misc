EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:s88n-adapter-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "S88N Adapter"
Date "2018-08-03"
Rev "1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RJ45 J1
U 1 1 5B643D34
P 4450 3850
F 0 "J1" H 4650 4350 50  0000 C CNN
F 1 "RJ45" H 4300 4350 50  0000 C CNN
F 2 "w_conn_pc:megb8-8" H 4450 3850 50  0001 C CNN
F 3 "" H 4450 3850 50  0000 C CNN
	1    4450 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 4300 4200 5675
$Comp
L CONN_01X02 P3
U 1 1 5B643FB6
P 3600 4400
F 0 "P3" H 3600 4250 50  0000 C CNN
F 1 "Power" V 3700 4400 50  0000 C CNN
F 2 "w_conn_pt-1,5:pt_1,5-2-3,5-h" V 3600 4400 50  0001 C CNN
F 3 "" H 3600 4400 50  0000 C CNN
	1    3600 4400
	-1   0    0    -1  
$EndComp
Text Label 4200 5675 1    60   ~ 0
DATA
$Comp
L CONN_01X02 P2
U 1 1 5B644093
P 3600 4700
F 0 "P2" H 3600 4850 50  0000 C CNN
F 1 "PWR" V 3700 4700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02" V 3600 4700 50  0001 C CNN
F 3 "" H 3600 4700 50  0000 C CNN
	1    3600 4700
	-1   0    0    1   
$EndComp
Wire Wire Line
	3800 4350 4100 4350
Wire Wire Line
	4100 4300 4100 4650
Text Label 3875 4350 0    60   ~ 0
eVcc
Wire Wire Line
	4300 4300 4300 5675
Wire Wire Line
	3800 4450 4500 4450
Connection ~ 4300 4450
Wire Wire Line
	4400 4300 4400 5675
Text Label 3875 4450 0    60   ~ 0
GND
$Comp
L CONN_01X06 P1
U 1 1 5B644280
P 4450 5875
F 0 "P1" H 4450 6225 50  0000 C CNN
F 1 "S88" V 4550 5875 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x06" H 4450 5875 50  0001 C CNN
F 3 "" H 4450 5875 50  0000 C CNN
	1    4450 5875
	0    -1   1    0   
$EndComp
Wire Wire Line
	4500 4450 4500 4300
Wire Wire Line
	4600 4300 4600 4550
Wire Wire Line
	4600 4550 4500 4550
Wire Wire Line
	4500 4550 4500 5675
Wire Wire Line
	4700 4300 4700 4650
Wire Wire Line
	4700 4650 4600 4650
Wire Wire Line
	4600 4650 4600 5675
Wire Wire Line
	4100 4650 3800 4650
Connection ~ 4100 4350
Wire Wire Line
	3800 4750 4700 4750
Wire Wire Line
	4700 4750 4700 5675
Text Label 4400 5675 1    60   ~ 0
Clock
Text Label 4500 5675 1    60   ~ 0
Load
Text Label 4600 5675 1    60   ~ 0
Reset
NoConn ~ 4800 4300
NoConn ~ 5000 3500
Text Label 4300 5675 1    60   ~ 0
GND
Text Label 4700 5675 1    60   ~ 0
Vcc
$Comp
L Conn_01x06 J2
U 1 1 5B649124
P 5100 5050
F 0 "J2" H 5100 5350 50  0000 C CNN
F 1 "Conn_01x06" V 5100 4650 50  0001 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 5100 5050 50  0001 C CNN
F 3 "" H 5100 5050 50  0001 C CNN
	1    5100 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 4850 4900 4850
Connection ~ 4200 4850
Wire Wire Line
	4900 4950 4300 4950
Connection ~ 4300 4950
Wire Wire Line
	4900 5050 4400 5050
Connection ~ 4400 5050
Wire Wire Line
	4900 5150 4500 5150
Connection ~ 4500 5150
Wire Wire Line
	4900 5250 4600 5250
Connection ~ 4600 5250
Wire Wire Line
	4900 5350 4700 5350
Connection ~ 4700 5350
$EndSCHEMATC
