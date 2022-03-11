#include <Arduino.h>
/*
*/

#define SWTCH_PIN PD2
#define LASER_PIN PD3
#define N_PULSES 4
#define MS_DELAY 10
#define US_DELAY 100
bool switch_on;

void setup() {
  // put your setup code here, to run once:
    // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SWTCH_PIN, INPUT_PULLUP);
  pinMode(LASER_PIN, OUTPUT);
    // Start serial port
  Serial.begin(115200);
  Serial.println("Hello Laser");
}

void laser_on() {
    pinMode(LASER_PIN, INPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void laser_off() {
    pinMode(LASER_PIN, OUTPUT);
    digitalWrite(LASER_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
}

void slow_loop() {
  unsigned long now = millis();
  static unsigned long nextTime = 0;
  const long print_interval = 1000;

  if ( now  > nextTime ) {
    if(digitalRead(SWTCH_PIN))
      switch_on = LOW;

    nextTime = now + print_interval;
    switch_on = !digitalRead(SWTCH_PIN);

    Serial.println("Laser:");
    Serial.println(switch_on, DEC);
  }
}

void loop() {
  int i;
  if(!digitalRead(SWTCH_PIN))
    switch_on = HIGH;

  if(switch_on){
    for (i = N_PULSES; i >0; i--){
      laser_on();
      delayMicroseconds(US_DELAY);
      //delay(MS_DELAY);
      laser_off();
      delayMicroseconds(US_DELAY);
      //delay(MS_DELAY);
    }
  }
  else {
    laser_off();
  }
  slow_loop();
}

