EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title "Sensors"
Date "2021-01-15"
Rev "2.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR0412
U 1 1 60AC640A
P 2700 1750
F 0 "#PWR0412" H 2700 1500 50  0001 C CNN
F 1 "GND" H 2705 1577 50  0000 C CNN
F 2 "" H 2700 1750 50  0001 C CNN
F 3 "" H 2700 1750 50  0001 C CNN
	1    2700 1750
	1    0    0    -1  
$EndComp
$Comp
L power:+1V8 #PWR0410
U 1 1 60AC6A91
P 2350 850
F 0 "#PWR0410" H 2350 700 50  0001 C CNN
F 1 "+1V8" H 2365 1023 50  0000 C CNN
F 2 "" H 2350 850 50  0001 C CNN
F 3 "" H 2350 850 50  0001 C CNN
	1    2350 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 1700 2700 1700
Wire Wire Line
	2700 1700 2700 1750
Wire Wire Line
	3050 2150 3000 2150
Wire Wire Line
	3000 2150 3000 1900
Wire Wire Line
	3000 1900 3150 1900
Wire Wire Line
	3150 1900 3150 1850
Wire Wire Line
	3250 1850 3250 1900
Wire Wire Line
	3250 1900 3400 1900
Wire Wire Line
	3400 1900 3400 2150
Wire Wire Line
	3400 2150 3350 2150
Wire Wire Line
	3650 1300 3700 1300
Wire Wire Line
	3700 1300 3700 1500
Wire Wire Line
	3700 1500 3750 1500
$Comp
L power:+1V8 #PWR0419
U 1 1 60ACA6DF
P 5300 1450
F 0 "#PWR0419" H 5300 1300 50  0001 C CNN
F 1 "+1V8" H 5315 1623 50  0000 C CNN
F 2 "" H 5300 1450 50  0001 C CNN
F 3 "" H 5300 1450 50  0001 C CNN
	1    5300 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 1500 5300 1500
Wire Wire Line
	5300 1500 5300 1450
$Comp
L power:GND #PWR0422
U 1 1 60ACC17F
P 5500 2150
F 0 "#PWR0422" H 5500 1900 50  0001 C CNN
F 1 "GND" H 5505 1977 50  0000 C CNN
F 2 "" H 5500 2150 50  0001 C CNN
F 3 "" H 5500 2150 50  0001 C CNN
	1    5500 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 2050 5300 2100
Wire Wire Line
	5300 2100 5500 2100
Wire Wire Line
	5700 2100 5700 2050
Wire Wire Line
	5500 2150 5500 2100
Connection ~ 5500 2100
Wire Wire Line
	5500 2100 5700 2100
Wire Wire Line
	5300 1700 5300 1750
Wire Wire Line
	5300 1700 5700 1700
Wire Wire Line
	5700 1700 5700 1750
Text GLabel 5750 1700 2    50   Output ~ 0
AMB_TEMP
Wire Wire Line
	5750 1700 5700 1700
Connection ~ 5700 1700
Wire Wire Line
	3650 1700 3700 1700
Wire Wire Line
	5250 1700 5300 1700
Connection ~ 5300 1700
Text GLabel 4250 900  2    50   Output ~ 0
IR_TEMP
$Comp
L power:GND #PWR0417
U 1 1 60AD1780
P 4100 1150
F 0 "#PWR0417" H 4100 900 50  0001 C CNN
F 1 "GND" H 4105 977 50  0000 C CNN
F 2 "" H 4100 1150 50  0001 C CNN
F 3 "" H 4100 1150 50  0001 C CNN
	1    4100 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 900  3700 900 
Wire Wire Line
	3750 1100 3700 1100
Wire Wire Line
	3700 1100 3700 900 
Connection ~ 3700 900 
Wire Wire Line
	3700 900  4250 900 
Wire Wire Line
	4050 1100 4100 1100
Wire Wire Line
	4100 1100 4100 1150
Wire Wire Line
	3700 1700 3700 2400
Connection ~ 3700 1700
Wire Wire Line
	3700 1700 3750 1700
$Comp
L power:GND #PWR0411
U 1 1 60ADA218
P 2350 1300
F 0 "#PWR0411" H 2350 1050 50  0001 C CNN
F 1 "GND" H 2355 1127 50  0000 C CNN
F 2 "" H 2350 1300 50  0001 C CNN
F 3 "" H 2350 1300 50  0001 C CNN
	1    2350 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 1900 1100 1900
Wire Wire Line
	1100 1900 1100 1700
Wire Wire Line
	1100 1700 1200 1700
Wire Wire Line
	1900 1800 1950 1800
Wire Wire Line
	1950 1800 1950 1900
Wire Wire Line
	1950 1900 1900 1900
Wire Wire Line
	1200 1800 1150 1800
Wire Wire Line
	1150 1800 1150 2100
Wire Wire Line
	1150 2100 1550 2100
Wire Wire Line
	1950 2100 1950 1900
Connection ~ 1950 1900
$Comp
L power:GND #PWR0406
U 1 1 60AE12EB
P 1550 2150
F 0 "#PWR0406" H 1550 1900 50  0001 C CNN
F 1 "GND" H 1555 1977 50  0000 C CNN
F 2 "" H 1550 2150 50  0001 C CNN
F 3 "" H 1550 2150 50  0001 C CNN
	1    1550 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 2150 1550 2100
Connection ~ 1550 2100
Wire Wire Line
	1550 2100 1950 2100
Wire Wire Line
	950  1750 950  1700
Wire Wire Line
	950  1700 1100 1700
Connection ~ 1100 1700
Wire Wire Line
	950  2050 950  2100
Wire Wire Line
	950  2100 1150 2100
Connection ~ 1150 2100
Wire Wire Line
	1950 2100 2100 2100
Wire Wire Line
	2100 2100 2100 2050
Connection ~ 1950 2100
Wire Wire Line
	1900 1700 2100 1700
Wire Wire Line
	2100 1700 2100 1750
$Comp
L power:+1V8 #PWR0401
U 1 1 60AEA499
P 950 1650
F 0 "#PWR0401" H 950 1500 50  0001 C CNN
F 1 "+1V8" H 965 1823 50  0000 C CNN
F 2 "" H 950 1650 50  0001 C CNN
F 3 "" H 950 1650 50  0001 C CNN
	1    950  1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  1650 950  1700
Connection ~ 950  1700
Wire Wire Line
	2750 1300 2550 1300
Wire Wire Line
	2550 1300 2550 1700
Wire Wire Line
	2550 2400 3700 2400
Wire Wire Line
	2100 1700 2550 1700
Connection ~ 2100 1700
Connection ~ 2550 1700
Wire Wire Line
	2550 1700 2550 2400
Wire Wire Line
	2350 950  2350 900 
Wire Wire Line
	2350 1300 2350 1250
Wire Wire Line
	2350 900  2750 900 
Wire Wire Line
	2350 850  2350 900 
Connection ~ 2350 900 
$Comp
L power:GND #PWR0431
U 1 1 5FE58AC3
P 10300 2900
F 0 "#PWR0431" H 10300 2650 50  0001 C CNN
F 1 "GND" H 10305 2727 50  0000 C CNN
F 2 "" H 10300 2900 50  0001 C CNN
F 3 "" H 10300 2900 50  0001 C CNN
	1    10300 2900
	1    0    0    -1  
$EndComp
Text GLabel 9750 1750 2    50   Input ~ 0
IMU_INT
Wire Wire Line
	9550 1750 9750 1750
Wire Wire Line
	9550 1550 9750 1550
Wire Wire Line
	9750 1550 9750 1450
Wire Wire Line
	9750 1150 9900 1150
Connection ~ 9750 1550
Wire Wire Line
	9750 1550 9900 1550
Wire Wire Line
	9550 1450 9750 1450
Connection ~ 9750 1450
Wire Wire Line
	9750 1450 9750 1150
$Comp
L power:+1V8 #PWR0428
U 1 1 5FE7D306
P 9750 1050
F 0 "#PWR0428" H 9750 900 50  0001 C CNN
F 1 "+1V8" H 9765 1223 50  0000 C CNN
F 2 "" H 9750 1050 50  0001 C CNN
F 3 "" H 9750 1050 50  0001 C CNN
	1    9750 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 1050 9750 1150
Connection ~ 9750 1150
Wire Wire Line
	9550 1950 9750 1950
Wire Wire Line
	9750 1950 9750 2100
Wire Wire Line
	9750 2100 9900 2100
Wire Wire Line
	10200 1150 10300 1150
Wire Wire Line
	10300 1150 10300 1550
Wire Wire Line
	10200 1550 10300 1550
Connection ~ 10300 1550
Wire Wire Line
	10300 1550 10300 2100
Wire Wire Line
	10200 2100 10300 2100
Connection ~ 10300 2100
Wire Wire Line
	10300 2100 10300 2300
Wire Wire Line
	9700 2300 10300 2300
Connection ~ 10300 2300
Wire Wire Line
	10300 2300 10300 2650
Wire Wire Line
	9550 2650 10300 2650
Connection ~ 10300 2650
Wire Wire Line
	10300 2650 10300 2900
Wire Wire Line
	9550 2250 9700 2250
Wire Wire Line
	9700 2250 9700 2300
Text GLabel 7750 1950 0    50   Output ~ 0
IMU_MISO
Text GLabel 7750 2050 0    50   Input ~ 0
IMU_SCK
Text GLabel 7750 2150 0    50   Input ~ 0
IMU_MOSI
Text GLabel 7750 2250 0    50   Input ~ 0
IMU_CS
Wire Wire Line
	7750 1950 7950 1950
Wire Wire Line
	7750 2050 7950 2050
Wire Wire Line
	7750 2150 7950 2150
Wire Wire Line
	7750 2250 7950 2250
$Comp
L power:GND #PWR0425
U 1 1 5FFEF9BA
P 7850 2900
F 0 "#PWR0425" H 7850 2650 50  0001 C CNN
F 1 "GND" H 7855 2727 50  0000 C CNN
F 2 "" H 7850 2900 50  0001 C CNN
F 3 "" H 7850 2900 50  0001 C CNN
	1    7850 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 2450 7850 2450
Wire Wire Line
	7850 2450 7850 2900
Wire Wire Line
	9100 4750 8950 4750
$Comp
L power:+1V8 #PWR0426
U 1 1 600CE4C3
P 8950 4050
F 0 "#PWR0426" H 8950 3900 50  0001 C CNN
F 1 "+1V8" H 8965 4223 50  0000 C CNN
F 2 "" H 8950 4050 50  0001 C CNN
F 3 "" H 8950 4050 50  0001 C CNN
	1    8950 4050
	1    0    0    -1  
$EndComp
$Comp
L power:+1V8 #PWR0430
U 1 1 600D19D1
P 9950 4050
F 0 "#PWR0430" H 9950 3900 50  0001 C CNN
F 1 "+1V8" H 9965 4223 50  0000 C CNN
F 2 "" H 9950 4050 50  0001 C CNN
F 3 "" H 9950 4050 50  0001 C CNN
	1    9950 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0427
U 1 1 600D69D3
P 8950 5400
F 0 "#PWR0427" H 8950 5150 50  0001 C CNN
F 1 "GND" H 8955 5227 50  0000 C CNN
F 2 "" H 8950 5400 50  0001 C CNN
F 3 "" H 8950 5400 50  0001 C CNN
	1    8950 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0429
U 1 1 600D7198
P 9900 5150
F 0 "#PWR0429" H 9900 4900 50  0001 C CNN
F 1 "GND" H 9905 4977 50  0000 C CNN
F 2 "" H 9900 5150 50  0001 C CNN
F 3 "" H 9900 5150 50  0001 C CNN
	1    9900 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 4950 8950 4750
Connection ~ 8950 4750
Wire Wire Line
	8950 5400 8950 5250
Wire Wire Line
	9900 4750 9900 5150
Wire Wire Line
	9950 4850 9950 4650
Wire Wire Line
	9950 4950 9950 4850
Connection ~ 9950 4850
Wire Wire Line
	9800 4950 9950 4950
Wire Wire Line
	9800 4850 9950 4850
Wire Wire Line
	9800 4750 9900 4750
Wire Wire Line
	9800 4650 9950 4650
Wire Wire Line
	9950 4450 9950 4650
Connection ~ 9950 4650
Wire Wire Line
	9950 4150 9950 4050
Wire Wire Line
	8950 4050 8950 4100
Wire Wire Line
	8800 4150 8800 4100
Wire Wire Line
	8800 4100 8950 4100
Connection ~ 8950 4100
Wire Wire Line
	8950 4100 8950 4750
Wire Wire Line
	8800 4450 8800 4850
Wire Wire Line
	8800 4850 9100 4850
Text GLabel 8650 4550 0    50   BiDi ~ 0
I2C_SDA
Wire Wire Line
	9100 4550 8650 4550
Text GLabel 8650 4650 0    50   Input ~ 0
I2C_SCL
Wire Wire Line
	9100 4650 8650 4650
Text GLabel 8650 4850 0    50   Output ~ 0
UV_INT
Wire Wire Line
	8650 4850 8800 4850
Connection ~ 8800 4850
$Comp
L power:GND #PWR0424
U 1 1 6015A6CD
P 6950 5300
F 0 "#PWR0424" H 6950 5050 50  0001 C CNN
F 1 "GND" H 6955 5127 50  0000 C CNN
F 2 "" H 6950 5300 50  0001 C CNN
F 3 "" H 6950 5300 50  0001 C CNN
	1    6950 5300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0421
U 1 1 6015AE61
P 5450 5300
F 0 "#PWR0421" H 5450 5050 50  0001 C CNN
F 1 "GND" H 5455 5127 50  0000 C CNN
F 2 "" H 5450 5300 50  0001 C CNN
F 3 "" H 5450 5300 50  0001 C CNN
	1    5450 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 5100 6950 5100
Wire Wire Line
	6950 5100 6950 5300
Wire Wire Line
	5450 5300 5450 4900
Wire Wire Line
	5450 4900 5600 4900
Text GLabel 5350 4700 0    50   Input ~ 0
I2C_SCL
Text GLabel 5350 4800 0    50   BiDi ~ 0
I2C_SDA
Wire Wire Line
	5350 4800 5600 4800
Wire Wire Line
	5600 4700 5350 4700
$Comp
L power:+1V8 #PWR0420
U 1 1 6016DC42
P 5450 4050
F 0 "#PWR0420" H 5450 3900 50  0001 C CNN
F 1 "+1V8" H 5465 4223 50  0000 C CNN
F 2 "" H 5450 4050 50  0001 C CNN
F 3 "" H 5450 4050 50  0001 C CNN
	1    5450 4050
	1    0    0    -1  
$EndComp
$Comp
L power:+1V8 #PWR0423
U 1 1 6016E7F3
P 6950 4000
F 0 "#PWR0423" H 6950 3850 50  0001 C CNN
F 1 "+1V8" H 6965 4173 50  0000 C CNN
F 2 "" H 6950 4000 50  0001 C CNN
F 3 "" H 6950 4000 50  0001 C CNN
	1    6950 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 4300 6950 4300
Wire Wire Line
	6950 4300 6950 4000
Wire Wire Line
	6800 4400 6950 4400
Wire Wire Line
	6950 4400 6950 4300
Connection ~ 6950 4300
Wire Wire Line
	5600 4600 5450 4600
Wire Wire Line
	5450 4600 5450 4050
Wire Wire Line
	7450 4600 7450 4400
Wire Wire Line
	7450 4400 6950 4400
Connection ~ 6950 4400
Wire Wire Line
	6950 4600 6950 4400
Wire Wire Line
	7450 4900 7450 5100
Wire Wire Line
	7450 5100 6950 5100
Connection ~ 6950 5100
Wire Wire Line
	6950 4900 6950 5100
$Comp
L power:+1V8 #PWR0418
U 1 1 601BCAA5
P 4300 2900
F 0 "#PWR0418" H 4300 2750 50  0001 C CNN
F 1 "+1V8" H 4315 3073 50  0000 C CNN
F 2 "" H 4300 2900 50  0001 C CNN
F 3 "" H 4300 2900 50  0001 C CNN
	1    4300 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0416
U 1 1 601BDA1B
P 4050 6200
F 0 "#PWR0416" H 4050 5950 50  0001 C CNN
F 1 "GND" H 4055 6027 50  0000 C CNN
F 2 "" H 4050 6200 50  0001 C CNN
F 3 "" H 4050 6200 50  0001 C CNN
	1    4050 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 5600 4050 5600
Wire Wire Line
	4050 5600 4050 5700
Wire Wire Line
	3700 5700 4050 5700
Connection ~ 4050 5700
Wire Wire Line
	4050 5700 4050 5800
Wire Wire Line
	3700 5800 4050 5800
Connection ~ 4050 5800
Wire Wire Line
	4050 5800 4050 6200
$Comp
L power:+1V8 #PWR0402
U 1 1 601EA9B4
P 1250 6500
F 0 "#PWR0402" H 1250 6350 50  0001 C CNN
F 1 "+1V8" H 1265 6673 50  0000 C CNN
F 2 "" H 1250 6500 50  0001 C CNN
F 3 "" H 1250 6500 50  0001 C CNN
	1    1250 6500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0403
U 1 1 601EB805
P 1250 7100
F 0 "#PWR0403" H 1250 6850 50  0001 C CNN
F 1 "GND" H 1255 6927 50  0000 C CNN
F 2 "" H 1250 7100 50  0001 C CNN
F 3 "" H 1250 7100 50  0001 C CNN
	1    1250 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 6950 1000 7050
Wire Wire Line
	1000 7050 1250 7050
Wire Wire Line
	1500 7050 1500 6950
Wire Wire Line
	1250 7100 1250 7050
Connection ~ 1250 7050
Wire Wire Line
	1250 7050 1500 7050
Wire Wire Line
	1500 6650 1500 6550
Wire Wire Line
	1000 6550 1000 6650
Wire Wire Line
	1500 6550 1250 6550
Wire Wire Line
	1250 6500 1250 6550
Connection ~ 1250 6550
Wire Wire Line
	1250 6550 1000 6550
$Comp
L power:GND #PWR0414
U 1 1 6022A340
P 3350 7100
F 0 "#PWR0414" H 3350 6850 50  0001 C CNN
F 1 "GND" H 3355 6927 50  0000 C CNN
F 2 "" H 3350 7100 50  0001 C CNN
F 3 "" H 3350 7100 50  0001 C CNN
	1    3350 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 6950 3100 7050
Wire Wire Line
	3100 7050 3350 7050
Wire Wire Line
	3600 7050 3600 6950
Wire Wire Line
	3350 7100 3350 7050
Connection ~ 3350 7050
Wire Wire Line
	3350 7050 3600 7050
Wire Wire Line
	3600 6650 3600 6550
Wire Wire Line
	3100 6550 3100 6650
Wire Wire Line
	3600 6550 3350 6550
Wire Wire Line
	3350 6500 3350 6550
Connection ~ 3350 6550
Wire Wire Line
	3350 6550 3100 6550
Text Notes 700  7400 0    50   ~ 0
Close to VDD_DIG and VDD_ANA
$Comp
L power:+3.3V #PWR0413
U 1 1 6023B2DF
P 3350 6500
F 0 "#PWR0413" H 3350 6350 50  0001 C CNN
F 1 "+3.3V" H 3365 6673 50  0000 C CNN
F 2 "" H 3350 6500 50  0001 C CNN
F 3 "" H 3350 6500 50  0001 C CNN
	1    3350 6500
	1    0    0    -1  
$EndComp
Text Notes 3100 7400 0    50   ~ 0
Close to VLED
Wire Wire Line
	2300 3800 2000 3800
Wire Wire Line
	2000 4100 2000 4150
Wire Wire Line
	2000 4150 2150 4150
Wire Wire Line
	2150 4150 2150 3900
Wire Wire Line
	2150 3900 2300 3900
Wire Wire Line
	2300 3600 1450 3600
$Comp
L power:GND #PWR0405
U 1 1 60293460
P 1450 4200
F 0 "#PWR0405" H 1450 3950 50  0001 C CNN
F 1 "GND" H 1455 4027 50  0000 C CNN
F 2 "" H 1450 4200 50  0001 C CNN
F 3 "" H 1450 4200 50  0001 C CNN
	1    1450 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 4100 1450 4200
Text GLabel 2100 3400 0    50   Output ~ 0
HEART_INT
Wire Wire Line
	2300 3400 2150 3400
Text GLabel 2100 3500 0    50   Input ~ 0
I2C_SCL
Wire Wire Line
	2100 3500 2300 3500
Text GLabel 2050 5200 0    50   BiDi ~ 0
I2C_SDA
Wire Wire Line
	2300 5200 2050 5200
$Comp
L power:+3.3V #PWR0404
U 1 1 602BB40F
P 1300 5250
F 0 "#PWR0404" H 1300 5100 50  0001 C CNN
F 1 "+3.3V" H 1315 5423 50  0000 C CNN
F 2 "" H 1300 5250 50  0001 C CNN
F 3 "" H 1300 5250 50  0001 C CNN
	1    1300 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 5400 1300 5400
Wire Wire Line
	1300 5400 1300 5250
Wire Wire Line
	2150 3300 2150 3400
Connection ~ 2150 3400
Wire Wire Line
	2150 3400 2100 3400
$Comp
L power:+1V8 #PWR0407
U 1 1 5FE5FB23
P 2150 2900
F 0 "#PWR0407" H 2150 2750 50  0001 C CNN
F 1 "+1V8" H 2165 3073 50  0000 C CNN
F 2 "" H 2150 2900 50  0001 C CNN
F 3 "" H 2150 2900 50  0001 C CNN
	1    2150 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2900 2150 3000
$Comp
L power:GND #PWR0415
U 1 1 5FEE4CC1
P 3800 3650
F 0 "#PWR0415" H 3800 3400 50  0001 C CNN
F 1 "GND" H 3805 3477 50  0000 C CNN
F 2 "" H 3800 3650 50  0001 C CNN
F 3 "" H 3800 3650 50  0001 C CNN
	1    3800 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 3600 3800 3650
Wire Wire Line
	3700 3200 3800 3200
Wire Wire Line
	3800 3300 3800 3200
Connection ~ 3800 3200
Wire Wire Line
	3800 3200 3900 3200
Wire Wire Line
	4300 2900 4300 3000
Wire Wire Line
	4300 3200 4200 3200
Wire Wire Line
	3700 3100 3800 3100
Wire Wire Line
	3800 3100 3800 3000
Wire Wire Line
	3800 3000 4300 3000
Connection ~ 4300 3000
Wire Wire Line
	4300 3000 4300 3200
Wire Wire Line
	1450 3600 1450 3800
$Comp
L Device:C C413
U 1 1 5FEB3B46
P 3800 3450
F 0 "C413" H 3915 3496 50  0000 L CNN
F 1 "1uF" H 3915 3405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 3838 3300 50  0001 C CNN
F 3 "~" H 3800 3450 50  0001 C CNN
	1    3800 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R401
U 1 1 5FE54B44
P 2150 3150
F 0 "R401" H 2220 3196 50  0000 L CNN
F 1 "1K" H 2220 3105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 2080 3150 50  0001 C CNN
F 3 "~" H 2150 3150 50  0001 C CNN
	1    2150 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:C C405
U 1 1 60267F53
P 2000 3950
F 0 "C405" H 2115 3996 50  0000 L CNN
F 1 "0.1uF" H 2115 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 2038 3800 50  0001 C CNN
F 3 "~" H 2000 3950 50  0001 C CNN
	1    2000 3950
	-1   0    0    1   
$EndComp
$Comp
L Device:C C403
U 1 1 6025D17B
P 1450 3950
F 0 "C403" H 1565 3996 50  0000 L CNN
F 1 "0.1uF" H 1565 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 1488 3800 50  0001 C CNN
F 3 "~" H 1450 3950 50  0001 C CNN
	1    1450 3950
	-1   0    0    1   
$EndComp
$Comp
L Device:C C412
U 1 1 6022A334
P 3600 6800
F 0 "C412" H 3715 6846 50  0000 L CNN
F 1 "10uF" H 3715 6755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 3638 6650 50  0001 C CNN
F 3 "~" H 3600 6800 50  0001 C CNN
	1    3600 6800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C410
U 1 1 6022A32E
P 3100 6800
F 0 "C410" H 3215 6846 50  0000 L CNN
F 1 "0.1uF" H 3215 6755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 3138 6650 50  0001 C CNN
F 3 "~" H 3100 6800 50  0001 C CNN
	1    3100 6800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C404
U 1 1 601E54FE
P 1500 6800
F 0 "C404" H 1615 6846 50  0000 L CNN
F 1 "4.7µF" H 1615 6755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 1538 6650 50  0001 C CNN
F 3 "~" H 1500 6800 50  0001 C CNN
	1    1500 6800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C402
U 1 1 601E00C7
P 1000 6800
F 0 "C402" H 1115 6846 50  0000 L CNN
F 1 "0.1uF" H 1115 6755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 1038 6650 50  0001 C CNN
F 3 "~" H 1000 6800 50  0001 C CNN
	1    1000 6800
	1    0    0    -1  
$EndComp
$Comp
L SYSC4907_Parts:MAX86150EFF+T U402
U 1 1 601A24E9
P 3000 4500
F 0 "U402" H 3000 6170 50  0000 C CNN
F 1 "MAX86150EFF+T" H 3000 6079 50  0000 C CNN
F 2 "SYSC4907:MAX86150EFF+T" H 3000 4500 50  0001 L BNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX86150.pdf" H 3000 4500 50  0001 L BNN
F 4 "MAX86150EFF+T" H 3000 4500 50  0001 C CNN "MPN"
	1    3000 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C417
U 1 1 6017BDBA
P 7450 4750
F 0 "C417" H 7565 4796 50  0000 L CNN
F 1 "0.1uF" H 7565 4705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 7488 4600 50  0001 C CNN
F 3 "~" H 7450 4750 50  0001 C CNN
	1    7450 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C416
U 1 1 6017B3E5
P 6950 4750
F 0 "C416" H 7065 4796 50  0000 L CNN
F 1 "0.1uF" H 7065 4705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 6988 4600 50  0001 C CNN
F 3 "~" H 6950 4750 50  0001 C CNN
	1    6950 4750
	1    0    0    -1  
$EndComp
$Comp
L SYSC4907_Parts:BME680 U405
U 1 1 6015851F
P 6200 4700
F 0 "U405" H 6200 5367 50  0000 C CNN
F 1 "BME680" H 6200 5276 50  0000 C CNN
F 2 "Package_LGA:Bosch_LGA-8_3x3mm_P0.8mm_ClockwisePinNumbering" H 6200 4700 50  0001 L BNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680-DS001.pdf" H 6200 4700 50  0001 L BNN
F 4 "BME680" H 6200 4700 50  0001 C CNN "MPN"
	1    6200 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R404
U 1 1 60130737
P 8800 4300
F 0 "R404" H 9050 4250 50  0000 R CNN
F 1 "10K" H 9000 4350 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 8730 4300 50  0001 C CNN
F 3 "~" H 8800 4300 50  0001 C CNN
	1    8800 4300
	-1   0    0    1   
$EndComp
$Comp
L SYSC4907_Parts:SI1133 U407
U 1 1 6001ABDC
P 9450 4450
F 0 "U407" H 9450 4575 50  0000 C CNN
F 1 "SI1133-AA00-GM" H 9450 4484 50  0000 C CNN
F 2 "SYSC4907:SI1133-AA00-GM" H 9450 4450 50  0001 C CNN
F 3 "https://www.silabs.com/documents/public/data-sheets/Si1133.pdf" H 9450 4450 50  0001 C CNN
F 4 "SI1133-AA00-GM" H 9450 4450 50  0001 C CNN "MPN"
	1    9450 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R405
U 1 1 600D27D0
P 9950 4300
F 0 "R405" H 9880 4254 50  0000 R CNN
F 1 "10K" H 9880 4345 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 9880 4300 50  0001 C CNN
F 3 "~" H 9950 4300 50  0001 C CNN
	1    9950 4300
	-1   0    0    1   
$EndComp
$Comp
L Device:C C418
U 1 1 600C561A
P 8950 5100
F 0 "C418" H 8835 5054 50  0000 R CNN
F 1 "1uF" H 8835 5145 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 8988 4950 50  0001 C CNN
F 3 "~" H 8950 5100 50  0001 C CNN
	1    8950 5100
	-1   0    0    1   
$EndComp
$Comp
L Device:C C421
U 1 1 5FE7FFBA
P 10050 2100
F 0 "C421" V 9798 2100 50  0000 C CNN
F 1 "0.1uF" V 9889 2100 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 10088 1950 50  0001 C CNN
F 3 "~" H 10050 2100 50  0001 C CNN
	1    10050 2100
	0    1    1    0   
$EndComp
$Comp
L Device:C C420
U 1 1 5FE7345D
P 10050 1550
F 0 "C420" V 9798 1550 50  0000 C CNN
F 1 "0.1uF" V 9889 1550 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 10088 1400 50  0001 C CNN
F 3 "~" H 10050 1550 50  0001 C CNN
	1    10050 1550
	0    1    1    0   
$EndComp
$Comp
L Device:C C419
U 1 1 5FE6633C
P 10050 1150
F 0 "C419" V 9798 1150 50  0000 C CNN
F 1 "0.1uF" V 9889 1150 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 10088 1000 50  0001 C CNN
F 3 "~" H 10050 1150 50  0001 C CNN
	1    10050 1150
	0    1    1    0   
$EndComp
$Comp
L SYSC4907_Parts:ICM-20948 U406
U 1 1 5FE557C8
P 8750 2050
F 0 "U406" H 8750 2917 50  0000 C CNN
F 1 "ICM-20948" H 8750 2826 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-24-1EP_3x3mm_P0.4mm_EP1.75x1.6mm" H 8750 2050 50  0001 L BNN
F 3 "https://www.invensense.com/wp-content/uploads/2016/06/DS-000189-ICM-20948-v1.3.pdf?ref_disty=digikey" H 8750 2050 50  0001 L BNN
F 4 "ICM-20948" H 8750 2050 50  0001 C CNN "MPN"
	1    8750 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C408
U 1 1 60AF9877
P 2350 1100
F 0 "C408" H 2465 1146 50  0000 L CNN
F 1 "0.1uF" H 2465 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 2388 950 50  0001 C CNN
F 3 "~" H 2350 1100 50  0001 C CNN
	1    2350 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C407
U 1 1 60AE58EA
P 2100 1900
F 0 "C407" H 2215 1946 50  0000 L CNN
F 1 "0.1uF" H 2215 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 2138 1750 50  0001 C CNN
F 3 "~" H 2100 1900 50  0001 C CNN
	1    2100 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C401
U 1 1 60AE1806
P 950 1900
F 0 "C401" H 1250 1950 50  0000 R CNN
F 1 "0.1uF" H 1250 1850 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 988 1750 50  0001 C CNN
F 3 "~" H 950 1900 50  0001 C CNN
	1    950  1900
	-1   0    0    -1  
$EndComp
$Comp
L SYSC4907_Parts:MCP1501 U401
U 1 1 60ADDBE6
P 1550 1600
F 0 "U401" H 1550 1700 50  0000 C CNN
F 1 "MCP1501" H 1550 1600 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6" H 1550 1600 50  0001 C CNN
F 3 "https://ww1.microchip.com/downloads/en/DeviceDoc/20005474E.pdf" H 1550 1600 50  0001 C CNN
F 4 "MCP1501T-10E/CHY" H 1550 1600 50  0001 C CNN "MPN"
	1    1550 1600
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C414
U 1 1 60ACDDB2
P 3900 1100
F 0 "C414" V 4050 1100 50  0000 C CNN
F 1 "1uF" V 3750 1100 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 3938 950 50  0001 C CNN
F 3 "~" H 3900 1100 50  0001 C CNN
	1    3900 1100
	0    1    -1   0   
$EndComp
$Comp
L Device:C C415
U 1 1 60ACBC30
P 5700 1900
F 0 "C415" H 5815 1946 50  0000 L CNN
F 1 "1uF" H 5815 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5738 1750 50  0001 C CNN
F 3 "~" H 5700 1900 50  0001 C CNN
	1    5700 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R403
U 1 1 60ACB709
P 5300 1900
F 0 "R403" H 5370 1946 50  0000 L CNN
F 1 "10K" H 5370 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5230 1900 50  0001 C CNN
F 3 "~" H 5300 1900 50  0001 C CNN
	1    5300 1900
	1    0    0    -1  
$EndComp
$Comp
L SYSC4907_Parts:ZTP-148SR U404
U 1 1 60AC9774
P 4500 1400
F 0 "U404" H 4500 1400 50  0000 C CNN
F 1 "ZTP-148SR" H 4500 1000 50  0000 C CNN
F 2 "SYSC4907:ZTP-148SR" H 4500 1400 50  0001 C CNN
F 3 "https://www.amphenol-sensors.com/en/component/edocman/427-thermometrics-ztp-148sr-thermopile-ir-sensor-datasheet/download" H 4500 1400 50  0001 C CNN
F 4 "ZTP-148SR" H 4500 1400 50  0001 C CNN "MPN"
	1    4500 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C411
U 1 1 60AC72BB
P 3200 2150
F 0 "C411" V 3050 2150 50  0000 C CNN
F 1 "0.01uF" V 3350 2150 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 3238 2000 50  0001 C CNN
F 3 "~" H 3200 2150 50  0001 C CNN
	1    3200 2150
	0    1    1    0   
$EndComp
$Comp
L SYSC4907_Parts:AD8293G80 U403
U 1 1 60AC46DB
P 3200 800
F 0 "U403" H 3200 925 50  0000 C CNN
F 1 "AD8293G80" H 3200 834 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-8" H 3200 800 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/AD8293G80_160.pdf" H 3200 800 50  0001 C CNN
F 4 "AD8293G80" H 3200 800 50  0001 C CNN "MPN"
	1    3200 800 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R402
U 1 1 5FEB4119
P 4050 3200
F 0 "R402" V 4150 3200 50  0000 L CNN
F 1 "4.7" V 3950 3150 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3980 3200 50  0001 C CNN
F 3 "~" H 4050 3200 50  0001 C CNN
	1    4050 3200
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
