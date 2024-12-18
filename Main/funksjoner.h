#include <Adafruit_NeoPixel.h>

// Bestemmer at Leser_For_Lag_1 skal være på pin 12
#define Leser_For_Lag_1 12
// Bestemmer at Leser_For_Lag_2 skal være på pin 14
#define Leser_For_Lag_2 14
// Disse boolene sier at Lag 1/2 ikke har fått mål enda
bool Fikk_Lag_1_Mål = false;
bool Fikk_Lag_2_Mål = false;
#define LED_PIN 27 //ledpins og antall definisjon, kan endres
#define LiED_PIN2 19
#define LED_COUNT  50
#define LED_COUNT2  50
#define BRIGHTNESS 255

Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT2, LED_PIN2, NEO_GRB + NEO_KHZ800);

float move = 0.0; //  verdier for funksjon
float animation = 0;
float fadeAnimation = 0;


// gjør dette foralltid
void loop() {
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
  
  if (Fikk_LAG_1_MÅL) {
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
  
  if (Fikk_LAG_2_MÅL) {
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

//Scoreboard

// Definer segmentkonfigurasjoner for tall (0-9)
// Segmentene er organisert i denne rekkefølgen: {A, B, C, D, E, F, G}
const bool digitSegments[10][7] = {
  {1, 1, 1, 0, 1, 1, 1}, // 0
  {0, 0, 1, 0, 1, 0, 0}, // 1
  {0, 1, 1, 1, 0, 1, 1}, // 2
  {0, 1, 1, 1, 1, 1, 0}, // 3
  {1, 0, 1, 1, 1, 0, 0}, // 4
  {1, 1, 0, 1, 1, 1, 0}, // 5
  {1, 1, 0, 1, 1, 1, 1}, // 6
  {0, 1, 1, 0, 1, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 1, 0, 0}  // 9
};

// Funksjon for å sette opp lysdiodene for et tall
void visGoal_1(int tall) {
  if (tall < 0 || tall > 9) return; // Valider at tallet er mellom 0 og 9

  for (int i = 0; i < 7; i++) {
    if (digitSegments[tall][i]) {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); // Rødt lys for segmentene som skal være på
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));   // Slå av segmentene som ikke er i bruk
    }
  }
}
void visGoal_2(int tall) {
  if (tall < 0 || tall > 9) return; // Valider at tallet er mellom 0 og 9

  for (int i = 6; i < 14; i++) {
    if (digitSegments[tall][i-7]) {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); // Rødt lys for segmentene som skal være på
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));   // Slå av segmentene som ikke er i bruk
    }
  }
}

///////////////////////////////////////////////////////////////////////////
////////////////////   Forskjellige animasjoner/deler  ////////////////////
///////////////////////////////////////////////////////////////////////////
void colorWipe(Adafruit_NeoPixel& strip, uint32_t color, int wait) {
  int center = strip.numPixels() / 2;
  
  for(int i = 0; i <= center; i++) {
    strip.setPixelColor(center + i, color);  
    strip.setPixelColor(center - i, color);
    strip.show();                    
    delay(30);                     
  }
}

void theaterChase(Adafruit_NeoPixel& strip, uint32_t color1, uint32_t color2, int wait) { 
  int center = strip.numPixels() / 2;
  for(int a = 0; a < 10; a++) {
    for(int b = 0; b < 3; b++) {
      strip.clear();
      for(int c = 0; c <= center; c++) {
        if ((c + b) % 3 == 0) {
          strip.setPixelColor(center + c, color1);
          strip.setPixelColor(center - c, color1);
        } else {
          strip.setPixelColor(center + c, color2);
          strip.setPixelColor(center - c, color2);
        }
      }
      strip.show();
      delay(wait);
    }
  }
}

void StuffForStart(Adafruit_NeoPixel& strip1, Adafruit_NeoPixel& strip2, uint32_t color1_1, uint32_t color1_2, uint32_t color2_1, uint32_t color2_2, int wait) {
  
  for (int i = 0; i < 3; ++i) { 
    for (int j = 0; j < LED_COUNT; ++j) { 
      strip1.setPixelColor(j, strip1.Color(255, 0, 0)); 
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

  int center1 = strip1.numPixels() / 2;
  int center2 = strip2.numPixels() / 2;

  for(int a = 0; a < 10; a++) {
    for(int b = 0; b < 3; b++) {
      strip1.clear();
      strip2.clear();
      for(int c = 0; c <= center1; c++) {
        if ((c + b) % 3 == 0) {
          strip1.setPixelColor(center1 + c, color1_1);
          strip1.setPixelColor(center1 - c, color1_1);
        } else {
          strip1.setPixelColor(center1 + c, color1_2);
          strip1.setPixelColor(center1 - c, color1_2);
        }
      }
      for(int c = 0; c <= center2; c++) {
        if ((c + b) % 3 == 0) {
          strip2.setPixelColor(center2 + c, color2_1);
          strip2.setPixelColor(center2 - c, color2_1);
        } else {
          strip2.setPixelColor(center2 + c, color2_2);
          strip2.setPixelColor(center2 - c, color2_2);
        }
      }
      strip1.show();
      strip2.show();
      delay(wait);
    }
  }
}

void bouncewave(Adafruit_NeoPixel& strip, uint32_t color, int wait){
  
  for (int i = 0; i < strip.numPixels(); ++i) { 
    float hue = i * (255.0 / strip.numPixels()); 
    hue += sin(move) * strip.numPixels(); 
    float brightness = i + sin(move) * 20; 
    brightness = sin(brightness); 
    brightness = (brightness + 1) / 2; 
    brightness *= 255; 
    uint32_t color = strip.Color((int)hue, 255, (int)brightness); 
    strip.setPixelColor(i, color);
  }

  strip.show();
  move += 0.08; 
  delay(40); 
}
