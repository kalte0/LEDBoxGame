void setup() {
  pinMode(6, INPUT);
  Serial.begin(9600); 
}

void loop() {
  if (digitalRead(6) == HIGH) {
Serial.println("HELLO WORLD"); 
delay(10); 
  }
}
