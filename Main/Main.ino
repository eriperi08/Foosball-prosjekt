#include <Funksjoner.h>

// Definer antall LED-piksler
#define Rod_Lys_Lag_1 26
#define Rod_Lys_Lag_2 27
#define Leser_For_Lag_1 25
#define Leser_For_Lag_2 14
bool Fikk_Lag_1_Mål = false;
bool Fikk_Lag_2_Mål = false;
int Lysnivå_Lag_1 = analogRead(Leser_For_Lag_1);
int Lysnivå_Lag_2 = analogRead(Leser_For_Lag_2);
 
void setup() {
  Serial.begin (115200);
  pinMode(Leser_For_Lag_1, INPUT);
  pinMode(Leser_For_Lag_2, INPUT);
  pinMode(Rod_Lys_Lag_1, OUTPUT);
  pinMode(Rod_Lys_Lag_2, OUTPUT);
  digitalWrite(Rod_Lys_Lag_1, HIGH);
  digitalWrite(Rod_Lys_Lag_2, HIGH);
  }
 
void loop() {
  if (Lysnivå_Lag_1 < 1000){
    Serial.print("Lag 1 fikk mål.");
    Fikk_Lag_1_Mål = true;
    for (int i = 0; i <= 5; i++){
    digitalWrite(Rod_Lys_Lag_1, LOW);
    delay(1000);
    digitalWrite(Rod_Lys_Lag_1, HIGH);
    delay(1000);
    }
  }
  if (Lysnivå_Lag_2 < 1000){
    Serial.print("Lag 2 fikk mål");
    Fikk_Lag_2_Mål = true;
    for (int i = 0; i <= 5; i++){
    digitalWrite(Rod_Lys_Lag_2, LOW);
    delay(1000);
    digitalWrite(Rod_Lys_Lag_2, HIGH);
    delay(1000);
    }
  }

///////////////////////////////////////////////////////////////////////////
////////////////////             animasjoner           ////////////////////
///////////////////////////////////////////////////////////////////////////

  if (KAMPSTART) {   
    StuffForStart(strip1, strip2, strip1.Color(255, 0, 0), strip1.Color(255, 128, 0), strip2.Color(0, 0, 255), strip2.Color(0, 128, 255), 50);
    strip1.clear();
    strip1.show();
    strip2.clear();
    strip2.show();
    bool KAMPSTART = false
  }
  
  if (Fikk_Lag_1_Mål) {
    colorWipe(strip1, strip1.Color(255, 0, 0), 25);
    colorWipe(strip1, strip1.Color(255, 128, 0), 25);
    colorWipe(strip1, strip1.Color(255, 255, 0), 25); 
    theaterChase(strip1, strip1.Color(255, 0, 0), strip1.Color(255, 128, 0), 50);
    for (int i = 0; i < 100; ++i) {
      bouncewave(strip1, strip1.Color(255, 255, 0), 50);
    }
    strip1.clear();
    strip1.show();
    Fikk_Lag_1_Mål = false
  }
  
  if (Fikk_Lag_2_Mål) {
    colorWipe(strip2, strip2.Color(0, 0, 255), 25);
    colorWipe(strip2, strip2.Color(0, 200, 255), 25);
    colorWipe(strip2, strip2.Color(0, 255, 255), 25);
    theaterChase(strip2, strip2.Color(0, 0, 255), strip2.Color(0, 128, 255), 50);
    for (int i = 0; i < 100; ++i) {
      bouncewave(strip2, strip2.Color(0, 255, 255), 50);
    }
    strip2.clear();
    strip2.show();
    Fikk_Lag_2_Mål = false
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
      bool LAG_1_VANT = false
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
      bool LAG_2_VANT = false
    }
  }
  // venter i et tiendels sekund, for å ikke overbelaste ESP32
  delay(100);
}
