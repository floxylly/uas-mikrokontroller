
const int ldrPin = A0;    
const int ledPin = 2;      
const int buzzerPin = 3;   
const int buttonPin = 4;  

int segPins[] = {5, 6, 7, 8, 9, 10, 11};

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

int score = 0;                  
bool gameActive = false;        
unsigned long ledStartTime = 0; 
unsigned long gameDelay = 2000; 

void setup() {

  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 

  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }

  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  displayNumber(0);
}

void loop() {

  if (digitalRead(buttonPin) == LOW && !gameActive) {
    delay(200);
    resetGame();
    gameActive = true;
  }
  
  if (gameActive) {
    if (millis() - ledStartTime > gameDelay) {
      ledStartTime = millis();
      digitalWrite(ledPin, HIGH); 
      delay(200);            

      int ldrValue = analogRead(ldrPin); 
      if (ldrValue < 500) {      
        digitalWrite(buzzerPin, HIGH);  
        delay(100);
        digitalWrite(buzzerPin, LOW);
        score++;                    
        displayNumber(score);       
      } else {
        gameOver();
        return;
      }

      digitalWrite(ledPin, LOW); 
      if (score == 9) {
        winGame();
        return;
      }
    }
  }
}

void displayNumber(int num) {
  int number = num % 10; 
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], digit[number][i]); 
  }
}

void resetGame() {
  score = 0;
  displayNumber(score);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
  ledStartTime = millis(); 
}

void winGame() {
  tone(buzzerPin, 262, 300); 
  delay(300);
  tone(buzzerPin, 294, 300); 
  delay(300);
  tone(buzzerPin, 330, 300);
  delay(300);
  noTone(buzzerPin);
  gameActive = false; 
  while (digitalRead(buttonPin) == HIGH) {
  }
  delay(200);
  resetGame();
  gameActive = true; 
}


void gameOver() {
  tone(buzzerPin, 262, 3000); 
  delay(3000);
  noTone(buzzerPin);
  gameActive = false; 

  
  while (digitalRead(buttonPin) == HIGH) {
  }
  delay(200); 
  resetGame();
  gameActive = true; 
}
