#include <Adafruit_NeoPixel.h>
#include <funksjoner.h>

void setup() {
  Serial.begin(115200); // Start Serial for å motta input
  strip.begin();
  strip.show(); // Slå av alle LED-er til å begynne med
  strip1.show();
  KAMPSTART = true;
  }

void loop() {
  Lysnivå_Lag_1 = analogRead(Leser_For_Lag_1);
  Lysnivå_Lag_2 = analogRead(Leser_For_Lag_2);
  visGoal_1(Goal_1);
  visGoal_2(Goal_2);
  strip.show();
  Serial.println(Lysnivå_Lag_1);
  Serial.println(Lysnivå_Lag_2);

  if (KAMPSTART) {   
    StuffForStart(strip1, strip2, strip1.Color(255, 0, 0), strip1.Color(255, 128, 0), strip2.Color(0, 0, 255), strip2.Color(0, 128, 255), 50);
    strip1.clear();
    strip1.show();
    strip2.clear();
    strip2.show();
    Serial.println("hjelp");
    KAMPSTART = false;
  }

 if (Lysnivå_Lag_1 < 1000) {
    Serial.println("Lag 1 fikk mål.");
    Goal_1 = Goal_1 + 1;
    visGoal_1(Goal_1);
    strip.show();
    Fikk_Lag_1_Mål = true;
    delay(100);
  } 

  if (Lysnivå_Lag_2 < 2000){
    Serial.print("Lag 2 fikk mål");
    Goal_2 = Goal_2 + 1;
    visGoal_2(Goal_2);
    strip.show();
    Fikk_Lag_2_Mål = true;
    delay(100);
  }
  if (Fikk_Lag_1_Mål) {
    Serial.println("Kjører animasjon for Lag1");
    colorWipe(strip1, strip1.Color(255, 0, 0), 25);
    colorWipe(strip1, strip1.Color(255, 128, 0), 25);
    colorWipe(strip1, strip1.Color(255, 255, 0), 25); 
    strip1.clear();
    strip1.show();
    Fikk_Lag_1_Mål = false;
    delay(8000);
  }
    if (Fikk_Lag_2_Mål) {
    Serial.println("Kjører animasjon for Lag2");
    colorWipe2(strip2, strip1.Color(255, 0, 0), 25);
    colorWipe2(strip2, strip1.Color(255, 128, 0), 25);
    colorWipe2(strip2, strip1.Color(255, 255, 0), 25); 
    strip2.clear();
    strip2.show();
    Fikk_Lag_2_Mål = false;
    delay(8000);
  }
    if (Goal_1=10){
    LAG_1_VANT = true;
  }
    if (Goal_2=10){
    LAG_2_VANT = true;
  }
}
  if (LAG_1_VANT) {
    for (int i = 0; i < 10; ++i) { // Blink all LEDs on both strips 
      for (int j = 0; j < LED_COUNT; ++j) { 
        strip1.setPixelColor(j, strip1.Color(255, 0, 0)); 
        strip2.setPixelColor(j, strip2.Color(255, 0, 0)); 
        } 
      strip1.show(); 
      strip2.show(); 
      delay(500);  
      for (int j = 0; j < LED_COUNT; ++j) {
        strip1.setPixelColor(j, strip1.Color(0, 0, 0)); 
        strip2.setPixelColor(j, strip2.Color(0, 0, 0));  
      } 
      strip1.show();
      strip2.show();
      delay(500); 
      LAG_1_VANT = false
    }
  }
  if (LAG_2_VANT) {
    for (int i = 0; i < 10; ++i) { // Blink all LEDs on both strips 
      for (int j = 0; j < LED_COUNT; ++j) { 
        strip1.setPixelColor(j, strip1.Color(0, 0, 255)); 
        strip2.setPixelColor(j, strip2.Color(0, 0, 255)); 
      } 
      strip1.show(); 
      strip2.show(); 
      delay(500);  
      for (int j = 0; j < LED_COUNT; ++j) {
        strip1.setPixelColor(j, strip1.Color(0, 0, 0)); 
        strip2.setPixelColor(j, strip2.Color(0, 0, 0));  
      } 
      strip1.show();
      strip2.show();
      delay(500);
      LAG_2_VANT = false
    }
  }
