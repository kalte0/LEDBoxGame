#define JUMP 1
#define RUN 2
#define NOPE 3
#define FIX 4
#define OFF 5
#define OBSNUM 4 //How many obstacles?

#define FROGGY 2
#define OBSTPIN1 3
#define OBSTPIN2 4
#define OBSTPIN3 5

long timeThis, timeLast;
long timeLast2;
long timeLast3;
long timeLast4;// for millis
int tog; // toggle used during animation.
int state; // the variable for changing states
int jump = 3;
int run1 = 2;
int run2 = 4;

int Check[10] = {0, 0, 0, 0, 1, 0, 0, 1, 0, 1};
int Obst[10] = {0, 0, 0, 0, 1, 0, 0, 1, 0, 1}; //The course values, 0 no light, 1 light.
void setup() {
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(run1, OUTPUT);
  digitalWrite(run1, HIGH);
  pinMode(jump, OUTPUT);
  pinMode(run2, OUTPUT);
  digitalWrite(run2, LOW);
  pinMode(5, INPUT); // input for jump
  Serial.begin(9600);
  state = OFF;
}

void loop() {
  timeThis = millis();
  if (digitalRead(jump) == LOW && digitalRead(6) == HIGH) {
    Serial.println("You failed :( try again?");
    state = OFF;
    //}
    delay(50);
  if (state != OFF) { // if the game is not off, start loading up obstacles
    if (timeThis - timeLast > 500) {
      timeLast = timeThis;
      for (int i = 0; i < OBSNUM; i++) {
        int o = i + 6; // used later to write to pins 6 - 9 on the board.
        int val = Obst[i];
        if (val == 0) { // if and else statement check the read value from array and assign HIGH or LOW value accordingly.
          digitalWrite(o, LOW);
        }
        else if (val == 1) {
          digitalWrite(o, HIGH);
        }
      }
      // Here I am shifting over the Array 1 value to the right. SHIFT
      int lastObst = Obst[9];
      for (int x = 9; x > 0; x--) {
        Obst[x] = Obst[x - 1];
      }
      Obst[0] = lastObst;
    }
  }
  // THIS OPTIONAL CODE iS TO PRINT THE VALUES.
  /*
    for (int p = 9; p > 0; p--) {
    Serial.print("Value of  "); Serial.print(p); Serial.print('\t'); Serial.println(Obst[p]);
    delay(10);
    }//
  */

  //Serial.print("state:");
  //Serial.print(state);
  switch (state) {
    case JUMP:
      digitalWrite(run2, LOW); // run animation off before jump begin
      digitalWrite(run1, LOW);
      digitalWrite(jump, HIGH);
      if (timeThis - timeLast2 > 750) {
        timeLast2 = timeThis;
        digitalWrite(jump, LOW);
        digitalWrite(run2, HIGH);
        state = NOPE; // after jump plays, go back to run so that the player can not float above the obstacles.
      }
      break;

    case FIX:
      timeLast2 = timeThis;
      state = JUMP;
      break;

    case RUN:
      if (timeThis - timeLast2 > 500) { // a half second loop
        timeLast2 = timeThis;
        digitalWrite(run2, !(digitalRead(run2))); // makes it become the opposite state
        digitalWrite(run1, !(digitalRead(run1))); // made sure to be alternating
      }
      if (digitalRead(5) == HIGH) { // checks for the button press
        delay (10);
        state = FIX;
      }
      break;

    case NOPE: // buffer state, because
      timeThis = millis();
      if (timeThis - timeLast2 > 500) {
        Serial.println(digitalRead(4));
        timeLast2 = timeThis;
        digitalWrite(4, !(digitalRead(4)));
        digitalWrite(2, !(digitalRead(2)));
      }
      if (digitalRead(5) == LOW) { // checks for the button press
        delay (10);
        state = RUN;
      }
      break;

    case OFF:
      for (int k = 2; k < 10; k++) {
        digitalWrite(k, LOW);
      }
     for (int z = 0; z < 10; z++) { // reset the Obstacle array to match the check array, basically what it was before the game started. 
      Obst[z] = Check[z]; 
     }
      if (digitalRead(5) == HIGH) {
        state = RUN;
      }
      break;
  }

  }
}
