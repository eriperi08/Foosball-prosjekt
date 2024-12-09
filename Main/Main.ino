#include <Adafruit_NeoPixel.h>

// Definer antall LED-piksler
#define NUMPIXELS 14
#define PIN 23
int Goal_1 = 5;
int Goal_2 = 4;

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Slå av alle LED-er til å begynne med
}

void loop() {
  //Scoreboard
  visGoal_1(Goal_1);  // Velg tallet som skal vises
  strip.show(); // Vis tallet på 7-segment displayet

  visGoal_2(Goal_2);
  strip.show();
  
}
