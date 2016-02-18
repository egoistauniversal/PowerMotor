void lcd_setup(void)
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  set_millis_from_EEPROM();
}

//---------------------------------------------lcd_clear_line--------------------------------------

void lcd_clear_line(byte line)
{
  int i;
  for (i=0; i<16; i++)
  {
    lcd.setCursor(i, line);
    lcd.print(" ");
  }
}

//---------------------------------------------lcd_print_mode_header---------------------------------

void lcd_print_mode_header()
{
  lcd_clear_line(0);
  lcd.setCursor(0, 0);
  switch (program_mode)
  {
    case 0:
      lcd.print("On");      
      break;
      
    case 1:
      lcd.print("Off");
      break;  
  }
  if (standby) lcd.print(" Standby");
}

void lcd_print_program_header()
{
  lcd_clear_line(0);
  lcd.setCursor(0,0);
  switch (program_mode)
  {
    case 0:
      lcd.print("Set Program On");
      break;
     
    case 1:
      lcd.print("Set Program Off");
      break;
  }
}

//-----------------------------------------lcd_print_countdown_time---------------------------------

void lcd_print_countdown_time(void)
{
  lcd.setCursor(0, 1);
  
  lcd_print_digits(hours);
  lcd.print(":");
  
  lcd_print_digits(minutes);
  lcd.print(":");
  
  lcd_print_digits(seconds);
}

//--------------------------------------lcd_print_digits--------------------------

void lcd_print_digits(byte value)
{
   if (value < 10) lcd.print("0");
   lcd.print(value);
}

//------------------------------lcd_clear_block
void lcd_clear_block(byte block)
{
  switch (block)
  {
    case 1:
      lcd.setCursor(0, 1);
    break;
    case 2:
      lcd.setCursor(3, 1);
    break;
    case 3:
      lcd.setCursor(6, 1);
    break;   
  }
  lcd.print("  ");
  block_is_blank = true;
}

//----------------------------------lcd_print_block
void lcd_print_block(byte block, byte num)
{
  switch (block)
  {
    case 1:
      lcd.setCursor(0, 1);
    break;
    case 2:
      lcd.setCursor(3, 1);
    break;
    case 3:
      lcd.setCursor(6, 1);
    break;   
  }
  lcd_print_digits(num);
  block_is_blank = false;
}
