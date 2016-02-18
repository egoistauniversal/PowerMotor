/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */
 
// include the library code:
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
boolean lcd_block_blank = false;

#include <EEPROM.h>

#include <Bounce2.h>
#define BUTTON_PIN_1 14
#define BUTTON_PIN_2 15
#define BUTTON_PIN_3 16
#define BUTTON_PIN_4 17

#define INTERRUPTOR_1 19

#define COUNTDOWN_INTERVAL 1000UL
#define BLINK_INTERVAL 500UL

// Instantiate a Bounce object
Bounce debouncer1 = Bounce(); 
Bounce debouncer2 = Bounce(); 
Bounce debouncer3 = Bounce(); 
Bounce debouncer4 = Bounce(); 

//Buttons
int program;
int left;
int right;
int startstop;

boolean block_is_blank = false; // Use for program_mode_temporizador()
byte program_button_steps = 0;
boolean previous_button = false;
boolean standby = true;
byte program_mode = 0; //Mode ON = 0, mode OFF = 1
boolean program_change = false; //Program button Activated/Deactivated
boolean mode_on_started = false;
boolean mode_off_started = false;

//Countdown variables
unsigned long alarmTimeInMillis;
unsigned long auxMillis;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

byte hours, minutes, seconds;

//--------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);
  pinMode(INTERRUPTOR_1, OUTPUT);
  //EEPROM_clear();
  EEPROM_setup();
  buttons_setup();
  lcd_setup();
}

void loop()
{
  check_buttons(); 
  if (!standby) countdown();
  else 
  {
    if (program_button_steps != 0) 
    {
      program_mode_temporizador();
    }
  }
}

//--------------------program_mode_temporizador-----------------

void program_mode_temporizador()
{
   // Get snapshot of time
   currentMillis = millis();
   if ((currentMillis - previousMillis) >= BLINK_INTERVAL)
   {
     // Use the snapshot to set track time until next event
     previousMillis = currentMillis;   
     if (block_is_blank)
     {
       switch (program_button_steps)
       {
         case 1:
           lcd_print_block(program_button_steps, hours);         
         break;
         
         case 2:
           lcd_print_block(program_button_steps, minutes);          
         break;
         
         case 3:
           lcd_print_block(program_button_steps, seconds);
         break;
       }
     }
     else
     {
       lcd_clear_block(program_button_steps);
     } //if (block_is_blank)
   } //if (countdown_millis >= 500UL)
}
    
//--------------------countdown-----------------

void countdown()
{
   // Get snapshot of time
   currentMillis = millis();
   // How much time has passed, accounting for rollover with subtraction!
   if ((currentMillis - previousMillis) >= COUNTDOWN_INTERVAL)
   {
     // Use the snapshot to set track time until next event
     previousMillis = currentMillis;     
     alarmTimeInMillis = alarmTimeInMillis - 1000UL;
     if (alarmTimeInMillis >= 100000000UL)
     { 
    
       mode_on_started = false;
       mode_off_started = false;
       if (program_mode == 0) 
       {
         program_mode = 1; // program_mode = OFF
         digitalWrite(INTERRUPTOR_1, LOW);
       }
       else 
       {
         program_mode = 0; // program_mode = ON 
         digitalWrite(INTERRUPTOR_1, HIGH);
       }
       set_millis_from_EEPROM();
     }
     update_time_from_millis(alarmTimeInMillis);  
     lcd_print_countdown_time(); 
   }
}

//--------------------update_time_from_millis-----------------

void update_time_from_millis(unsigned long milliseconds)
{
   hours = milliseconds / 3600000;
   minutes = (milliseconds % 3600000) / 60000;
   seconds = ((milliseconds % 3600000) % 60000) / 1000;  
}




