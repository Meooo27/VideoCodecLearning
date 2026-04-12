## Full Range of chanel:
    Y,R,G,B  = [0:1]      = [0:255]
    UV       = [-0.5:0.5] = [-128:128]

| ColorSpace | kr     | kg     | kb     |
|------------|:------:|:------:|:------:|
| BT601      | 0.299  | 0.587  | 0.144  |
| BT709      | 0.2126 | 0.7152 | 0.0722 |
| BT2020     | 0.2627 | 0.6780 | 0.0593 |

------------------------------------------------------------------------------------------------------
## YUV -> RGB formular
```
| R |      |      1                  0                 2*(1-kr)        |   | Y |
| G |    = |      1           -2*(1-kb)*kb/kg     -2*(1-kr)*kr/kg      | * | U |
| B |      |      1               2*(1-kb)                0            |   | V |
```

------------------------------------------------------------------------------------------------------
## RGB -> YUV formular
```
| Y    |   |      kr                 kg                   kb           |   | R |
| U Cb | = | -0.5*kr/(1-kb)    -0.5*kg/(1-kb)             0.5          | * | G |
| V Cr |   |      0.5          -0.5*kg/(1-kr)         -0.5*kb/(1-kr)   |   | B |
```

------------------------------------------------------------------------------------------------------
## YUV420p Planar
- YUV bitstream structure:
> YYYY...YYYY UU...UU VV....VV

- YUV converts into Matrix:
```
0_______________________X              How Y use UV plane? (4Y used same 1UV)
|---w------
| Y Y Y Y |                             Y00(0)   Y01(1)  |  Y02(2)   Y03(3)
| Y Y Y Y | h                           Y10(4)   Y11(5)  |  Y12(6)   Y13(7)
| Y Y Y Y |                             U00(0)   V00(0)  |  U01(1)   V01(1)
| Y Y Y Y |                             -----------------------------------
|-w/2-                                  Y20(8)   Y21(9)  |  Y22(10)  Y23(11)
| U U | h/2                             Y30(12)  Y31(13) |  Y32(14)  Y33(15)
| U U |                                 U10(2)   V10(2)  |  U11(3)   V11(3)
|-w/2-
| V V | h/2                             Loop by UV: 
| V V |                                       uvIndex   = currentRowUV * (width/2) + currentColUV;
|
|                                       From UV postion we can calculate Y Postion: (based on: YWidth = 2 * UVWidth)
|                                             YColStart = currentColUV * 2;
|                                             YRowStart = currentRowUV * 2;
|                                             y00Index  = YRowStart       * w + YColStart;
|                                             y01Index  = YRowStart       * w + YColStart + 1;
|                                             y10Index  = (YRowStart + 1) * w + YColStart;
|                                             y11Index  = (YRowStart + 1) * w + YColStart + 1;
|
Y
```

------------------------------------------------------------------------------------------------------
## YUV420p Semi Planar
- YUV bitstream structure:
> YYYY...YYYY UVUV..UVUV

- YUV converts into Matrix:
```
0_______________________X              How Y use UV plane? (4Y used same 1UV)
|---w------
| Y Y Y Y |                             Y00(0)   Y01(1)  |  Y02(2)   Y03(3)
| Y Y Y Y | h                           Y10(4)   Y11(5)  |  Y12(6)   Y13(7)
| Y Y Y Y |                             U00(0)   V00(0)  |  U01(1)   V01(1)
| Y Y Y Y |                             -----------------------------------
|---w-----                              Y20(8)   Y21(9)  |  Y22(10)  Y23(11)
| U V U V | h/2                         Y30(12)  Y31(13) |  Y32(14)  Y33(15)
| U V U V |                             U10(2)   V10(2)  |  U11(3)   V11(3)
|
|                                       Loop by UV: 
|                                             uIndex   = currentRowUV * width + currentColUV;
|                                             vIndex   = uIndex + 1;
|
|                                       From UV postion we can calculate Y Postion: (based on: YWidth = 2 * UVWidth)
|                                             YColStart = currentColUV * 2;
|                                             YRowStart = currentRowUV * 2;
|                                             y00Index  = YRowStart       * w + YColStart;
|                                             y01Index  = YRowStart       * w + YColStart + 1;
|                                             y10Index  = (YRowStart + 1) * w + YColStart;
|                                             y11Index  = (YRowStart + 1) * w + YColStart + 1;
|
Y
```

------------------------------------------------------------------------------------------------------
## RGB888
- RGB bitstream structure: 
> R0G0B0...R99G99B99...RnGnBn

- RGB converts into Matrix
```
0_______________________________________________________________X            Then, How to change RGB to YUV?
|--------------------------- w --------------------------- |                       Y00 = R00 G00 B00
| R00 G00 B00    R01 G01 B01    R02 G02 B02    R03 G03 B03 |                       Y01 = R01 G01 B01
| R10 G10 B10    R11 G11 B11    R12 G12 B12    R13 G13 B13 |                       Y10 = R10 G10 B10
| R20 G20 B20    R21 G21 B21    R22 G22 B22    R23 G23 B23 |                       Y11 = R11 G11 B11
| R30 G30 B30    R31 G31 B31    R32 G32 B32    R33 G33 B33 |                 Loop by Y:      
| R40 G40 B40    R41 G41 B41    R42 G42 B42    R43 G43 B43 | h                     yIndex  = currentRow * width + currentCol;
| R50 G50 B50    R51 G51 B51    R52 G52 B52    R53 G53 B53 |                       RIndex  = yIndex * 3 + 0;
| R60 G60 B60    R61 G61 B61    R62 G62 B62    R63 G63 B63 |                       GIndex  = yIndex * 3 + 1;
| R70 G70 B70    R71 G71 B71    R72 G72 B72    R73 G73 B73 |                       BIndex  = yIndex * 3 + 2;
| R80 G80 B80    R81 G81 B81    R82 G82 B82    R83 G83 B83 |
| R90 G90 B90    R91 G91 B91    R92 G92 B92    R93 G93 B93 |
|
Y
```
------------------------------------------------------------------------------------------------------
To be continue...
