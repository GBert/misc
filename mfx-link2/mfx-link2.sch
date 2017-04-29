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
LIBS:microchip_pic16mcu
LIBS:PIC16F1709
LIBS:mfx-link2-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "mfx-Booster/Link II"
Date "2017-04-25"
Rev "0.1"
Comp "GBe"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PIC16F1709 U?
U 1 1 58FE1162
P 4900 5700
F 0 "U?" H 4900 5250 50  0000 C CNN
F 1 "PIC16F1709" H 4900 5150 50  0000 C CNN
F 2 "w_smd_dil:soic-20" H 4900 6250 50  0001 C CNN
F 3 "" H 4900 5000 50  0001 C CNN
	1    4900 5700
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 58FE11BC
P 3175 5350
F 0 "R?" V 3255 5350 50  0000 C CNN
F 1 "22" V 3175 5350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3105 5350 50  0001 C CNN
F 3 "" H 3175 5350 50  0000 C CNN
	1    3175 5350
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 58FE132A
P 4650 4475
F 0 "C?" H 4675 4575 50  0000 L CNN
F 1 "C" H 4675 4375 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4688 4325 50  0001 C CNN
F 3 "" H 4650 4475 50  0000 C CNN
	1    4650 4475
	1    0    0    -1  
$EndComp
$Comp
L 530x0C T?
U 1 1 58FE2425
P 1775 5550
F 0 "T?" H 1775 5800 50  0000 C CNN
F 1 "53040C" H 1775 5250 50  0000 C CNN
F 2 "Divers:530x0C" H 1775 5550 50  0001 C CNN
F 3 "" H 1775 5550 50  0000 C CNN
	1    1775 5550
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P?
U 1 1 58FE254A
P 1000 5550
F 0 "P?" H 1000 5700 50  0000 C CNN
F 1 "Rail" V 1100 5550 50  0000 C CNN
F 2 "" H 1000 5550 50  0000 C CNN
F 3 "" H 1000 5550 50  0000 C CNN
	1    1000 5550
	-1   0    0    1   
$EndComp
$Comp
L D D?
U 1 1 58FE266D
P 2300 5550
F 0 "D?" H 2300 5650 50  0000 C CNN
F 1 "1N4148" H 2300 5450 50  0000 C CNN
F 2 "w_smd_diode:sod323" H 2300 5550 50  0001 C CNN
F 3 "" H 2300 5550 50  0000 C CNN
	1    2300 5550
	0    -1   -1   0   
$EndComp
$Comp
L D D?
U 1 1 58FE27B5
P 2575 5550
F 0 "D?" H 2575 5450 50  0000 C CNN
F 1 "1N4148" H 2575 5675 50  0000 C CNN
F 2 "w_smd_diode:sod323" H 2575 5550 50  0001 C CNN
F 3 "" H 2575 5550 50  0000 C CNN
	1    2575 5550
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 58FE2988
P 3400 5550
F 0 "R?" V 3480 5550 50  0000 C CNN
F 1 "82" V 3400 5550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3330 5550 50  0001 C CNN
F 3 "" H 3400 5550 50  0000 C CNN
	1    3400 5550
	-1   0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 58FE2A6C
P 2900 5550
F 0 "C?" H 2925 5650 50  0000 L CNN
F 1 "1nF" H 2925 5450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2938 5400 50  0001 C CNN
F 3 "" H 2900 5550 50  0000 C CNN
	1    2900 5550
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 58FE2E72
P 4125 5025
F 0 "#PWR?" H 4125 4875 50  0001 C CNN
F 1 "+5V" H 4125 5165 50  0000 C CNN
F 2 "" H 4125 5025 50  0000 C CNN
F 3 "" H 4125 5025 50  0000 C CNN
	1    4125 5025
	1    0    0    -1  
$EndComp
$Comp
L 7805 U?
U 1 1 58FE3224
P 2150 4375
F 0 "U?" H 2300 4179 50  0000 C CNN
F 1 "7805" H 2150 4575 50  0000 C CNN
F 2 "78xx:TO-252-2Lead" H 2150 4375 50  0001 C CNN
F 3 "" H 2150 4375 50  0000 C CNN
	1    2150 4375
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 58FE32D3
P 2700 4625
F 0 "C?" H 2725 4725 50  0000 L CNN
F 1 "C" H 2725 4525 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2738 4475 50  0001 C CNN
F 3 "" H 2700 4625 50  0000 C CNN
	1    2700 4625
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 58FE3358
P 1600 4625
F 0 "C?" H 1625 4725 50  0000 L CNN
F 1 "C" H 1625 4525 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1638 4475 50  0001 C CNN
F 3 "" H 1600 4625 50  0000 C CNN
	1    1600 4625
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P?
U 1 1 58FE3599
P 1000 4375
F 0 "P?" H 1000 4525 50  0000 C CNN
F 1 "18V" V 1100 4375 50  0000 C CNN
F 2 "w_conn_pt-1,5:pt_1,5-2-3,5-h" V 1000 4375 50  0001 C CNN
F 3 "" H 1000 4375 50  0000 C CNN
	1    1000 4375
	-1   0    0    1   
$EndComp
$Comp
L PWR_FLAG #FLG?
U 1 1 58FE36B5
P 2700 4200
F 0 "#FLG?" H 2700 4295 50  0001 C CNN
F 1 "PWR_FLAG" H 2700 4380 50  0000 C CNN
F 2 "" H 2700 4200 50  0000 C CNN
F 3 "" H 2700 4200 50  0000 C CNN
	1    2700 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 58FE372A
P 2150 4950
F 0 "#PWR?" H 2150 4700 50  0001 C CNN
F 1 "GND" H 2150 4800 50  0000 C CNN
F 2 "" H 2150 4950 50  0000 C CNN
F 3 "" H 2150 4950 50  0000 C CNN
	1    2150 4950
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 58FE383D
P 3025 4225
F 0 "#PWR?" H 3025 4075 50  0001 C CNN
F 1 "+5V" H 3025 4365 50  0000 C CNN
F 2 "" H 3025 4225 50  0000 C CNN
F 3 "" H 3025 4225 50  0000 C CNN
	1    3025 4225
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 58FE386F
P 4650 4225
F 0 "#PWR?" H 4650 4075 50  0001 C CNN
F 1 "+5V" H 4650 4365 50  0000 C CNN
F 2 "" H 4650 4225 50  0000 C CNN
F 3 "" H 4650 4225 50  0000 C CNN
	1    4650 4225
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 58FE38A1
P 4650 4725
F 0 "#PWR?" H 4650 4475 50  0001 C CNN
F 1 "GND" H 4650 4575 50  0000 C CNN
F 2 "" H 4650 4725 50  0000 C CNN
F 3 "" H 4650 4725 50  0000 C CNN
	1    4650 4725
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG?
U 1 1 58FE3ABA
P 3000 4800
F 0 "#FLG?" H 3000 4895 50  0001 C CNN
F 1 "PWR_FLAG" H 3000 4980 50  0000 C CNN
F 2 "" H 3000 4800 50  0000 C CNN
F 3 "" H 3000 4800 50  0000 C CNN
	1    3000 4800
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 58FE4546
P 4000 4925
F 0 "R?" V 4080 4925 50  0000 C CNN
F 1 "4k7" V 4000 4925 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3930 4925 50  0001 C CNN
F 3 "" H 4000 4925 50  0000 C CNN
	1    4000 4925
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR?
U 1 1 58FE47CE
P 4000 4325
F 0 "#PWR?" H 4000 4175 50  0001 C CNN
F 1 "+5V" H 4000 4465 50  0000 C CNN
F 2 "" H 4000 4325 50  0000 C CNN
F 3 "" H 4000 4325 50  0000 C CNN
	1    4000 4325
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky D?
U 1 1 58FE485B
P 4000 4575
F 0 "D?" H 3850 4525 50  0000 C CNN
F 1 "BAT43" H 4000 4475 50  0000 C CNN
F 2 "w_smd_diode:sod323" H 4000 4575 50  0001 C CNN
F 3 "" H 4000 4575 50  0000 C CNN
	1    4000 4575
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 58FE4B9C
P 3400 6000
F 0 "R?" V 3480 6000 50  0000 C CNN
F 1 "4k7" V 3400 6000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3330 6000 50  0001 C CNN
F 3 "" H 3400 6000 50  0000 C CNN
	1    3400 6000
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 58FE4CA7
P 3400 5100
F 0 "R?" V 3480 5100 50  0000 C CNN
F 1 "4k7" V 3400 5100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3330 5100 50  0001 C CNN
F 3 "" H 3400 5100 50  0000 C CNN
	1    3400 5100
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR?
U 1 1 58FE4D35
P 3400 4875
F 0 "#PWR?" H 3400 4725 50  0001 C CNN
F 1 "+5V" H 3400 5015 50  0000 C CNN
F 2 "" H 3400 4875 50  0000 C CNN
F 3 "" H 3400 4875 50  0000 C CNN
	1    3400 4875
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 58FE51E0
P 3400 6225
F 0 "#PWR?" H 3400 5975 50  0001 C CNN
F 1 "GND" H 3400 6075 50  0000 C CNN
F 2 "" H 3400 6225 50  0000 C CNN
F 3 "" H 3400 6225 50  0000 C CNN
	1    3400 6225
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X04 P?
U 1 1 58FE5EB8
P 7425 5550
F 0 "P?" H 7425 5800 50  0000 C CNN
F 1 "IBT-2" H 7425 5300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x04" H 7425 4350 50  0001 C CNN
F 3 "" H 7425 4350 50  0000 C CNN
	1    7425 5550
	1    0    0    -1  
$EndComp
Text Label 7250 5200 0    60   ~ 0
ENABLE
$Comp
L GND #PWR?
U 1 1 58FE66BD
P 7775 5925
F 0 "#PWR?" H 7775 5675 50  0001 C CNN
F 1 "GND" H 7775 5775 50  0000 C CNN
F 2 "" H 7775 5925 50  0000 C CNN
F 3 "" H 7775 5925 50  0000 C CNN
	1    7775 5925
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 58FE6714
P 6850 5100
F 0 "#PWR?" H 6850 4950 50  0001 C CNN
F 1 "+5V" H 6850 5240 50  0000 C CNN
F 2 "" H 6850 5100 50  0000 C CNN
F 3 "" H 6850 5100 50  0000 C CNN
	1    6850 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2075 5350 3025 5350
Wire Wire Line
	2075 5750 3700 5750
Wire Wire Line
	3400 5700 3400 5850
Wire Wire Line
	2300 5700 2300 5750
Connection ~ 2300 5750
Wire Wire Line
	2575 5700 2575 5750
Connection ~ 2575 5750
Wire Wire Line
	2300 5400 2300 5350
Connection ~ 2300 5350
Wire Wire Line
	2575 5400 2575 5350
Connection ~ 2575 5350
Wire Wire Line
	2900 5400 2900 5350
Connection ~ 2900 5350
Wire Wire Line
	2900 5700 2900 5750
Connection ~ 2900 5750
Wire Wire Line
	3325 5350 4150 5350
Wire Wire Line
	3400 5250 3400 5400
Wire Wire Line
	1200 5500 1400 5500
Wire Wire Line
	1400 5500 1400 5350
Wire Wire Line
	1400 5350 1475 5350
Wire Wire Line
	1475 5750 1400 5750
Wire Wire Line
	1400 5750 1400 5600
Wire Wire Line
	1400 5600 1200 5600
Connection ~ 3400 5350
Wire Wire Line
	3700 5750 3700 5450
Wire Wire Line
	3700 5450 4150 5450
Connection ~ 3400 5750
Wire Wire Line
	4125 5250 4150 5250
Wire Wire Line
	4125 5250 4125 5025
Wire Wire Line
	2550 4325 3025 4325
Wire Wire Line
	2700 4200 2700 4475
Wire Wire Line
	2150 4625 2150 4950
Wire Wire Line
	1350 4850 3000 4850
Wire Wire Line
	2700 4850 2700 4775
Wire Wire Line
	1600 4850 1600 4775
Connection ~ 2150 4850
Wire Wire Line
	1200 4325 1750 4325
Wire Wire Line
	1600 4325 1600 4475
Connection ~ 1600 4325
Wire Wire Line
	1200 4425 1350 4425
Wire Wire Line
	1350 4425 1350 4850
Connection ~ 1600 4850
Connection ~ 2700 4325
Wire Wire Line
	4650 4225 4650 4325
Wire Wire Line
	4650 4625 4650 4725
Wire Wire Line
	3025 4325 3025 4225
Wire Wire Line
	3000 4850 3000 4800
Connection ~ 2700 4850
Wire Wire Line
	4150 5550 4000 5550
Wire Wire Line
	4000 5550 4000 5075
Wire Wire Line
	4000 4775 4000 4725
Wire Wire Line
	4000 4425 4000 4325
Wire Wire Line
	3400 4875 3400 4950
Wire Wire Line
	3400 6150 3400 6225
Wire Wire Line
	7175 5500 7075 5500
Wire Wire Line
	7075 5500 7075 5200
Wire Wire Line
	7075 5200 7775 5200
Wire Wire Line
	7775 5200 7775 5500
Wire Wire Line
	7775 5500 7675 5500
Wire Wire Line
	7175 5700 6850 5700
Wire Wire Line
	6850 5700 6850 5100
Wire Wire Line
	7675 5700 7775 5700
Wire Wire Line
	7775 5700 7775 5925
$Comp
L R R?
U 1 1 58FE77FA
P 3800 4925
F 0 "R?" V 3880 4925 50  0000 C CNN
F 1 "1k5" V 3800 4925 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3730 4925 50  0001 C CNN
F 3 "" H 3800 4925 50  0000 C CNN
	1    3800 4925
	-1   0    0    1   
$EndComp
Wire Wire Line
	4150 5650 3800 5650
Wire Wire Line
	3800 5650 3800 5075
$Comp
L Led_Small D?
U 1 1 58FE78BA
P 3800 4575
F 0 "D?" H 3750 4700 50  0000 L CNN
F 1 "Led_Small" H 3625 4475 50  0000 L CNN
F 2 "" V 3800 4575 50  0000 C CNN
F 3 "" V 3800 4575 50  0000 C CNN
	1    3800 4575
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3800 4675 3800 4775
$Comp
L +5V #PWR?
U 1 1 58FE7B78
P 3800 4325
F 0 "#PWR?" H 3800 4175 50  0001 C CNN
F 1 "+5V" H 3800 4465 50  0000 C CNN
F 2 "" H 3800 4325 50  0000 C CNN
F 3 "" H 3800 4325 50  0000 C CNN
	1    3800 4325
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 4475 3800 4325
Text Label 6000 5650 2    60   ~ 0
ENABLE
Wire Wire Line
	7175 5600 7000 5600
Wire Wire Line
	7000 5600 7000 5075
Wire Wire Line
	7000 5075 7875 5075
Wire Wire Line
	7875 5075 7875 5600
Wire Wire Line
	7875 5600 7675 5600
Text Label 7250 5075 0    60   ~ 0
SENSE
Wire Wire Line
	5650 5550 6000 5550
Text Label 6000 5550 2    60   ~ 0
SENSE
Wire Wire Line
	5650 5350 5900 5350
Wire Wire Line
	5650 5450 5900 5450
Wire Wire Line
	5650 5250 6200 5250
Wire Wire Line
	6200 5250 6200 5350
$Comp
L GND #PWR?
U 1 1 58FE942D
P 6200 5350
F 0 "#PWR?" H 6200 5100 50  0001 C CNN
F 1 "GND" H 6200 5200 50  0000 C CNN
F 2 "" H 6200 5350 50  0000 C CNN
F 3 "" H 6200 5350 50  0000 C CNN
	1    6200 5350
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 58FE9775
P 7650 4875
F 0 "R?" V 7730 4875 50  0000 C CNN
F 1 "10k" V 7650 4875 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7580 4875 50  0001 C CNN
F 3 "" H 7650 4875 50  0000 C CNN
	1    7650 4875
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7650 5025 7650 5075
Connection ~ 7650 5075
$Comp
L +5V #PWR?
U 1 1 58FE99F3
P 7650 4625
F 0 "#PWR?" H 7650 4475 50  0001 C CNN
F 1 "+5V" H 7650 4765 50  0000 C CNN
F 2 "" H 7650 4625 50  0000 C CNN
F 3 "" H 7650 4625 50  0000 C CNN
	1    7650 4625
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 4725 7650 4625
Wire Wire Line
	5650 5650 6000 5650
Text Label 4025 5550 0    60   ~ 0
MCLR
Wire Wire Line
	7175 5400 6575 5400
Wire Wire Line
	6575 5400 6575 5750
Wire Wire Line
	6575 5750 5650 5750
Wire Wire Line
	5650 5850 8050 5850
Wire Wire Line
	8050 5850 8050 5400
Wire Wire Line
	8050 5400 7675 5400
Text Label 5975 5750 2    60   ~ 0
RPWM
Text Label 5975 5850 2    60   ~ 0
LPWM
$Comp
L CONN_01X04 P?
U 1 1 58FEA831
P 3950 6900
F 0 "P?" H 3950 7150 50  0000 C CNN
F 1 "UART" V 4050 6900 50  0000 C CNN
F 2 "w_conn_jst-ph:b4b-ph-kl" V 3950 6900 50  0001 C CNN
F 3 "" H 3950 6900 50  0000 C CNN
	1    3950 6900
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X04 P?
U 1 1 58FEAA03
P 4950 6900
F 0 "P?" H 4950 7150 50  0000 C CNN
F 1 "I2C" V 5050 6900 50  0000 C CNN
F 2 "w_conn_jst-ph:b4b-ph-kl" V 4950 6900 50  0001 C CNN
F 3 "" H 4950 6900 50  0000 C CNN
	1    4950 6900
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 58FF775E
P 4200 7100
F 0 "#PWR?" H 4200 6850 50  0001 C CNN
F 1 "GND" H 4200 6950 50  0000 C CNN
F 2 "" H 4200 7100 50  0000 C CNN
F 3 "" H 4200 7100 50  0000 C CNN
	1    4200 7100
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 58FF7859
P 4200 6600
F 0 "#PWR?" H 4200 6450 50  0001 C CNN
F 1 "+5V" H 4200 6740 50  0000 C CNN
F 2 "" H 4200 6600 50  0000 C CNN
F 3 "" H 4200 6600 50  0000 C CNN
	1    4200 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 7050 4200 7050
Wire Wire Line
	4200 7050 4200 7100
Wire Wire Line
	4150 6950 4200 6950
Wire Wire Line
	4200 6950 4200 6600
Wire Wire Line
	4150 6850 4425 6850
Wire Wire Line
	4150 6750 4425 6750
Text Label 4425 6750 2    60   ~ 0
RxD
Text Label 4425 6850 2    60   ~ 0
TxD
$Comp
L GND #PWR?
U 1 1 58FF8516
P 5200 7100
F 0 "#PWR?" H 5200 6850 50  0001 C CNN
F 1 "GND" H 5200 6950 50  0000 C CNN
F 2 "" H 5200 7100 50  0000 C CNN
F 3 "" H 5200 7100 50  0000 C CNN
	1    5200 7100
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 58FF8A1B
P 5200 6625
F 0 "#PWR?" H 5200 6475 50  0001 C CNN
F 1 "+5V" H 5200 6765 50  0000 C CNN
F 2 "" H 5200 6625 50  0000 C CNN
F 3 "" H 5200 6625 50  0000 C CNN
	1    5200 6625
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 6625 5200 6950
Wire Wire Line
	5150 7050 5200 7050
Wire Wire Line
	5200 7050 5200 7100
Wire Wire Line
	5200 6950 5150 6950
Wire Wire Line
	5150 6850 5400 6850
Wire Wire Line
	5150 6750 5400 6750
Text Label 5400 6750 2    60   ~ 0
SDA
Text Label 5400 6850 2    60   ~ 0
SCL
Wire Wire Line
	4150 5750 4000 5750
Wire Wire Line
	4150 5850 4000 5850
Wire Wire Line
	4150 5950 4000 5950
Wire Wire Line
	4150 6050 4000 6050
Text Label 4000 5750 0    60   ~ 0
RxD
Text Label 4000 5850 0    60   ~ 0
TxD
Text Label 4000 5950 0    60   ~ 0
SDA
Text Label 4000 6050 0    60   ~ 0
SCL
Text Label 4000 5650 0    60   ~ 0
LED
$Comp
L CONN_01X03 P?
U 1 1 58FFAE00
P 6250 4800
F 0 "P?" H 6250 5000 50  0000 C CNN
F 1 "POTI" V 6350 4800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" V 6250 4800 50  0001 C CNN
F 3 "" H 6250 4800 50  0000 C CNN
	1    6250 4800
	1    0    0    1   
$EndComp
Wire Wire Line
	5900 5350 5900 4800
Wire Wire Line
	5900 4800 6050 4800
Wire Wire Line
	6050 4900 6000 4900
Wire Wire Line
	6000 4900 6000 5050
Wire Wire Line
	6050 4700 6000 4700
Wire Wire Line
	6000 4700 6000 4550
$Comp
L GND #PWR?
U 1 1 58FFB153
P 6000 5050
F 0 "#PWR?" H 6000 4800 50  0001 C CNN
F 1 "GND" H 6000 4900 50  0000 C CNN
F 2 "" H 6000 5050 50  0000 C CNN
F 3 "" H 6000 5050 50  0000 C CNN
	1    6000 5050
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 58FFB1C2
P 6000 4550
F 0 "#PWR?" H 6000 4400 50  0001 C CNN
F 1 "+5V" H 6000 4690 50  0000 C CNN
F 2 "" H 6000 4550 50  0000 C CNN
F 3 "" H 6000 4550 50  0000 C CNN
	1    6000 4550
	1    0    0    -1  
$EndComp
NoConn ~ 4150 6150
Wire Wire Line
	5650 5950 6275 5950
Wire Wire Line
	6275 5950 6275 6425
Wire Wire Line
	6275 6425 6725 6425
Wire Wire Line
	5650 6050 6175 6050
Wire Wire Line
	6175 6050 6175 6525
Wire Wire Line
	6175 6525 6725 6525
NoConn ~ 5650 6150
Text Label 6725 6425 2    60   ~ 0
GB_IN
Text Label 6725 6525 2    60   ~ 0
GB_OUT
$Comp
L BC817-40 Q?
U 1 1 58FFC481
P 7125 3200
F 0 "Q?" H 7325 3275 50  0000 L CNN
F 1 "BC817-40" H 7325 3200 50  0000 L CNN
F 2 "SOT-23" H 7325 3125 50  0000 L CIN
F 3 "" H 7125 3200 50  0000 L CNN
	1    7125 3200
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 58FFCD58
P 7225 2575
F 0 "R?" V 7275 2725 50  0000 C CNN
F 1 "1K5" V 7225 2575 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 7155 2575 50  0001 C CNN
F 3 "" H 7225 2575 50  0000 C CNN
	1    7225 2575
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 58FFD00D
P 6700 3200
F 0 "R?" V 6780 3200 50  0000 C CNN
F 1 "1k" V 6700 3200 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6630 3200 50  0001 C CNN
F 3 "" H 6700 3200 50  0000 C CNN
	1    6700 3200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6850 3200 6925 3200
$Comp
L R R?
U 1 1 58FFD390
P 7375 2575
F 0 "R?" V 7425 2725 50  0000 C CNN
F 1 "1K5" V 7375 2575 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 7305 2575 50  0001 C CNN
F 3 "" H 7375 2575 50  0000 C CNN
	1    7375 2575
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 58FFD424
P 7525 2575
F 0 "R?" V 7575 2725 50  0000 C CNN
F 1 "1K5" V 7525 2575 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 7455 2575 50  0001 C CNN
F 3 "" H 7525 2575 50  0000 C CNN
	1    7525 2575
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 58FFD74D
P 7675 2575
F 0 "R?" V 7725 2725 50  0000 C CNN
F 1 "1K5" V 7675 2575 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 7605 2575 50  0001 C CNN
F 3 "" H 7675 2575 50  0000 C CNN
	1    7675 2575
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X03 P?
U 1 1 58FFDB5A
P 7175 2025
F 0 "P?" H 7175 2225 50  0000 C CNN
F 1 "CONN_02X03" H 7175 1825 50  0000 C CNN
F 2 "" H 7175 825 50  0000 C CNN
F 3 "" H 7175 825 50  0000 C CNN
	1    7175 2025
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 58FFDD14
P 7075 2575
F 0 "R?" V 7125 2725 50  0000 C CNN
F 1 "1K5" V 7075 2575 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 7005 2575 50  0001 C CNN
F 3 "" H 7075 2575 50  0000 C CNN
	1    7075 2575
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 58FFDDAC
P 6925 2575
F 0 "R?" V 6975 2725 50  0000 C CNN
F 1 "1K5" V 6925 2575 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 6855 2575 50  0001 C CNN
F 3 "" H 6925 2575 50  0000 C CNN
	1    6925 2575
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 58FFDEB5
P 6775 2575
F 0 "R?" V 6825 2725 50  0000 C CNN
F 1 "1K5" V 6775 2575 50  0000 C CNN
F 2 "Resistors_SMD:R_1206_HandSoldering" V 6705 2575 50  0001 C CNN
F 3 "" H 6775 2575 50  0000 C CNN
	1    6775 2575
	1    0    0    -1  
$EndComp
Wire Wire Line
	7675 2350 7675 2425
Wire Wire Line
	7225 2350 7675 2350
Wire Wire Line
	7275 2350 7275 2275
Wire Wire Line
	7225 2425 7225 2350
Connection ~ 7275 2350
Wire Wire Line
	7375 2350 7375 2425
Connection ~ 7375 2350
Wire Wire Line
	7525 2350 7525 2450
Connection ~ 7525 2350
Wire Wire Line
	6925 2425 6925 2350
Wire Wire Line
	6925 2350 7175 2350
Wire Wire Line
	7075 2350 7075 2425
Wire Wire Line
	7175 2350 7175 2275
Connection ~ 7075 2350
Wire Wire Line
	7075 2275 6775 2275
Wire Wire Line
	6775 2275 6775 2425
$Comp
L D D?
U 1 1 58FFE9CD
P 4875 1300
F 0 "D?" H 4875 1400 50  0000 C CNN
F 1 "D" H 4875 1200 50  0000 C CNN
F 2 "w_smd_diode:sod323" H 4875 1300 50  0001 C CNN
F 3 "" H 4875 1300 50  0000 C CNN
	1    4875 1300
	0    -1   -1   0   
$EndComp
$Comp
L D D?
U 1 1 58FFEBB8
P 4650 1300
F 0 "D?" H 4650 1400 50  0000 C CNN
F 1 "D" H 4650 1200 50  0000 C CNN
F 2 "w_smd_diode:sod323" H 4650 1300 50  0001 C CNN
F 3 "" H 4650 1300 50  0000 C CNN
	1    4650 1300
	0    -1   -1   0   
$EndComp
$Comp
L D D?
U 1 1 58FFEC40
P 4275 1300
F 0 "D?" H 4275 1400 50  0000 C CNN
F 1 "D" H 4275 1200 50  0000 C CNN
F 2 "w_smd_diode:sod323" H 4275 1300 50  0001 C CNN
F 3 "" H 4275 1300 50  0000 C CNN
	1    4275 1300
	0    1    1    0   
$EndComp
$Comp
L D D?
U 1 1 58FFECE5
P 4050 1300
F 0 "D?" H 4050 1400 50  0000 C CNN
F 1 "D" H 4050 1200 50  0000 C CNN
F 2 "w_smd_diode:sod323" H 4050 1300 50  0001 C CNN
F 3 "" H 4050 1300 50  0000 C CNN
	1    4050 1300
	0    1    1    0   
$EndComp
Wire Wire Line
	6775 2725 6775 2850
Wire Wire Line
	6775 2850 7675 2850
Wire Wire Line
	7675 2850 7675 2725
Wire Wire Line
	6925 2725 6925 2850
Connection ~ 6925 2850
Wire Wire Line
	7075 2725 7075 2850
Connection ~ 7075 2850
Wire Wire Line
	7225 2725 7225 3000
Connection ~ 7225 2850
Wire Wire Line
	7375 2725 7375 2850
Connection ~ 7375 2850
Wire Wire Line
	7525 2725 7525 2850
Connection ~ 7525 2850
$Comp
L 7805 U?
U 1 1 5900103C
P 3150 1950
F 0 "U?" H 3300 1754 50  0000 C CNN
F 1 "7805" H 3150 2150 50  0000 C CNN
F 2 "78xx:TO-252-2Lead" H 3150 1950 50  0001 C CNN
F 3 "" H 3150 1950 50  0000 C CNN
	1    3150 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3200 6175 3200
Text Label 6175 3200 0    60   ~ 0
_GB_OUT
$Comp
L GNDA #PWR?
U 1 1 59001DF4
P 7225 3450
F 0 "#PWR?" H 7225 3200 50  0001 C CNN
F 1 "GNDA" H 7225 3300 50  0000 C CNN
F 2 "" H 7225 3450 50  0000 C CNN
F 3 "" H 7225 3450 50  0000 C CNN
	1    7225 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7225 3400 7225 3450
$Comp
L CONN_01X02 P?
U 1 1 59002303
P 1675 2700
F 0 "P?" H 1675 2850 50  0000 C CNN
F 1 "CONN_01X02" V 1775 2700 50  0000 C CNN
F 2 "" H 1675 2700 50  0000 C CNN
F 3 "" H 1675 2700 50  0000 C CNN
	1    1675 2700
	-1   0    0    1   
$EndComp
Wire Bus Line
	500  3900 10950 3900
$EndSCHEMATC