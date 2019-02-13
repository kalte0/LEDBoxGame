long timeThis, timeLast;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  timeThis = millis();
  if (timeThis - timeLast > 500) {
    Serial.println(digitalRead(4));
    timeLast = timeThis;
    digitalWrite(4, !(digitalRead(4)));
    digitalWrite(2, !(digitalRead(2)));
  }
  // if (timeThis - timeLast == 1000) {
  //   Serial.println("Second State");
  //   timeThis = timeLast;
  // }
}
