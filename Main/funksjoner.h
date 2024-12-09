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
