/*
 * Laser  4W 
 * Laser lens pitch 2 turns / mm
 * Lens Pos./ mm  |  focal dist. / mm (distance from laser body
 *   2 mm         |   25 (? datasheet?)
 *
 *   1 mm         |   40 
 *   0.75         |   43 
 *   0.50         |   50
 *   0.25         |   53
 *   0.00         |   60

*/

#include <Arduino.h>
#include <Debouncer.h>

#define SWTCH_PIN PD5
#define CW_SWTCH_PIN PD4
#define LASER_PWM SCL
//#define FLASHT_PIN PD4 // TRIAC Flash B24
#define N_PULSES 1
#define PULSE_WIDTH 200000 // in us . 4 W * 0.1 s = 400 mJ
//#define MS_DELAY 10
//  (1 us = 10 mm @10km/s)
// #define US_DELAY 30 // ~30 cm
bool switch_on;

bool laser_cw = false;
unsigned long laserCycle = 0;
// unsigned long laserPwmOff = 0;

const unsigned long laserPwmPeriod = 1000; // 1000us ->f=kHz  PWM 0-5KHz , 20us ->
const unsigned long laserPwmDC = 50; // 5 % 

int rise_count = 0;
int fall_count = 0;

unsigned long nextTime = 0;
unsigned long holdTime = 0;
const long print_interval = 2000;

const int db_duration_ms = 50;

// default
Debouncer debouncer(SWTCH_PIN, db_duration_ms);
Debouncer cw_debouncer(CW_SWTCH_PIN, db_duration_ms);

void cw_laser( unsigned long now_us) {
    if (laser_cw) {
        if ( now_us > laserCycle) {
            laserCycle  = now_us + laserPwmPeriod ;
            //laserPwmOff = now_us + laserPwmDC ;
            digitalWrite(LED_BUILTIN, HIGH);
            digitalWrite(LASER_PWM, HIGH);
            delayMicroseconds(PULSE_WIDTH);
            digitalWrite(LASER_PWM, LOW);
            digitalWrite(LED_BUILTIN, LOW);
        }
        /*
        else if (now_us > laserPwmOff) {
            digitalWrite(LASER_PWM, LOW);
            digitalWrite(LED_BUILTIN, LOW);
        }
        */
    }
    else {
        digitalWrite(LASER_PWM, LOW);
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void laser_pulse() {

    digitalWrite(LED_BUILTIN, HIGH);
    // 50 % D.C.
    for(int i = 0; i < N_PULSES; i++) {
        digitalWrite(LASER_PWM, HIGH);
        delayMicroseconds(PULSE_WIDTH);
        digitalWrite(LASER_PWM, LOW);
        delayMicroseconds(PULSE_WIDTH);
    }
    digitalWrite(LED_BUILTIN, LOW);
}

void onPulseFall(const int state) {
    laser_pulse();
    Serial.print("onPulseFall: ");
    Serial.println(fall_count++);
}

void onCwFall(const int state) {
    laser_cw = true;
    Serial.println("oncWFall: ");
}

void onCwRise(const int state) {
    laser_cw = false;
    Serial.println("onCwRise: ");
}

void setup() {
    // put your setup code here, to run once:
    // initialize LED digital pin as an output.
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    pinMode(LASER_PWM, OUTPUT);
    digitalWrite(LASER_PWM, LOW);
    pinMode(SWTCH_PIN, INPUT_PULLUP);
    pinMode(CW_SWTCH_PIN, INPUT_PULLUP);
    debouncer.subscribe(Debouncer::Edge::FALL, onPulseFall);
    cw_debouncer.subscribe(Debouncer::Edge::FALL, onCwFall);
    cw_debouncer.subscribe(Debouncer::Edge::RISE, onCwRise);
//    pinMode(FLASHT_PIN, OUTPUT);
//    digitalWrite(FLASHT_PIN, LOW);
    // Start serial port
    Serial.begin(115200);
    Serial.print(". Hello Laser. ");
    Serial.print(N_PULSES);
    Serial.print(" LED PULSES:");
    Serial.print(PULSE_WIDTH/1000.0);
    Serial.println(" ms");
//    Serial.print("LED PIN ");
//    Serial.print(LED_BUILTIN);
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

    if ( now  > nextTime ) {
        nextTime = now + print_interval;
        Serial.print("Laser: ");
        Serial.println(switch_on, DEC);
    }
}

void loop() {
    //int i;
    debouncer.update();
    cw_debouncer.update();
    //unsigned long now = millis();
    unsigned long us = micros();

    cw_laser(us);

    //trigger_loop();
    //slow_loop();
}

