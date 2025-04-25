#define PIN0 2 // Pin til scoreboard
#define NUMPIXELS0 14 // Antall pixler på scoreboard
#define PIN1 25 // Pin til scoreboardside-strip
#define NUMPIXELS1 42 // Antall pixler på scoreboardside-strip
#define PIN2 26 // Pin til den andre stripen
#define NUMPIXELS2 38 // Antall pixler på den andre stripen

#define Leser_For_Lag_1 33 // Pin til lyssensor på scoreboardside
#define Leser_For_Lag_2 14 // Pin til den andre lyssensoren

bool Fikk_Lag_1_Mål = false;
bool Fikk_Lag_2_Mål = false;
bool KAMPSTART = false;
bool LAG_1_VANT = false;
bool LAG_2_VANT = false;

int Goal_1 = 0;
int Goal_2 = 0;

Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(NUMPIXELS0, PIN0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUMPIXELS1, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUMPIXELS2, PIN2, NEO_GRB + NEO_KHZ800);

/* Scoreboard kode */ 

const bool digitSegments[10][7] = {
  {1, 1, 1, 0, 1, 1, 1}, // 0
  {1, 0, 0, 0, 0, 0, 1}, // 1
  {1, 1, 0, 1, 1, 1, 0}, // 2
  {1, 1, 0, 1, 0, 1, 1 }, // 3
  {1, 0, 1, 1, 0, 0, 1}, // 4
  {0, 1, 1, 1, 0, 1, 1}, // 5
  {0, 1, 1, 1, 1, 1, 1}, // 6
  {1, 1, 0, 0, 0, 0, 1}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};


// Funksjon for å vise et tall på den første 7-segment (0–6)
void visGoal_1(int Goal_1) {
  if (Goal_1 < 0 || Goal_1 > 9) return; // Valider at tallet er mellom 0 og 9
    for (int i = 0; i < 7; i++) {
      if (digitSegments[Goal_1][i]) {
      strip0.setPixelColor(i, strip0.Color(255, 0, 0)); // Rødt lys for segmentene som skal være på
    } 
      else {
      strip0.setPixelColor(i, strip0.Color(0, 0, 0));   // Slå av segmentene som ikke er i bruk
    }
  }
}

// Funksjon for å vise et tall på den andre 7-segment (7–13)
void visGoal_2(int Goal_2) {
  if (Goal_2 < 0 || Goal_2 > 9) return; // Valider at tallet er mellom 0 og 9
    for (int i = 7; i < 14; i++) {
      if (digitSegments[Goal_2][i - 7]) {
      strip0.setPixelColor(i, strip0.Color(0, 0, 255)); // Rødt lys for segmentene som skal være på
    } 
      else {
      strip0.setPixelColor(i, strip0.Color(0, 0, 0));   // Slå av segmentene som ikke er i bruk
    }
  }
}

/* Animasjoner */

void colorWipe(Adafruit_NeoPixel& strip1, uint32_t color, int wait) {
  int center = NUMPIXELS1 / 2;
  
  for(int i = 0; i <= center; i++) {
    strip1.setPixelColor(center + i, color);  
    strip1.setPixelColor(center - i, color);
    strip1.show();                    
    delay(30);                     
  }
}

void Blink(Adafruit_NeoPixel& strip, Adafruit_NeoPixel& strip2, uint32_t color1_1, uint32_t color1_2, uint32_t color2_1, uint32_t color2_2, int wait) {
  
  for (int i = 0; i < 2; ++i) { 
    for (int j = 0; j < NUMPIXELS1; ++j) { 
      strip1.setPixelColor(j, strip1.Color(255, 0, 0)); 
      strip2.setPixelColor(j, strip2.Color(0, 0, 255)); 
      } 
    strip1.show(); 
    strip2.show(); 
    delay(500);  
    for (int j = 0; j < NUMPIXELS1; ++j) {
      strip1.setPixelColor(j, strip1.Color(0, 0, 0)); 
      strip2.setPixelColor(j, strip2.Color(0, 0, 0));  
    } 
    strip1.show();
    strip2.show();
    delay(500); 
  }
}

void WinBlink(Adafruit_NeoPixel& strip, Adafruit_NeoPixel& strip2, uint32_t color1_1, uint32_t color1_2, uint32_t color2_1, uint32_t color2_2, int wait) {
  
  for (int i = 0; i < 3; ++i) { 
    for (int j = 0; j < NUMPIXELS1; ++j) { 
      strip1.setPixelColor(j, strip1.Color(255, 0, 0)); 
      strip2.setPixelColor(j, strip2.Color(255, 0, 0)); 
      } 
    strip1.show(); 
    strip2.show(); 
    delay(500);  
    for (int j = 0; j < NUMPIXELS1; ++j) {
      strip1.setPixelColor(j, strip1.Color(0, 0, 0)); 
      strip2.setPixelColor(j, strip2.Color(0, 0, 0));  
    } 
    strip1.show();
    strip2.show();
    delay(500); 
  }
}
void WinBlink2(Adafruit_NeoPixel& strip1, Adafruit_NeoPixel& strip2, uint32_t color1_1, uint32_t color1_2, uint32_t color2_1, uint32_t color2_2, int wait) {
  
  for (int i = 0; i < 3; ++i) { 
    for (int j = 0; j < NUMPIXELS1; ++j) { 
      strip1.setPixelColor(j, strip1.Color(0, 0, 255)); 
      strip2.setPixelColor(j, strip2.Color(0, 0, 255)); 
      } 
    strip1.show(); 
    strip2.show(); 
    delay(500);  
    for (int j = 0; j < NUMPIXELS1; ++j) {
      strip1.setPixelColor(j, strip1.Color(0, 0, 0)); 
      strip2.setPixelColor(j, strip2.Color(0, 0, 0));  
    } 
    strip1.show();
    strip2.show();
    delay(500); 
  }
}
