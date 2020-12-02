// // **************************************************
// // EDGE, 0x0C00 ~ 0x0D00
// // **************************************************
// // EDGE ADJ control
#define EDGE_CTRL 0x0C00
// field ENABLE          [ 0 ..  0], default 0x1, decimal
// {enable the edge control function or not}
// endf
// endr
//
// // edge point 00, x=0, y=0
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P00 0x0C10
// field SHIFT_Y         [25 .. 16], default 0x032, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x012, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 10, x=1, y=0
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P10 0x0C14
// field SHIFT_Y         [25 .. 16], default 0x071, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x39f, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 20, x=2, y=0
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P20 0x0C18
// field SHIFT_Y         [25 .. 16], default 0x039, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x3f7, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 30, x=3, y=0
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P30 0x0C1C
// field SHIFT_Y         [25 .. 16], default 0x058, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x074, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 40, x=4, y=0
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P40 0x0C20
// field SHIFT_Y         [25 .. 16], default 0x01f, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x053, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 50, x=5, y=0
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P50 0x0C24
// field SHIFT_Y         [25 .. 16], default 0x07f, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x031, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 60, x=6, y=0
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P60 0x0C28
// field SHIFT_Y         [25 .. 16], default 0x022, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x011, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 70, x=7, y=0
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P70 0x0C2C
// field SHIFT_Y         [25 .. 16], default 0x063, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x067, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 80, x=8, y=0
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P80 0x0C30
// field SHIFT_Y         [25 .. 16], default 0x03e, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x3fe, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 04, x=0, y=4
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P04 0x0C40
// field SHIFT_Y         [25 .. 16], default 0x3ef, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x071, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 14, x=1, y=4
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P14 0x0C44
// field SHIFT_Y         [25 .. 16], default 0x39a, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x035, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 24, x=2, y=4
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P24 0x0C48
// field SHIFT_Y         [25 .. 16], default 0x3ea, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x02f, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 34, x=3, y=4
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P34 0x0C4C
// field SHIFT_Y         [25 .. 16], default 0x3dd, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x017, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 44, x=4, y=4
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P44 0x0C50
// field SHIFT_Y         [25 .. 16], default 0x3ee, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x3ef, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 54, x=5, y=4
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P54 0x0C54
// field SHIFT_Y         [25 .. 16], default 0x39a, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x3ea, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 64, x=6, y=4
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P64 0x0C58
// field SHIFT_Y         [25 .. 16], default 0x3ae, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x03f, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 74, x=7, y=4
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P74 0x0C5C
// field SHIFT_Y         [25 .. 16], default 0x3b1, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x031, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 84, x=8, y=4
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P84 0x0C60
// field SHIFT_Y         [25 .. 16], default 0x3f1, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x3b7, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 01, x=0, y=1
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P01 0x0C70
// field SHIFT_Y         [25 .. 16], default 0x3d3, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x053, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 02, x=0, y=2
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P02 0x0C74
// field SHIFT_Y         [25 .. 16], default 0x034, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x027, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 03, x=0, y=3
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P03 0x0C78
// field SHIFT_Y         [25 .. 16], default 0x03f, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x03f, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 81, x=8, y=1
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P81 0x0C80
// field SHIFT_Y         [25 .. 16], default 0x3c3, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x3de, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 82, x=8, y=2
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P82 0x0C84
// field SHIFT_Y         [25 .. 16], default 0x025, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x3de, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
//
// // edge point 83, x=8, y=3
// // basic rule, can not let pixel out of 3940x2160 display range
#define EDGE_P83 0x0C88
// field SHIFT_Y         [25 .. 16], default 0x01f, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// field SHIFT_X         [ 9 ..  0], default 0x3f2, decimal
// {10 bit signed value, default location add this shift will be the final result}
// endf
// endr
