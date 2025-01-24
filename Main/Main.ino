#include <Funksjoner.h>


//Definerer oeiner til rfide, kanper og kjerm
#define I2C_SDA 4 
#define I2C_SCL 5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define blueButtonPin 13
#define redButtonPin 12
int blueButtonState;
int redButtonState;

#define SS_PIN 21
#define RST_PIN 22
#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16

// Definer antall LED-piksler
#define Rod_Lys_Lag_1 26
#define Rod_Lys_Lag_2 27
@ -18,9 +38,72 @@ void setup() {
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
@ -40,6 +123,7 @@ void loop() {
    digitalWrite(Rod_Lys_Lag_2, HIGH);
    delay(1000);
    }
  
  }

///////////////////////////////////////////////////////////////////////////
