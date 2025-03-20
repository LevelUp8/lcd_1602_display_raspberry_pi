#include <wiringPi.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define RS 26
#define E 19
#define D4 13
#define D5 6
#define D6 5
#define D7 22



void lcd_send_nibble(int data) {
    digitalWrite(D4, (data & 0x01));
    digitalWrite(D5, (data & 0x02) >> 1);
    digitalWrite(D6, (data & 0x04) >> 2);
    digitalWrite(D7, (data & 0x08) >> 3);
    
    digitalWrite(E, HIGH);
    delayMicroseconds(500);
    digitalWrite(E, LOW);
    delayMicroseconds(500);
}

void lcd_send_byte(int bits, int mode) {
    digitalWrite(RS, mode);
    delayMicroseconds(100);
    lcd_send_nibble(bits >> 4);
    delayMicroseconds(100);
    lcd_send_nibble(bits & 0x0F);
    delayMicroseconds(100);
}

void lcd_init() {
    pinMode(RS, OUTPUT);
    pinMode(E, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    
    delay(50);
    
    lcd_send_nibble(0x03);
    delay(5);
    lcd_send_nibble(0x03);
    delayMicroseconds(200);
    lcd_send_nibble(0x03);
    delayMicroseconds(200);
    lcd_send_nibble(0x02);

    lcd_send_byte(0x28, 0); // 2 lines, 5x8 font
    lcd_send_byte(0x0C, 0); // Display on, cursor off
    lcd_send_byte(0x06, 0); // Auto-increment cursor
}


void lcd_clear() {
    lcd_send_byte(0x01, 0);
    delay(2);
}

void lcd_set_cursor(int line, int pos) {
    int addr = (line == 1) ? 0x80 : 0xC0;
    lcd_send_byte(addr + pos, 0);
}

void lcd_print(char *str) {
    while (*str) {
        lcd_send_byte(*str++, 1);
    }
}


void update_lcd() {
    char line1[17], line2[17];
    char line1_old[17] = "";
    char line2_old[17] = "";
    time_t now;
    struct tm *t;

    while (1) {
        time(&now);
        t = localtime(&now);

        // Format first line: "Tue 12:34:56"
        snprintf(line1, 17, "%s %02d:%02d:%02d", 
                 (char *[]){"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}[t->tm_wday],
                 t->tm_hour, t->tm_min, t->tm_sec);

        // Format second line: "19-Mar-2024"
        snprintf(line2, 17, "%02d-%s-%d", 
                 t->tm_mday, 
                 (char *[]){"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}[t->tm_mon],
                 t->tm_year + 1900);
                 
    

        // Display on LCD
        /*lcd_clear();
        lcd_set_cursor(1, 0);
        lcd_print(line1);
        lcd_set_cursor(2, 0);
        lcd_print(line2);*/
        
         // Only update if text changed (prevent flicker)
        if (strcmp(line1, line1_old) != 0) {
            lcd_set_cursor(1, 0);
            lcd_print(line1);
            strncpy(line1_old, line1, 17);
        }
        if (strcmp(line2, line2_old) != 0) {
            lcd_set_cursor(2, 0);
            lcd_print(line2);
            strncpy(line2_old, line2, 17);
        }

        delay(1000); // Update every second
    }
}

int main() {
    wiringPiSetupGpio();
    lcd_init();
    lcd_clear();

    update_lcd(); // Start updating time & date

    return 0;
}
