#include <Funksjoner.h>

// Definer antall LED-piksler
#define NUMPIXELS 14
#define PIN 23
int Goal_1 = 0;
int Goal_2 = 0;
bool KAMPSTART = false;
Fikk_Lag_1_Mål = false
Fikk_Lag_2_Mål = false
bool LAG_1_VANT = false;
bool LAG_2_VANT = false;

Adafruit_NeoPixel strip1(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(NUMPIXELS, LED_PIN2, NEO_GRB + NEO_KHZ800);

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
  if (Serial.available() > 1) {
    char input1 = Serial.read(); // Les første karakter
    char input2 = Serial.read(); // Les andre karakter

    // Valider at begge inputene er tall
    if (input1 >= '0' && input1 <= '9' && input2 >= '0' && input2 <= '9') {
      Goal_1 = input1 - '0'; // Konverter fra ASCII til tall
      Goal_2 = input2 - '0'; // Konverter fra ASCII til tall

      Serial.print("Viser Goal_1: ");
      Serial.print(Goal_1);
      Serial.print(" og Goal_2: ");
      Serial.println(Goal_2);

      // Vis tallene på displayene
      visGoal_1(Goal_1);
      visGoal_2(Goal_2);
      strip.show();
    } else {
      Serial.println("Ugyldig input! Send to tall mellom 0 og 9 som 'XY'.");
    }
  }
  //Scoreboard
  visGoal_1(Goal_1);  // Velg tallet som skal vises
  visGoal_2(Goal_2);
  strip.show();
   // leser verdien til Leser_For_Lag_1/2, samt lagrer de i variablene Lysnivå_Lag_1/2
  int Lysnivå_Lag_1 = analogRead(Leser_For_Lag_1);
  int Lysnivå_Lag_2 = analogRead(Leser_For_Lag_2);

  // hvis lyset til Lysnivå_Lag_1 er under tusen er det mål
  if (Lysnivå_Lag_1 < 800){
    Serial.print("Lag 1 fikk mål.");
    Goal_1 +=1;
    visGoal_1(Goal_1);
    Fikk_Lag_1_Mål = true;
  }
  // hvis lyset til Lysnivå_Lag_2 er under 300, er det mål
  if (Lysnivå_Lag_2 < 800){
    Serial.print("Lag 2 fikk mål");
    Goal_2 +=1;
    visGoal_2(Goal_2);
    Fikk_Lag_2_Mål = true;
  }

  // printer verdien til Lysnivå_Lag_1/2 hver gang den looper
  Serial.print("Lag_1 = "); 
  Serial.println(Lysnivå_Lag_1);
  Serial.print("Lag_2 = ");
  Serial.println(Lysnivå_Lag_2);
  Serial.print("Viser Goal_1: ");
  Serial.print(Goal_1);
  Serial.print(" og Goal_2: ");
  Serial.println(Goal_2);

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
