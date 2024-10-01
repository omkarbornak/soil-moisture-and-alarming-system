#ifndef LCD_H
#define LCD_H

#include <xc.h>

#define _XTAL_FREQ 20000000  // Define your crystal frequency (20 MHz)
#define RS LATD2
#define EN LATD3
#define D4 LATD4
#define D5 LATD5
#define D6 LATD6
#define D7 LATD7

// Function declarations
void LCD_Init();
void LCD_Command(char);
void LCD_Char(char);
void LCD_Write_String(const char*);
void LCD_Set_Cursor(char, char);
void LCD_Clear();

#endif
