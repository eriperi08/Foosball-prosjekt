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

float move = 0.0; //  verdier for funksjon
float animation = 0;
float fadeAnimation = 0;

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
