@ -1,4 +1,9 @@
#include <Adafruit_NeoPixel.h>
#include <MFRC522.h>  // Library responsible for communicating with the module RFID-RC522
#include <SPI.h>      // Library responsible for communicating with SPI bus
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Bestemmer at Leser_For_Lag_1 skal være på pin 35
#define Leser_For_Lag_1 35
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
    return true;
}

void printUID(byte* uid) {
    for (byte i = 0; i < 4; i++) {
        Serial.print(uid[i] < 0x10 ? " 0" : " ");
        Serial.print(uid[i], HEX);
    }
    Serial.println();
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
}

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

        Serial.print("Skann andre kort: ");
        Person* scannedPerson2 = nullptr;

        while (scannedPerson2 != person2) {
            scannedPerson2 = scanCard();
            if (scannedPerson2 != person2) {
                Serial.println("Skann riktig kort for " + person2->name + ".");
            }
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

void displayAllScores() {
    Serial.println("Poengsum for alle spillere:");
    for (int i = 0; i < sizeof(registeredPersons) / sizeof(registeredPersons[0]); i++) {
        Serial.print(registeredPersons[i].name);
        Serial.print(": ");
        Serial.println(registeredPersons[i].poengsum);
    }
}
