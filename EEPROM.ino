void EEPROM_clear(void)
{
  EEPROM_write_alarm_on(0, 0, 0);
  EEPROM_write_alarm_off(0, 0, 0);
}

boolean EEPROM_setup(void)
{
  byte *p;
  
  p = EEPROM_read_alarm_on();
  if(*(p+0) == 0 && *(p+1) == 0 && *(p+2) == 0) EEPROM_write_alarm_on(0, 5, 0);
 
  p = EEPROM_read_alarm_off();
  if(*(p+0) == 0 && *(p+1) == 0 && *(p+2) == 0) EEPROM_write_alarm_off(0, 1, 0);
}

byte * EEPROM_read_alarm_on(void)
{
  byte i;
  static byte r[3];
  
  for (i=0; i<3; ++i)
  {
    r[i] = EEPROM.read(i);
  }
  return r;
}

byte * EEPROM_read_alarm_off(void)
{
  byte i;
  static byte r[3];
  
  for (i=0; i<3; ++i)
  {
    r[i] = EEPROM.read(i+3);
  }
  return r;
}

void EEPROM_write_alarm_on(byte hours, byte minutes, byte seconds)
{
  EEPROM.write(0, hours);
  EEPROM.write(1, minutes);
  EEPROM.write(2, seconds);
}

void EEPROM_write_alarm_off(byte hours, byte minutes, byte seconds)
{
  EEPROM.write(3, hours);
  EEPROM.write(4, minutes);
  EEPROM.write(5, seconds);  
}

void set_millis_from_EEPROM()
{
  byte *p;

  switch (program_mode)
  {
   case 0:
     if (mode_on_started) update_time_from_millis(auxMillis);
     else
     {
       p = EEPROM_read_alarm_on();
       alarmTimeInMillis = ((*(p+0) * 3600000) + (*(p+1) * 60000) + (*(p+2) * 1000));
       update_time_from_EEPROM(*(p+0), *(p+1), *(p+2));        
     }
     break;
      
   case 1:
     if (mode_off_started) update_time_from_millis(auxMillis);
     else
     {
       p = EEPROM_read_alarm_off();
       alarmTimeInMillis = ((*(p+0) * 3600000) + (*(p+1) * 60000) + (*(p+2) * 1000));
       update_time_from_EEPROM(*(p+0), *(p+1), *(p+2));  
     }      
     break;      
  }
  lcd_print_mode_header();
  lcd_print_countdown_time(); 
}

void update_time_from_EEPROM(byte h, byte m, byte s)
{
  hours = h;
  minutes = m;
  seconds = s;
}

