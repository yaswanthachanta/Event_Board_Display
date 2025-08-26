//event_board.c

#include <lpc21xx.h>          // Header file for LPC21xx microcontroller
#include <string.h>           // For string functions
#include "delay.h"            // Custom delay functions
#include "types.h"            // Custom data type definitions (like u8, s32, etc.)
#include "lcd.h"              // LCD interface functions
#include "lcd_defines.h"      // LCD pin and command definitions
#include "kpm.h"              // Keypad module functions
#include "rtc.h"              // Real-Time Clock functions
#include "pin_connect_block.h"// For configuring pin functions
#include "interrupts_defines.h" // Interrupt-related definitions
#include "settings.h"         // Custom settings function
#include "adc.h"              // ADC (Analog to Digital Converter) functions

#define TIME_LIMIT 14        // Time limit for message display

#define TOTAL_MESSAGES 10     // Number of scheduled messages
#define LED_RED 25            // Red LED connected to P1.25
#define LED_GREEN 26          // Green LED connected to P1.26
#define LCD_WIDTH 16          // LCD screen is 16 characters wide

// Variables to hold current date and time
s32 hour, min, sec, date, month, year, day;
u32 msg_flag = 1;             // Flag to control message display
u8 len;                       // Length of message
s32 i = 0, startHour, startMin; // Loop counter and message start time

// Structure to hold message information
typedef struct
{
    u8 hour;                  // Hour when message should display
    u8 minute;                // Minute when message should display
    char text[80];            // The actual message
    u8 enabled;               // 1 = show message, 0 = skip
} Message;

// Function declarations
void msg_scroll(const char *, u32, u32); // Function to scroll message
int find_strlen(const char *);           // Function to find string length
void eint0_isr(void) __irq;              // External interrupt ISR function

// List of all scheduled messages
Message messageList[TOTAL_MESSAGES] = {
    {7, 45,  "               Good Morning! Classes Start Soon ", 1},
    {13, 45, "               C Programming Session in Class room number 2 ", 1},
    {11, 15, "               C module theory exam in 4th floor lab1 ", 1},
    {10, 15, "               C module lab exam in 4th floor lab2 and middle lab at 10:30AM ", 1},
    {12, 45, "               Lunch Break from 1PM - 2PM ", 1},
    {9, 45,  "               ARM Workshop on external interrupts in LAB1 at 10AM ", 1},
    {10, 00,  "               ARM kit issue time from 10AM - 10:30AM in middle lab ", 1},
    {15, 15, "               Only 15 mins break time for next ARM session ", 1},
    {17, 00, "               Revise today's Class programs at home! ", 1},
    {17, 45, "               End of Day - See You Tomorrow! ", 1}
};

int main()
{
    // Initialize LCD, keypad, RTC, and ADC
    InitLCD();
    InitKPM();
    RTC_Init();
    Init_ADC();

    IODIR1 |= 3 << LED_RED;  // Set P1.25 and P1.26 (LED pins) as output

    // Set initial RTC time (07:44:55 AM)
    SetRTCTimeInfo(07, 44, 55);
    // Set initial RTC date (15 July 2025)
    SetRTCDateInfo(15, 7, 2025);
    // Set RTC day (Tuesday)
    SetRTCDay(2);

    // Configure external interrupt on P0.1 (EINT0)
    cfgPortPin(0, 1, EINT0_PIN_FUNC);
    VICIntSelect = 0;                         // Select as IRQ
    VICIntEnable = 1 << EINT0_VIC_CHNO;       // Enable interrupt channel
    VICVectCntl0 = (1 << 5) | EINT0_VIC_CHNO; // Enable and assign slot
    VICVectAddr0 = (u32)eint0_isr;            // Set ISR address
    EXTMODE = 1 << 0;                         // Set EINT0 as edge-sensitive

    while (1)
    {
        msg_flag = 1;                 // Reset message flag
        IOSET1 = 1 << LED_RED;        // Turn ON red LED
        IOCLR1 = 1 << LED_GREEN;      // Turn OFF green LED

        CmdLCD(GOTO_LINE1_POS0);      // Set cursor to first line start
        GetRTCTimeInfo(&hour, &min, &sec); // Get current time
        DisplayRTCTime(hour, min, sec);    // Display time on LCD
        GetRTCDateInfo(&date, &month, &year); // Get current date
        DisplayRTCDate(date, month, year);    // Display date on LCD
        GetRTCDay(&day);                     // Get current day of week
        DisplayRTCDay(day);                  // Display day on LCD

        CmdLCD(GOTO_LINE2_POS0 + 11); // Move cursor to second line, position 11
        U32LCD(Read_LM35());          // Read temperature sensor (LM35) and display value
        CharLCD(223);                 // Display degree symbol
        CharLCD('C');                 // Display 'C' for Celsius

        // Loop through all messages to check if one is scheduled now
        for (i = 0; i < TOTAL_MESSAGES; i++)
        {
            if (messageList[i].hour == hour && messageList[i].minute == min && messageList[i].enabled == 1)
            {
                startHour = hour; // Store start time
                startMin = min;

                // Start scrolling message until time changes or user interrupts
                while (1)
                {
                    IOSET1 = 1 << LED_GREEN; // Turn ON green LED
                    IOCLR1 = 1 << LED_RED;   // Turn OFF red LED

                    GetRTCTimeInfo(&hour, &min, &sec); // Update time

                    // Exit if minute changes forward or backward or if user interrupts
                    if ((((hour * 60 + min) - (startHour * 60 + startMin)) > TIME_LIMIT) || 
                        (((hour * 60 + min) - (startHour * 60 + startMin)) < 0))
                        break;
                    if (msg_flag == 0)
                        break;

                    len = find_strlen(messageList[i].text); // Get message length
                    msg_scroll(messageList[i].text, len, 200); // Scroll message slowly
                }

                CmdLCD(CLEAR_LCD); // Clear LCD after message display
            }
        }
    }
}

// Scrolls the message across LCD screen
void msg_scroll(const char *p, u32 size, u32 timems)
{
    char window[LCD_WIDTH + 1]; // Temporary window for 16 characters
    u32 i, j;

    // Scroll through entire message
    for (i = 0; i <= size; i++)
    {
        for (j = 0; j < LCD_WIDTH; j++)
        {
            GetRTCTimeInfo(&hour, &min, &sec); // Continuously check time

            if ((i + j) < size)
                window[j] = p[i + j]; // Copy next character
            else
                window[j] = ' ';      // Add space if end of string

        }
        window[LCD_WIDTH] = '\0'; // Null-terminate string

        CmdLCD(GOTO_LINE1_POS0);     // Go to first line
        StrLCD(window);              // Display scrolled window

        CmdLCD(GOTO_LINE2_POS0);     // Go to second line
        StrLCD("  EVENT BOARD   ");  // Display fixed text

        delay_ms(timems);            // Wait for scrolling effect

        // Exit scroll if time changes or user interrupts
        if ((((hour * 60 + min) - (startHour * 60 + startMin)) > TIME_LIMIT) || 
            (((hour * 60 + min) - (startHour * 60 + startMin)) < 0))
            break;
        if (msg_flag == 0)
            break;
    }
}

// Finds the length of a string
int find_strlen(const char *p)
{
    u32 i = 0;
    while (p[i]) i++; // Count until null character
    return i;
}

// Interrupt Service Routine for external interrupt EINT0
void eint0_isr(void) __irq
{
    IOCLR1 = 1 << LED_GREEN; // Turn off green LED
    IOCLR1 = 1 << LED_RED;   // Turn off red LED
    settings();              // Call settings menu or function
    CmdLCD(CLEAR_LCD);       // Clear LCD screen
    EXTINT = 1 << 0;         // Clear external interrupt flag
    VICVectAddr = 0;         // Signal end of interrupt
}
