
#ifndef DISPLAY_H
#define DISPLAY_H

#if ARDUINO >= 100
    #include "Arduino.h"
#else 
   #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif   
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include "Arduino.h"
class Display{
  
  private:
  LiquidCrystal_I2C lcd;
  void printNum(int collumn, int NumberToprint);
  int LastDisplay;
  public:
  Display(byte adress, byte arg1, byte arg2, byte arg3, byte arg4,byte arg5, byte arg6, byte arg7, byte arg8);  
  void begin(void);
  void printTime(DateTime Time,bool SS,bool alOn);
  void DisplaySetup(int timeH, int timeM , char*  am_pm ,bool alramOn, int AlarmH, 
                            int AlarmM , char * Al_am_pm,bool SSOn , int index);

};




#endif /* DISPLAY_H */
