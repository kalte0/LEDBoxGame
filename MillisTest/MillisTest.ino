long timeThis, timeLast, timeLast2;

#define LED1 2
#define LED2 3

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

}

void loop() {
  timeThis = millis();

  if (timeThis - timeLast > 500) {
    digitalWrite(LED1, !(digitalRead(LED1)));
     timeLast = timeThis;  
     Serial.println("tick");

  }
  if (timeThis - timeLast2 > 250) {
    digitalWrite(LED2, !(digitalRead(LED2)));
     timeLast2 = timeThis; 
  }

 
}
