void buttons_setup(void)
{
  // Setup the first button with an internal pull-up :
  pinMode(BUTTON_PIN_1,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer1.attach(BUTTON_PIN_1);
  debouncer1.interval(5); // interval in ms
  
   // Setup the second button with an internal pull-up :
  pinMode(BUTTON_PIN_2,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer2.attach(BUTTON_PIN_2);
  debouncer2.interval(5); // interval in ms
  
     // Setup the third button with an internal pull-up :
  pinMode(BUTTON_PIN_3,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer3.attach(BUTTON_PIN_3);
  debouncer3.interval(5); // interval in ms
  
     // Setup the fourth button with an internal pull-up :
  pinMode(BUTTON_PIN_4,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer4.attach(BUTTON_PIN_4);
  debouncer4.interval(5); // interval in ms  
}

void check_buttons(void)
{
  // Update the Bounce instances :
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();
  debouncer4.update();  

  // Get the updated value :
  program = debouncer1.read();
  left = debouncer2.read();
  right = debouncer3.read();
  startstop = debouncer4.read();
  
  if (program == HIGH && left == HIGH && right == HIGH && startstop == HIGH)
  {
    if (previous_button == true)
    {
      previous_button = false;
    }
  }
  else
  {
    //PROGRAM BUTTON ---------------------------------------------------------------
    if (program == LOW && previous_button == false)
    {
      previous_button = true;
      if (standby)
      {  
        if (program_button_steps == 0)
        {
          program_change = true;
          byte *p;
          switch (program_mode)
          {
            case 0:
              p = EEPROM_read_alarm_on();                  
            break; 
            case 1:
              p = EEPROM_read_alarm_off();          
            break;
          } //switch (program_mode)
          lcd_print_program_header();          
          update_time_from_EEPROM(*(p+0), *(p+1), *(p+2)); 
          lcd_print_countdown_time();                
        } //if (program_button_steps == 0)
        ++program_button_steps;
        lcd_print_countdown_time();
        if (program_button_steps == 4) 
        {
          switch (program_mode)
          {
            case 0:
              EEPROM_write_alarm_on(hours, minutes, seconds);            
            break; 
            case 1:
              EEPROM_write_alarm_off(hours, minutes, seconds);
            break;
          } //switch (program_mode)          
          program_button_steps = 0;
          //lcd_print_mode_header();
          set_millis_from_EEPROM();
          program_change = false;
        }
      } //if (standby)
    } //if (program == LOW && previous_button == false)
    
    //LEFT BUTTON (RESET) (DOWN) ------------------------------------------------------------------
    if (left == LOW && previous_button == false)
    {
      previous_button = true;
      if (program_change)
      {
        switch (program_button_steps)
        {
          case 1:
            ++hours;
            if (hours == 24) hours = 0;
          break;
          case 2:
            ++minutes;
            if (minutes == 60) minutes = 0;
          break;
          case 3:
            ++seconds;
            if (seconds == 60) seconds = 0;
          break;
        }
        lcd_print_countdown_time();
      }
      else 
      {
        if (standby)
        {
          if (program_mode == 0 && mode_on_started) 
          {
            mode_on_started = false;
            set_millis_from_EEPROM();
          }
          if (program_mode == 1 && mode_off_started) 
          {
            mode_off_started = false;
            set_millis_from_EEPROM();
          }
        }
      }
    } //if (left == LOW && previous_button == false)  
  
    //RIGHT BUTTON (Select ON-Mode / OFF mode) (UP)------------------------------------------------------------------
    if (right == LOW && previous_button == false)
    {
      previous_button = true;
      if (program_change)
      {
        switch (program_button_steps)
        {
          case 1:
            --hours;
            if (hours == 255) hours = 23;
          break;
          case 2:
            --minutes;
            if (minutes == 255) minutes = 59;
          break;
          case 3:
            --seconds;
            if (seconds == 255) seconds = 59;
          break;          
        }
        lcd_print_countdown_time();        
      }
      else
      {
        if (standby)
        {
          if (program_mode == 0) //if program_mode = ON
          {
            program_mode = 1; // program_mode = OFF
          }
          else //if program_mode = OFF
          {
            program_mode = 0; // program_mode = ON
          }
          set_millis_from_EEPROM();
        } 
      }      
    } //if (right == LOW && previous_button == false)  
    
    //START-STOP BUTTON ------------------------------------------------------------------    
    if (startstop == LOW && previous_button == false)
    {
      previous_button = true;
      if (program_change)
      {
        program_button_steps = 0;
        set_millis_from_EEPROM();
        program_change = false;
      }
      else
      {
        if (standby)
        {
          standby = false;
          if (program_mode == 0)
          {
            if (mode_on_started) alarmTimeInMillis = auxMillis;
            else mode_off_started = false;
            digitalWrite(INTERRUPTOR_1, HIGH);
          }
          else
          {
            if (mode_off_started) alarmTimeInMillis = auxMillis;
            else mode_on_started = false; 
            digitalWrite(INTERRUPTOR_1, LOW);
          }
        }
        else
        {
          standby = true;
          if (program_mode == 0) mode_on_started = true; //If program_mode = On
          else mode_off_started = true;
          auxMillis = alarmTimeInMillis;
        }
        lcd_print_mode_header();
      } //if (!program_change)
    } //if (startstop == LOW && previous_button == false)     
  }    
}

