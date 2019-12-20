#include <Metro.h>

#include <SoftwareSerial.h>

#define CONNECT 128
#define INIT  64
#define FIN 32

#define NUMBUTTONS 5
#define button1 13
#define Led1 12
#define button2 4
#define Led2 5
#define button3 6      
#define Led3 7
#define button4 8
#define Led4 9
#define button5 10
#define Led5 11

#define MaxLen 5
#define MinLen 3
#define Step 1
const byte Buttons[] = {button1, button2, button3, button4, button5};
const byte Leds[] = {Led1, Led2, Led3, Led4, Led5};
bool state[NUMBUTTONS];

byte sequence [MaxLen];
byte curlen;


SoftwareSerial SSBT(3,2);
#define BEAT 15



void setup() {
  // put your setup code here, to run once:
  SSBT.begin(38400);
  // declaring values for
  for (int i = 0; i < NUMBUTTONS; i++) {
    pinMode(Leds[i], OUTPUT);
    digitalWrite(Leds[i], LOW);
    pinMode(Buttons[i], INPUT);
    state[i] = false;
  }
  pinMode(BEAT,INPUT);
  curlen = MinLen;
  randomSeed(analogRead(0));
  Serial.begin(9600);
  Serial.print("started");
}

  

void loop() {
  // put your main code here, to run repeatedly:
  bool timeForAlarm=false;
  Serial.print("Here");
  //check to see if time for the alarm to go off
  if(SSBT.available() )
  {
    byte val =SSBT.read();
    Serial.print(val);
    if (val ==INIT)
    timeForAlarm=true;
  }
  if(timeForAlarm){
    
  for (bool start = false; !start;) // wait for user to start game
  {
      PlayAll();
      for (int i = 0; i < NUMBUTTONS; ++i)
      {
        if (digitalRead(Buttons[i])==HIGH) {
          start = true;
          break;
          
        }
      }
    }
    Serial.println("out of start");
    //iterates through all the rounds
    curlen=MinLen;
    while (curlen < MaxLen)
    {
      Generate(sequence, curlen); // make a new sequence for the round
      PlaySeq(sequence, curlen); // intially play the sequence
      if(!PlayRound(sequence, curlen)) // wait for the round to finish, if the round failed we can terminate
          break;
      PlayAll();
      Serial.println("round done");
      curlen+=Step;
    }
    if(curlen==MaxLen)
        SSBT.write(FIN);
    timeForAlarm=false;
  }
  
}


void Generate( byte newSeq[], byte len)
{
  for (byte i = 0; i < len; ++i)
  {
    newSeq[i] = random(NUMBUTTONS);
  }
}

/*
   plays the current sequence of a round
*/
void PlaySeq( byte Seq [], byte len)
{
  for (int i = 0; i < len; ++i)
  {
    digitalWrite(Leds[Seq[i]], HIGH);
    delay(1000);
    digitalWrite(Leds[Seq[i]], LOW);
    delay(500);
  }

}
/*
   light up all LEDs to indicate the start of a round
*/
void PlayAll()
{
  for (int i = 0; i < NUMBUTTONS; ++i)
  {
    digitalWrite(Leds[i], HIGH);
  }
  delay(500);
  for (int i = 0; i < NUMBUTTONS; ++i)
  {
    digitalWrite(Leds[i], LOW);
  }
  delay(250);
}


/**
 * Function play Round Continues To 
 */

bool PlayRound(byte seq [], byte len)
{
  bool Connected =false; // assume we are still connected
  for (byte index = 0; index < len; )
  {
    if(digitalRead(BEAT)==LOW){
        return false;
    }
    for (byte i = 0; i < NUMBUTTONS; ++i) //checking to see if a button has been pressed
    {
      

      if (digitalRead(Buttons[i]) == HIGH) {
        if (!state[i]) // makes sure to not recheck a button that was already pressed
        {
          state[i]=true;
          digitalWrite(Leds[i], HIGH);
          if (seq[index] == i) { //the case that the correct button was pressed
            ++index;
            break;
          }
          else // user pressed wrongButton
          {
            for (byte i = 0; i < NUMBUTTONS; ++i) // reset all states Leds
            {
              digitalWrite(Leds[i], LOW);
              state[i] = false;
            }
            index = 0; // reset the index
            PlaySeq(seq, len); // replay the sequence
            break; // start over
          }
        }
      }
      else if (state[i]) // resets state and shuts LED off if button was released
      {
        digitalWrite(Leds[i], LOW);
        state[i] = false;
      }
    }
  }
  return true;
}
