@@ -1,167 +1,193 @@
@ -1,4 +1,9 @@
#include <Adafruit_NeoPixel.h>
#include <MFRC522.h>  // Library responsible for communicating with the module RFID-RC522
#include <SPI.h>      // Library responsible for communicating with SPI bus
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Bestemmer at Leser_For_Lag_1 skal være på pin 35
#define Leser_For_Lag_1 35
// Bestemmer at Leser_For_Lag_2 skal være på pin 34
#define Leser_For_Lag_2 34
// Disse boolene sier at Lag 1/2 ikke har fått mål enda
bool Fikk_Lag_1_Mål = false;
bool Fikk_Lag_2_Mål = false;
#define LED_PIN 27 //ledpins og antall definisjon, kan endres
#define LED_PIN2 19
#define LED_COUNT  50
#define LED_COUNT2  50
#define BRIGHTNESS 255
#define NUMPIXELS 14
#define PIN 2

float move = 0.0; //  verdier for funksjon
float animation = 0;
float fadeAnimation = 0;

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Scoreboard

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
@ -39,6 +44,28 @@ const bool digitSegments[10][7] = {
};


// Structure to hold UID, name, and poengsum
struct Person {
    byte uid[4];
    String name;
    int poengsum;
};

// Eksempel på personer som er registrert
Person registeredPersons[] = {
    {{0x03, 0xA4, 0x4B, 0x25}, "Test Perso", 100},
    {{0x0A, 0x61, 0xD5, 0x08}, "Hans", 100},
    {{0xEA, 0x33, 0xBE, 0x08}, "Noah", 100},
    {{0x4A, 0xB6, 0xCC, 0x08}, "Emil", 100},
    {{0x0A, 0xFC, 0xCF, 0x08}, "Mykhailo", 100},
    {{0xEA, 0x12, 0xC9, 0x08}, "Benjamin", 100},
    {{0x4A, 0x5C, 0xD0, 0x08}, "Ingvill", 100},
    {{0x8A, 0x1E, 0xD7, 0x08}, "Ask", 100},
    {{0x2A, 0xC9, 0xD9, 0x0F}, "Ingve", 100},
    {{0x8A, 0xE4, 0xCE, 0x08}, "Karsten", 100}
};


// Funksjon for å sette opp lysdiodene for et tall
void visGoal_1(int Goal_1) {
if (Goal_1 < 0 || Goal_1 > 9) return; // Valider at tallet er mellom 0 og 9
@ -165,3 +192,153 @@ void bouncewave(Adafruit_NeoPixel& strip, uint32_t color, int wait){
  move += 0.08; 
  delay(40); 
}

  for (int i = 0; i < 7; i++) {
    if (digitSegments[Goal_1][i]) {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); // Rødt lys for segmentene som skal være på
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));   // Slå av segmentene som ikke er i bruk
///////////////////////////////////////////////////////////////////////////
////////////////////  Funksjoener for rfid skaner  ////////////////////
///////////////////////////////////////////////////////////////////////////

void printToDisplay(String text){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}

bool compareUID(byte* uid1, byte* uid2) {
    for (byte i = 0; i < 4; i++) {
        if (uid1[i] != uid2[i]) {
            return false;
        }
}
  }
    return true;
}
void visGoal_2(int Goal_2) {
  if (Goal_2 < 0 || Goal_2 > 9) return; // Valider at tallet er mellom 0 og 9

  for (int i = 7; i < 14; i++) {
    if (digitSegments[Goal_2][i-7]) {
      strip.setPixelColor(i, strip.Color(0, 0, 255)); // Rødt lys for segmentene som skal være på
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));   // Slå av segmentene som ikke er i bruk
void printUID(byte* uid) {
    for (byte i = 0; i < 4; i++) {
        Serial.print(uid[i] < 0x10 ? " 0" : " ");
        Serial.print(uid[i], HEX);
}
  }
    Serial.println();
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
Person* scanCard() {
    // Vent på at et kort skal nærme seg
    while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        delay(100);
    }

    byte* scannedUID = mfrc522.uid.uidByte;

    // Sjekk om skannet UID er registrert
    for (Person& person : registeredPersons) {
        if (compareUID(scannedUID, person.uid)) {
            Serial.print("UID: ");
            printUID(scannedUID);
            Serial.print("Kort tilhører: ");
            Serial.println(person.name);
            
            printToDisplay(person.name);
            
            return &person;  // Returner en peker til personen
        }
    }

    Serial.println("Ikke registrert");
    return nullptr;  // Returner null hvis personen ikke er registrert
}

void sortPersonsByPoengsum() {
    // Sorter personene etter poengsum med boblesortering
    for (int i = 0; i < sizeof(registeredPersons) / sizeof(registeredPersons[0]) - 1; i++) {
        for (int j = 0; j < sizeof(registeredPersons) / sizeof(registeredPersons[0]) - i - 1; j++) {
            if (registeredPersons[j].poengsum < registeredPersons[j + 1].poengsum) {
                Person temp = registeredPersons[j];
                registeredPersons[j] = registeredPersons[j + 1];
                registeredPersons[j + 1] = temp;
            }
        }
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
void generateMatches() {
    sortPersonsByPoengsum();

    Serial.println("Genererer kamper basert på poengsum:");
    for (int i = 0; i < sizeof(registeredPersons) / sizeof(registeredPersons[0]); i += 2) {
        if (i + 1 < sizeof(registeredPersons) / sizeof(registeredPersons[0])) {
            Serial.print(registeredPersons[i].name);
            Serial.print(" vs ");
            Serial.println(registeredPersons[i + 1].name);
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
void playMatch(Person* person1, Person* person2, bool scanRequired) {
    if (scanRequired) {
        Serial.print("Skann første kort: ");
        Person* scannedPerson1 = nullptr;

        while (scannedPerson1 != person1) {
            scannedPerson1 = scanCard();
            if (scannedPerson1 != person1) {
                Serial.println("Skann riktig kort for " + person1->name + ".");
            } else if (redButtonState == LOW) {
              return;
            }
}
      }
      for(int c = 0; c <= center2; c++) {
        if ((c + b) % 3 == 0) {
          strip2.setPixelColor(center2 + c, color2_1);
          strip2.setPixelColor(center2 - c, color2_1);
        } else {
          strip2.setPixelColor(center2 + c, color2_2);
          strip2.setPixelColor(center2 - c, color2_2);

        Serial.print("Skann andre kort: ");
        Person* scannedPerson2 = nullptr;

        while (scannedPerson2 != person2) {
            scannedPerson2 = scanCard();
            if (scannedPerson2 != person2) {
                Serial.println("Skann riktig kort for " + person2->name + ".");
            }
}
      }
      strip1.show();
      strip2.show();
      delay(wait);
}
  }

    Serial.print(person1->name + "s poengsum: ");
    Serial.println(person1->poengsum);
    Serial.print(person2->name + "s poengsum: ");
    Serial.println(person2->poengsum);

    printToDisplay(person1->name + "\n" + person1->poengsum + "\n" + person2->name + "\n" + person2->poengsum);
    delay(5000);

    // Vent på at brukeren bestemmer hvem som skal få poeng
    Serial.println("1 for " + person1->name + ", 2 for " + person2->name);
    while (!Serial.available());  // Vent på brukerinntasting

    char choice = Serial.readStringUntil('\n').charAt(0);  // Les brukerinntasting

    if (choice == '1') {
        person1->poengsum += 5;
        person2->poengsum -= 5;
        Serial.print(person1->name + " vant kampen");
    } else if (choice == '2') {
        person2->poengsum += 5;
        person1->poengsum -= 5;
        Serial.print(person2->name + " vant kampen");
    } else {
        Serial.println("Ugyldig valg. Ingen poeng legges til.");
    }

    // Oppdaterte poengsummer etter kamp
    Serial.println("Oppdaterte poengsummer etter kampen:");
    Serial.print(person1->name + ": ");
    Serial.println(person1->poengsum);
    Serial.print(person2->name + ": ");
    Serial.println(person2->poengsum);
    
    printToDisplay(person1->name + "\n" + person1->poengsum + "\n" + person2->name + "\n" + person2->poengsum);
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
void displayAllScores() {
    Serial.println("Poengsum for alle spillere:");
    for (int i = 0; i < sizeof(registeredPersons) / sizeof(registeredPersons[0]); i++) {
        Serial.print(registeredPersons[i].name);
        Serial.print(": ");
        Serial.println(registeredPersons[i].poengsum);
    }
}
