#include "display.h"
#include <RTClib.h>
#include <SoftwareSerial.h>
#include "pitches.h"


#define CONNECT 128
#define INIT  64
#define FIN 32
#define Beat 11


//PHYSICAL DEVICES
RTC_DS1307 Clock;
Display lcd(0x27,2,1,0,4,5,6,7,3);
SoftwareSerial ACBT(13,12);


//LOCAL FLAGS AND VARIABLEs
DateTime alarmTime;
bool AlarmEnabled;
bool alarmOn;
bool SSon;
bool AlarmStopped;
int melody[] = {NOTE_C4, NOTE_FS4,NOTE_C4, NOTE_CS4 };
#define MELLEN 4
#define NOTEDURATION 250
int curNote;
#define speaker 8

///BUTTONS AND BUTTON STATES
#define upButton 50
#define downButton 52
#define control 46

#define upState 0
#define downState 1
#define controlState 2
bool buttonState [3];




/***
 * 
 * code Here
 * 
 * 
 * 
 * 
 * 
 */
void setup() {
  // put your setup code here, to run once:
  ACBT.begin(38400);
  lcd.begin();  
  Clock.begin();
  if(!Clock.isrunning())
  { // set to default time if rtc not battery powered
  Clock.adjust(DateTime(2018,12,22,12,00,00));
  }
  pinMode(upButton, INPUT);
  pinMode (downButton, INPUT);
  pinMode (control, INPUT);
  pinMode (speaker, OUTPUT);
  pinMode (Beat,INPUT);
  buttonState[upState]=false;
  buttonState [downState]=false;
  buttonState[controlState]=false;
  AlarmEnabled=false;
  alarmOn=false;
  SSon=false;
  alarmTime= DateTime(2018,12,22,12,00,00);
  AlarmStopped=false;
  Serial.begin(9600);
  
}




void loop() {
  // put your main code here, to run repeatedly:
 
  //updateDisplay with  current Time
  DateTime t=Clock.now();
  lcd.printTime(Clock.now(),SSon, AlarmEnabled);
  //start alarm if time
  if(AlarmEnabled && isAlarmTime(t)&& !alarmOn)
  {
    if(!AlarmStopped)
    {
      alarmOn=true;
      if(SSon)
        ACBT.write(INIT);
      curNote =0;
    }
  }
  else
  {
   AlarmStopped=false; 
  }
  if(alarmOn)
    alarmNote();
  if(alarmOn && SSon) 
  {
    if(digitalRead(Beat)==LOW)
    {
      SSon=false;
    }
    else if(ACBT.available())
    {
      int val=ACBT.read();
      if(val==FIN)
      {
        alarmOn=false;
        AlarmStopped=true; 
      }
    }
    
    
    
    //ensure connection
    // check to see if alarm can be shut off
    // if no connection shut ssOFF
    
  }
  
  if(digitalRead(control)==HIGH)
  {
    if(!buttonState[controlState])
    {
      buttonState[controlState]=true;
      // can only go into settings when the alarm is not currently going off
      if(!alarmOn)
        config(t);
      else if(!SSon) // if simon says is not going shut the alarm off
      {
        alarmOn=false;
        AlarmStopped=true; 
      }
    }
  }
  else
  {
    buttonState[controlState]=false;
  }
  

}
/***
 * 
 * HELPER FUNCTIONS
 * 
 * 
 */



void alarmNote(){
  tone(speaker, melody[curNote], NOTEDURATION);
  delay(NOTEDURATION);
  noTone(speaker);
  curNote= (curNote+1)%MELLEN;
}
/**
 * Function that checks if the current time is the alarm time
 */
bool isAlarmTime(DateTime cur)
{
   return  (cur.hour()==alarmTime.hour())&&(cur.minute()==alarmTime.minute());
}


/***
 * Function responible for handling updating graphics and configuring
 * settings, may update time/alarm time
 */
void config(DateTime t)
{
/// go into the configuration mode and handling turning alarm off in the case that Simon says is off
 
    //setting up value for config mode
    bool TimeChanged=false;
    byte tH=t.hour();
    byte tM=t.minute();
    char t_am_pm[3]="am";
    if(tH>=12)
    {
      if(tH!=12)
        tH-=12;
      t_am_pm[0]='p';
    }
    else if(tH==0){tH=12; }
    byte aH=alarmTime.hour();
    byte aM=alarmTime.minute();
    char a_am_pm[3]="am";
    if(aH>=12)
    {
      if(aH!=12)
        aH-=12;
      a_am_pm[0]='p';
    }
    else if(aH==0){aH=12; }
    // now entering the config mode
    int configstate=0;
    int configOptions;
    if(!AlarmEnabled)
    {
      configOptions=3;
    }
    else
    {
      configOptions=6;
    }

    /**
     * Config LOOP
     * 
     * 
     */
    
    while(configstate<configOptions)
    {
      lcd.DisplaySetup(tH,tM,t_am_pm,AlarmEnabled,aH,aM,a_am_pm,SSon,configstate);

      // case for handling the up button
      if(digitalRead(upButton)==HIGH&& !buttonState[upState] &&digitalRead(downButton)==LOW)
      {
        buttonState[upState]=true;

        if(configstate==0){
          if(tH==12)
            tH=1;
          else
          { 
            if(tH==11){ 
              if(t_am_pm[0]=='p')
                t_am_pm[0]='a';
              else
                 t_am_pm[0]='p';    
            }
            tH++;
          }
       } 
        else if(configstate==1){tM= (++tM)%60;}
        else if(configstate==2)
        {
          AlarmEnabled=!AlarmEnabled;
          if(AlarmEnabled){ configOptions=6; }
          else {configOptions=3;}         
        }
        else if(configstate==3){
           if(aH==12)
            aH=1;
          else
          { 
            if(aH==11){ 
              if(a_am_pm[0]=='p')
                a_am_pm[0]='a';
              else
                 a_am_pm[0]='p';    
            }
            aH++;
          }}
        else if(configstate==4){aM= (++aM)%60;}
        else if(configstate==5){ SSon=!SSon;  }
      }
      else
      {
        buttonState[upState]=false;
      }
      if(digitalRead(downButton)==HIGH&& !buttonState[downState] &&digitalRead(upButton)==LOW)
      {
        buttonState[downState]=true;
             if(configstate==0){   
               if(tH==1)
                tH=12;
               else
               { 
                 if(tH==12){ 
                  if(t_am_pm[0]=='p')
                    t_am_pm[0]='a';
                  else
                     t_am_pm[0]='p';    
                }
                tH--;
              }     
             } 
        else if(configstate==1){if(tM==0) tM=59; else tM--;}
        else if(configstate==2)
        {
          AlarmEnabled=!AlarmEnabled;
          if(AlarmEnabled){ configOptions=6; }
          else {configOptions=3;}         
        }
        else if(configstate==3){ if(aH==1)
                aH=12;
               else
               { 
                 if(aH==12){ 
                  if(a_am_pm[0]=='p')
                    a_am_pm[0]='a';
                  else
                     a_am_pm[0]='p';    
                }
                aH--;
              }   
          }
        else if(configstate==4){if(aM==0) aM=59; else aM--;}
        else if(configstate==5){ SSon=!SSon;  }
      }
      else
      {
        buttonState[downState]=false;
      }

      
      if(digitalRead(control)==HIGH&& !buttonState[controlState])
      {
        buttonState[controlState]=true;
        ++configstate;
      }
      else
      {
       buttonState[controlState]=false; 
      }
    }
    while(digitalRead(control)==HIGH);
    buttonState[controlState]=false;
     
    //update time if necessary
    if(t_am_pm[0]=='p')
    {
      if(tH<12)
      {
        tH+=12;
      }
    }
    else
    {
      if(tH==12)
      {
        tH=0;
      }
    }
     if(tH!=t.hour()|| tM!=t.minute())
      Clock.adjust(DateTime(2018,12,22,tH,tM,00));
    // can always update alarm time
       if(a_am_pm[0]=='p')
    {
      if(aH<12)
      {
        aH+=12;
      }
    }
    else
    {
      if(aH==12)
      {
        aH=0;
      }
    }
    alarmTime =DateTime(2018,12,22,aH,aM,00);
    AlarmStopped=false;
}
