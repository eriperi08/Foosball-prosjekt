#include <funksjoner.h>
 
void setup() {
  Serial.begin (115200);
  pinMode(Leser_For_Lag_1, INPUT);
  pinMode(Leser_For_Lag_2, INPUT);
  pinMode(Rod_Lys_Lag_1, OUTPUT);
  pinMode(Rod_Lys_Lag_2, OUTPUT);
  digitalWrite(Rod_Lys_Lag_1, HIGH);
  digitalWrite(Rod_Lys_Lag_2, HIGH);
  
  // rfid start
  Serial.begin(115200);
  SPI.begin();  // Init SPI bus
  
  pinMode(blueButtonPin, INPUT_PULLUP);
  pinMode(redButtonPin, INPUT_PULLUP);

  // Init MFRC522
  mfrc522.PCD_Init();
  Serial.println("Approach your reader card...");
  Serial.println();
  Wire.begin(4, 5); // SDA på pin 4 og SCL på pin 5
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.display();
  
  }

void loop() {
  printToDisplay("Blaa for\nturnering\nRod for\nkamp");
  
  blueButtonState = digitalRead(blueButtonPin); 
  redButtonState = digitalRead(redButtonPin); 
  if (blueButtonState==LOW) {
    printToDisplay("turnering");
    generateMatches();

    for (int i = 0; i < sizeof(registeredPersons) / sizeof(registeredPersons[0]); i += 2) {
      if (i + 1 < sizeof(registeredPersons) / sizeof(registeredPersons[0])) {
        Serial.print("Neste kamp: ");
        Serial.print(registeredPersons[i].name);
        Serial.print(" vs ");
        Serial.println(registeredPersons[i + 1].name);
        Serial.println("Skriv '4' for å starte kampen uten å skanne kortene, eller '5' for å vise poengsummene til alle spillerne.");
        
        printToDisplay("Neste kamp\n" + registeredPersons[i].name + "\nvs \n" + registeredPersons[i + 1].name);
        
        playMatch(&registeredPersons[i], &registeredPersons[i + 1], true);  // Skann kortene og start kampen
        
        /*while (!Serial.available());  // Vent på brukerinntasting
        char choice = Serial.readStringUntil('\n').charAt(0);  // Les brukerinntasting

        if (choice == '4') {
            Serial.println("Starter kampen uten å skanne kortene.");
            playMatch(&registeredPersons[i], &registeredPersons[i + 1], false);  // Start kampen uten å skanne kortene
        } else if (choice == '5') {
            displayAllScores();  // Vis poengsummene til alle spillerne
            i -= 2;  // Gå tilbake til forrige par for å vise kampen igjen
        } else {
            playMatch(&registeredPersons[i], &registeredPersons[i + 1], true);  // Skann kortene og start kampen
        }*/
      }
    }
  }  
  else if (redButtonState == LOW) {
    printToDisplay("kamp");
    //Når vi kobler det opp til sensorenen skal sensorene aktvieres her.
  }
  
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
