#include <xc.h>
#include "lcd.h"   // Custom library to interface with the LCD

// Configuration bits
#pragma config FOSC = HS        // Oscillator Selection
#pragma config PWRT = OFF       // Power-up Timer Enable
#pragma config WDT = OFF        // Watchdog Timer Enable
#pragma config MCLRE = ON       // MCLR Pin Enable
#pragma config LVP = OFF        // Low Voltage In-Circuit Serial Programming

#define _XTAL_FREQ 20000000     // 20 MHz clock frequency
#define SOIL_SENSOR_THRESHOLD 500 // Threshold for dry soil (adjust based on your sensor)

void ADC_Init() {
    ADCON1 = 0x0E;  // Configure AN0 as analog input
    ADCON2 = 0xA9;  // Right justify result, Fosc/8, acquisition time
    ADCON0 = 0x01;  // Enable ADC, select AN0
}

unsigned int ADC_Read(unsigned char channel) {
    ADCON0 &= 0xC5;  // Select the ADC channel
    ADCON0 |= channel << 3;
    __delay_ms(2);   // Acquisition time
    GO_nDONE = 1;    // Start conversion
    while(GO_nDONE); // Wait for conversion to complete
    return ((ADRESH << 8) + ADRESL); // Return the result
}

void init_buzzer() {
    TRISB0 = 0;  // Set RB0 as output for the buzzer
    LATB0 = 0;   // Initially turn off the buzzer
}

void trigger_buzzer() {
    LATB0 = 1;   // Turn on the buzzer
    __delay_ms(500);
    LATB0 = 0;   // Turn off the buzzer
}

void main() {
    unsigned int soil_moisture_value;

    // Initialize modules
    TRISD = 0x00;        // PORTD as output for LCD
    LCD_Init();          // Initialize LCD
    ADC_Init();          // Initialize ADC
    init_buzzer();       // Initialize buzzer

    LCD_Clear();
    LCD_Set_Cursor(1, 1);
    LCD_Write_String("Soil Moisture:");
    
    while(1) {
        soil_moisture_value = ADC_Read(0); // Read soil moisture from AN0
        
        LCD_Set_Cursor(2, 1);
        LCD_Write_String("Value: ");
        LCD_Write_Integer(soil_moisture_value); // Display the soil moisture value
        
        // Check if soil moisture is below the threshold
        if(soil_moisture_value < SOIL_SENSOR_THRESHOLD) {
            LCD_Set_Cursor(2, 10);
            LCD_Write_String("Dry!");
            trigger_buzzer(); // Trigger buzzer when dry
        } else {
            LCD_Set_Cursor(2, 10);
            LCD_Write_String("Wet ");
            LATB0 = 0; // Make sure the buzzer is off
        }

        __delay_ms(1000);  // Update every 1 second
    }
}
