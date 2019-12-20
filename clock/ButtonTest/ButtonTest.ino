

void setup() {
  Serial.begin(9600);
  pinMode(50, INPUT);
  pinMode(52, INPUT);
  pinMode(46, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(50)==HIGH)
  {
    Serial.print("50");
  }
  if(digitalRead(52)== HIGH)
  {
    Serial.print("52");
  }
  if(digitalRead(46)== HIGH)
  {
    Serial.print("46");    
  }
}
