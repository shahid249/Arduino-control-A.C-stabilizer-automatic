/*Code by SHAHID
  How to make this stabilizer See the
  Instructables -> https://www.instructables.com/id/How-to-Make-AC-220-Volt-Automatic-Stabilizer-Using/

  This is 3 steps Automatic Voltage stabilizer
  1st steps is normal/output
  2nd steps adds 20voltage to output
  3rd steps adds 50voltage to output
*/
/***************************************************************************************************************/

// include the library code start
#include <ACS712.h> //ACS712 current sensor library | Download - https://github.com/shahid249/ACS712-Arduino-Library
#include <LiquidCrystal.h> // LCD library
// include the library code end

/***************************************************************************************************************/

//LCD pin Configuration - Start // do not edit
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//LCD pin Configuration - end // do not edit

/***************************************************************************************************************/

// Editable region - start //

     //Output pin setup - Start
     int relay = 7;  //relay1 pin
     int relay1 = 8; //relay2 pin
     int relay2 = 9; //relay3 pin
     int relay3 = 10;//relay4 pin
     int fan = 13;//Fan pin
     int displaywrite = 10; //LCD display write enable pin | pin use to control display write functin LOW mean write enable, HIGH mean write disable
     //Output pin setup - end

     //Setup temperature set point - start//
     float maxtemperature = 50; //fan will start when reach set point
     float minimumtempoerature = 42;//fan will stop when reach set point
     //Setup temperature set point - start//

     // Calibrating area - start//
     float temp_calibrate = 465; // use to calibrate temperature
     float ac_frequency = 50; // AC Voltage frequency 50/60hz
     float pF = 0.85; //Power factor
     float volatge_calibration = 0.3915348837209302; //use to calibrate voltage
       /*
        How to Calibrate / how to get this(0.3915348837209302) calibrate value
        To get calibrate value you need multimeter,
        Check the ac voltage using multimeter and then check the value of analog pin in arduino.
        If the ac voltage is 220 and the analog value is 655 then 220 / 655 = 0.3358778625954198 - you get the calibration value.
       */
     // Calibrating area - end//

     //Here you can set transfor steps voltage - Note this is 3 steps transformer - start//
           int steps_1 = 0; //start 0 voltage to show no line
      
           // Low voltage, In this voltage range power will be cut - start
           int steps_2_1 = 1; //if greater than 1V
           int steps_2_2 = 130; //if greater than 130V
           // Low voltage, In this voltage range power will be cut - end
      /* 
      Before adding transformer steps check the voltage of each steps, then set the desire voltage when to switch the steps
      example - if voltage is 180v step 3 will add up 50v = 230v
      */
       //transformer steps 3 start//
           // In this voltage range relay 2 will activate - start
           int steps_3_1 = 130; // if greater than 130V
           int steps_3_2 = 180; //if smaller than 180V
           int boost_addup_2 = 50; //The voltage addup in this steps by transformer. Use for displaying output voltage, your might be differnt
           // In this voltage range relay 2 will activate - end
       //transformer steps 3 end//

       //transformer steps 2 start//
           // In this voltage range relay 1 will activate, relay 2 will deactivate - start
           int step_4_1 = 180; // if greater than 180V
           int step_4_2 = 210; //if smaller than 210V
           int boost_addup_1 = 20;//The voltage addup in this steps by transformer. Use for displaying output voltage, your might be differnt
           // In this voltage range relay 1 will activate, 2 will deactivate - end
       //transformer steps 2 end//

       //transformer steps 1 start//
           // In this voltage range relay 1 will deactivate, relay 2 will deactivate cus we dont need boost voltage is ok - start
           int step_5_1 = 210;// if greater than 210V
           int step_5_2 = 235;// if smaller than 235V
           // In this voltage range relay 1 will deactivate, relay 2 will deactivate cus we dont need boost voltage is ok - end
       //transformer steps 1 end//

           // HIGH voltage, In this voltage range power will be cut  - start
           int step_6_1 = 235;// if greater than 235V
           int step_6_2 = 300;// if greater than 300V
           // HIGH voltage, In this voltage range power will be cut  - end
           

    //Here you can set transfor steps voltage - Note this is 3 steps transformer - end//

     //input pin setup - Start
     int voltagesens = A0; //voltage sensor input setup
     ACS712 sensor(ACS712_30A, A1); //current sens input setup. If your sensor is 30A-> ACS712_30A | 20A-> ACS712_20A | 5A-> ACS712_05B
     const int temperature = A2; //Temperature sensor input
     int protection = A3; // Protection awitch input
     //input pin setup - end

     //display mill setup - Start
     unsigned long displayMillis = 0;
     unsigned long previousdMillis = 0;
     const long intervaldisplay = 1000; //use to contol the refresh rate of display 1 sec = 2 second for refresh time.
     //display mill setup - end

// Editable region - end //

/***************************************************************************************************************/

//initialise setup - do not edit - Start//

//Default action -start
bool displayread = false; //Used for lcd
bool serialread = false; //Used for Serial
//Default action -end
//variable setup - Start  => Variables will change:
int displayreadenable = 1;
int stepps = 1;
int prote = 1;
int fanu = 1;
//variable setup - end
//Voltage
int a;
int outputvolt;
float n;
float o;
float p;
//Voltage
//Temperature
float tempc;  //variable to store temperature in degree Celsius
float tempf;  //variable to store temperature in Fahreinheit
float vout;  //temporary variable to hold sensor reading
//Temperature

//initialise setup - do not edit - end//

/***************************************************************************************************************/

void setup()
{
  sensor.calibrate(); // To calibrate ACS712
  Serial.begin(9600);// begin serial communication between arduino and pc
  pinMode(voltagesens, INPUT); // set pin as input pin
  pinMode(temperature, INPUT); // set pin as input pin
  pinMode(protection, INPUT);  // set pin as input pin
  pinMode(relay, OUTPUT);  //setup output
  pinMode(relay1, OUTPUT); //setup output
  pinMode(relay2, OUTPUT); //setup output
  pinMode(displaywrite, OUTPUT); //setup output
  pinMode(fan, OUTPUT);//setup output
  digitalWrite(relay, HIGH); //pull pin to high, relay use
  digitalWrite(relay1, HIGH);//pull pin to high, relay use
  digitalWrite(relay2, HIGH);//pull pin to high, relay use
  digitalWrite(displaywrite, LOW);//LCD write enable 
  lcd.begin(16, 2);// set up the LCD's number of columns and rows: // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);// set the cursor to column 0, line 1
  lcd.print("BOOTING...");//lcd print
  delay(100);//delay
}

/***************************************************************************************************************/

void loop()
{
/***************************************************************************************************************/  

  //start- Enable lcd display & Serial//
  displayread = true; //true mean lcd will display the value or false = blank.
  serialread = false; //true mean serial print on
  //end- Enable lcd display & Serial//

/***************************************************************************************************************/

  //start- temperature sensing code//
  vout = analogRead(temperature); //Read value from the pin
  vout = (vout * temp_calibrate) / 1023; //calculation of value to celsius
  tempc = vout; // Storing value in Degree Celsius
  tempf = (vout * 1.8) + 32; // Converting to Fahrenheit
  //end- temperature sensing code//

/***************************************************************************************************************/

  //start- Current sensing code//
  float U = outputvolt;//Calculated output voltage
  float I = sensor.getCurrentAC(ac_frequency);// setup the frequency
  float amper =  I;//Current in Amp
  float csew = pF * amper * U;//Convert to Watt
  //end- Current sensing code//

/***************************************************************************************************************/

  //start- Voltage sensing code//
  a = analogRead(voltagesens); // read analog values from pin A0
  o = (a * volatge_calibration);
  p = o / sqrt(1); //for calibrating purpose
  n = p;
  //end- Voltage sensing code//

/***************************************************************************************************************/

  //start- stabelising code//
  if (n >= steps_1) {//If voltage is less than 0 this will active:
    stepps = 1; // value which control LCD printing
    prote = 3; // value which control LCD printing
  }
  if ((n <= steps_2_2) && (n >= steps_2_1)) {//If voltage is less than 130 & greater than 1 this will active: change asper your transformer output
    stepps = 2;// value which control LCD printing
    if (digitalRead(protection) == HIGH) { //if protection switch is on this will activate
      prote = 1;// value which control LCD printing
      digitalWrite(relay1, HIGH);
      digitalWrite(relay, HIGH);
      digitalWrite(relay2, HIGH);
    }
    else {
      prote = 2;// value which control LCD printing
      digitalWrite(relay1, LOW);
      digitalWrite(relay, LOW);
    }
  }
  if ((n <= steps_3_2) && (n >= steps_3_1)) { //If voltage is less than 180 & greater than 130 this will active: change asper your transformer output
    outputvolt = n + boost_addup_2; //adding up the steps voltage with input voltage for output volatge for display
    digitalWrite(relay, LOW);
    digitalWrite(relay1, LOW);
    stepps = 3; // value which control LCD printing
    if (digitalRead(protection) == HIGH) { //if protection switch is on this will activate
      digitalWrite(relay2, LOW);
      prote = 1; // value which control LCD printing
    }
    else {
      digitalWrite(relay2, LOW);
      prote = 2;// value which control LCD printing
    }
  }
  if ((n <= step_4_2) && (n >= step_4_1)) {//If voltage is less than 210 & greater than 180 this will active: change asper your transformer output
    outputvolt = n + boost_addup_1;//adding up the steps voltage with input voltage for output volatge for display
    digitalWrite(relay, LOW);
    digitalWrite(relay1, HIGH);
    stepps = 4;// value which control LCD printing
    if (digitalRead(protection) == HIGH) { //if protection switch is on this will activate
      digitalWrite(relay2, LOW);
      prote = 1;// value which control LCD printing
    }
    else {
      digitalWrite(relay2, LOW);
      prote = 2;// value which control LCD printing
    }
  }
  if ((n <= step_5_2) && (n >= step_5_1)) {//If voltage is less than 235 & greater than 210 this will active:
    outputvolt = n;
    stepps = 5;// value which control LCD printing
    digitalWrite(relay, HIGH);
    digitalWrite(relay1, HIGH);
    if (digitalRead(protection) == HIGH) { //if protection switch is on this will activate
      digitalWrite(relay2, LOW);
      prote = 1;// value which control LCD printing
    }
    else {
      digitalWrite(relay2, LOW);
      prote = 2;// value which control LCD printing
    }
  }
  if ((n <= step_6_2) && (n >= step_6_1)) {//If voltage is less than 300 & greater than 235 this will active:
    stepps = 6;// value which control LCD printing
    if (digitalRead(protection) == HIGH) { //if protection switch is on this will activate
      digitalWrite(relay, HIGH);
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      prote = 1;// value which control LCD printing
    }
    else {
      digitalWrite(relay, HIGH);
      digitalWrite(relay1, LOW);
      prote = 2;// value which control LCD printing
    }
  }

  //end- stabelising code//

/***************************************************************************************************************/

  //fan on off codes start//

  if ((tempc <= minimumtempoerature) && (tempc >= 0)) {
    digitalWrite(fan, LOW);
    fanu = 1;// value which control LCD printing
  }
  if ((tempc <= 300 ) && (tempc >= maxtemperature)) {
    digitalWrite(fan, HIGH);
    fanu = 2;// value which control LCD printing
  }

  //fan on off codes end//

/***************************************************************************************************************/

  delay(500); //Delay

/***************************************************************************************************************/

  //start- Serial printing code//

  if (serialread) { // if true this will enable
    Serial.print(" Analog V " ); Serial.print(a);
    Serial.print(" - Sqrt Volt "); Serial.print(p);
    Serial.print(" - Final Volt "); Serial.print(n) ;
    Serial.print(String(" - C = ") + csew + " Watts");
    Serial.print(" - Temp "); Serial.print(tempc);
    Serial.print(" - Analogp"); Serial.print(digitalRead(protection));
    Serial.println();
  }
  //end- Serial printing code//

/***************************************************************************************************************/

  //start- display printing code//

  if (displayread) { // if true this will enable
    unsigned long displayMillis = millis();

    if (displayMillis - previousdMillis >= intervaldisplay) {
      // save the last time you Show
      previousdMillis = displayMillis;


      // if its is off turn it on and vice-versa:
      if (displayreadenable == 1) {

        digitalWrite(displaywrite, LOW); //LCD write enable
        delay(100);//delay
        lcd.begin(16, 2);//setup LCD

        //Print Voltage Input - start
        lcd.setCursor(0, 0);// set the cursor to column 0, line 0
        lcd.print(">"); lcd.print(n, 0); lcd.print("V");
        //Print Voltage Input - end


        //print steps - start
        if (stepps == 1) {
          byte sinewavenoline[8] = {//custom char -> Generate from here - https://omerk.github.io/lcdchargen/
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
          lcd.write((uint8_t)5);//lcd print custom char
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("No Line! ");//lcd print
        }
        if (stepps == 2) {
          byte sinewavelow[8] = {//custom char -> Generate from here - https://omerk.github.io/lcdchargen/
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
          lcd.write((uint8_t)4);//lcd print custom char
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("Low Volt!");//lcd print
        }
        if (stepps == 3) {
          byte sinewaveuplus[8] = {//custom char -> Generate from here - https://omerk.github.io/lcdchargen/
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
          lcd.write((uint8_t)2);//lcd print custom char
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("<"); lcd.print(outputvolt); lcd.print("V"); //lcd print
        }
        if (stepps == 4) {
          byte sinewaveup[8] = {//custom char -> Generate from here - https://omerk.github.io/lcdchargen/
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
          lcd.write((uint8_t)1);//lcd print custom char
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("<"); lcd.print(outputvolt); lcd.print("V"); //lcd print
        }
        if (stepps == 5) {
          byte sinewaveequal[8] = {//custom char -> Generate from here - https://omerk.github.io/lcdchargen/
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
          lcd.write((uint8_t)10);//lcd print custom char
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("<"); lcd.print(outputvolt); lcd.print("V"); //lcd print
        }
        if (stepps == 6) {
          byte sinewavehigh[8] = {//custom char -> Generate from here - https://omerk.github.io/lcdchargen/
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
          lcd.write((uint8_t)3);//lcd print custom char
          lcd.setCursor(0, 1);// set the cursor to column 0, line 1
          lcd.print("Hi Volt! "); //lcd print
        }
        //print steps - end

        //print protection start
        if (prote == 1) {
          byte proteconn[8] = {//custom char -> Generate from here - https://omerk.github.io/lcdchargen/
            0b00000,
            0b00000,
            0b00001,
            0b00010,
            0b10100,
            0b01000,
            0b00000,
            0b00000
          };
          lcd.createChar(7, proteconn);
          lcd.setCursor(7, 0);// set the cursor to column 7, line 0
          lcd.write((uint8_t)7);//lcd print custom char
        }
        if (prote == 2) {
          byte protecoff[8] = {//custom char -> Generate from here - https://omerk.github.io/lcdchargen/
            0b00000,
            0b00000,
            0b00000,
            0b01010,
            0b00100,
            0b01010,
            0b00000,
            0b00000
          };
          lcd.createChar(8, protecoff);
          lcd.setCursor(7, 0);// set the cursor to column 7, line 0
          lcd.write((uint8_t)8);//lcd print custom char
        }
        //print protection end

        //print fan off on start
        if (fanu == 1) {
          lcd.setCursor(8, 0);// set the cursor to column 13, line 1
          lcd.print(" ");//lcd print // blank out the area
        }
        if (fanu == 2) {
          lcd.setCursor(8, 0);// set the cursor to column 13, line 1
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
        lcd.print(csew, 3);
        lcd.setCursor(15, 1);// set the cursor to column 15, line 1
        lcd.print("W");//lcd print
        //Printing Current Input - start

        displayreadenable = 2;
      }


      else {
        displayreadenable = 1;
      }
    }
  }

  //end- display printing code//

}
