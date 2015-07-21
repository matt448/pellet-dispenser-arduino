/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.
 
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch
 
 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).
 
 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state
 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND
 
 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 
*/

#include "HX711.h"
#include <LiquidCrystal.h>
#include <Keypad.h>

//Configure LCD screen pins
LiquidCrystal lcd(51, 50, 49, 48, 47, 46);

//Configure HX711 pins
#define DOUT 5
#define CLK  6
HX711 scale(DOUT, CLK);

//Configure Keypad
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {42, 40, 38, 36}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {34, 32, 30}; //connect to the column pinouts of the keypad

// Instantiate the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



float calibration_factor = 2125; //-7050 worked for my 440lb max scale setup
float units;
float ounces;

const int calibration_button_up = 44;
const int calibration_button_dn = 45;
int upbuttonstate = 0;
int dnbuttonstate = 0;


void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("Starting system");
  lcd.setCursor(0, 2);
  lcd.print("Remove all weight");
  lcd.setCursor(0, 3);
  lcd.print("from scale");
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  delay(500);
  scale.set_scale();
  scale.tare();	//Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {
 scale.set_scale(calibration_factor); //Adjust to this calibration factor
 char key = menu_page1();
 
 if(key) {
   Serial.print("Key pressed: ");
   Serial.println(key);
   if(key == '3') {
       calibration_factor = calibrate_scale();
       Serial.print("Main menu Key val: ");
       Serial.println(key);
       Serial.print("calib factor: ");
       Serial.print(calibration_factor);
   }//ifkey3
 }//ifkey

}//mainloop



char menu_page1(){
  lcd.setCursor(0,0);
  lcd.print("1.Enter weight      ");
  lcd.setCursor(0,1);
  lcd.print("2.Calcualte weight  ");
  lcd.setCursor(0,2);
  lcd.print("3.Calibrate scale   ");
  lcd.setCursor(0,3);
  lcd.print("                    ");
  char key = keypad.getKey();
  return key;
}


float calibrate_scale(){
  char key = ' ';
  while(key != '*') {
    Serial.print("Reading: ");
    units = scale.get_units(), 10;
    if (units < 0) {
      units = 0.00;
    }
  
    Serial.print(units);
    ounces = units * 0.035274;
    Serial.print(" grams"); 
    Serial.print(" calibration_factor: ");
    Serial.print(calibration_factor);
    Serial.println();
    lcd.setCursor(0, 0);
    lcd.print("Calibrating. Exit=*");
    lcd.setCursor(0, 1);
    lcd.print("Grams: ");
    lcd.print(units);
    lcd.print("   ");
    lcd.setCursor(0, 2);
    lcd.print("Ounce: ");
    lcd.print(ounces);
    lcd.print("   ");
    lcd.setCursor(0, 3);
    lcd.print("Calbr: ");
    lcd.print(calibration_factor);
    lcd.print("   ");

    //Check button states for calibration buttons
    upbuttonstate = digitalRead(calibration_button_up);
    dnbuttonstate = digitalRead(calibration_button_dn);
    if(upbuttonstate == HIGH) {
      calibration_factor += 0.1;
    }
    if(dnbuttonstate == HIGH) {
      calibration_factor -= 0.1;
    }
    scale.set_scale(calibration_factor);
    char key = keypad.getKey();
    Serial.print("Calib Key val: ");
    Serial.println(key);
    if(key == '*') { break; }
  }
  return(calibration_factor);
}
  
