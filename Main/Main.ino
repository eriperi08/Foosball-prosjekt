#include <Adafruit_NeoPixel.h>
#include <funksjoner.h>

void setup() {
  strip0.setBrightness(25);
  strip1.setBrightness(25);
  strip2.setBrightness(25);
  strip0.begin(); // Gjør alle stripene klare
  strip0.show();
  strip1.begin();
  strip1.show(); 
  strip2.begin();
  strip2.show();
  Serial.begin(115200); // Start serial kommunikasjon
  KAMPSTART = true;
  Goal_1 = 0;
  Goal_2 = 0;
}

void loop() {
  int Lysnivå_Lag_1 = analogRead(Leser_For_Lag_1);
  int Lysnivå_Lag_2 = analogRead(Leser_For_Lag_2);

  if (KAMPSTART) {   
    Serial.println("Kampen starter");
    Blink(strip1, strip2, strip1.Color(255, 0, 0), strip1.Color(255, 128, 0), strip2.Color(0, 0, 255), strip2.Color(0, 128, 255), 50);
    strip1.clear();
    strip1.show();
    strip2.clear();
    strip2.show();
    KAMPSTART = false;
    delay(500);
  }

  if (Lysnivå_Lag_1 < 2000) {
    Serial.println("Lag 1 fikk mål");
    Goal_1 = Goal_1 + 1;
    Serial.print("Lag 1 mål: ");
    Serial.println(Goal_1);
    Fikk_Lag_1_Mål = true;
    visGoal_1(Goal_1);
    strip0.show();
    delay(100);
  }
  if (Lysnivå_Lag_2 < 3000) {
    Serial.println("Lag 2 fikk mål");
    Goal_2 = Goal_2 + 1;
    Serial.print("Lag 2 mål: ");
    Serial.println(Goal_2);
    Fikk_Lag_2_Mål = true;
    visGoal_2(Goal_2);
    strip0.show();
    delay(100);
  }

  if (Fikk_Lag_1_Mål) {
    Serial.println("Kjører animasjon for Lag 1");
    colorWipe(strip1, strip1.Color(255, 0, 255), 25);
    colorWipe(strip1, strip1.Color(0, 255, 128), 25);
    colorWipe(strip1, strip1.Color(0, 0, 255), 25);
    Blink(strip1, strip2, strip1.Color(255, 0, 0), strip1.Color(255, 128, 0), strip2.Color(0, 0, 255), strip2.Color(0, 128, 255), 50);
    delay(1000);
    strip1.clear();
    strip1.show();
    Fikk_Lag_1_Mål = false;
    if (Goal_1 == 10) {
      LAG_1_VANT = true;
    }
    else {
    delay(6000);
    }
  }
    if (Fikk_Lag_2_Mål) {
    Serial.println("Kjører animasjon for Lag 2");
    colorWipe(strip2, strip1.Color(255, 255, 0), 25);
    colorWipe(strip2, strip1.Color(255, 100, 0), 25);
    colorWipe(strip2, strip1.Color(255, 0, 0), 25);
    Blink(strip1, strip2, strip1.Color(0, 0, 255), strip1.Color(0, 128, 255), strip2.Color(255, 0, 0), strip2.Color(255, 128, 0), 50);
    delay(1000);
    strip2.clear();
    strip2.show();
    Fikk_Lag_2_Mål = false;
    if (Goal_2 == 2) {
      LAG_2_VANT = true;
    }
    else {
    delay(6000);
    }
  }

  if (LAG_1_VANT) {
    Serial.println("Kjører LAG_1_VANT");
    WinBlink(strip1, strip2, strip1.Color(0, 0, 255), strip1.Color(0, 128, 255), strip2.Color(0, 0, 255), strip2.Color(0, 128, 255), 50);
    strip1.clear();
    strip1.show();
    strip2.clear();
    strip2.show();
    Goal_1 = 0;
    Goal_2 = 0;
    LAG_1_VANT = false;
    delay(4000);
  }

  if (LAG_2_VANT) {
    Serial.println("Kjører LAG_2_VANT");
    WinBlink2(strip1, strip2, strip1.Color(255, 0, 0), strip1.Color(255, 128, 0), strip2.Color(255, 0, 0), strip2.Color(255, 128, 0), 50);
    strip1.clear();
    strip1.show();
    strip2.clear();
    strip2.show();
    Goal_1 = 0;
    Goal_2 = 0;
    LAG_2_VANT = false;
    delay(4000);
  }
}
