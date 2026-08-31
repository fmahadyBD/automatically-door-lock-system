#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Keypad.h>

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myServo;

// Keypad setup - 4x4 matrix using pins 1-9
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {1, 2, 3, 4};
byte colPins[COLS] = {5, 6, 7, 8};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Pin definitions
const int SERVO_PIN = 10;
const int LED_PIN = 13;
const int IR_SENSOR_PIN = 11;

// Password settings
String correctPassword = "6666";
String enteredPassword = "";
int maxAttempts = 3;
int attempts = 0;
bool isLocked = false;

// Auto-reset settings
bool autoResetActive = false;
unsigned long lockTime = 0;
const unsigned long AUTO_RESET_DELAY = 5000;  // 5 seconds

// Servo states
enum ServoState {
  WAITING_FOR_PASSWORD,
  SERVO_AT_180,
  WAITING_FOR_IR,
  SERVO_AT_0
};

ServoState currentState = WAITING_FOR_PASSWORD;
unsigned long stateStartTime = 0;
unsigned long lastIRTime = 0;
const unsigned long UNLOCK_DURATION = 3000;
const unsigned long IR_DEBOUNCE = 500;

void setup() {
  Serial.begin(9600);
  
  // Setup pins
  myServo.attach(SERVO_PIN);
  myServo.write(0);
  pinMode(LED_PIN, OUTPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Show startup message
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  lcd.setCursor(0, 1);
  lcd.print("Enter Password");
  
  // Startup test - blink LED
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
  
  Serial.println("========================================");
  Serial.println("PASSWORD + IR SENSOR SYSTEM");
  Serial.println("========================================");
  Serial.println("LOG:System Started");
}

void loop() {
  char key = keypad.getKey();
  
  // Check auto-reset timer
  if (autoResetActive) {
    unsigned long elapsed = millis() - lockTime;
    int remaining = (AUTO_RESET_DELAY - elapsed) / 1000 + 1;
    
    if (elapsed >= AUTO_RESET_DELAY) {
      // Auto-reset the system
      performReset();
      return;
    } else {
      // Show countdown on LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SYSTEM LOCKED!");
      lcd.setCursor(0, 1);
      lcd.print("Reset in ");
      lcd.print(remaining);
      lcd.print("s");
      
      // Show countdown on Serial every second
      static unsigned long lastSerialUpdate = 0;
      if (millis() - lastSerialUpdate >= 1000) {
        lastSerialUpdate = millis();
        Serial.print("STATUS:Auto-reset in ");
        Serial.print(remaining);
        Serial.println(" seconds");
      }
    }
    return;  // Skip key processing while locked
  }
  
  if (key) {
    if (isLocked) {
      // Start auto-reset timer if not already started
      if (!autoResetActive) {
        lockTime = millis();
        autoResetActive = true;
        Serial.println("STATUS:System Locked - Auto-reset in 5 seconds");
      }
      return;
    }
    handleKeyInput(key);
  }
  
  // State machine for servo control
  switch (currentState) {
    case WAITING_FOR_PASSWORD:
      break;
      
    case SERVO_AT_180:
      if (millis() - stateStartTime >= UNLOCK_DURATION) {
        Serial.println("STATUS:3 seconds passed - Ready for IR detection");
        currentState = WAITING_FOR_IR;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("IR Active");
        lcd.setCursor(0, 1);
        lcd.print("Wave hand to lock");
        Serial.println("Wave hand in front of IR sensor to lock");
        Serial.println("----------------------------------------");
      }
      break;
      
    case WAITING_FOR_IR: {
      int irValue = digitalRead(IR_SENSOR_PIN);
      if (irValue == LOW && (millis() - lastIRTime > IR_DEBOUNCE)) {
        lastIRTime = millis();
        Serial.println("IR Sensor - OBJECT DETECTED!");
        lockServo();
      }
      break;
    }
      
    case SERVO_AT_0:
      break;
  }
}

void handleKeyInput(char key) {
  Serial.print("KEY:");
  Serial.println(key);
  
  if (key == '*') {
    enteredPassword = "";
    Serial.println("Password cleared");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cleared");
    lcd.setCursor(0, 1);
    lcd.print("Enter Password");
    delay(150);
    blinkKeyPress();
    delay(150);
    blinkKeyPress();
  }
  else if (key == '#') {
    Serial.println("SUBMIT:Password submitted");
    blinkSubmit();
    checkPassword();
  }
  else if (key >= '0' && key <= '9') {
    enteredPassword += key;
    blinkKeyPress();
    
    // Show asterisks on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password:");
    lcd.setCursor(0, 1);
    for (int i = 0; i < enteredPassword.length(); i++) {
      lcd.print("*");
    }
    
    // Send asterisks to Serial for Python to display
    Serial.print("Asterisks:");
    for (int i = 0; i < enteredPassword.length(); i++) {
      Serial.print("*");
    }
    Serial.println();
  }
  else {
    // Letters A, B, C, D - IGNORED
    Serial.println("Invalid key! Only numbers 0-9 accepted");
  }
}

void checkPassword() {
  Serial.println("");
  Serial.println("Checking password...");
  
  if (enteredPassword.length() == 0) {
    Serial.println("Please enter a password first!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Password");
    lcd.setCursor(0, 1);
    lcd.print("Enter 6666");
    blinkError();
    return;
  }
  
  if (enteredPassword == correctPassword) {
    Serial.println("LOG:Access Granted!");
    Serial.println("ACCESS GRANTED!");
    unlockServo();
    attempts = 0;
    enteredPassword = "";
  } else {
    attempts++;
    Serial.print("WRONG PASSWORD! (Attempt ");
    Serial.print(attempts);
    Serial.print("/");
    Serial.print(maxAttempts);
    Serial.println(")");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WRONG!");
    lcd.setCursor(0, 1);
    lcd.print("Attempt ");
    lcd.print(attempts);
    lcd.print("/");
    lcd.print(maxAttempts);
    
    blinkError();
    enteredPassword = "";
    
    Serial.println("LOG:Wrong Password Attempt!");
    
    if (attempts >= maxAttempts) {
      isLocked = true;
      lockTime = millis();
      autoResetActive = true;
      
      Serial.println("");
      Serial.println("SYSTEM LOCKED!");
      Serial.println("Too many failed attempts!");
      Serial.println("Auto-reset in 5 seconds");
      Serial.println("========================================");
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SYSTEM LOCKED!");
      lcd.setCursor(0, 1);
      lcd.print("Reset in 5s");
      
      blinkLocked();
      Serial.println("LOG:System Locked!");
    }
  }
}

void performReset() {
  Serial.println("");
  Serial.println("STATUS:AUTO-RESET - System unlocked");
  Serial.println("LOG:System Reset");
  Serial.println("========================================");
  
  // Reset all variables
  isLocked = false;
  autoResetActive = false;
  attempts = 0;
  enteredPassword = "";
  currentState = WAITING_FOR_PASSWORD;
  myServo.write(0);
  digitalWrite(LED_PIN, LOW);
  
  // Show reset message on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Reset");
  lcd.setCursor(0, 1);
  lcd.print("Enter Password");
  
  delay(1000);
  
  // Clear the message after a moment
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  lcd.setCursor(0, 1);
  lcd.print("Enter Password");
}

void unlockServo() {
  Serial.println("");
  Serial.println("UNLOCKING - Moving servo to 180 degrees");
  myServo.write(180);
  digitalWrite(LED_PIN, HIGH);
  currentState = SERVO_AT_180;
  stateStartTime = millis();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UNLOCKED");
  lcd.setCursor(0, 1);
  lcd.print("180 degrees");
  
  Serial.println("Servo at 180 degrees - Waiting 3 seconds");
  Serial.println("----------------------------------------");
}

void lockServo() {
  Serial.println("");
  Serial.println("LOCKING - Moving servo to 0 degrees");
  myServo.write(0);
  digitalWrite(LED_PIN, LOW);
  currentState = SERVO_AT_0;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LOCKED");
  lcd.setCursor(0, 1);
  lcd.print("0 degrees");
  
  Serial.println("Servo locked at 0 degrees");
  Serial.println("Enter password again to unlock");
  Serial.println("========================================");
}

// ============ LED FEEDBACK FUNCTIONS ============

void blinkKeyPress() {
  digitalWrite(LED_PIN, HIGH);
  delay(150);
  digitalWrite(LED_PIN, LOW);
  delay(100);
}

void blinkSubmit() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void blinkError() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }
}

void blinkLocked() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
}
