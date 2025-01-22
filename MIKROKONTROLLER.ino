// Deklarasi pin
const int ldrPin = A0;     // LDR terhubung ke A0
const int ledPin = 2;      // LED terhubung ke pin 2
const int buzzerPin = 3;   // Buzzer terhubung ke pin 3
const int buttonPin = 4;   // Push button terhubung ke pin 4

// Pin seven segment (misalnya pin 5 hingga 11)
int segPins[] = {5, 6, 7, 8, 9, 10, 11};

// Konfigurasi angka untuk seven segment (ANODA)
int digit[10][7] = {
  {0, 0, 0, 0, 0, 0, 1}, // 0
  {1, 0, 0, 1, 1, 1, 1}, // 1
  {0, 0, 1, 0, 0, 1, 0}, // 2
  {0, 0, 0, 0, 1, 1, 0}, // 3
  {1, 0, 0, 1, 1, 0, 0}, // 4
  {0, 1, 0, 0, 1, 0, 0}, // 5
  {0, 1, 0, 0, 0, 0, 0}, // 6
  {0, 0, 0, 1, 1, 1, 1}, // 7
  {0, 0, 0, 0, 0, 0, 0}, // 8
  {0, 0, 0, 0, 1, 0, 0}  // 9
};

int score = 0;                  // Skor permainan
bool gameActive = false;        // Status permainan
unsigned long ledStartTime = 0; // Waktu LED menyala
unsigned long gameDelay = 2000; // Waktu tunggu untuk LED (2 detik)

void setup() {
  // Inisialisasi pin
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Tombol menggunakan resistor pull-up

  // Inisialisasi seven segment
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }

  // Kondisi awal
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  displayNumber(0); // Tampilkan skor 0
}

void loop() {
  // Mulai permainan jika tombol ditekan
  if (digitalRead(buttonPin) == LOW && !gameActive) {
    delay(200); // Debounce tombol
    resetGame();
    gameActive = true; // Permainan dimulai
  }

  // Jalankan logika permainan hanya jika aktif
  if (gameActive) {
    if (millis() - ledStartTime > gameDelay) {
      // Nyalakan LED secara acak
      ledStartTime = millis();
      digitalWrite(ledPin, HIGH); // Nyalakan LED
      delay(200);                 // Tunggu sebentar

      int ldrValue = analogRead(ldrPin); // Baca nilai LDR
      if (ldrValue < 500) {             // Jika LDR tertutup
        digitalWrite(buzzerPin, HIGH);  // Nyalakan buzzer sekejap
        delay(100);
        digitalWrite(buzzerPin, LOW);
        score++;                        // Tambah skor
        displayNumber(score);           // Tampilkan skor
      } else {
        // Jika pemain gagal menutup LDR
        gameOver();
        return;
      }

      digitalWrite(ledPin, LOW); // Matikan LED
      if (score == 9) {
        // Jika mencapai skor maksimum
        winGame();
        return;
      }
    }
  }
}

// Fungsi untuk menampilkan angka di seven segment
void displayNumber(int num) {
  int number = num % 10; // Ambil digit satuan
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], digit[number][i]); // Tampilkan angka
  }
}

// Fungsi untuk mengatur ulang permainan
void resetGame() {
  score = 0;
  displayNumber(score);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
  ledStartTime = millis(); // Reset waktu LED
}

// Fungsi untuk menang
void winGame() {
  // Bunyi nada do-re-mi
  tone(buzzerPin, 262, 300); // Do
  delay(300);
  tone(buzzerPin, 294, 300); // Re
  delay(300);
  tone(buzzerPin, 330, 300); // Mi
  delay(300);
  noTone(buzzerPin);
  gameActive = false; // Matikan permainan

  // Tunggu tombol ditekan untuk memulai ulang permainan
  while (digitalRead(buttonPin) == HIGH) {
    // Tunggu sampai tombol ditekan
  }
  delay(200); // Debounce tombol
  resetGame();
  gameActive = true; // Mulai permainan baru
}

// Fungsi untuk game over
void gameOver() {
  tone(buzzerPin, 262, 3000); // Bunyi nada do selama 3 detik
  delay(3000);
  noTone(buzzerPin);
  gameActive = false; // Matikan permainan

  // Tunggu tombol ditekan untuk memulai ulang permainan
  while (digitalRead(buttonPin) == HIGH) {
    // Tunggu sampai tombol ditekan
  }
  delay(200); // Debounce tombol
  resetGame();
  gameActive = true; // Mulai permainan baru
}