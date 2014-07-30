/*

LCD Pins for Mega 2560
 * LCD RS pin to digital pin 42
 * LCD Enable pin to digital pin 41
 * LCD D4 pin to digital pin 33
 * LCD D5 pin to digital pin 32
 * LCD D6 pin to digital pin 31
 * LCD D7 pin to digital pin 30
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(42, 41, 33, 32, 31, 30);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
}

