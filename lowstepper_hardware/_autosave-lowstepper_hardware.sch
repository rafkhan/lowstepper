EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A2 23386 16535
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Transistor_BJT:MMBT3904 Q1
U 1 1 5FCB3E1B
P 3100 2450
F 0 "Q1" H 3291 2496 50  0000 L CNN
F 1 "MMBT3904" H 3291 2405 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3300 2375 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 3100 2450 50  0001 L CNN
	1    3100 2450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5FCB4BBD
P 3200 2900
F 0 "#PWR07" H 3200 2650 50  0001 C CNN
F 1 "GND" H 3205 2727 50  0000 C CNN
F 2 "" H 3200 2900 50  0001 C CNN
F 3 "" H 3200 2900 50  0001 C CNN
	1    3200 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R1
U 1 1 5FCB8814
P 2350 2450
F 0 "R1" V 2145 2450 50  0000 C CNN
F 1 "100K" V 2236 2450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2350 2450 50  0001 C CNN
F 3 "~" H 2350 2450 50  0001 C CNN
	1    2350 2450
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5FCBBDAD
P 2650 2900
F 0 "#PWR04" H 2650 2650 50  0001 C CNN
F 1 "GND" H 2655 2727 50  0000 C CNN
F 2 "" H 2650 2900 50  0001 C CNN
F 3 "" H 2650 2900 50  0001 C CNN
	1    2650 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 2800 2650 2900
Wire Wire Line
	2450 2450 2650 2450
Wire Wire Line
	2650 2450 2650 2600
Connection ~ 2650 2450
Wire Wire Line
	2650 2450 2900 2450
Wire Wire Line
	3200 2650 3200 2900
$Comp
L Device:R_Small_US R4
U 1 1 5FCC32EF
P 2650 2700
F 0 "R4" H 2582 2654 50  0000 R CNN
F 1 "100M" H 2582 2745 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2582 2791 50  0001 R CNN
F 3 "~" H 2650 2700 50  0001 C CNN
	1    2650 2700
	-1   0    0    1   
$EndComp
Wire Wire Line
	3200 1950 3200 2100
Wire Wire Line
	3200 2100 3600 2100
Connection ~ 3200 2100
Wire Wire Line
	3200 2100 3200 2250
$Comp
L Connector:AudioJack2 J1
U 1 1 5FCCABB7
P 1800 2550
F 0 "J1" H 1832 2875 50  0000 C CNN
F 1 "CLOCK_IN" H 1832 2784 50  0000 C CNN
F 2 "Connector_Audio:Jack_3.5mm_QingPu_WQP-PJ398SM_Vertical_CircularHoles" H 1800 2550 50  0001 C CNN
F 3 "~" H 1800 2550 50  0001 C CNN
	1    1800 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 2450 2250 2450
$Comp
L power:GND #PWR01
U 1 1 5FCCB89A
P 2000 2900
F 0 "#PWR01" H 2000 2650 50  0001 C CNN
F 1 "GND" H 2005 2727 50  0000 C CNN
F 2 "" H 2000 2900 50  0001 C CNN
F 3 "" H 2000 2900 50  0001 C CNN
	1    2000 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 2550 2000 2900
$Comp
L Transistor_BJT:MMBT3904 Q2
U 1 1 5FCD3389
P 3100 5050
F 0 "Q2" H 3291 5096 50  0000 L CNN
F 1 "MMBT3904" H 3291 5005 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3300 4975 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 3100 5050 50  0001 L CNN
	1    3100 5050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5FCD3393
P 3200 5500
F 0 "#PWR08" H 3200 5250 50  0001 C CNN
F 1 "GND" H 3205 5327 50  0000 C CNN
F 2 "" H 3200 5500 50  0001 C CNN
F 3 "" H 3200 5500 50  0001 C CNN
	1    3200 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5FCD33A7
P 2650 5500
F 0 "#PWR05" H 2650 5250 50  0001 C CNN
F 1 "GND" H 2655 5327 50  0000 C CNN
F 2 "" H 2650 5500 50  0001 C CNN
F 3 "" H 2650 5500 50  0001 C CNN
	1    2650 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 5400 2650 5500
Wire Wire Line
	3200 4350 3200 4150
Text Label 3200 4150 0    50   ~ 0
3v3
Wire Wire Line
	2650 5050 2650 5200
Connection ~ 2650 5050
Wire Wire Line
	3200 5250 3200 5500
$Comp
L Device:R_Small_US R5
U 1 1 5FCD33B9
P 2650 5300
F 0 "R5" H 2582 5254 50  0000 R CNN
F 1 "100M" H 2582 5345 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2582 5391 50  0001 R CNN
F 3 "~" H 2650 5300 50  0001 C CNN
	1    2650 5300
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R8
U 1 1 5FCD33C3
P 3200 4450
F 0 "R8" H 3132 4404 50  0000 R CNN
F 1 "10K" H 3132 4495 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3200 4450 50  0001 C CNN
F 3 "~" H 3200 4450 50  0001 C CNN
	1    3200 4450
	-1   0    0    1   
$EndComp
Wire Wire Line
	3200 4550 3200 4700
Connection ~ 3200 4700
Wire Wire Line
	3200 4700 3200 4850
$Comp
L Connector:AudioJack2 J2
U 1 1 5FCD33D2
P 1800 5150
F 0 "J2" H 1832 5475 50  0000 C CNN
F 1 "TRIG" H 1832 5384 50  0000 C CNN
F 2 "Connector_Audio:Jack_3.5mm_QingPu_WQP-PJ398SM_Vertical_CircularHoles" H 1800 5150 50  0001 C CNN
F 3 "~" H 1800 5150 50  0001 C CNN
	1    1800 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5FCD33DD
P 2000 5500
F 0 "#PWR02" H 2000 5250 50  0001 C CNN
F 1 "GND" H 2005 5327 50  0000 C CNN
F 2 "" H 2000 5500 50  0001 C CNN
F 3 "" H 2000 5500 50  0001 C CNN
	1    2000 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 5150 2000 5500
$Comp
L Device:R_Small_US R2
U 1 1 5FCD339D
P 2350 5050
F 0 "R2" V 2145 5050 50  0000 C CNN
F 1 "100K" V 2236 5050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2350 5050 50  0001 C CNN
F 3 "~" H 2350 5050 50  0001 C CNN
	1    2350 5050
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x08_Top_Bottom J8
U 1 1 5FD1ED66
P 20100 10000
F 0 "J8" H 20150 10517 50  0000 C CNN
F 1 "DONT_FUCK_UP_RED_LINE" H 20150 10426 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x08_P2.54mm_Vertical" H 20100 10000 50  0001 C CNN
F 3 "~" H 20100 10000 50  0001 C CNN
	1    20100 10000
	1    0    0    -1  
$EndComp
Wire Wire Line
	19650 9700 19900 9700
Text Label 19650 9700 2    50   ~ 0
-12V
Wire Wire Line
	19900 9800 19900 9900
Connection ~ 19900 9900
Wire Wire Line
	19900 9900 19900 10000
$Comp
L power:GND #PWR018
U 1 1 5FD721A8
P 19600 9900
F 0 "#PWR018" H 19600 9650 50  0001 C CNN
F 1 "GND" V 19605 9772 50  0000 R CNN
F 2 "" H 19600 9900 50  0001 C CNN
F 3 "" H 19600 9900 50  0001 C CNN
	1    19600 9900
	0    1    1    0   
$EndComp
Wire Wire Line
	19600 9900 19900 9900
Wire Wire Line
	19900 10100 19650 10100
Text Label 19650 10100 2    50   ~ 0
+12V
Wire Wire Line
	20400 9800 20400 9900
Connection ~ 20400 9900
Wire Wire Line
	20400 9900 20400 10000
Wire Wire Line
	20400 9900 19900 9900
Wire Wire Line
	19900 10100 20400 10100
Connection ~ 19900 10100
Wire Wire Line
	19900 10200 19650 10200
Text Label 19650 10200 2    50   ~ 0
5V
Wire Wire Line
	20400 10200 19900 10200
Connection ~ 19900 10200
$Comp
L power:GND #PWR017
U 1 1 5FD1B354
P 12200 3500
F 0 "#PWR017" H 12200 3250 50  0001 C CNN
F 1 "GND" H 12205 3327 50  0000 C CNN
F 2 "" H 12200 3500 50  0001 C CNN
F 3 "" H 12200 3500 50  0001 C CNN
	1    12200 3500
	1    0    0    -1  
$EndComp
Text Label 11900 3500 3    50   ~ 0
5v
Wire Wire Line
	11900 3350 11900 3500
Wire Wire Line
	12200 3350 12200 3500
Connection ~ 12050 3050
Wire Wire Line
	11900 3050 12050 3050
Wire Wire Line
	11900 3150 11900 3050
$Comp
L Device:R_Small_US R20
U 1 1 5FD198C5
P 11900 3250
F 0 "R20" H 11968 3296 50  0000 L CNN
F 1 "3.3K" H 11968 3205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 11900 3250 50  0001 C CNN
F 3 "~" H 11900 3250 50  0001 C CNN
	1    11900 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R21
U 1 1 5FD1940A
P 12200 3250
F 0 "R21" H 12268 3296 50  0000 L CNN
F 1 "10K" H 12268 3205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 12200 3250 50  0001 C CNN
F 3 "~" H 12200 3250 50  0001 C CNN
	1    12200 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	12200 3050 12200 3150
Wire Wire Line
	12050 3050 12200 3050
Wire Wire Line
	12050 2800 12050 3050
Wire Wire Line
	11900 2800 12050 2800
Connection ~ 11050 2700
Wire Wire Line
	11050 2700 10650 2700
Wire Wire Line
	11050 2700 11300 2700
Wire Wire Line
	11050 2150 11050 2700
Wire Wire Line
	11550 2150 11050 2150
Wire Wire Line
	12050 2600 12250 2600
Connection ~ 12050 2600
Wire Wire Line
	12050 2150 11750 2150
Wire Wire Line
	12050 2600 12050 2150
Wire Wire Line
	11900 2600 12050 2600
$Comp
L Device:R_Small_US R19
U 1 1 5FCFC0DB
P 11650 2150
F 0 "R19" V 11445 2150 50  0000 C CNN
F 1 "10K" V 11536 2150 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 11650 2150 50  0001 C CNN
F 3 "~" H 11650 2150 50  0001 C CNN
	1    11650 2150
	0    1    1    0   
$EndComp
Wire Wire Line
	10650 3000 10650 2800
$Comp
L power:GND #PWR016
U 1 1 5FCF07F7
P 10650 3000
F 0 "#PWR016" H 10650 2750 50  0001 C CNN
F 1 "GND" H 10655 2827 50  0000 C CNN
F 2 "" H 10650 3000 50  0001 C CNN
F 3 "" H 10650 3000 50  0001 C CNN
	1    10650 3000
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MMBT3904 Q3
U 1 1 5FE64053
P 3100 7600
F 0 "Q3" H 3291 7646 50  0000 L CNN
F 1 "MMBT3904" H 3291 7555 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3300 7525 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 3100 7600 50  0001 L CNN
	1    3100 7600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5FE6405D
P 3200 8050
F 0 "#PWR09" H 3200 7800 50  0001 C CNN
F 1 "GND" H 3205 7877 50  0000 C CNN
F 2 "" H 3200 8050 50  0001 C CNN
F 3 "" H 3200 8050 50  0001 C CNN
	1    3200 8050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5FE64067
P 2650 8050
F 0 "#PWR06" H 2650 7800 50  0001 C CNN
F 1 "GND" H 2655 7877 50  0000 C CNN
F 2 "" H 2650 8050 50  0001 C CNN
F 3 "" H 2650 8050 50  0001 C CNN
	1    2650 8050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 7950 2650 8050
Wire Wire Line
	3200 6900 3200 6700
Text Label 3200 6700 0    50   ~ 0
3v3
Wire Wire Line
	2650 7600 2650 7750
Connection ~ 2650 7600
Wire Wire Line
	3200 7800 3200 8050
$Comp
L Device:R_Small_US R6
U 1 1 5FE64079
P 2650 7850
F 0 "R6" H 2582 7804 50  0000 R CNN
F 1 "100M" H 2582 7895 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2582 7941 50  0001 R CNN
F 3 "~" H 2650 7850 50  0001 C CNN
	1    2650 7850
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small_US R9
U 1 1 5FE64083
P 3200 7000
F 0 "R9" H 3132 6954 50  0000 R CNN
F 1 "10K" H 3132 7045 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3200 7000 50  0001 C CNN
F 3 "~" H 3200 7000 50  0001 C CNN
	1    3200 7000
	-1   0    0    1   
$EndComp
Wire Wire Line
	3200 7100 3200 7250
Connection ~ 3200 7250
Wire Wire Line
	3200 7250 3200 7400
$Comp
L Connector:AudioJack2 J3
U 1 1 5FE64092
P 1800 7700
F 0 "J3" H 1832 8025 50  0000 C CNN
F 1 "RESET" H 1832 7934 50  0000 C CNN
F 2 "Connector_Audio:Jack_3.5mm_QingPu_WQP-PJ398SM_Vertical_CircularHoles" H 1800 7700 50  0001 C CNN
F 3 "~" H 1800 7700 50  0001 C CNN
	1    1800 7700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5FE6409D
P 2000 8050
F 0 "#PWR03" H 2000 7800 50  0001 C CNN
F 1 "GND" H 2005 7877 50  0000 C CNN
F 2 "" H 2000 8050 50  0001 C CNN
F 3 "" H 2000 8050 50  0001 C CNN
	1    2000 8050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 7700 2000 8050
$Comp
L Device:R_Small_US R3
U 1 1 5FE640A8
P 2350 7600
F 0 "R3" V 2145 7600 50  0000 C CNN
F 1 "100K" V 2236 7600 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2350 7600 50  0001 C CNN
F 3 "~" H 2350 7600 50  0001 C CNN
	1    2350 7600
	0    1    1    0   
$EndComp
Wire Wire Line
	12450 2600 12750 2600
Text Label 12750 2600 0    50   ~ 0
DAC0_OUT
$Comp
L Device:R_POT_Small RV1
U 1 1 5FF5DF45
P 15500 2550
F 0 "RV1" V 15303 2550 50  0000 C CNN
F 1 "10K" V 15394 2550 50  0000 C CNN
F 2 "Potentiometer_THT:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical" H 15500 2550 50  0001 C CNN
F 3 "~" H 15500 2550 50  0001 C CNN
	1    15500 2550
	0    1    1    0   
$EndComp
Wire Wire Line
	15600 2550 15750 2550
Text Label 15750 2550 0    50   ~ 0
3V3
Wire Wire Line
	15400 2550 15200 2550
Text Label 15200 2550 2    50   ~ 0
GND
Wire Wire Line
	15500 2650 15500 2800
Text Label 15500 2800 0    50   ~ 0
MORPH_POT
$Comp
L Device:R_POT_Small RV2
U 1 1 5FF6E6BC
P 15500 3200
F 0 "RV2" V 15303 3200 50  0000 C CNN
F 1 "10K" V 15394 3200 50  0000 C CNN
F 2 "Potentiometer_THT:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical" H 15500 3200 50  0001 C CNN
F 3 "~" H 15500 3200 50  0001 C CNN
	1    15500 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	15600 3200 15750 3200
Text Label 15750 3200 0    50   ~ 0
3V3
Wire Wire Line
	15400 3200 15200 3200
Text Label 15200 3200 2    50   ~ 0
GND
Wire Wire Line
	15500 3300 15500 3450
Text Label 15500 3450 0    50   ~ 0
RATE_POT
$Comp
L Device:R_POT_Small RV3
U 1 1 5FF754DE
P 15500 3850
F 0 "RV3" V 15303 3850 50  0000 C CNN
F 1 "10K" V 15394 3850 50  0000 C CNN
F 2 "Potentiometer_THT:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical" H 15500 3850 50  0001 C CNN
F 3 "~" H 15500 3850 50  0001 C CNN
	1    15500 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	15600 3850 15750 3850
Text Label 15750 3850 0    50   ~ 0
3V3
Wire Wire Line
	15400 3850 15200 3850
Text Label 15200 3850 2    50   ~ 0
GND
Wire Wire Line
	15500 3950 15500 4100
Text Label 15500 4100 0    50   ~ 0
CHUNK_POT
$Comp
L Device:R_Small_US R22
U 1 1 5FCFB1E9
P 12350 2600
F 0 "R22" V 12145 2600 50  0000 C CNN
F 1 "3.3K" V 12236 2600 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 12350 2600 50  0001 C CNN
F 3 "~" H 12350 2600 50  0001 C CNN
	1    12350 2600
	0    1    1    0   
$EndComp
$Comp
L Amplifier_Operational:TL072 U1
U 1 1 60042A89
P 11600 2700
F 0 "U1" H 11600 2333 50  0000 C CNN
F 1 "TL072" H 11600 2424 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 11600 2700 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 11600 2700 50  0001 C CNN
	1    11600 2700
	-1   0    0    1   
$EndComp
$Comp
L Amplifier_Operational:TL072 U1
U 2 1 60044E74
P 11650 5150
F 0 "U1" H 11650 5517 50  0000 C CNN
F 1 "TL072" H 11650 5426 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 11650 5150 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 11650 5150 50  0001 C CNN
	2    11650 5150
	-1   0    0    1   
$EndComp
$Comp
L Amplifier_Operational:TL072 U1
U 3 1 6004C2C2
P 11900 7350
F 0 "U1" H 11858 7396 50  0000 L CNN
F 1 "TL072" H 11858 7305 50  0000 L CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 11900 7350 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tl071.pdf" H 11900 7350 50  0001 C CNN
	3    11900 7350
	1    0    0    -1  
$EndComp
Connection ~ 19900 9700
Wire Wire Line
	19900 9700 20400 9700
$Comp
L power:GND #PWR0102
U 1 1 6004447B
P 12250 5950
F 0 "#PWR0102" H 12250 5700 50  0001 C CNN
F 1 "GND" H 12255 5777 50  0000 C CNN
F 2 "" H 12250 5950 50  0001 C CNN
F 3 "" H 12250 5950 50  0001 C CNN
	1    12250 5950
	1    0    0    -1  
$EndComp
Text Label 11950 5950 3    50   ~ 0
5v
Wire Wire Line
	11950 5800 11950 5950
Wire Wire Line
	12250 5800 12250 5950
Connection ~ 12100 5500
Wire Wire Line
	11950 5500 12100 5500
Wire Wire Line
	11950 5600 11950 5500
$Comp
L Device:R_Small_US R24
U 1 1 6004448B
P 11950 5700
F 0 "R24" H 12018 5746 50  0000 L CNN
F 1 "3.3K" H 12018 5655 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 11950 5700 50  0001 C CNN
F 3 "~" H 11950 5700 50  0001 C CNN
	1    11950 5700
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small_US R25
U 1 1 60044495
P 12250 5700
F 0 "R25" H 12318 5746 50  0000 L CNN
F 1 "10K" H 12318 5655 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 12250 5700 50  0001 C CNN
F 3 "~" H 12250 5700 50  0001 C CNN
	1    12250 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	12250 5500 12250 5600
Wire Wire Line
	12100 5500 12250 5500
Wire Wire Line
	12100 5250 12100 5500
Wire Wire Line
	11950 5250 12100 5250
Connection ~ 11100 5150
Wire Wire Line
	11100 5150 10700 5150
Wire Wire Line
	11100 5150 11350 5150
Wire Wire Line
	11100 4600 11100 5150
Wire Wire Line
	11600 4600 11100 4600
Wire Wire Line
	12100 5050 12300 5050
Connection ~ 12100 5050
Wire Wire Line
	12100 4600 11800 4600
Wire Wire Line
	12100 5050 12100 4600
Wire Wire Line
	11950 5050 12100 5050
$Comp
L Device:R_Small_US R23
U 1 1 600444AD
P 11700 4600
F 0 "R23" V 11495 4600 50  0000 C CNN
F 1 "10K" V 11586 4600 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 11700 4600 50  0001 C CNN
F 3 "~" H 11700 4600 50  0001 C CNN
	1    11700 4600
	0    1    1    0   
$EndComp
Wire Wire Line
	10700 5450 10700 5250
$Comp
L power:GND #PWR0103
U 1 1 600444B8
P 10700 5450
F 0 "#PWR0103" H 10700 5200 50  0001 C CNN
F 1 "GND" H 10705 5277 50  0000 C CNN
F 2 "" H 10700 5450 50  0001 C CNN
F 3 "" H 10700 5450 50  0001 C CNN
	1    10700 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	12500 5050 12800 5050
$Comp
L Device:R_Small_US R26
U 1 1 600444CE
P 12400 5050
F 0 "R26" V 12195 5050 50  0000 C CNN
F 1 "3.3K" V 12286 5050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 12400 5050 50  0001 C CNN
F 3 "~" H 12400 5050 50  0001 C CNN
	1    12400 5050
	0    1    1    0   
$EndComp
Text Notes 19100 10600 0    50   ~ 0
Footprint is incorrect, see docs in OKA-12
Wire Wire Line
	11800 7650 11800 7750
Text Label 11800 7750 0    50   ~ 0
-12V
Wire Wire Line
	11800 6900 11800 7050
Text Label 11800 6900 0    50   ~ 0
+12V
Text Notes 18250 9300 0    50   ~ 0
Everything from the power bus probably needs filtering of sorts
Text Label 12800 5050 0    50   ~ 0
DAC1_OUT
$Comp
L teensy:Teensy3.5 U3
U 1 1 5FD57561
P 19850 4050
F 0 "U3" H 19850 6789 60  0000 C CNN
F 1 "Teensy3.5" H 19850 6683 60  0000 C CNN
F 2 "teensy:Teensy35_36" H 19700 5900 60  0000 C CNN
F 3 "https://www.pjrc.com/teensy/card8a_rev2.pdf" H 19850 6471 60  0000 C CNN
F 4 "https://www.pjrc.com/teensy/pinout.html" H 19850 6373 50  0000 C CNN "Pinouts"
	1    19850 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	18250 1900 18700 1900
Text Label 18350 5500 2    50   ~ 0
DAC0_OUT
Wire Wire Line
	18350 5500 18700 5500
$Comp
L power:GND #PWR0101
U 1 1 5FD645ED
P 18250 1900
F 0 "#PWR0101" H 18250 1650 50  0001 C CNN
F 1 "GND" H 18255 1727 50  0000 C CNN
F 2 "" H 18250 1900 50  0001 C CNN
F 3 "" H 18250 1900 50  0001 C CNN
	1    18250 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	18700 5600 18350 5600
Text Label 18350 5600 2    50   ~ 0
DAC1_OUT
Wire Wire Line
	18700 5900 18350 5900
Wire Wire Line
	18700 6000 18350 6000
Wire Wire Line
	18700 6100 18350 6100
Wire Wire Line
	18700 6200 18350 6200
Text Label 18350 5900 2    50   ~ 0
MORPH_POT
Text Label 18350 6000 2    50   ~ 0
RATE_POT
Text Label 18350 6100 2    50   ~ 0
CHUNK_POT
Text Label 18350 6200 2    50   ~ 0
MORPH_CV
Wire Wire Line
	21000 6200 21350 6200
Wire Wire Line
	21000 6100 21350 6100
Text Label 21350 6200 0    50   ~ 0
RATE_CV
Text Label 21350 6100 0    50   ~ 0
CHUNK_CV
Wire Wire Line
	18700 2300 18250 2300
Text Label 18250 2300 2    50   ~ 0
LED_R
Wire Wire Line
	18700 2400 18250 2400
Text Label 18250 2400 2    50   ~ 0
LED_G
Wire Wire Line
	18700 2500 18250 2500
Text Label 18250 2500 2    50   ~ 0
LED_B
Wire Wire Line
	18700 2600 18250 2600
Text Label 18250 2600 2    50   ~ 0
BTN_MODE_SELECT
Wire Wire Line
	18700 3400 18250 3400
Wire Wire Line
	18700 3500 18250 3500
Text Label 18250 3400 2    50   ~ 0
CLK_IN
Text Label 18250 3500 2    50   ~ 0
TRIG_IN
Wire Wire Line
	18700 3600 18250 3600
Text Label 18250 3600 2    50   ~ 0
RESET_IN
$Comp
L Device:R_Small_US R7
U 1 1 5FCC3A1D
P 3200 1850
F 0 "R7" H 3132 1804 50  0000 R CNN
F 1 "10K" H 3132 1895 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3200 1850 50  0001 C CNN
F 3 "~" H 3200 1850 50  0001 C CNN
	1    3200 1850
	-1   0    0    1   
$EndComp
Text Label 3200 1550 0    50   ~ 0
3v3
Wire Wire Line
	3200 1750 3200 1550
Text Notes 6750 10000 0    50   ~ 0
Should this be analog ground / analog\n3v3 or a special ground reference?\n
Wire Wire Line
	18700 3700 18250 3700
Text Label 18250 3700 2    50   ~ 0
EOC_0
Wire Wire Line
	18700 3800 18250 3800
Text Label 18250 3800 2    50   ~ 0
EOC_1
Text Notes 5150 10000 0    50   ~ 0
Stub out last op-amp to avoid\ninterference - not sure how LOL
$Comp
L Amplifier_Operational:MCP6004 U2
U 5 1 6009C402
P 7550 9100
F 0 "U2" H 7508 9146 50  0000 L CNN
F 1 "MCP6004" H 7508 9055 50  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 7500 9200 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 7600 9300 50  0001 C CNN
	5    7550 9100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 600BACFF
P 7450 9500
F 0 "#PWR0104" H 7450 9250 50  0001 C CNN
F 1 "GND" H 7455 9327 50  0000 C CNN
F 2 "" H 7450 9500 50  0001 C CNN
F 3 "" H 7450 9500 50  0001 C CNN
	1    7450 9500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 8800 7450 8600
Text Label 7450 8600 0    50   ~ 0
3v3
Wire Wire Line
	7450 9400 7450 9500
$Comp
L Amplifier_Operational:MCP6004 U2
U 3 1 60095AE5
P 7000 7500
F 0 "U2" H 7000 7133 50  0000 C CNN
F 1 "MCP6004" H 7000 7224 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 6950 7600 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 7050 7700 50  0001 C CNN
	3    7000 7500
	1    0    0    1   
$EndComp
$Comp
L Amplifier_Operational:MCP6004 U2
U 2 1 60083EC0
P 7000 5250
F 0 "U2" H 7000 4883 50  0000 C CNN
F 1 "MCP6004" H 7000 4974 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 6950 5350 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 7050 5450 50  0001 C CNN
	2    7000 5250
	1    0    0    1   
$EndComp
Wire Wire Line
	6550 5150 6700 5150
Wire Wire Line
	6450 5350 6700 5350
$Comp
L Amplifier_Operational:MCP6004 U2
U 1 1 60078823
P 7000 2650
F 0 "U2" H 7000 3017 50  0000 C CNN
F 1 "MCP6004" H 7000 2926 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 6950 2750 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 7050 2850 50  0001 C CNN
	1    7000 2650
	1    0    0    1   
$EndComp
$Comp
L Device:R_Small_US R17
U 1 1 5FD95CCE
P 7000 2150
F 0 "R17" V 6795 2150 50  0000 C CNN
F 1 "33k" V 6886 2150 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7000 2150 50  0001 C CNN
F 3 "~" H 7000 2150 50  0001 C CNN
	1    7000 2150
	0    1    1    0   
$EndComp
Text Label 3600 7250 0    50   ~ 0
RESET_IN
Text Label 7800 7500 0    50   ~ 0
RATE_CV
$Comp
L Connector:AudioJack2 J4
U 1 1 5FDD3B45
P 5650 7500
F 0 "J4" H 5682 7825 50  0000 C CNN
F 1 "RATE" H 5682 7734 50  0000 C CNN
F 2 "Connector_Audio:Jack_3.5mm_QingPu_WQP-PJ398SM_Vertical_CircularHoles" H 5650 7500 50  0001 C CNN
F 3 "~" H 5650 7500 50  0001 C CNN
	1    5650 7500
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2 J5
U 1 1 5FD23EB7
P 5650 2650
F 0 "J5" H 5682 2975 50  0000 C CNN
F 1 "MORPH" H 5682 2884 50  0000 C CNN
F 2 "Connector_Audio:Jack_3.5mm_QingPu_WQP-PJ398SM_Vertical_CircularHoles" H 5650 2650 50  0001 C CNN
F 3 "~" H 5650 2650 50  0001 C CNN
	1    5650 2650
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2 J6
U 1 1 5FDBA4EE
P 5650 5250
F 0 "J6" H 5682 5575 50  0000 C CNN
F 1 "CHUNK" H 5682 5484 50  0000 C CNN
F 2 "Connector_Audio:Jack_3.5mm_QingPu_WQP-PJ398SM_Vertical_CircularHoles" H 5650 5250 50  0001 C CNN
F 3 "~" H 5650 5250 50  0001 C CNN
	1    5650 5250
	1    0    0    -1  
$EndComp
Text Label 5900 6750 2    50   ~ 0
AREF_-10V
Wire Wire Line
	5900 6750 6150 6750
Connection ~ 7550 7500
Wire Wire Line
	7550 7500 7800 7500
Wire Wire Line
	6450 7600 6700 7600
Wire Wire Line
	6450 7700 6450 7600
$Comp
L power:GND #PWR013
U 1 1 5FDD3B94
P 6450 7700
F 0 "#PWR013" H 6450 7450 50  0001 C CNN
F 1 "GND" H 6455 7527 50  0000 C CNN
F 2 "" H 6450 7700 50  0001 C CNN
F 3 "" H 6450 7700 50  0001 C CNN
	1    6450 7700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5FDD3B8A
P 5850 7700
F 0 "#PWR010" H 5850 7450 50  0001 C CNN
F 1 "GND" H 5855 7527 50  0000 C CNN
F 2 "" H 5850 7700 50  0001 C CNN
F 3 "" H 5850 7700 50  0001 C CNN
	1    5850 7700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 7500 5850 7700
Wire Wire Line
	5850 7400 6150 7400
Wire Wire Line
	6550 6750 6550 6550
Connection ~ 6550 6750
Wire Wire Line
	6350 6750 6550 6750
Connection ~ 7550 7000
Wire Wire Line
	7550 6550 7550 7000
Wire Wire Line
	7100 6550 7550 6550
Connection ~ 6550 7000
Wire Wire Line
	6550 6550 6900 6550
Wire Wire Line
	6550 7000 6550 6750
Wire Wire Line
	7550 7500 7300 7500
Wire Wire Line
	7550 7000 7550 7500
Wire Wire Line
	7100 7000 7550 7000
Wire Wire Line
	6550 7400 6700 7400
Connection ~ 6550 7400
Wire Wire Line
	6550 7000 6900 7000
Wire Wire Line
	6550 7400 6550 7000
Wire Wire Line
	6350 7400 6550 7400
$Comp
L Device:C_Small C1
U 1 1 5FDD3B6D
P 7000 6550
F 0 "C1" V 6771 6550 50  0000 C CNN
F 1 "1n" V 6862 6550 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7000 6550 50  0001 C CNN
F 3 "~" H 7000 6550 50  0001 C CNN
	1    7000 6550
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R10
U 1 1 5FDD3B63
P 6250 6750
F 0 "R10" V 6455 6750 50  0000 C CNN
F 1 "200k" V 6364 6750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6250 6750 50  0001 C CNN
F 3 "~" H 6250 6750 50  0001 C CNN
	1    6250 6750
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small_US R11
U 1 1 5FDD3B59
P 6250 7400
F 0 "R11" V 6045 7400 50  0000 C CNN
F 1 "100k" V 6136 7400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6250 7400 50  0001 C CNN
F 3 "~" H 6250 7400 50  0001 C CNN
	1    6250 7400
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R16
U 1 1 5FDD3B4F
P 7000 7000
F 0 "R16" V 6795 7000 50  0000 C CNN
F 1 "33k" V 6886 7000 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7000 7000 50  0001 C CNN
F 3 "~" H 7000 7000 50  0001 C CNN
	1    7000 7000
	0    1    1    0   
$EndComp
Text Label 5900 4500 2    50   ~ 0
AREF_-10V
Wire Wire Line
	5900 4500 6150 4500
Text Label 7800 5250 0    50   ~ 0
CHUNK_CV
Connection ~ 7550 5250
Wire Wire Line
	7550 5250 7800 5250
Wire Wire Line
	6450 5450 6450 5350
$Comp
L power:GND #PWR015
U 1 1 5FDBA547
P 6450 5450
F 0 "#PWR015" H 6450 5200 50  0001 C CNN
F 1 "GND" H 6455 5277 50  0000 C CNN
F 2 "" H 6450 5450 50  0001 C CNN
F 3 "" H 6450 5450 50  0001 C CNN
	1    6450 5450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5FDBA53D
P 5850 5450
F 0 "#PWR012" H 5850 5200 50  0001 C CNN
F 1 "GND" H 5855 5277 50  0000 C CNN
F 2 "" H 5850 5450 50  0001 C CNN
F 3 "" H 5850 5450 50  0001 C CNN
	1    5850 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 5250 5850 5450
Wire Wire Line
	5850 5150 6150 5150
Wire Wire Line
	6550 4500 6550 4300
Connection ~ 6550 4500
Wire Wire Line
	6350 4500 6550 4500
Connection ~ 7550 4750
Wire Wire Line
	7550 4300 7550 4750
Wire Wire Line
	7100 4300 7550 4300
Connection ~ 6550 4750
Wire Wire Line
	6550 4300 6900 4300
Wire Wire Line
	6550 4750 6550 4500
Wire Wire Line
	7550 5250 7300 5250
Wire Wire Line
	7550 4750 7550 5250
Wire Wire Line
	7100 4750 7550 4750
Connection ~ 6550 5150
Wire Wire Line
	6550 4750 6900 4750
Wire Wire Line
	6550 5150 6550 4750
Wire Wire Line
	6350 5150 6550 5150
$Comp
L Device:C_Small C3
U 1 1 5FDBA520
P 7000 4300
F 0 "C3" V 6771 4300 50  0000 C CNN
F 1 "1n" V 6862 4300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7000 4300 50  0001 C CNN
F 3 "~" H 7000 4300 50  0001 C CNN
	1    7000 4300
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R14
U 1 1 5FDBA516
P 6250 4500
F 0 "R14" V 6045 4500 50  0000 C CNN
F 1 "200k" V 6136 4500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6250 4500 50  0001 C CNN
F 3 "~" H 6250 4500 50  0001 C CNN
	1    6250 4500
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R15
U 1 1 5FDBA50C
P 6250 5150
F 0 "R15" V 6045 5150 50  0000 C CNN
F 1 "100k" V 6136 5150 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6250 5150 50  0001 C CNN
F 3 "~" H 6250 5150 50  0001 C CNN
	1    6250 5150
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R18
U 1 1 5FDBA502
P 7000 4750
F 0 "R18" V 6795 4750 50  0000 C CNN
F 1 "33k" V 6886 4750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7000 4750 50  0001 C CNN
F 3 "~" H 7000 4750 50  0001 C CNN
	1    7000 4750
	0    1    1    0   
$EndComp
Text Label 5900 1900 2    50   ~ 0
AREF_-10V
Wire Wire Line
	5900 1900 6150 1900
Text Label 7800 2650 0    50   ~ 0
MORPH_CV
Connection ~ 7550 2650
Wire Wire Line
	7550 2650 7800 2650
Wire Wire Line
	6450 2750 6700 2750
Wire Wire Line
	6450 2850 6450 2750
$Comp
L power:GND #PWR014
U 1 1 5FDB38A1
P 6450 2850
F 0 "#PWR014" H 6450 2600 50  0001 C CNN
F 1 "GND" H 6455 2677 50  0000 C CNN
F 2 "" H 6450 2850 50  0001 C CNN
F 3 "" H 6450 2850 50  0001 C CNN
	1    6450 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5FDB2B7B
P 5850 2850
F 0 "#PWR011" H 5850 2600 50  0001 C CNN
F 1 "GND" H 5855 2677 50  0000 C CNN
F 2 "" H 5850 2850 50  0001 C CNN
F 3 "" H 5850 2850 50  0001 C CNN
	1    5850 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 2650 5850 2850
Wire Wire Line
	5850 2550 6150 2550
Wire Wire Line
	6550 1900 6550 1700
Connection ~ 6550 1900
Wire Wire Line
	6350 1900 6550 1900
Connection ~ 7550 2150
Wire Wire Line
	7550 1700 7550 2150
Wire Wire Line
	7100 1700 7550 1700
Connection ~ 6550 2150
Wire Wire Line
	6550 1700 6900 1700
Wire Wire Line
	6550 2150 6550 1900
Wire Wire Line
	7550 2650 7300 2650
Wire Wire Line
	7550 2150 7550 2650
Wire Wire Line
	7100 2150 7550 2150
Wire Wire Line
	6550 2550 6700 2550
Connection ~ 6550 2550
Wire Wire Line
	6550 2150 6900 2150
Wire Wire Line
	6550 2550 6550 2150
Wire Wire Line
	6350 2550 6550 2550
$Comp
L Device:C_Small C2
U 1 1 5FDA1C7A
P 7000 1700
F 0 "C2" V 6771 1700 50  0000 C CNN
F 1 "1n" V 6862 1700 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7000 1700 50  0001 C CNN
F 3 "~" H 7000 1700 50  0001 C CNN
	1    7000 1700
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R12
U 1 1 5FD9ABA8
P 6250 1900
F 0 "R12" V 6045 1900 50  0000 C CNN
F 1 "200k" V 6136 1900 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6250 1900 50  0001 C CNN
F 3 "~" H 6250 1900 50  0001 C CNN
	1    6250 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R13
U 1 1 5FD97CBF
P 6250 2550
F 0 "R13" V 6045 2550 50  0000 C CNN
F 1 "100k" V 6136 2550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6250 2550 50  0001 C CNN
F 3 "~" H 6250 2550 50  0001 C CNN
	1    6250 2550
	0    1    1    0   
$EndComp
Text Label 3600 4700 0    50   ~ 0
TRIG_IN
Text Label 3600 2100 0    50   ~ 0
CLK_IN
Wire Wire Line
	3200 7250 3600 7250
Wire Wire Line
	2650 7600 2900 7600
Wire Wire Line
	3200 4700 3600 4700
Wire Wire Line
	2650 5050 2900 5050
Wire Wire Line
	2450 7600 2650 7600
Wire Wire Line
	2450 5050 2650 5050
Wire Wire Line
	2000 7600 2250 7600
Wire Wire Line
	2000 5050 2250 5050
Text Notes 12150 5450 0    50   ~ 0
1.29V
Text Notes 12100 3000 0    50   ~ 0
1.29V
$Comp
L Amplifier_Operational:MCP6004 U2
U 4 1 60098D77
P 5800 9550
F 0 "U2" H 5800 9917 50  0000 C CNN
F 1 "MCP6004" H 5800 9826 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 5750 9650 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 5850 9750 50  0001 C CNN
	4    5800 9550
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2 J9
U 1 1 600444C2
P 10500 5250
F 0 "J9" H 10532 5575 50  0000 C CNN
F 1 "OUT" H 10532 5484 50  0000 C CNN
F 2 "Connector_Audio:Jack_3.5mm_QingPu_WQP-PJ398SM_Vertical_CircularHoles" H 10500 5250 50  0001 C CNN
F 3 "~" H 10500 5250 50  0001 C CNN
	1    10500 5250
	1    0    0    -1  
$EndComp
$Comp
L Connector:AudioJack2 J7
U 1 1 5FCCE0C8
P 10450 2800
F 0 "J7" H 10482 3125 50  0000 C CNN
F 1 "OUT" H 10482 3034 50  0000 C CNN
F 2 "Connector_Audio:Jack_3.5mm_QingPu_WQP-PJ398SM_Vertical_CircularHoles" H 10450 2800 50  0001 C CNN
F 3 "~" H 10450 2800 50  0001 C CNN
	1    10450 2800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW1
U 1 1 6005B351
P 15700 5250
F 0 "SW1" H 15700 5485 50  0000 C CNN
F 1 "SW_SPST" H 15700 5394 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_SPST_PTS645" H 15700 5250 50  0001 C CNN
F 3 "~" H 15700 5250 50  0001 C CNN
	1    15700 5250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
