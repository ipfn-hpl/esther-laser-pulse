#include <Arduino.h>
/*
*/

#define SWTCH_PIN PD2
#define LASER_PIN PD3
#define N_PULSES 4
#define MS_DELAY 10
#define US_DELAY 100
bool switch_on;

unsigned long nextTime = 0;
const long print_interval = 1000;
void setup() {
    // put your setup code here, to run once:
    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(SWTCH_PIN, INPUT_PULLUP);
    pinMode(LASER_PIN, OUTPUT);
    // Start serial port
    Serial.begin(115200);
    Serial.print("LED PIN ");
    Serial.print(LED_BUILTIN);
    Serial.println(". Hello Laser");
}
/*
   void laser_on() {
   pinMode(LASER_PIN, INPUT);
   digitalWrite(LED_BUILTIN, HIGH);
   }

   void laser_off() {
   pinMode(LASER_PIN, OUTPUT);
   digitalWrite(LASER_PIN, LOW);
   digitalWrite(LED_BUILTIN, LOW);
   }
   */
void slow_loop() {
    unsigned long now = millis();

    if(switch_on) {
        digitalWrite(LASER_PIN, LOW);
        digitalWrite(LED_BUILTIN, LOW);
    }
    else {
        digitalWrite(LASER_PIN, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
    }
    if ( now  > nextTime ) {
        nextTime = now + print_interval;
        if(digitalRead(SWTCH_PIN)) {
            switch_on = LOW;
            //switch_on = !digitalRead(SWTCH_PIN);
        }
        Serial.print("Laser: ");
        Serial.println(switch_on, DEC);
    }
}

void loop() {
    int i;
    unsigned long now = millis();
    if(!digitalRead(SWTCH_PIN)){
        switch_on = HIGH;
        //Serial.println("Laser ON");
        //nextTime = now + print_interval;
    }
    slow_loop();
    /*
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
    */
}

