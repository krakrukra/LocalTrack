EESchema Schematic File Version 4
LIBS:LocalTrack-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "2020-10-17"
Rev "1.3"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_ST_STM32F0:STM32F072C8Tx U3
U 1 1 5C726180
P 4600 5600
F 0 "U3" H 4550 4014 50  0000 C CNN
F 1 "STM32F072C8T6" H 4550 3923 50  0000 C CNN
F 2 "Package_QFP:LQFP-48_7x7mm_P0.5mm" H 4000 4200 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00090510.pdf" H 4600 5600 50  0001 C CNN
F 4 "STMicroelectronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "ARM cortex M0 microcontroller" H 0   0   50  0001 C CNN "Short Description"
	1    4600 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C8
U 1 1 5C72629B
P 3850 3950
F 0 "C8" H 3965 3996 50  0000 L CNN
F 1 "100nF" H 3965 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3888 3800 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 3850 3950 50  0001 C CNN
F 4 "TDK" H -400 0   50  0001 C CNN "Manufacturer"
F 5 "X7R 10% 50V   eg. C1608X7R1H104K" H -400 0   50  0001 C CNN "Short Description"
	1    3850 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 5C72636B
P 3400 3950
F 0 "C6" H 3515 3996 50  0000 L CNN
F 1 "100nF" H 3515 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3438 3800 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 3400 3950 50  0001 C CNN
F 4 "TDK" H -350 0   50  0001 C CNN "Manufacturer"
F 5 "X7R 10% 50V   eg. C1608X7R1H104K" H -350 0   50  0001 C CNN "Short Description"
	1    3400 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5C726391
P 2950 3950
F 0 "C5" H 3065 3996 50  0000 L CNN
F 1 "100nF" H 3065 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2988 3800 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 2950 3950 50  0001 C CNN
F 4 "TDK" H -300 0   50  0001 C CNN "Manufacturer"
F 5 "X7R 10% 50V   eg. C1608X7R1H104K" H -300 0   50  0001 C CNN "Short Description"
	1    2950 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 4100 4700 4100
Wire Wire Line
	4700 4100 4600 4100
Connection ~ 4700 4100
Wire Wire Line
	4600 4100 4500 4100
Connection ~ 4600 4100
Wire Wire Line
	4500 4100 4400 4100
Connection ~ 4500 4100
Connection ~ 4400 4100
$Comp
L power:GND #PWR0102
U 1 1 5C72656C
P 2950 3800
F 0 "#PWR0102" H 2950 3550 50  0001 C CNN
F 1 "GND" H 2955 3627 50  0000 C CNN
F 2 "" H 2950 3800 50  0001 C CNN
F 3 "" H 2950 3800 50  0001 C CNN
	1    2950 3800
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5C726583
P 3400 3800
F 0 "#PWR0103" H 3400 3550 50  0001 C CNN
F 1 "GND" H 3405 3627 50  0000 C CNN
F 2 "" H 3400 3800 50  0001 C CNN
F 3 "" H 3400 3800 50  0001 C CNN
	1    3400 3800
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 5C726596
P 3850 3800
F 0 "#PWR0104" H 3850 3550 50  0001 C CNN
F 1 "GND" H 3855 3627 50  0000 C CNN
F 2 "" H 3850 3800 50  0001 C CNN
F 3 "" H 3850 3800 50  0001 C CNN
	1    3850 3800
	-1   0    0    1   
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 5C7265E7
P 2950 4750
F 0 "Y1" V 2904 4620 50  0000 R CNN
F 1 "8MHz" V 2995 4620 50  0000 R CNN
F 2 "Crystal:Crystal_SMD_5032-2Pin_5.0x3.2mm" H 2950 4750 50  0001 C CNN
F 3 "https://www.ndk.com/images/products/catalog/c_NX5032GA_e.pdf" H 2950 4750 50  0001 C CNN
F 4 "Nihon Dempa Kogyo" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "NX5032GA-8MHZ(STD-CSK-7) crystal resonator" H 0   0   50  0001 C CNN "Short Description"
	1    2950 4750
	0    1    1    0   
$EndComp
Wire Wire Line
	3900 4700 3150 4700
Wire Wire Line
	3150 4700 3150 4500
Wire Wire Line
	3150 4500 2950 4500
Wire Wire Line
	2950 4500 2950 4600
Wire Wire Line
	2950 4900 2950 5000
Wire Wire Line
	2950 5000 3150 5000
Wire Wire Line
	3150 4800 3900 4800
Wire Wire Line
	3150 4800 3150 5000
$Comp
L Device:C C4
U 1 1 5C72694F
P 2800 5000
F 0 "C4" V 2960 5000 50  0000 C CNN
F 1 "18pF" V 3051 5000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2838 4850 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 2800 5000 50  0001 C CNN
F 4 "TDK" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "C0G 5%   50V   eg. C1608C0G1H180J" H 0   0   50  0001 C CNN "Short Description"
	1    2800 5000
	0    1    1    0   
$EndComp
Connection ~ 2950 5000
$Comp
L Device:C C3
U 1 1 5C7269B5
P 2800 4500
F 0 "C3" V 2548 4500 50  0000 C CNN
F 1 "18pF" V 2639 4500 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2838 4350 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 2800 4500 50  0001 C CNN
F 4 "TDK" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "C0G 5%   50V   eg. C1608C0G1H180J" H 0   0   50  0001 C CNN "Short Description"
	1    2800 4500
	0    1    1    0   
$EndComp
Connection ~ 2950 4500
$Comp
L power:GND #PWR0105
U 1 1 5C726A1D
P 2650 5000
F 0 "#PWR0105" H 2650 4750 50  0001 C CNN
F 1 "GND" V 2655 4872 50  0000 R CNN
F 2 "" H 2650 5000 50  0001 C CNN
F 3 "" H 2650 5000 50  0001 C CNN
	1    2650 5000
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 5C726A4B
P 2650 4500
F 0 "#PWR0106" H 2650 4250 50  0001 C CNN
F 1 "GND" V 2655 4372 50  0000 R CNN
F 2 "" H 2650 4500 50  0001 C CNN
F 3 "" H 2650 4500 50  0001 C CNN
	1    2650 4500
	0    1    1    0   
$EndComp
Wire Wire Line
	4700 7100 4600 7100
Wire Wire Line
	4600 7100 4500 7100
Connection ~ 4600 7100
Wire Wire Line
	4500 7100 4400 7100
Connection ~ 4500 7100
Wire Wire Line
	4400 7100 4250 7100
Connection ~ 4400 7100
$Comp
L power:GND #PWR0107
U 1 1 5C726F5B
P 4250 7100
F 0 "#PWR0107" H 4250 6850 50  0001 C CNN
F 1 "GND" V 4255 6972 50  0000 R CNN
F 2 "" H 4250 7100 50  0001 C CNN
F 3 "" H 4250 7100 50  0001 C CNN
	1    4250 7100
	0    1    1    0   
$EndComp
$Comp
L LocalTrack:ADXL345BCCZ U1
U 1 1 5C726FD8
P 1750 6150
F 0 "U1" H 2250 6650 60  0000 L CNN
F 1 "ADXL345BCCZ" H 2250 6500 60  0000 L CNN
F 2 "Package_LGA:LGA-14_3x5mm_P0.8mm_LayoutBorder1x6y" H 2300 5650 60  0001 C CNN
F 3 "http://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf" H 2300 5650 60  0001 C CNN
F 4 "Analog Devices" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "MEMS accelerometer" H 0   0   50  0001 C CNN "Short Description"
	1    1750 6150
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 5C72D3CF
P 1450 6900
F 0 "#PWR0108" H 1450 6650 50  0001 C CNN
F 1 "GND" V 1455 6772 50  0000 R CNN
F 2 "" H 1450 6900 50  0001 C CNN
F 3 "" H 1450 6900 50  0001 C CNN
	1    1450 6900
	0    1    1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 5C72D3F2
P 1500 5250
F 0 "C2" H 1615 5296 50  0000 L CNN
F 1 "100nF" H 1615 5205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1538 5100 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 1500 5250 50  0001 C CNN
F 4 "TDK" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "X7R 10% 50V   eg. C1608X7R1H104K" H 0   0   50  0001 C CNN "Short Description"
	1    1500 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 5400 1350 5400
Connection ~ 1500 5400
$Comp
L power:GND #PWR0109
U 1 1 5C72E7AA
P 1500 5100
F 0 "#PWR0109" H 1500 4850 50  0001 C CNN
F 1 "GND" V 1505 4972 50  0000 R CNN
F 2 "" H 1500 5100 50  0001 C CNN
F 3 "" H 1500 5100 50  0001 C CNN
	1    1500 5100
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 5C7329E6
P 2750 5900
F 0 "R1" H 2700 5950 50  0000 R CNN
F 1 "1k" H 2700 5850 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2680 5900 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 2750 5900 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0102G" H 0   0   50  0001 C CNN "Short Description"
	1    2750 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5C734B3A
P 3050 5900
F 0 "R3" H 3000 5950 50  0000 R CNN
F 1 "1k" H 3000 5850 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2980 5900 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 3050 5900 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0102G" H 0   0   50  0001 C CNN "Short Description"
	1    3050 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 5750 2750 5400
Wire Wire Line
	3050 5750 3050 5400
Wire Wire Line
	3050 5400 2750 5400
Connection ~ 2350 5400
Wire Wire Line
	2750 6050 2750 6200
Wire Wire Line
	3050 6050 3050 6300
Text GLabel 1150 6250 0    50   Input ~ 0
int1
Text GLabel 950  6150 0    50   Input ~ 0
int2
Wire Wire Line
	950  6150 1150 6150
Text GLabel 3650 5200 0    50   Input ~ 0
int1
Text GLabel 3900 5100 0    50   Input ~ 0
int2
Text GLabel 6400 5700 0    50   Input ~ 0
cs
Text GLabel 5450 5700 2    50   Input ~ 0
cs
$Comp
L power:GND #PWR0110
U 1 1 5C757114
P 6850 5950
F 0 "#PWR0110" H 6850 5700 50  0001 C CNN
F 1 "GND" H 6855 5777 50  0000 C CNN
F 2 "" H 6850 5950 50  0001 C CNN
F 3 "" H 6850 5950 50  0001 C CNN
	1    6850 5950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C14
U 1 1 5C75716F
P 6750 5000
F 0 "C14" H 6865 5046 50  0000 L CNN
F 1 "100nF" H 6865 4955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6788 4850 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 6750 5000 50  0001 C CNN
F 4 "TDK" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "X7R 10% 50V   eg. C1608X7R1H104K" H 0   0   50  0001 C CNN "Short Description"
	1    6750 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 5C75A7BE
P 6750 4850
F 0 "#PWR0111" H 6750 4600 50  0001 C CNN
F 1 "GND" H 6755 4677 50  0000 C CNN
F 2 "" H 6750 4850 50  0001 C CNN
F 3 "" H 6750 4850 50  0001 C CNN
	1    6750 4850
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 5C75AB68
P 3250 4300
F 0 "#PWR0112" H 3250 4050 50  0001 C CNN
F 1 "GND" V 3250 4200 50  0000 R CNN
F 2 "" H 3250 4300 50  0001 C CNN
F 3 "" H 3250 4300 50  0001 C CNN
	1    3250 4300
	0    1    -1   0   
$EndComp
Wire Wire Line
	3600 4300 3900 4300
$Comp
L LocalTrack:SIM28 U9
U 1 1 5C76F536
P 8750 4850
F 0 "U9" H 9200 4100 60  0000 C CNN
F 1 "SIM28" H 9200 4000 60  0000 C CNN
F 2 "LocalTrack:SIM28" H 8850 5050 60  0001 C CNN
F 3 "http://www.sabreadv.com/wp-content/uploads/SIM28_Hardware-Design_V1.06.pdf" H 8850 5050 60  0001 C CNN
F 4 "SIMCom Wireless Solutions" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "GPS receiver module" H 0   0   50  0001 C CNN "Short Description"
	1    8750 4850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 5C774564
P 8450 5700
F 0 "#PWR0113" H 8450 5450 50  0001 C CNN
F 1 "GND" V 8455 5572 50  0000 R CNN
F 2 "" H 8450 5700 50  0001 C CNN
F 3 "" H 8450 5700 50  0001 C CNN
	1    8450 5700
	0    1    1    0   
$EndComp
NoConn ~ 8050 4800
NoConn ~ 8050 4900
NoConn ~ 8050 5100
NoConn ~ 8050 5200
NoConn ~ 8050 5300
NoConn ~ 8050 5400
Wire Wire Line
	6750 5150 6500 5150
Connection ~ 6750 5150
$Comp
L Device:C C19
U 1 1 5C7AEAAA
P 8600 3850
F 0 "C19" H 8715 3896 50  0000 L CNN
F 1 "100nF" H 8715 3805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8638 3700 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 8600 3850 50  0001 C CNN
F 4 "TDK" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "X7R 10% 50V   eg. C1608X7R1H104K" H 0   0   50  0001 C CNN "Short Description"
	1    8600 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 4000 8450 4000
Connection ~ 8600 4000
$Comp
L power:GND #PWR0114
U 1 1 5C7B1B66
P 8600 3700
F 0 "#PWR0114" H 8600 3450 50  0001 C CNN
F 1 "GND" H 8605 3527 50  0000 C CNN
F 2 "" H 8600 3700 50  0001 C CNN
F 3 "" H 8600 3700 50  0001 C CNN
	1    8600 3700
	-1   0    0    1   
$EndComp
$Comp
L Device:R R12
U 1 1 5C7C4311
P 10000 4150
F 0 "R12" H 10070 4196 50  0000 L CNN
F 1 "10k" H 10070 4105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9930 4150 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 10000 4150 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0103G" H 0   0   50  0001 C CNN "Short Description"
	1    10000 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:Antenna AE2
U 1 1 5C7CE435
P 9600 4200
F 0 "AE2" H 9650 4050 50  0000 L CNN
F 1 "1575R-A" H 9450 3950 50  0000 L CNN
F 2 "LocalTrack:1575R-A" H 9600 4200 50  0001 C CNN
F 3 "https://www.aliexpress.com/item/32607867436.html?spm=2114.12010615.8148356.1.6eef2d29Tl6l2M" H 9600 4200 50  0001 C CNN
F 4 "~" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "GPS patch antenna, 1575.42MHz, 25x25mm, 4mm height" H 0   0   50  0001 C CNN "Short Description"
	1    9600 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 4400 9600 4400
NoConn ~ 9450 4300
$Comp
L Device:LED D3
U 1 1 5C7C4219
P 10000 4450
F 0 "D3" V 10038 4528 50  0000 L CNN
F 1 "BLUE" V 9947 4528 50  0000 L CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 10000 4450 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1912191207_Foshan-NationStar-Optoelectronics-NCD0603B3_C158101.pdf" H 10000 4450 50  0001 C CNN
F 4 "Foshan NationStar " H 0   0   50  0001 C CNN "Manufacturer"
F 5 "eg. NCD0603B3" H 0   0   50  0001 C CNN "Short Description"
	1    10000 4450
	0    1    -1   0   
$EndComp
$Comp
L LocalTrack:ST-LINK_V2 CON1
U 1 1 5C7DD649
P 900 2800
F 0 "CON1" H 900 2450 50  0000 C CNN
F 1 "ST-LINK_V2" H 900 2350 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" V 150 2850 50  0001 C CNN
F 3 "https://www.aliexpress.com/item/32596261414.html?spm=a2g0s.9042311.0.0.27424c4dZ8Ol8n" H 1100 2800 50  0001 C CNN
F 4 "~" H -100 750 50  0001 C CNN "Manufacturer"
F 5 "~" H -100 750 50  0001 C CNN "Short Description"
	1    900  2800
	1    0    0    1   
$EndComp
$Comp
L Connector:USB_B_Micro J1
U 1 1 5C7DFC5F
P 950 1050
F 0 "J1" H 1005 1517 50  0000 C CNN
F 1 "USB_B_Micro" H 1005 1426 50  0000 C CNN
F 2 "LocalTrack:MicroUSB_B" H 1100 1000 50  0001 C CNN
F 3 "https://www.aliexpress.com/item/32765504799.html?spm=a2g0s.9042311.0.0.27424c4d25ZRpn" H 1100 1000 50  0001 C CNN
F 4 "~" H -100 0   50  0001 C CNN "Manufacturer"
F 5 "USB micro B receptacle" H 950 1050 50  0001 C CNN "Short Description"
	1    950  1050
	1    0    0    -1  
$EndComp
$Comp
L LocalTrack:IP4220CZ6 U2
U 1 1 5C7DFE9D
P 1900 1700
F 0 "U2" H 1573 1753 60  0000 R CNN
F 1 "IP4220CZ6" H 1573 1647 60  0000 R CNN
F 2 "Package_SO:TSOP-6_1.65x3.05mm_P0.95mm" H 1900 1600 60  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/IP4220CZ6.pdf" H 1900 1600 60  0001 C CNN
F 4 "Nexperia" H -350 -900 50  0001 C CNN "Manufacturer"
F 5 "ESD protection diode for USB" H -350 -900 50  0001 C CNN "Short Description"
	1    1900 1700
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:MCP1700-3002E_SOT23 U7
U 1 1 5C7E2528
P 7900 1050
F 0 "U7" H 7900 1300 50  0000 C CNN
F 1 "MCP1700-3302E" H 7900 1200 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7900 1275 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001826C.pdf" H 7900 1050 50  0001 C CNN
F 4 "Microchip Technology" H 300 0   50  0001 C CNN "Manufacturer"
F 5 "3.3V LDO linear voltage regulator" H 300 0   50  0001 C CNN "Short Description"
	1    7900 1050
	1    0    0    -1  
$EndComp
$Comp
L Battery_Management:MCP73831-2-OT U4
U 1 1 5C7EE99F
P 5650 1150
F 0 "U4" H 5150 900 50  0000 C CNN
F 1 "MCP73831T-2DCI" H 5150 800 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 5700 900 50  0001 L CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001984g.pdf" H 5500 1100 50  0001 C CNN
F 4 "Microchip Technology" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "single li-po cell charging IC" H 0   0   50  0001 C CNN "Short Description"
	1    5650 1150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 5C7EEB78
P 7250 1250
F 0 "C11" H 7365 1296 50  0000 L CNN
F 1 "10uF" H 7365 1205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7288 1100 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 7250 1250 50  0001 C CNN
F 4 "TDK" H 650 0   50  0001 C CNN "Manufacturer"
F 5 "X5R 10% 16V   eg. C2012X5R1C106K" H 650 0   50  0001 C CNN "Short Description"
	1    7250 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5C7FB68B
P 1550 2700
F 0 "C1" V 1800 2700 50  0000 C CNN
F 1 "10uF" V 1700 2700 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1588 2550 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 1550 2700 50  0001 C CNN
F 4 "TDK" H 50  400 50  0001 C CNN "Manufacturer"
F 5 "X5R 10% 16V   eg. C2012X5R1C106K" H 50  400 50  0001 C CNN "Short Description"
	1    1550 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7250 1400 7250 1450
Wire Wire Line
	6050 1050 6400 1050
Wire Wire Line
	6900 1100 6900 1050
$Comp
L power:GND #PWR0115
U 1 1 5C80FA26
P 1150 2800
F 0 "#PWR0115" H 1150 2550 50  0001 C CNN
F 1 "GND" V 1155 2672 50  0000 R CNN
F 2 "" H 1150 2800 50  0001 C CNN
F 3 "" H 1150 2800 50  0001 C CNN
	1    1150 2800
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D2
U 1 1 5C821566
P 3300 1550
F 0 "D2" V 3338 1629 50  0000 L CNN
F 1 "RED" V 3247 1629 50  0000 L CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 3300 1550 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1912191206_Foshan-NationStar-Optoelectronics-NCD0603R2_C90692.pdf" H 3300 1550 50  0001 C CNN
F 4 "Foshan NationStar " H 0   0   50  0001 C CNN "Manufacturer"
F 5 "eg. NCD0603R2" H 0   0   50  0001 C CNN "Short Description"
	1    3300 1550
	0    1    -1   0   
$EndComp
$Comp
L Device:LED D1
U 1 1 5C8217A6
P 2900 1550
F 0 "D1" V 2938 1432 50  0000 R CNN
F 1 "GREEN" V 2847 1432 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 2900 1550 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1912191206_Foshan-NationStar-Optoelectronics-NCD0603G2_C87326.pdf" H 2900 1550 50  0001 C CNN
F 4 "Foshan NationStar" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "eg. NCD0603G2" H 0   0   50  0001 C CNN "Short Description"
	1    2900 1550
	0    1    -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5C821A58
P 3100 1100
F 0 "R2" H 3200 1150 50  0000 L CNN
F 1 "10k" H 3200 1050 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3030 1100 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 3100 1100 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0103G" H 0   0   50  0001 C CNN "Short Description"
	1    3100 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 1400 3300 1350
Wire Wire Line
	2900 1400 2900 1350
Text GLabel 6700 1050 0    50   Input ~ 0
4.2V
Text GLabel 2750 850  0    50   Input ~ 0
5V
NoConn ~ 1250 1250
Text GLabel 1500 1050 0    50   Input ~ 0
d+
Text GLabel 1700 1150 0    50   Input ~ 0
d-
Wire Wire Line
	950  1450 900  1450
$Comp
L power:GND #PWR0118
U 1 1 5C872BFB
P 900 1450
F 0 "#PWR0118" H 900 1200 50  0001 C CNN
F 1 "GND" H 900 1300 50  0000 C CNN
F 2 "" H 900 1450 50  0001 C CNN
F 3 "" H 900 1450 50  0001 C CNN
	1    900  1450
	1    0    0    -1  
$EndComp
Connection ~ 900  1450
Wire Wire Line
	900  1450 850  1450
Wire Wire Line
	2900 1350 3100 1350
$Comp
L power:GND #PWR0119
U 1 1 5C8C1C44
P 5650 1450
F 0 "#PWR0119" H 5650 1200 50  0001 C CNN
F 1 "GND" H 5650 1300 50  0000 C CNN
F 2 "" H 5650 1450 50  0001 C CNN
F 3 "" H 5650 1450 50  0001 C CNN
	1    5650 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0120
U 1 1 5C8C1CA3
P 7250 1450
F 0 "#PWR0120" H 7250 1200 50  0001 C CNN
F 1 "GND" H 7250 1300 50  0000 C CNN
F 2 "" H 7250 1450 50  0001 C CNN
F 3 "" H 7250 1450 50  0001 C CNN
	1    7250 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 5C8C2041
P 1900 2050
F 0 "#PWR0123" H 1900 1800 50  0001 C CNN
F 1 "GND" H 1900 1900 50  0000 C CNN
F 2 "" H 1900 2050 50  0001 C CNN
F 3 "" H 1900 2050 50  0001 C CNN
	1    1900 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 1250 3100 1350
Connection ~ 3100 1350
Wire Wire Line
	3100 1350 3300 1350
$Comp
L power:GND #PWR0124
U 1 1 5C965A89
P 7900 1450
F 0 "#PWR0124" H 7900 1200 50  0001 C CNN
F 1 "GND" H 7900 1300 50  0000 C CNN
F 2 "" H 7900 1450 50  0001 C CNN
F 3 "" H 7900 1450 50  0001 C CNN
	1    7900 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 1450 7900 1350
Text GLabel 5350 6500 2    50   Input ~ 0
d-
Text GLabel 5550 6600 2    50   Input ~ 0
d+
Wire Wire Line
	5550 6600 5200 6600
$Comp
L Device:C C7
U 1 1 5CA0E90C
P 3750 1100
F 0 "C7" H 3868 1146 50  0000 L CNN
F 1 "10uF" H 3868 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3788 950 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 3750 1100 50  0001 C CNN
F 4 "TDK" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "X5R 10% 16V   eg. C2012X5R1C106K" H 0   0   50  0001 C CNN "Short Description"
	1    3750 1100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0127
U 1 1 5CA0E9C6
P 3750 1350
F 0 "#PWR0127" H 3750 1100 50  0001 C CNN
F 1 "GND" H 3750 1200 50  0000 C CNN
F 2 "" H 3750 1350 50  0001 C CNN
F 3 "" H 3750 1350 50  0001 C CNN
	1    3750 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 1350 3750 1250
Text GLabel 5950 6700 2    50   Input ~ 0
swdio
Text GLabel 6250 6800 2    50   Input ~ 0
swclk
Wire Wire Line
	5350 6500 5200 6500
Wire Wire Line
	5650 6800 5200 6800
Text GLabel 1400 2900 2    50   Input ~ 0
swdio
Text GLabel 1700 3000 2    50   Input ~ 0
swclk
$Comp
L Device:R R7
U 1 1 5CA58372
P 5350 6700
F 0 "R7" V 5400 6900 50  0000 C CNN
F 1 "33" V 5350 6700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5280 6700 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 5350 6700 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0330G" H 0   0   50  0001 C CNN "Short Description"
	1    5350 6700
	0    1    1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 5CA58640
P 5800 6800
F 0 "R9" V 5850 7000 50  0000 C CNN
F 1 "33" V 5800 6800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5730 6800 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 5800 6800 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0330G" H 0   0   50  0001 C CNN "Short Description"
	1    5800 6800
	0    1    1    0   
$EndComp
Wire Wire Line
	5500 6700 5950 6700
Wire Wire Line
	6250 6800 5950 6800
Text GLabel 9650 4950 3    50   Input ~ 0
1pps
Text GLabel 5450 5500 2    50   Input ~ 0
1pps
Wire Wire Line
	6000 6100 6000 5600
Wire Wire Line
	6000 5600 6400 5600
Wire Wire Line
	5900 5500 6400 5500
Wire Wire Line
	6400 5400 5800 5400
Wire Wire Line
	5800 5400 5800 5900
Wire Wire Line
	5900 6000 5900 5500
$Comp
L Device:R R8
U 1 1 5C91C3CE
P 5600 5900
F 0 "R8" V 5650 6100 50  0000 C CNN
F 1 "33" V 5600 5900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5530 5900 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 5600 5900 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0330G" H 0   0   50  0001 C CNN "Short Description"
	1    5600 5900
	0    1    1    0   
$EndComp
Wire Wire Line
	5450 5900 5200 5900
Wire Wire Line
	5200 6100 6000 6100
Wire Wire Line
	5200 6000 5900 6000
Wire Wire Line
	5750 5900 5800 5900
Text GLabel 3550 6000 0    50   Input ~ 0
mcu_tx
Text GLabel 3900 6100 0    50   Input ~ 0
mcu_rx
Text GLabel 7700 4600 0    50   Input ~ 0
mcu_rx
Text GLabel 8050 4500 0    50   Input ~ 0
mcu_tx
Wire Wire Line
	7700 4600 8050 4600
Wire Wire Line
	3900 6000 3550 6000
$Comp
L power:GND #PWR0143
U 1 1 5C840684
P 5200 6900
F 0 "#PWR0143" H 5200 6650 50  0001 C CNN
F 1 "GND" V 5200 6700 50  0000 C CNN
F 2 "" H 5200 6900 50  0001 C CNN
F 3 "" H 5200 6900 50  0001 C CNN
	1    5200 6900
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0140
U 1 1 5C840E7D
P 3650 5400
F 0 "#PWR0140" H 3650 5150 50  0001 C CNN
F 1 "GND" V 3650 5200 50  0000 C CNN
F 2 "" H 3650 5400 50  0001 C CNN
F 3 "" H 3650 5400 50  0001 C CNN
	1    3650 5400
	0    1    1    0   
$EndComp
Wire Wire Line
	3650 5400 3900 5400
$Comp
L power:GND #PWR0144
U 1 1 5C8827E9
P 3650 6800
F 0 "#PWR0144" H 3650 6550 50  0001 C CNN
F 1 "GND" V 3650 6600 50  0000 C CNN
F 2 "" H 3650 6800 50  0001 C CNN
F 3 "" H 3650 6800 50  0001 C CNN
	1    3650 6800
	0    1    1    0   
$EndComp
Wire Wire Line
	5200 6400 5250 6400
Wire Wire Line
	3650 6800 3900 6800
Wire Wire Line
	2350 5400 2350 6000
Wire Wire Line
	1500 5400 1650 5400
Wire Wire Line
	1450 6900 1600 6900
Connection ~ 1600 6900
Wire Wire Line
	1600 6900 1700 6900
Connection ~ 1700 6900
Wire Wire Line
	1700 6900 1800 6900
Connection ~ 1800 6900
Wire Wire Line
	1800 6900 1900 6900
Connection ~ 1850 5400
Wire Wire Line
	1850 5400 2350 5400
Connection ~ 1750 5400
Wire Wire Line
	1750 5400 1850 5400
Connection ~ 1650 5400
Wire Wire Line
	1650 5400 1750 5400
$Comp
L Device:Battery_Cell BT1
U 1 1 5D16F076
P 6900 1300
F 0 "BT1" H 7018 1396 50  0000 L CNN
F 1 "3000mAh" H 7018 1305 50  0000 L CNN
F 2 "Connector_JST:JST_PH_B2B-PH-K_1x02_P2.00mm_Vertical" V 6900 1360 50  0001 C CNN
F 3 "https://www.aliexpress.com/item/32879938149.html?spm=2114.12010615.8148356.3.24ac1e7bH18zEq" V 6900 1360 50  0001 C CNN
F 4 "EHAO Technology Co Ltd" H -50 0   50  0001 C CNN "Manufacturer"
F 5 "103565 li-po battery" H -50 0   50  0001 C CNN "Short Description"
	1    6900 1300
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0141
U 1 1 5D16F2FB
P 6900 1450
F 0 "#PWR0141" H 6900 1200 50  0001 C CNN
F 1 "GND" H 6900 1300 50  0000 C CNN
F 2 "" H 6900 1450 50  0001 C CNN
F 3 "" H 6900 1450 50  0001 C CNN
	1    6900 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 1450 6900 1400
Wire Wire Line
	7250 1100 7250 1050
$Comp
L power:GND #PWR0147
U 1 1 5D211DB4
P 3850 5900
F 0 "#PWR0147" H 3850 5650 50  0001 C CNN
F 1 "GND" V 3850 5700 50  0000 C CNN
F 2 "" H 3850 5900 50  0001 C CNN
F 3 "" H 3850 5900 50  0001 C CNN
	1    3850 5900
	0    1    1    0   
$EndComp
Wire Wire Line
	3850 5900 3900 5900
Wire Wire Line
	7300 5150 7300 5500
Wire Wire Line
	6750 5150 6850 5150
Wire Wire Line
	8600 4000 8700 4000
$Comp
L LocalTrack:W25N01GVZEIG U6
U 1 1 5E670D47
P 6850 5550
F 0 "U6" H 6300 5250 50  0000 C CNN
F 1 "W25N01GVZEIG" H 6400 5150 50  0000 C CNN
F 2 "LocalTrack:WSON-8_8x6mm" H 6850 5500 50  0001 C CNN
F 3 "https://www.winbond.com/resource-files/w25n01gv revg 032116.pdf" H 6850 5500 50  0001 C CNN
F 4 "Winbond Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "128MiB serial NAND flash memory" H 0   0   50  0001 C CNN "Short Description"
	1    6850 5550
	1    0    0    -1  
$EndComp
Connection ~ 6850 5150
Wire Wire Line
	6850 5150 7300 5150
Wire Wire Line
	8450 5700 8600 5700
Wire Wire Line
	8600 5700 8700 5700
Connection ~ 8600 5700
Wire Wire Line
	8700 5700 8800 5700
Connection ~ 8700 5700
Wire Wire Line
	8800 5700 8900 5700
Connection ~ 8800 5700
Connection ~ 8700 4000
Wire Wire Line
	8700 4000 8800 4000
Connection ~ 8800 4000
Wire Wire Line
	8800 4000 10000 4000
Wire Wire Line
	7300 5500 7300 5600
Connection ~ 7300 5500
Text GLabel 3900 4500 0    50   Input ~ 0
boot0
Text GLabel 2200 2600 0    50   Input ~ 0
boot0
$Comp
L Device:R R4
U 1 1 5E6CF474
P 2550 2800
F 0 "R4" H 2480 2846 50  0000 R CNN
F 1 "10k" H 2480 2755 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2480 2800 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 2550 2800 50  0001 C CNN
F 4 "LIZ Electronics" H 1100 -250 50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0103G" H 1100 -250 50  0001 C CNN "Short Description"
	1    2550 2800
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C9
U 1 1 5E6CF5D3
P 2300 2800
F 0 "C9" H 2415 2846 50  0000 L CNN
F 1 "100nF" H 2415 2755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2338 2650 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 2300 2800 50  0001 C CNN
F 4 "TDK" H 1150 -250 50  0001 C CNN "Manufacturer"
F 5 "X7R 10% 50V   eg. C1608X7R1H104K" H 1150 -250 50  0001 C CNN "Short Description"
	1    2300 2800
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5E6DBBFA
P 2300 3000
F 0 "#PWR0101" H 2300 2750 50  0001 C CNN
F 1 "GND" H 2300 2850 50  0000 C CNN
F 2 "" H 2300 3000 50  0001 C CNN
F 3 "" H 2300 3000 50  0001 C CNN
	1    2300 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 5E6DBC5D
P 2550 3000
F 0 "#PWR0117" H 2550 2750 50  0001 C CNN
F 1 "GND" H 2550 2850 50  0000 C CNN
F 2 "" H 2550 3000 50  0001 C CNN
F 3 "" H 2550 3000 50  0001 C CNN
	1    2550 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 3000 2550 2950
Wire Wire Line
	2300 2950 2300 3000
$Comp
L Switch:SW_SPST SW1
U 1 1 5E750CE5
P 9150 900
F 0 "SW1" H 9150 1135 50  0000 C CNN
F 1 "KCD1-11" H 9150 1044 50  0000 C CNN
F 2 "LocalTrack:KCD1-11" H 9150 900 50  0001 C CNN
F 3 "https://www.aliexpress.com/item/32629402383.html?spm=2114.12010615.8148356.2.36266a0aCvHnTc" H 9150 900 50  0001 C CNN
F 4 "~" H -200 -400 50  0001 C CNN "Manufacturer"
F 5 "rocker switch with optional waterproof cover" H -200 -400 50  0001 C CNN "Short Description"
	1    9150 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R11
U 1 1 5E8158CD
P 5050 1250
F 0 "R11" V 4843 1250 50  0000 C CNN
F 1 "1k" V 4934 1250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4980 1250 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 5050 1250 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0102G" H 0   0   50  0001 C CNN "Short Description"
	1    5050 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 5E822FF0
P 4700 1250
F 0 "R6" V 4493 1250 50  0000 C CNN
F 1 "1k" V 4584 1250 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4630 1250 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 4700 1250 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0102G" H 0   0   50  0001 C CNN "Short Description"
	1    4700 1250
	0    1    1    0   
$EndComp
Wire Wire Line
	5200 1250 5250 1250
Wire Wire Line
	4900 1250 4850 1250
$Comp
L Battery_Management:MCP73831-2-OT U5
U 1 1 5E7726B0
P 5650 2100
F 0 "U5" H 5150 1850 50  0000 C CNN
F 1 "MCP73831T-2DCI" H 5150 1750 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 5700 1850 50  0001 L CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001984g.pdf" H 5500 2050 50  0001 C CNN
F 4 "Microchip Technology" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "single li-po cell charging IC" H 0   0   50  0001 C CNN "Short Description"
	1    5650 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 5E78BA55
P 5050 2200
F 0 "R13" V 4843 2200 50  0000 C CNN
F 1 "1k" V 4934 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4980 2200 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 5050 2200 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0102G" H 0   0   50  0001 C CNN "Short Description"
	1    5050 2200
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 5E78BAE1
P 4700 2200
F 0 "R10" V 4493 2200 50  0000 C CNN
F 1 "1k" V 4584 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4630 2200 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 4700 2200 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0102G" H 0   0   50  0001 C CNN "Short Description"
	1    4700 2200
	0    1    1    0   
$EndComp
Text GLabel 4450 2200 0    50   Input ~ 0
chg2
Text GLabel 4450 1250 0    50   Input ~ 0
chg1
$Comp
L power:GND #PWR0121
U 1 1 5E7B27D2
P 5650 2400
F 0 "#PWR0121" H 5650 2150 50  0001 C CNN
F 1 "GND" H 5650 2250 50  0000 C CNN
F 2 "" H 5650 2400 50  0001 C CNN
F 3 "" H 5650 2400 50  0001 C CNN
	1    5650 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 950  3750 850 
Wire Wire Line
	4900 2200 4850 2200
Wire Wire Line
	5250 2200 5200 2200
Wire Wire Line
	5650 1800 5650 1750
Wire Wire Line
	5650 1750 4100 1750
Wire Wire Line
	4100 1750 4100 850 
Connection ~ 4100 850 
Wire Wire Line
	4100 850  5650 850 
NoConn ~ 6050 1250
Wire Wire Line
	6050 2000 6400 2000
Wire Wire Line
	6400 2000 6400 1050
$Comp
L Device:C C10
U 1 1 5E8BF25C
P 9650 1250
F 0 "C10" H 9768 1296 50  0000 L CNN
F 1 "10uF" H 9768 1205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 9688 1100 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 9650 1250 50  0001 C CNN
F 4 "TDK" H 1600 0   50  0001 C CNN "Manufacturer"
F 5 "X5R 10% 16V   eg. C2012X5R1C106K" H 1600 0   50  0001 C CNN "Short Description"
	1    9650 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 1100 9650 1050
$Comp
L power:GND #PWR0125
U 1 1 5E8C6C79
P 9650 1450
F 0 "#PWR0125" H 9650 1200 50  0001 C CNN
F 1 "GND" H 9650 1300 50  0000 C CNN
F 2 "" H 9650 1450 50  0001 C CNN
F 3 "" H 9650 1450 50  0001 C CNN
	1    9650 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 1450 9650 1400
Wire Wire Line
	4450 1250 4550 1250
Connection ~ 3750 850 
Wire Wire Line
	3750 850  4100 850 
Wire Wire Line
	3100 950  3100 850 
Connection ~ 3100 850 
Wire Wire Line
	3100 850  3750 850 
$Comp
L Device:R R5
U 1 1 5E8997FE
P 4500 2400
F 0 "R5" H 4300 2450 50  0000 L CNN
F 1 "10k" H 4300 2350 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4430 2400 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 4500 2400 50  0001 C CNN
F 4 "LIZ Electronics" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0103G" H 0   0   50  0001 C CNN "Short Description"
	1    4500 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0126
U 1 1 5E8A894A
P 4500 2600
F 0 "#PWR0126" H 4500 2350 50  0001 C CNN
F 1 "GND" H 4500 2450 50  0000 C CNN
F 2 "" H 4500 2600 50  0001 C CNN
F 3 "" H 4500 2600 50  0001 C CNN
	1    4500 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 2600 4500 2550
Text GLabel 9800 1050 2    50   Input ~ 0
3.3V
Text GLabel 8450 4000 0    50   Input ~ 0
3.3V
Text GLabel 6500 5150 0    50   Input ~ 0
3.3V
Text GLabel 2850 4100 0    50   Input ~ 0
3.3V
Text GLabel 1350 5400 0    50   Input ~ 0
3.3V
Text GLabel 3350 2000 2    50   Input ~ 0
stat
Wire Wire Line
	3300 2000 3300 1700
Wire Wire Line
	2900 1700 2900 1800
Text GLabel 10000 5000 3    50   Input ~ 0
led
Text GLabel 7500 2000 0    50   Input ~ 0
5V
$Comp
L Device:Q_NMOS_GSD Q1
U 1 1 5E973F1F
P 3000 2000
F 0 "Q1" H 3206 2046 50  0000 L CNN
F 1 "SI2302" H 3206 1955 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3200 2100 50  0001 C CNN
F 3 "http://doc.hotking.com/wr/uploadfiles/2020/0102/SI2302.pdf" H 3000 2000 50  0001 C CNN
F 4 "Guangdong Hottech" H 3000 2000 50  0001 C CNN "Manufacturer"
F 5 "N-channel signal mosfet" H 0   0   50  0001 C CNN "Short Description"
	1    3000 2000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7700 2000 7700 2050
$Comp
L power:GND #PWR0122
U 1 1 5E9A5E5D
P 7700 2400
F 0 "#PWR0122" H 7700 2150 50  0001 C CNN
F 1 "GND" H 7700 2250 50  0000 C CNN
F 2 "" H 7700 2400 50  0001 C CNN
F 3 "" H 7700 2400 50  0001 C CNN
	1    7700 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 2400 7700 2350
$Comp
L Device:R R14
U 1 1 5E974052
P 7700 2200
F 0 "R14" H 7631 2246 50  0000 R CNN
F 1 "10k" H 7631 2155 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7630 2200 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 7700 2200 50  0001 C CNN
F 4 "LIZ Electronics" H -950 700 50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0103G" H -950 700 50  0001 C CNN "Short Description"
	1    7700 2200
	1    0    0    -1  
$EndComp
Text GLabel 5250 6200 2    50   Input ~ 0
chg1
Text GLabel 5500 6300 2    50   Input ~ 0
chg2
Text GLabel 1350 2700 0    50   Input ~ 0
3.3V
Wire Wire Line
	4500 2200 4450 2200
Wire Wire Line
	4550 2200 4500 2200
Connection ~ 4500 2200
Wire Wire Line
	4500 2250 4500 2200
Text GLabel 6100 2200 2    50   Input ~ 0
stat
Wire Wire Line
	6100 2200 6050 2200
Wire Wire Line
	3350 2000 3300 2000
Connection ~ 3300 2000
Wire Wire Line
	3300 2000 3200 2000
$Comp
L power:GND #PWR0128
U 1 1 5EACA908
P 2900 2250
F 0 "#PWR0128" H 2900 2000 50  0001 C CNN
F 1 "GND" H 2900 2100 50  0000 C CNN
F 2 "" H 2900 2250 50  0001 C CNN
F 3 "" H 2900 2250 50  0001 C CNN
	1    2900 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 2250 2900 2200
Text GLabel 5200 5400 2    50   Input ~ 0
led
$Comp
L Device:Q_NMOS_GSD Q2
U 1 1 5EAE3FA3
P 9050 2000
F 0 "Q2" H 9255 2046 50  0000 L CNN
F 1 "SI2302" H 9255 1955 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 9250 2100 50  0001 C CNN
F 3 "http://doc.hotking.com/wr/uploadfiles/2020/0102/SI2302.pdf" H 9050 2000 50  0001 C CNN
F 4 "Guangdong Hottech" H 100 700 50  0001 C CNN "Manufacturer"
F 5 "N-channel signal mosfet" H 100 700 50  0001 C CNN "Short Description"
	1    9050 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NMOS_GSD Q3
U 1 1 5EB71CEC
P 9900 4800
F 0 "Q3" H 10105 4846 50  0000 L CNN
F 1 "SI2302" H 10105 4755 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 10100 4900 50  0001 C CNN
F 3 "http://doc.hotking.com/wr/uploadfiles/2020/0102/SI2302.pdf" H 9900 4800 50  0001 C CNN
F 4 "Guangdong Hottech" H 0   0   50  0001 C CNN "Manufacturer"
F 5 "N-channel signal mosfet" H 0   0   50  0001 C CNN "Short Description"
	1    9900 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 4800 9450 4800
Wire Wire Line
	9650 4950 9650 4800
Wire Wire Line
	9700 4800 9650 4800
Connection ~ 9650 4800
NoConn ~ 8050 4300
NoConn ~ 9450 4600
NoConn ~ 3900 5800
Wire Wire Line
	3250 4300 3300 4300
$Comp
L power:GND #PWR0129
U 1 1 5E9C8978
P 3700 5000
F 0 "#PWR0129" H 3700 4750 50  0001 C CNN
F 1 "GND" V 3700 4800 50  0000 C CNN
F 2 "" H 3700 5000 50  0001 C CNN
F 3 "" H 3700 5000 50  0001 C CNN
	1    3700 5000
	0    1    1    0   
$EndComp
Wire Wire Line
	3650 5200 3900 5200
Wire Wire Line
	3900 5000 3700 5000
Text GLabel 5200 6000 2    50   Input ~ 0
miso
Text GLabel 5200 5800 2    50   Input ~ 0
miso
Wire Wire Line
	5450 5700 5200 5700
Wire Wire Line
	3900 6200 2750 6200
Wire Wire Line
	3900 6300 3050 6300
Wire Wire Line
	5450 5500 5200 5500
NoConn ~ 3900 6500
$Comp
L power:GND #PWR0131
U 1 1 5EA3B559
P 3850 6400
F 0 "#PWR0131" H 3850 6150 50  0001 C CNN
F 1 "GND" V 3850 6200 50  0000 C CNN
F 2 "" H 3850 6400 50  0001 C CNN
F 3 "" H 3850 6400 50  0001 C CNN
	1    3850 6400
	0    1    1    0   
$EndComp
Wire Wire Line
	3850 6400 3900 6400
NoConn ~ 3900 5600
$Comp
L power:GND #PWR0132
U 1 1 5E98D4C8
P 2350 6100
F 0 "#PWR0132" H 2350 5850 50  0001 C CNN
F 1 "GND" V 2355 5972 50  0000 R CNN
F 2 "" H 2350 6100 50  0001 C CNN
F 3 "" H 2350 6100 50  0001 C CNN
	1    2350 6100
	0    -1   -1   0   
$EndComp
Connection ~ 2750 5400
Connection ~ 2750 6200
Wire Wire Line
	2350 5400 2750 5400
Wire Wire Line
	2350 6200 2750 6200
Connection ~ 3050 6300
Wire Wire Line
	2350 6300 3050 6300
Wire Wire Line
	9350 1200 9350 1050
Wire Wire Line
	8950 900  8950 1050
Wire Wire Line
	9800 1050 9650 1050
Connection ~ 9350 1050
Wire Wire Line
	9350 1050 9350 900 
Connection ~ 8950 1050
Wire Wire Line
	8950 1050 8950 1200
$Comp
L Device:Q_PMOS_GSD Q4
U 1 1 5F38D022
P 9150 1300
F 0 "Q4" V 9150 1500 50  0000 C CNN
F 1 "SI2301" V 9050 1550 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 9350 1400 50  0001 C CNN
F 3 "http://doc.hotking.com/wr/uploadfiles/2020/0102/SI2301.pdf" H 9150 1300 50  0001 C CNN
F 4 "Guangdong Hottech" V 9150 1300 50  0001 C CNN "Manufacturer"
F 5 "P-channel signal mosfet" H 150 0   50  0001 C CNN "Short Description"
	1    9150 1300
	0    1    -1   0   
$EndComp
$Comp
L Device:R R16
U 1 1 5F3A5BA6
P 8800 1250
F 0 "R16" H 8850 1300 50  0000 L CNN
F 1 "100k" H 8850 1200 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8730 1250 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 8800 1250 50  0001 C CNN
F 4 "LIZ Electronics" H 150 -250 50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0104G" H 150 -250 50  0001 C CNN "Short Description"
	1    8800 1250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8800 1100 8800 1050
$Comp
L power:GND #PWR0133
U 1 1 5F3D6062
P 9150 2400
F 0 "#PWR0133" H 9150 2150 50  0001 C CNN
F 1 "GND" H 9150 2250 50  0000 C CNN
F 2 "" H 9150 2400 50  0001 C CNN
F 3 "" H 9150 2400 50  0001 C CNN
	1    9150 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 2400 9150 2200
Text GLabel 8800 2400 3    50   Input ~ 0
pwr
Wire Wire Line
	8800 2000 8800 2400
Text GLabel 3900 6700 0    50   Input ~ 0
pwr
Connection ~ 9650 1050
Wire Wire Line
	9650 1050 9350 1050
Text GLabel 3900 5700 0    50   Input ~ 0
3.3V
Text GLabel 5200 5600 2    50   Input ~ 0
3.3V
Wire Wire Line
	9150 1500 9150 1800
Wire Wire Line
	8800 1400 8800 1500
NoConn ~ 1800 1400
NoConn ~ 1700 1400
Wire Wire Line
	1900 2050 1900 2000
Wire Wire Line
	1150 2800 1100 2800
Wire Wire Line
	2550 2600 2550 2650
Wire Wire Line
	7700 2000 7500 2000
Wire Wire Line
	7250 1050 7600 1050
Connection ~ 7250 1050
$Comp
L power:GND #PWR0135
U 1 1 5F93A478
P 6500 2800
F 0 "#PWR0135" H 6500 2550 50  0001 C CNN
F 1 "GND" H 6500 2650 50  0000 C CNN
F 2 "" H 6500 2800 50  0001 C CNN
F 3 "" H 6500 2800 50  0001 C CNN
	1    6500 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 2800 6500 2750
Wire Wire Line
	6500 2450 6500 2400
Text GLabel 7150 2400 2    50   Input ~ 0
meas
Wire Wire Line
	6650 2400 6500 2400
Connection ~ 6500 2400
$Comp
L Device:R R15
U 1 1 5F962A8A
P 8400 2000
F 0 "R15" V 8193 2000 50  0000 C CNN
F 1 "100k" V 8284 2000 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8330 2000 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 8400 2000 50  0001 C CNN
F 4 "LIZ Electronics" H -250 500 50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0104G" H -250 500 50  0001 C CNN "Short Description"
	1    8400 2000
	0    1    1    0   
$EndComp
$Comp
L Device:R R17
U 1 1 5F9638E0
P 6400 2200
F 0 "R17" H 6470 2246 50  0000 L CNN
F 1 "100k" H 6470 2155 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6330 2200 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 6400 2200 50  0001 C CNN
F 4 "LIZ Electronics" H -2250 700 50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0104G" H -2250 700 50  0001 C CNN "Short Description"
	1    6400 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R18
U 1 1 5F963C34
P 6300 2600
F 0 "R18" H 6231 2646 50  0000 R CNN
F 1 "100k" H 6231 2555 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6230 2600 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 6300 2600 50  0001 C CNN
F 4 "LIZ Electronics" H -2350 1100 50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0104G" H -2350 1100 50  0001 C CNN "Short Description"
	1    6300 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 850  3100 850 
Text GLabel 1900 1350 1    50   Input ~ 0
5V
Wire Wire Line
	1900 1350 1900 1400
Wire Wire Line
	2000 1150 2000 1400
Wire Wire Line
	1250 1150 2000 1150
Wire Wire Line
	2100 1050 2100 1400
Wire Wire Line
	1250 1050 2100 1050
Wire Wire Line
	1100 3000 1700 3000
Wire Wire Line
	1100 2900 1400 2900
$Comp
L power:GND #PWR0116
U 1 1 5FA41060
P 1750 2700
F 0 "#PWR0116" H 1750 2450 50  0001 C CNN
F 1 "GND" V 1650 2650 50  0000 C CNN
F 2 "" H 1750 2700 50  0001 C CNN
F 3 "" H 1750 2700 50  0001 C CNN
	1    1750 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1750 2700 1700 2700
Wire Wire Line
	1100 2700 1400 2700
Wire Wire Line
	2300 2650 2300 2600
Connection ~ 2300 2600
Wire Wire Line
	2300 2600 2550 2600
$Comp
L power:GND #PWR0136
U 1 1 5FAE3521
P 5250 6400
F 0 "#PWR0136" H 5250 6150 50  0001 C CNN
F 1 "GND" V 5250 6200 50  0000 C CNN
F 2 "" H 5250 6400 50  0001 C CNN
F 3 "" H 5250 6400 50  0001 C CNN
	1    5250 6400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5250 6200 5200 6200
Wire Wire Line
	5200 6300 5500 6300
Text GLabel 3900 5500 0    50   Input ~ 0
meas
NoConn ~ 3900 6600
NoConn ~ 3900 6900
$Comp
L Device:C C12
U 1 1 5F7B834D
P 4300 3950
F 0 "C12" H 4415 3996 50  0000 L CNN
F 1 "100nF" H 4415 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4338 3800 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 4300 3950 50  0001 C CNN
F 4 "TDK" H 1050 0   50  0001 C CNN "Manufacturer"
F 5 "X7R 10% 50V   eg. C1608X7R1H104K" H 1050 0   50  0001 C CNN "Short Description"
	1    4300 3950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0130
U 1 1 5F7CCCDE
P 4300 3800
F 0 "#PWR0130" H 4300 3550 50  0001 C CNN
F 1 "GND" H 4305 3627 50  0000 C CNN
F 2 "" H 4300 3800 50  0001 C CNN
F 3 "" H 4300 3800 50  0001 C CNN
	1    4300 3800
	-1   0    0    1   
$EndComp
Connection ~ 4300 4100
Wire Wire Line
	4300 4100 4400 4100
Connection ~ 2950 4100
Connection ~ 3400 4100
Connection ~ 3850 4100
Wire Wire Line
	3850 4100 4300 4100
Wire Wire Line
	2850 4100 2950 4100
Wire Wire Line
	2950 4100 3400 4100
Wire Wire Line
	3400 4100 3850 4100
Connection ~ 6400 2000
Wire Wire Line
	6400 2050 6400 2000
Text GLabel 6850 2700 3    50   Input ~ 0
3.3V
Wire Wire Line
	7050 2400 7150 2400
$Comp
L Device:Q_NMOS_GSD Q5
U 1 1 5F820773
P 6850 2500
F 0 "Q5" V 7193 2500 50  0000 C CNN
F 1 "SI2302" V 7102 2500 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7050 2600 50  0001 C CNN
F 3 "http://doc.hotking.com/wr/uploadfiles/2020/0102/SI2302.pdf" H 6850 2500 50  0001 C CNN
F 4 "Guangdong Hottech" H -2100 1200 50  0001 C CNN "Manufacturer"
F 5 "N-channel signal mosfet" H -2100 1200 50  0001 C CNN "Short Description"
	1    6850 2500
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R19
U 1 1 5F84995D
P 6500 2600
F 0 "R19" H 6570 2646 50  0000 L CNN
F 1 "100k" H 6570 2555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6430 2600 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 6500 2600 50  0001 C CNN
F 4 "LIZ Electronics" H -2150 1100 50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0104G" H -2150 1100 50  0001 C CNN "Short Description"
	1    6500 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0137
U 1 1 5F849C60
P 6300 2800
F 0 "#PWR0137" H 6300 2550 50  0001 C CNN
F 1 "GND" H 6300 2650 50  0000 C CNN
F 2 "" H 6300 2800 50  0001 C CNN
F 3 "" H 6300 2800 50  0001 C CNN
	1    6300 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 2800 6300 2750
Wire Wire Line
	6500 2400 6400 2400
Wire Wire Line
	6300 2400 6300 2450
Wire Wire Line
	6400 2400 6400 2350
Connection ~ 6400 2400
Wire Wire Line
	6400 2400 6300 2400
Connection ~ 6400 1050
$Comp
L Switch:SW_SPST TP1
U 1 1 5F83D98D
P 9150 900
F 0 "TP1" V 9104 998 50  0001 L CNN
F 1 "test pins" V 9195 998 50  0001 L CNN
F 2 "LocalTrack:TestPins" H 9150 900 50  0001 C CNN
F 3 "2x1 straight pin header with 2.54mm pitch, mounted sideways" H 9150 900 50  0001 C CNN
F 4 "~" H -200 -400 50  0001 C CNN "Manufacturer"
F 5 "alternative power switch used before full assembly" H -200 -400 50  0001 C CNN "Short Description"
	1    9150 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C16
U 1 1 5F8872BD
P 3450 4300
F 0 "C16" V 3610 4300 50  0000 C CNN
F 1 "100nF" V 3701 4300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3488 4150 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/mlcc_commercial_general_en.pdf" H 3450 4300 50  0001 C CNN
F 4 "TDK" H -300 350 50  0001 C CNN "Manufacturer"
F 5 "X7R 10% 50V   eg. C1608X7R1H104K" H -300 350 50  0001 C CNN "Short Description"
	1    3450 4300
	0    1    1    0   
$EndComp
Connection ~ 8800 2000
Wire Wire Line
	8800 2000 8850 2000
Wire Wire Line
	8550 2000 8600 2000
$Comp
L Device:R R20
U 1 1 5FA18189
P 8600 2200
F 0 "R20" H 8531 2154 50  0000 R CNN
F 1 "100k" H 8531 2245 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8530 2200 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811151223_LIZ-Elec-CR0603JA0102G_C101253.pdf" H 8600 2200 50  0001 C CNN
F 4 "LIZ Electronics" H -50 700 50  0001 C CNN "Manufacturer"
F 5 "5% 50V  eg. CR0603JA0104G" H -50 700 50  0001 C CNN "Short Description"
	1    8600 2200
	1    0    0    1   
$EndComp
Wire Wire Line
	8600 2050 8600 2000
Connection ~ 8600 2000
Wire Wire Line
	8600 2000 8800 2000
$Comp
L power:GND #PWR0138
U 1 1 5FA22F26
P 8600 2400
F 0 "#PWR0138" H 8600 2150 50  0001 C CNN
F 1 "GND" H 8600 2250 50  0000 C CNN
F 2 "" H 8600 2400 50  0001 C CNN
F 3 "" H 8600 2400 50  0001 C CNN
	1    8600 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 2400 8600 2350
Connection ~ 8950 900 
Connection ~ 9350 900 
Connection ~ 9150 1500
Wire Wire Line
	8800 1500 9150 1500
Text GLabel 8700 1050 0    50   Input ~ 0
3.3V_BKUP
Connection ~ 8800 1050
Wire Wire Line
	8800 1050 8950 1050
Wire Wire Line
	8200 1050 8800 1050
$Comp
L Device:Q_PMOS_GSD Q6
U 1 1 6012C1ED
P 8000 2100
F 0 "Q6" V 8343 2100 50  0000 C CNN
F 1 "SI2301" V 8252 2100 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8200 2200 50  0001 C CNN
F 3 "http://doc.hotking.com/wr/uploadfiles/2020/0102/SI2301.pdf" H 8000 2100 50  0001 C CNN
F 4 "Guangdong Hottech" V 8000 2100 50  0001 C CNN "Manufacturer"
F 5 "P-channel signal mosfet" H -1000 800 50  0001 C CNN "Short Description"
	1    8000 2100
	0    1    -1   0   
$EndComp
Text GLabel 8000 2300 3    50   Input ~ 0
3.3V_BKUP
Wire Wire Line
	7800 2000 7700 2000
Connection ~ 7700 2000
Wire Wire Line
	8250 2000 8200 2000
Connection ~ 6900 1050
Wire Wire Line
	6900 1050 7250 1050
Wire Wire Line
	6400 1050 6900 1050
Text GLabel 5650 6100 2    50   Input ~ 0
mosi
Wire Wire Line
	1100 2600 2300 2600
$EndSCHEMATC
