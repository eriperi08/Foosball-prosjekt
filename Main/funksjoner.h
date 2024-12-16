// Bestemmer at Leser_For_Lag_1 skal være på pin 12
#define Leser_For_Lag_1 12
// Bestemmer at Leser_For_Lag_2 skal være på pin 14
#define Leser_For_Lag_2 14
// Disse boolene sier at Lag 1/2 ikke har fått mål enda
bool Fikk_Lag_1_Mål = false;
bool Fikk_Lag_2_Mål = false;

// gjør dette en gang i begynnelsen av programmet
void setup() {
  // bestemmer hastigheten på signalet
  Serial.begin (115200);
  // bestemmer hvor ESP32 skal ta imot signal for begge lagene
  pinMode(Leser_For_Lag_1, INPUT);
  pinMode(Leser_For_Lag_2, INPUT);
  }

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
