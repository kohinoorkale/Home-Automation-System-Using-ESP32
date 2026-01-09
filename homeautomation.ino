#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <AceButton.h>
using namespace ace_button;

// ---------------- USER SETTINGS ----------------
// 1. Wi-Fi Credentials
const char* ssid = "";      // Enter WiFi Name
const char* password = "";  // Enter WiFi Password

// 2. Firebase Credentials
#define API_KEY ""
#define DATABASE_URL ""
#define USER_EMAIL ""
#define USER_PASSWORD ""

// ---------------- PIN DEFINITIONS ----------------
// Relay GPIOs (Active LOW)
#define RELAY1 23
#define RELAY2 19
#define RELAY3 18
#define RELAY4 5

// Physical Button GPIOs
#define SwitchPin1 13
#define SwitchPin2 12
#define SwitchPin3 14
#define SwitchPin4 27

// Security GPIOs (NEW!)
#define PIR_PIN 26     // Connect PIR Sensor Output here
#define BUZZER_PIN 25  // Connect Buzzer Positive (+) here

// ---------------- GLOBALS ----------------
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Security Variables
bool isSecurityArmed = false; // Stores if the alarm system is active
bool motionDetected = false;

// AceButtons Setup
AceButton button1(SwitchPin1);
AceButton button2(SwitchPin2);
AceButton button3(SwitchPin3);
AceButton button4(SwitchPin4);

// ---------------- BUTTON HANDLER (TOGGLE LOGIC) ----------------
void handleEvent(AceButton* button, uint8_t eventType, uint8_t /* buttonState */) {
  // Only trigger when button is released (natural switch feel)
  if (eventType != AceButton::kEventReleased) return;

  int id = button->getPin();
  bool currentState;

  // Toggle Logic: Read current state -> Flip it -> Update Cloud
  switch (id) {
    case SwitchPin1:
      currentState = digitalRead(RELAY1) == LOW; // True if ON
      digitalWrite(RELAY1, currentState ? HIGH : LOW);
      Firebase.RTDB.setBool(&fbdo, "/relay1", !currentState);
      break;
    case SwitchPin2:
      currentState = digitalRead(RELAY2) == LOW;
      digitalWrite(RELAY2, currentState ? HIGH : LOW);
      Firebase.RTDB.setBool(&fbdo, "/relay2", !currentState);
      break;
    case SwitchPin3:
      currentState = digitalRead(RELAY3) == LOW;
      digitalWrite(RELAY3, currentState ? HIGH : LOW);
      Firebase.RTDB.setBool(&fbdo, "/relay3", !currentState);
      break;
    case SwitchPin4:
      currentState = digitalRead(RELAY4) == LOW;
      digitalWrite(RELAY4, currentState ? HIGH : LOW);
      Firebase.RTDB.setBool(&fbdo, "/relay4", !currentState);
      break;
  }
}

void setup() {
  Serial.begin(115200);

  // 1. Setup Relay Pins
  pinMode(RELAY1, OUTPUT); pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT); pinMode(RELAY4, OUTPUT);
  // Turn all OFF initially (Active LOW, so HIGH is OFF)
  digitalWrite(RELAY1, HIGH); digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH); digitalWrite(RELAY4, HIGH);

  // 2. Setup Security Pins (NEW)
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Buzzer OFF initially

  // 3. Setup Button Pins
  pinMode(SwitchPin1, INPUT_PULLUP); pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP); pinMode(SwitchPin4, INPUT_PULLUP);

  // 4. Configure AceButtons
  ButtonConfig* config1 = button1.getButtonConfig(); config1->setEventHandler(handleEvent);
  ButtonConfig* config2 = button2.getButtonConfig(); config2->setEventHandler(handleEvent);
  ButtonConfig* config3 = button3.getButtonConfig(); config3->setEventHandler(handleEvent);
  ButtonConfig* config4 = button4.getButtonConfig(); config4->setEventHandler(handleEvent);

  // 5. Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); delay(500);
  }
  Serial.println(" Connected!");

  // 6. Connect to Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // --- PART A: CLOUD SYNC ---
  if (Firebase.ready()) {
    // 1. Sync Relays (Downlink: App -> ESP32)
    bool r1, r2, r3, r4;
    if (Firebase.RTDB.getBool(&fbdo, "/relay1")) {
      r1 = fbdo.boolData(); digitalWrite(RELAY1, r1 ? LOW : HIGH);
    }
    if (Firebase.RTDB.getBool(&fbdo, "/relay2")) {
      r2 = fbdo.boolData(); digitalWrite(RELAY2, r2 ? LOW : HIGH);
    }
    if (Firebase.RTDB.getBool(&fbdo, "/relay3")) {
      r3 = fbdo.boolData(); digitalWrite(RELAY3, r3 ? LOW : HIGH);
    }
    if (Firebase.RTDB.getBool(&fbdo, "/relay4")) {
      r4 = fbdo.boolData(); digitalWrite(RELAY4, r4 ? LOW : HIGH);
    }

    // 2. Sync Security Mode (Downlink: App -> ESP32)
    // Create a switch in your app linked to VIRTUAL PIN or Database path "/security_mode"
    if (Firebase.RTDB.getBool(&fbdo, "/security_mode")) {
       isSecurityArmed = fbdo.boolData();
    }
  }

  // --- PART B: PHYSICAL BUTTONS ---
  button1.check();
  button2.check();
  button3.check();
  button4.check();

  // --- PART C: SECURITY LOGIC (NEW) ---
  if (isSecurityArmed) {
    // Read the PIR Sensor
    int motion = digitalRead(PIR_PIN);

    if (motion == HIGH) {
      Serial.println("MOTION DETECTED! ALARM ON!");
      digitalWrite(BUZZER_PIN, HIGH); // Turn Buzzer ON
      
      // Optional: Send alert to Firebase (so App shows "INTRUDER")
      // Firebase.RTDB.setString(&fbdo, "/alert_status", "INTRUDER DETECTED");
    } else {
      digitalWrite(BUZZER_PIN, LOW);  // Turn Buzzer OFF
    }
  } else {
    // If not armed, ensure buzzer is OFF
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  delay(10); // Stability delay
}
