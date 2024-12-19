#include <Adafruit_NeoPixel.h>

// Definer antall LED-piksler
#define NUMPIXELS 14
#define PIN 23
int Goal_1 = 5;
int Goal_2 = 4;

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT2, LED_PIN2, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Slå av alle LED-er til å begynne med
    // bestemmer hastigheten på signalet
  Serial.begin (115200);
  // bestemmer hvor ESP32 skal ta imot signal for begge lagene
  pinMode(Leser_For_Lag_1, INPUT);
  pinMode(Leser_For_Lag_2, INPUT);
  
  strip1.begin();          
  strip1.show();            
  strip1.setBrightness(BRIGHTNESS); 
  strip2.begin();          
  strip2.show();            
  strip2.setBrightness(BRIGHTNESS);
}

void loop() {
  //Scoreboard
  visGoal_1(Goal_1);  // Velg tallet som skal vises
  strip.show(); // Vis tallet på 7-segment displayet

  visGoal_2(Goal_2);
  strip.show();

// Marius sin del
// leser verdien til Leser_For_Lag_1/2, samt lagrer de i variablene Lysnivå_Lag_1/2
int Lysnivå_Lag_1 = analogRead(Leser_For_Lag_1);
int Lysnivå_Lag_2 = analogRead(Leser_For_Lag_2);

// hvis lyset til Lysnivå_Lag_1 er under tusen er det mål
if (Lysnivå_Lag_1 < 1000){
  Serial.print("Lag 1 fikk mål.");
  Fikk_Lag_1_Mål = true;
  delay(5000);
  Fikk_Lag_1_Mål = false;
  }
// hvis lyset til Lysnivå_Lag_2 er under 300, er det mål
if (Lysnivå_Lag_2 < 300){
  Serial.print("Lag 2 fikk mål");
  Fikk_Lag_2_Mål = true;
  delay(5000);
  Fikk_Lag_2_Mål = false;
  }

// printer verdien til Lysnivå_Lag_1/2 hver gang den looper
Serial.print("Lag_1 = "); 
Serial.println(Lysnivå_Lag_1);
Serial.print("Lag_2 = ");
Serial.println(Lysnivå_Lag_2);
// Marius sin del slutt

// Aktiveringskode for animasjoner
  bool Fikk_LAG_1_MÅL = true;  // to be removed once all code is put together
  bool Fikk_LAG_2_MÅL = false; // same
  bool KAMPSTART = false; //same
  bool LAG_1_VANT = false; //same
  bool LAG_2_VANT = false; //same

  if (KAMPSTART) {   
    StuffForStart(strip1, strip2, strip1.Color(255, 0, 0), strip1.Color(255, 128, 0), strip2.Color(0, 0, 255), strip2.Color(0, 128, 255), 50);
    strip1.clear();
    strip1.show();
    strip2.clear();
    strip2.show();
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
    }
  }
  // venter i et tiendels sekund, for å ikke overbelaste ESP32
  delay(100);
}
