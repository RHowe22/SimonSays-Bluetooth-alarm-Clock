

#include "display.h"
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <stdio.h>
#define NumSize 12
#define numRow 4
#define numCol 3
 
/** 
 *  This section is used for defining the custom characters for large Numbers
 */
 byte full[8] ={
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

 byte tophalf [8] ={
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};
 byte botthalf [8] ={
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111
};

 byte diagonal [8] = {
  B00001,
  B00011,
  B00011,
  B00111,
  B00111,
  B01111,
  B01111,
  B11111
};

 byte empty [8] ={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

/**
 * This section is responsible for keeping track of what custom chars
 * are needed to print a number
 */
const byte Zero [NumSize] = {0, 0, 0, 
                             0, 3, 0, 
                             0, 3, 0,
                             0, 0, 0};
                                
const byte One [NumSize] = {3, 4, 0,
                               3, 3, 0, 
                               3, 3, 0, 
                               3, 3, 0};
                               
const byte Two [NumSize] = {0, 0, 0,
                            2, 2, 0, 
                            0, 1, 1,
                            0, 0, 0};

const byte Three [NumSize] = {0, 0, 0,
                              2, 2, 0,                               
                              1, 1, 0,
                              0, 0, 0};

const byte Four [NumSize] =  { 0, 3, 0,
                               0, 2, 0,
                               1, 1, 0,
                               3, 3, 0 };
                                  
const byte Five [NumSize] = { 0, 0, 0,
                              0, 2, 2,
                              1, 1, 0,
                              0, 0, 0};
                                  
const byte Six [NumSize] = { 0, 1, 1,
                             0, 2, 2,
                             0, 3, 0,
                             0, 0, 0};
                               
const byte Seven [NumSize] = {  0, 0, 0,
                                3, 3, 0,
                                3, 3, 0,
                                3, 3, 0};
                                   
const byte Eight [NumSize] = { 0,1,0,
                               0,2,0,
                               0,1,0,
                               0,2,0 };
                                  
const byte Nine [NumSize] = { 0, 1, 0,
                              0, 2, 0,
                              1, 1, 0,
                              3, 3, 0 };
                                 
const byte * numTable [10]= {Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine};       


// value for keeping Display clean
#define CONFIG 1
#define CLOCK 0
Display::Display(byte address,  byte arg1, byte arg2, byte arg3, byte arg4,byte arg5, byte arg6, byte arg7, byte arg8)
      : lcd(address, arg1, arg2, arg3,  arg4, arg5, arg6, arg7,arg8, POSITIVE)
{
 
}
void Display::begin()
{
  lcd.begin(20,4);
  lcd.backlight();
  lcd.createChar(0, full);
  lcd.createChar(1, tophalf);
  lcd.createChar(2, botthalf);
  lcd.createChar(3, empty);
  lcd.createChar(4, diagonal);
}

/** Function to display the current time onto the lcd;
 *  Param Time is a DateTime Object that stores the time to display
 *  Param SS is boolean to indicate to display if Simon Says mode is enabled
 */
void Display::printTime(DateTime Time,bool SS, bool alOn){
  // clear the display
  if(LastDisplay== CONFIG)
  {
    lcd.setCursor(0,0);
    lcd.print("                    ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print("                    ");
    lcd.setCursor(0,0);
    LastDisplay= CLOCK;
  }


  
  int h = Time.hour();
  bool AM = (h)<12 ;
  
  if(h>12)
    h-=12;
  if(h==0)
    h=12;  
  int m =Time.minute();
  //print hour
  printNum(0,h/10);
  // leave a padding of 1 collumn
  printNum(4,h%10);
  // print time divider
  lcd.setCursor(7,1);
  lcd.write(byte(1));
  lcd.setCursor(7,2);
  lcd.write(byte(1));
  printNum(9, m/10);
  printNum(13, m%10);
  // if simon Says is enabled display that info
 if(alOn)
 {    
    lcd.setCursor(18,0);
    if(SS)
    {
      lcd.print("SS");
    }
    else
    {
      lcd.print("Al");
    }
 }
  lcd.setCursor(18,2);
  if(AM)
  {
    lcd.print("AM");
  }
  else
  {
    lcd.print("PM");
  }
}


void Display::DisplaySetup(int timeH, int timeM , char* am_pm ,bool alarmOn, int AlarmH, 
                            int AlarmM , char * Al_am_pm,bool SSOn , int index)
{
  char buff[20];
  // clear the display
  if(LastDisplay==CLOCK)
  {
    lcd.setCursor(0,0);
    lcd.print("                    ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print("                    ");
    LastDisplay=CONFIG;
  } 
  
  lcd.setCursor(0,0);
  sprintf(buff,"set Time: %02d:%02d %s",timeH,timeM, am_pm); 
  lcd.print(buff);

  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,1);
  if(alarmOn){
    lcd.print("Alarm: ON");
    lcd.setCursor(0,2);
    sprintf(buff,"set Alarm: %02d:%02d %s",AlarmH,AlarmM, Al_am_pm); 
    lcd.print(buff);
    lcd.setCursor(0,3);
    if(SSOn)
    {
     lcd.print("Simon Says Mode: On");
    }
    else
     {
     lcd.print("Simon Says Mode: Off");
    }
  }
  else
  {
    lcd.print("Alarm: OFF");
  }
  delay(200);
  //update for blinking display
  if(index ==0)
  {
    lcd.setCursor(0,0);
    sprintf(buff,"set Time:   :%02d %s",timeM, am_pm); 
    lcd.print(buff);
  }
  else if(index ==1){
     lcd.setCursor(0,0);
     sprintf(buff,"set Time: %02d:   %s",timeH, am_pm); 
     lcd.print(buff);
  }
  else if(index ==2)
  {
     lcd.setCursor(0,1);
     lcd.print("Alarm:    ");
  }
  else if(index ==3)
  {
    lcd.setCursor(0,2);
    sprintf(buff,"set Alarm:   :%02d %s",AlarmM, Al_am_pm); 
    lcd.print(buff);
  }
  else if(index ==4)
  {
    lcd.setCursor(0,2);
    sprintf(buff,"set Alarm: %02d:   %s",AlarmH, Al_am_pm); 
    lcd.print(buff);
  }
  else if(  index ==5)
  {
      lcd.setCursor(0,3);
      lcd.print("Simon Says Mode:    ");
  }
  delay(200);

}

/** Function printNum is used to print a number using the custom characters
 *  Param collumn represent from which collumn to start printing the number from
 *  Param NumberToPrint is the number we wish to represent using the cutom defined characters
 */
void Display::printNum(int collumn, int NumberToPrint)
{
  int col;
  int row;
  const byte * Printchar = numTable[NumberToPrint];
  for(int i =0; i <NumSize; ++i)
  {
    row= i/numCol;
    col = collumn+(i%numCol);
    lcd.setCursor(col,row);
    lcd.write(byte(Printchar[i]));
  }
}
