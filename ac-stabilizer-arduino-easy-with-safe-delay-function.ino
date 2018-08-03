/*
  How to make this stabilizer See the
  Instructables -> https://www.instructables.com/id/How-to-Make-AC-220-Volt-Automatic-Stabilizer-Using/
  This is 3 steps Automatic Voltage stabilizer
  1st steps is normal/output
  2nd steps adds 20voltage to output
  3rd steps adds 50voltage to output

  want to edit the steps voltage go to ->//start- stabelising code// area
  there you can edit the voltage when to change the steps.

  Note : output voltage is calcuted not real reading, if you want real mesurement use another current sensor at out put.

*/
//include the library//
#include <ACS712.h> //ACS712 current sensor library | Download - https://github.com/shahid249/ACS712-arduino-1
#include <LiquidCrystal.h> // LCD library
//include the library//

/**********************************************************************************************************************************************************************/

//LCD pin Configuration - Start
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//LCD pin Configuration - end

/**********************************************************************************************************************************************************************/

//Output pin setup - Start
int relay = 7;  //relay1 pin
int relay1 = 8; //relay2 pin
int relay2 = 9; //relay3 pin
int relay3 = 10;//relay4 pin
int fan = 13;//Fan pin
int writevalue = 10; //LCD display write enable pin | pin use to control display write functin LOW mean write enable, HIGH mean write disable
//Output pin setup - end

/**********************************************************************************************************************************************************************/

//in put pin setup - Start
int voltagesens = A0; //voltage sensor input setup
ACS712 sensor(ACS712_30A, A1); //current sens input setup. If your sensor is 30A-> ACS712_30A | 20A-> ACS712_20A | 5A-> ACS712_05B
const int temperature = A2; //Temperature sensor input
int protection = A3; // Protection awitch input
//in put pin setup - end

/**********************************************************************************************************************************************************************/

//initalize setup//
int input_voltage_read;
float input_voltage_read_1;
float input_voltage_show;
int output_voltage_show;
float current_sensor_read;
float amper;
float watt_show;
//initilize setup//

//variable setup - Start //
int stepps = 0;
int stabelizeroenable = 1;
int fanu = 1;
int prote = 1;
//variable setup - Start //

//Default action -start//
bool displayenable = false;
bool serialread = false;
//Default action -end//

//display mill setup - Start
unsigned long displayMillis = 0;
unsigned long previousdMillis = 0;
const long intervaldisplay = 2000; //use to control the display refresh
//display mill setup - end

//setup current frequency//
float current_frequency = 50; // 50/60hz
//setup current frequency//

//Temperature
float tempc;  //variable to store temperature in degree Celsius
float tempf;  //variable to store temperature in Fahreinheit
float temperature_sens;  //temporary variable to hold sensor reading
//Temperature

void setup()
{
  sensor.calibrate();
  Serial.begin(9600);// begin serial communication between arduino and pc
  pinMode(voltagesens, INPUT); // set pin as input pin
  pinMode(temperature, INPUT); // set pin as input pin
  pinMode(protection, INPUT);
  pinMode(relay, OUTPUT); //setup output
  pinMode(relay1, OUTPUT); //setup output
  pinMode(relay2, OUTPUT); //setup output
  pinMode(writevalue, OUTPUT); //setup output
  pinMode(fan, OUTPUT);//setup output
  digitalWrite(relay, HIGH); //pull pin to high if relay use
  digitalWrite(relay1, HIGH);//pull pin to high if relay use
  digitalWrite(relay2, HIGH);//pull pin to high if relay use
  digitalWrite(writevalue, LOW);//pin default action
  lcd.begin(16, 2);// set up the LCD's number of columns and rows:
  lcd.setCursor(0, 1);// set the cursor to column 0, line 1
  lcd.print("BOOTING...");//lcd print
  delay(100);//delay
}

void loop()
{
  //start- Enable lcd display & Serial//
  displayenable = true;
  serialread = false;
  //end- Enable lcd display & Serial//


  //start- Voltage sensing code//

  input_voltage_read = analogRead(voltagesens); // read analog values from pin A0 across capacitor
  /*
    0.32969558101473 0.3533728549141966 0.3040078201368524
    converts analog value(x) into input ac supply value using this formula ( voltage 311 X 1023 arduino input)
  */
  input_voltage_read_1 = (input_voltage_read * 0.3915348837209302);//0.343356890459364 vguard
  input_voltage_show = input_voltage_read_1 / sqrt(1); //1.17 Ac voltage sqrt for lest calibration

  //end- Voltage sensing code//

  /**********************************************************************************************************************************************************************/

  //start- temperature sensing code//

  temperature_sens = analogRead(temperature);
  temperature_sens = (temperature_sens * 465) / 1023;
  tempc = temperature_sens; // Storing value in Degree Celsius
  tempf = (temperature_sens * 1.8) + 32; // Converting to Fahrenheit

  //end- temperature sensing code//

  /**********************************************************************************************************************************************************************/

  //start- stabelising code//

  if (input_voltage_show >= 0) {//If voltage is less than 0 this will active:
    stepps = 1;
  }
  if ((input_voltage_show <= 130) && (input_voltage_show >= 1)) {//If voltage is less than 130 & greater than 1 this will active:
    stepps = 2;
    digitalWrite(relay1, HIGH);
    digitalWrite(relay, HIGH);
  }
  if ((input_voltage_show <= 180) && (input_voltage_show >= 130)) {//If voltage is less than 180 & greater than 130 this will active:
    digitalWrite(relay, LOW); //in this steps transformer adds 50V
    digitalWrite(relay1, LOW);
    output_voltage_show = input_voltage_show +50; // adding voltage for showing on LCD output voltage
    stepps = 3;
  }
  if ((input_voltage_show <= 210) && (input_voltage_show >= 180)) {//If voltage is less than 210 & greater than 180 this will active:
    stepps = 4;
    if ((input_voltage_show <= 210) && (input_voltage_show >= 185)) {//If voltage is less than 210 & greater than 185 this will active: this gap of 5V acts as a difference when to trigger relay
      digitalWrite(relay, LOW); //in this steps transformer adds 20V
      digitalWrite(relay1, HIGH);
      output_voltage_show = input_voltage_show +20; // adding voltage for showing on LCD output voltage
    }
  }
  if ((input_voltage_show <= 245) && (input_voltage_show >= 210)) {//If voltage is less than 245 & greater than 210 this will active:
    stepps = 5;
    if ((input_voltage_show <= 245) && (input_voltage_show >= 215)) {//If voltage is less than 245 & greater than 215 this will active: this gap of 5V acts as a difference when to trigger relay
      digitalWrite(relay, HIGH);
      digitalWrite(relay1, HIGH);
      output_voltage_show = input_voltage_show;
    }
  }
  if ((input_voltage_show <= 300) && (input_voltage_show >= 245)) {//If voltage is less than 300 & greater than 245 this will active:
    stepps = 6;
    digitalWrite(relay, HIGH);
    digitalWrite(relay1, HIGH);
  }

  //end- stabelising code//

  /**********************************************************************************************************************************************************************/


  //fan on off codes start//

  if ((tempc <= 40) && (tempc >= 0)) { //if below 40 degree fan will turn off
    digitalWrite(fan, LOW);
    fanu = 1;
  }
  if ((tempc <= 300 ) && (tempc >= 50)) {//if above 50 degree fan will turn on
    digitalWrite(fan, HIGH);
    fanu = 2;
  }

  //fan on off codes end//

  /**********************************************************************************************************************************************************************/

  // voltage protection //
  if (digitalRead(protection) == HIGH) {
    prote = 1;
    if ((output_voltage_show <= 245) && (output_voltage_show >= 130)) {
      digitalWrite(relay2, HIGH);
    }
    else {
      digitalWrite(relay2, LOW);
    }
  }
  else {
    prote = 2;
    digitalWrite(relay2, HIGH);
  }
  // voltage protection //

  /**********************************************************************************************************************************************************************/

  //display enable code, shows the value on LCD//
  if (displayenable) {
    unsigned long displayMillis = millis();
    if (displayMillis - previousdMillis >= intervaldisplay) {
      // save the last time you Show
      previousdMillis = displayMillis;


      // if its is off turn it on and vice-versa:
      if (stabelizeroenable == 1) {


        digitalWrite(writevalue, LOW);
        lcd.begin(16, 2);

        //Print Voltage Input - start
        lcd.setCursor(0, 0);// set the cursor to column 0, line 1 // (note: line 1 is the second row, since counting begins with 0):
        lcd.print(">"); lcd.print(input_voltage_show, 0); lcd.print("V");
        //Print Voltage Input - end

        //print steps - start
        if (stepps == 1) {
          byte sinewavenoline[8] = {
            0b00000,
            0b01010,
            0b00100,
            0b01010,
            0b00010,
            0b10101,
            0b01000,
            0b00000
          };
          lcd.createChar(5, sinewavenoline);
          lcd.setCursor(6, 0);// set the cursor to column 6, line 0
          lcd.write((uint8_t)5);//lcd print
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("No Line! ");//lcd print
        }
        if (stepps == 2) {
          byte sinewavelow[8] = {
            0b00000,
            0b10010,
            0b10101,
            0b11010,
            0b00010,
            0b10101,
            0b01000,
            0b00000
          };
          lcd.createChar(4, sinewavelow);
          lcd.setCursor(6, 0);// set the cursor to column 6, line 0
          lcd.write((uint8_t)4);//lcd print
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("Low Volt!");//lcd print
        }
        if (stepps == 3) {
          byte sinewaveuplus[8] = {
            0b00001,
            0b00011,
            0b00111,
            0b00000,
            0b01000,
            0b10101,
            0b00010,
            0b00000
          };
          lcd.createChar(2, sinewaveuplus);
          lcd.setCursor(6, 0);// set the cursor to column 6, line 0
          lcd.write((uint8_t)2);//lcd print
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("<"); lcd.print(output_voltage_show, 0); lcd.print("V"); //lcd print
        }
        if (stepps == 4) {
          byte sinewaveup[8] = {
            0b00000,
            0b00010,
            0b00110,
            0b00000,
            0b01000,
            0b10101,
            0b00010,
            0b00000
          };
          lcd.createChar(1, sinewaveup);
          lcd.setCursor(6, 0);// set the cursor to column 6, line 0
          lcd.write((uint8_t)1);//lcd print
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("<"); lcd.print(output_voltage_show, 0); lcd.print("V"); //lcd print
        }
        if (stepps == 5) {
          byte sinewaveequal[8] = {
            0b00000,
            0b00000,
            0b00100,
            0b00000,
            0b01000,
            0b10101,
            0b00010,
            0b00000
          };
          lcd.createChar(10, sinewaveequal);
          lcd.setCursor(6, 0);// set the cursor to column 6, line 0
          lcd.write((uint8_t)10);//lcd print
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("<"); lcd.print(output_voltage_show, 0); lcd.print("V"); //lcd print
        }
        if (stepps == 6) {
          byte sinewavehigh[8] = {
            0b00000,
            0b10101,
            0b11101,
            0b10101,
            0b00010,
            0b10101,
            0b01000,
            0b00000
          };
          lcd.createChar(3, sinewavehigh);
          lcd.setCursor(6, 0);// set the cursor to column 6, line 0
          lcd.write((uint8_t)3);//lcd print
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("Hi Volt! "); //lcd print
        }
        //print steps - end

        //print protection start
        if (prote == 3) {
          lcd.setCursor(7, 0);// set the cursor to column 7, line 0
          lcd.print(" ");//lcd print
        }
        if (prote == 1) {
          byte proteconn[8] = {
            0b00000,
            0b11000,
            0b11000,
            0b10001,
            0b00010,
            0b10100,
            0b01000,
            0b00000
          };
          lcd.createChar(7, proteconn);
          lcd.setCursor(7, 0);// set the cursor to column 7, line 0
          lcd.write((uint8_t)7);//lcd print
        }
        if (prote == 2) {
          byte protecoff[8] = {
            0b00000,
            0b11000,
            0b11000,
            0b10101,
            0b00010,
            0b00101,
            0b01000,
            0b00000
          };
          lcd.createChar(8, protecoff);
          lcd.setCursor(7, 0);// set the cursor to column 7, line 0
          lcd.write((uint8_t)8);//lcd print
        }
        //print protection end

        //print fan off on start
        if (fanu == 1) {
          lcd.setCursor(8, 0);// set the cursor to column 8, line 0
          lcd.print(" ");//lcd print
        }
        if (fanu == 2) {
          lcd.setCursor(8, 0);// set the cursor to column 8, line 0
          lcd.print("*");//lcd print
        }

        //print fan off on end

        //print Temperature - start
        if ((tempc <= 99) && (tempc >= 0)) {
          lcd.setCursor(12, 0);
          lcd.print(" "); lcd.print(tempc, 0); lcd.print("C");
        }
        if ((tempc <= 300) && (tempc >= 99)) {
          lcd.setCursor(12, 0);
          lcd.print(tempc, 0); lcd.print("C");
        }
        //print Temperature - end

        //Printing Current Input - start
        lcd.setCursor(10, 1);// set the cursor to column 10, line 1
        lcd.print(watt_show, 3);
        lcd.setCursor(15, 1);// set the cursor to column 15, line 1
        lcd.print("W");//lcd print
        //Printing Current Input - start


        stabelizeroenable = 2;
      }
      else {

        //start- Current sensing code//
        current_sensor_read = sensor.getCurrentAC(current_frequency);
        amper =  current_sensor_read;
        watt_show = 0.85 * amper * output_voltage_show;
        //end- Current sensing code//


        stabelizeroenable = 1;
      }
    }
  }
  //display enable code, shows the value on LCD - End//

  /**********************************************************************************************************************************************************************/

  //start- Serial printing code//
  if (serialread) {
    Serial.print(" IN A Val " ); Serial.print(input_voltage_read);
    Serial.print(" - IN F Vol "); Serial.print(input_voltage_show);
    Serial.print(" - Temp "); Serial.print(tempc);
    Serial.print(" - Analogp"); Serial.print(digitalRead(protection));
    Serial.println();
  }
  //end- Serial printing code//

  delay(500);
}
