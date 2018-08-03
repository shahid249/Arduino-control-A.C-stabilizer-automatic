# Arduino-control-A.C-stabilizer-automatic {Read This Important}
Automatic voltage stabilizer with arduino that will show AC voltage, watt, steps, 
transformer temperature &amp; auto fan on-off on LCD display

Introduction of codes, & what it will do

It will monitor AC voltage through pin A0 of arduino and control which relay to be active in that time. 
Example -> If arduino get 199VAC then it will active relay 1 which will increase voltage to 219VAC. 
In brief if voltage is less than 210 and also greater than 180 it will active relay 1 which will boost 20V, 
If voltage is greater than 210 and less than 230 it will deactivate relay 1.

The monitoring AC voltage will also display on LCD, 
and it also display output voltage by adding steps voltage to input voltage which will show the output voltage. 
Note:- output voltage is less accurate when more load is connect because there is no sensor on output voltage.

ACS712 module sense how much current is drawn from output, 
then arduino will calculate in watt and show in to the LCD display.

It also monitor temperature of the transformer, 
if temperature is getting high of the set point, it will turn on the fan.

Make this stabilizer Follow Instructables -> https://www.instructables.com/id/How-to-Make-AC-220-Volt-Automatic-Stabilizer-Using/


# Actual file description and programming

//ac-stabilizer-arduino-complex-process-no-delay-used-for-relay.ino//
This ino file program is written in complex method, user can only change the variable that is declare. This ino file is missing delay function, which is used to trigger relay in specif voltage difference. Although Its is safe but as there is no voltage difference, relay will trigger abnormally if voltage is hanging in between trigger voltage.

//ac-stabilizer-arduino-easy-with-safe-delay-function.ino//
This ino file program is written in easy method, users have to change value in function as well as in variable. This ino file is contain delay function, which is used to trigger relay in specif voltage difference. And it much safer and protects current spikes and fluctuation from rapid relay trigger.
