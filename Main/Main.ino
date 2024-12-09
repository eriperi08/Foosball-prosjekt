#include <Adafruit_NeoPixel.h>

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
