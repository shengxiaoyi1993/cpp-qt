# on zcu102, 4k image edge adj

2020/11/30

## 1. Port Define

3.3V IO 接口，注意usb->uart 接口电压

```
set_property PACKAGE_PIN H14 [get_ports us_sys_tx_port]
set_property PACKAGE_PIN J14 [get_ports us_sys_rx_port]
set_property IOSTANDARD LVCMOS33 [get_ports us_sys_tx_port]
set_property IOSTANDARD LVCMOS33 [get_ports us_sys_rx_port]
```

## 2. Uart format

串口通讯速率115200

通讯规范： 写入寄存器X"0C10", 数据X"00120013"
```
                  后续字节数
          / 同步字 \   v  读  /地址\  /  数据      \
  接收    FA  19  79  06  00  10  0C  13  00  12  00
  发送                    \     CRC 范围           /  41
                                                    CRC 反馈，sum of all
```

通讯规范： 读取寄存器X"0C10", 读取数据X"00120013"
```
                  后续字节数
          / 同步字 \   v  写  /地址\
  接收    FA  19  79  02  01  10  0C
  发送                    \CRC 范围/  1D  13  00  12  00
                                     CRC  \ 数据       /
```



## 3. Reg Define

调节分模块，3840x2160 被分割成8x4个调节块
调节块按调节点分割，如下图所示，P00 默认点为（0， 0）， P10 默认点为（480，0）
所以第一个调节块的四个默认角点为 (0,0), (479,0), (0,539), (479,539),以此类推

以下这些点是默认点，不会改变。
P11，P21，P31，P41，P51，P61，P71
P12，P22，P32，P42，P52，P62，P72
P13，P23，P33，P43，P53，P63，P73

可以调节的点为：
P00, P10, P20, P30, P40, P50, P60, P70, P80
P01,                                    P81
P02,                                    P82
P03,                                    P83
P04, P14, P24, P34, P44, P54, P64, P74, P84

调节点基于基准位置调节，调节量为10bit signed， X 和 Y 方向都是
调节只能往内侧调节，不能超出3840x2160的范围

有一个限制，所有同一行的调节点，Y方向的调节量差异不能超过120个点。


```
  -- adj block layout
  -- * 4k2k layout, 3840 x 2160, divide into 8 block for horizontal, divide into 4 block for vertical
  --   so every adj_blk size is 480 x 540
  --   ------------------------------------------------------------------------->
  -- |     0,0     1,0     2,0     3,0     4,0     5,0     6,0     7,0     8,0
  -- | 0,0 x       x       x       x       x       x       x       x       x   0
  -- |       b00     b10     b20     b30     b40     b50     b60     b70
  -- | 0,1 x       o       o       o       o       o       o       o       x   540
  -- |       b01     b11     b21     b31     b41     b51     b61     b71
  -- | 0,2 x       o                                               o       x   1080
  -- |       b02     b12     b22     b32     b42     b52     b62     b72
  -- | 0,3 x       o       o       o       o       o       o       o       x   1620
  -- |       b03     b13     b23     b33     b43     b53     b63     b73
  -- | 0,4 x       x       x       x       x       x       x       x       x   2159
  -- |
  -- |     0       480     960     1440    1920    2400    2880    3360    3839
  -- v
```


```
// **************************************************
// EDGE, 0x0C00 ~ 0x0D00
// **************************************************
// EDGE ADJ control
register EDGE_CTRL at 0x0C00, split, frame "SYSTEM"
field ENABLE          [ 0 ..  0], default 0x1, decimal
{enable the edge control function or not}
endf
endr

// edge point 00, x=0, y=0
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P00 at 0x0C10, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x032, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x012, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 10, x=1, y=0
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P10 at 0x0C14, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x071, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x39f, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 20, x=2, y=0
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P20 at 0x0C18, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x039, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x3f7, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 30, x=3, y=0
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P30 at 0x0C1C, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x058, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x074, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 40, x=4, y=0
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P40 at 0x0C20, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x01f, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x053, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 50, x=5, y=0
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P50 at 0x0C24, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x07f, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x031, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 60, x=6, y=0
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P60 at 0x0C28, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x022, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x011, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 70, x=7, y=0
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P70 at 0x0C2C, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x063, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x067, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 80, x=8, y=0
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P80 at 0x0C30, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x03e, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x3fe, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 04, x=0, y=4
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P04 at 0x0C40, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x3ef, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x071, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 14, x=1, y=4
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P14 at 0x0C44, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x39a, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x035, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 24, x=2, y=4
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P24 at 0x0C48, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x3ea, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x02f, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 34, x=3, y=4
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P34 at 0x0C4C, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x3dd, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x017, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 44, x=4, y=4
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P44 at 0x0C50, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x3ee, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x3ef, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 54, x=5, y=4
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P54 at 0x0C54, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x39a, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x3ea, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 64, x=6, y=4
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P64 at 0x0C58, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x3ae, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x03f, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 74, x=7, y=4
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P74 at 0x0C5C, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x3b1, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x031, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 84, x=8, y=4
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P84 at 0x0C60, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x3f1, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x3b7, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 01, x=0, y=1
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P01 at 0x0C70, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x3d3, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x053, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 02, x=0, y=2
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P02 at 0x0C74, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x034, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x027, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 03, x=0, y=3
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P03 at 0x0C78, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x03f, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x03f, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 81, x=8, y=1
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P81 at 0x0C80, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x3c3, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x3de, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 82, x=8, y=2
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P82 at 0x0C84, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x025, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x3de, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr

// edge point 83, x=8, y=3
// basic rule, can not let pixel out of 3940x2160 display range
register EDGE_P83 at 0x0C88, split, frame "SYSTEM"
field SHIFT_Y         [25 .. 16], default 0x01f, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
field SHIFT_X         [ 9 ..  0], default 0x3f2, decimal
{10 bit signed value, default location add this shift will be the final result}
endf
endr




```





