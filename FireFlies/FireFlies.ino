/*
 Fade
 
 This example shows how to fade an LED on pin 9
 using the analogWrite() function.
 
 This example code is in the public domain.
 */

const int LEDNUM = 6;
const int SLEEPINTERVAL = 1;

int leds[] = {3, 5, 6, 9, 10, 11};
int brightness[LEDNUM];
int fade[LEDNUM];
int sleep[LEDNUM];

// the setup routine runs once when you press reset:
void setup()  { 
  randomSeed(analogRead(0));
  for (int i = 0; i < LEDNUM; i++) {
    pinMode(leds[i], OUTPUT);
    brightness[i] = 0;
    fade[i] = 15;
    sleep[i] = random(10);
  }
} 

// the loop routine runs over and over again forever:
void loop()  {
  for (int i = 0; i < LEDNUM; i++)
  {
    if (sleep[i] > 0) {
      sleep[i] = sleep[i] - SLEEPINTERVAL;
    } else {
      brightness[i] = brightness[i] + fade[i];
      if (brightness[i] < 0) {
        brightness[i] = 0;
        sleep[i] = random(1000);
        fade[i] = -fade[i];
      }
      if (brightness[i] > 254) {
        brightness[i] = 254;
        fade[i] = -fade[i];
      }
      analogWrite(leds[i], brightness[i]);
    }
  }
  // wait for 30 milliseconds to see the dimming effect    
  delay(30);                            
}

