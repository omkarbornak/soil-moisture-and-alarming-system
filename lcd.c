#include "lcd.h"

// Send command to LCD
void LCD_Command(char cmd) {
    RS = 0;  // Command mode
    D4 = (cmd >> 4) & 0x01;
    D5 = (cmd >> 5) & 0x01;
    D6 = (cmd >> 6) & 0x01;
    D7 = (cmd >> 7) & 0x01;
    EN = 1;
    __delay_ms(4);
    EN = 0;

    D4 = cmd & 0x01;
    D5 = (cmd >> 1) & 0x01;
    D6 = (cmd >> 2) & 0x01;
    D7 = (cmd >> 3) & 0x01;
    EN = 1;
    __delay_ms(4);
    EN = 0;
}

// Initialize the LCD
void LCD_Init() {
    TRISD = 0x00;  // PORTD as output
    __delay_ms(20);
    LCD_Command(0x02);  // Initialize LCD in 4-bit mode
    LCD_Command(0x28);  // 4-bit mode, 2 lines, 5x7 format
    LCD_Command(0x0C);  // Display on, cursor off
    LCD_Command(0x06);  // Increment cursor
    LCD_Command(0x01);  // Clear display
    __delay_ms(2);
}

// Send character to LCD
void LCD_Char(char data) {
    RS = 1;  // Data mode
    D4 = (data >> 4) & 0x01;
    D5 = (data >> 5) & 0x01;
    D6 = (data >> 6) & 0x01;
    D7 = (data >> 7) & 0x01;
    EN = 1;
    __delay_ms(4);
    EN = 0;

    D4 = data & 0x01;
    D5 = (data >> 1) & 0x01;
    D6 = (data >> 2) & 0x01;
    D7 = (data >> 3) & 0x01;
    EN = 1;
    __delay_ms(4);
    EN = 0;
}

// Write string to LCD
void LCD_Write_String(const char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

// Set cursor position
void LCD_Set_Cursor(char row, char column) {
    char temp;
    if (row == 1) {
        temp = 0x80 + column - 1;  // First row address
        LCD_Command(temp);
    } else if (row == 2) {
        temp = 0xC0 + column - 1;  // Second row address
        LCD_Command(temp);
    }
}

// Clear the LCD
void LCD_Clear() {
    LCD_Command(0x01);  // Clear display
    __delay_ms(2);
}
