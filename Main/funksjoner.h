// Disse boolene sier at Lag 1/2 ikke har fått mål enda
#define LED_PIN1 25 //ledpins og antall definisjon, kan endres
#define LED_PIN2 26
#define NUMPIXELS1 42
#define BRIGHTNESS 255
#define NUMPIXELS 14
#define PIN 2

#define Leser_For_Lag_1 33
#define Leser_For_Lag_2 14
bool KAMPSTART = false;
bool Fikk_Lag_1_Mål = false;
bool Fikk_Lag_2_Mål = false;
bool LAG_1_VANT = false;
bool LAG_2_VANT = false;
int Lysnivå_Lag_1 = analogRead(Leser_For_Lag_1);
int Lysnivå_Lag_2 = analogRead(Leser_For_Lag_2);

int Goal_1 = 0; // Startverdi for første tall
int Goal_2 = 0; // Startverdi for andre tall

// Definer segmentkonfigurasjoner for tall (0-9)
// Segmentene er organisert i denne rekkefølgen: {A, B, C, D, E, F, G}
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

float move = 0.0; //  verdier for funksjon
float animation = 0;
float fadeAnimation = 0;

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip1(NUMPIXELS1, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(NUMPIXELS1, LED_PIN2, NEO_GRB + NEO_KHZ800);

void colorWipe(Adafruit_NeoPixel& strip1, uint32_t color, int wait) {
  int center = NUMPIXELS1 / 2;
  
  for(int i = 0; i <= center; i++) {
    strip1.setPixelColor(center + i, color);  
    strip1.setPixelColor(center - i, color);
    strip1.show();                    
    delay(30);                     
  }
}
void colorWipe2(Adafruit_NeoPixel& strip2, uint32_t color, int wait) {
  int center = NUMPIXELS1 / 2;
  
  for(int i = 0; i <= center; i++) {
    strip2.setPixelColor(center + i, color);  
    strip2.setPixelColor(center - i, color);
    strip2.show();                    
    delay(30);                     
  }
}
void StuffForStart(Adafruit_NeoPixel& strip1, Adafruit_NeoPixel& strip2, uint32_t color1_1, uint32_t color1_2, uint32_t color2_1, uint32_t color2_2, int wait) {

  for (int i = 0; i < 3; ++i) { 
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

// Funksjon for å vise et tall på første 7-segment (0–6)
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

// Funksjon for å vise et tall på andre 7-segment (7–13)
void visGoal_2(int tall) {
  if (tall < 0 || tall > 9) return; // Valider at tallet er mellom 0 og 9

  for (int i = 7; i < 14; i++) {
    if (digitSegments[tall][i - 7]) {
      strip.setPixelColor(i, strip.Color(0, 0, 255)); // Rødt lys for segmentene som skal være på
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));   // Slå av segmentene som ikke er i bruk
    }
  }
}
