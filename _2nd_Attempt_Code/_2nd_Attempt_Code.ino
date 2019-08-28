#include <avr/io.h>
#include <avr/interrupt.h>
#define JUMP 1
#define RUN 2
#define NOPE 3
#define FIX 4
#define OFF 5
#define OBSNUM 4 // # of obstacles
#define PINJUMP 3 // Here are the pins for Jump, Run frame 1, and rune frame 2.
#define PINRUN1 2
#define PINRUN2 4
#define CHECK 0x0540
#define set_bit(var, pin)    var |= 1<<(unsigned char) pin
#define clr_bit(var, pin)   var &= ~(1<<(unsigned char)pin)
#define test_bit(var, pin)    ((var & (1<<(unsigned char) pin)) >> pin)
#define toggle_bit(var, pin)  var ^= 1<<(unsigned char) pin
int tog;
int state;
int Obst = CHECK;

int main(void) {
  pinMode(PINJUMP, OUTPUT);  // Later change to registers etc.
  pinMode (PINRUN1, OUTPUT);
  pinMode(PINRUN2, OUTPUT);
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(PINRUN1, HIGH);
  digitalWrite(PINRUN2, LOW);
  Serial.begin(9600);
  state = OFF;
   TCCR0A = 0x00;
  sei();
  

  while (1) {

  }
}

