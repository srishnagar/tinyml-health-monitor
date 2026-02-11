#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
#define TEMP_PIN A0
#define FLEX_PIN A1
#define HEART_PIN A2
#define PIR_PIN 2
#define BUZZER 8

// ===== SCALER VALUES =====
float mean[4] = {38.1163, 544.8272, 654.3803, 0.3333};
float stdv[4] = {1.2867, 237.5971, 236.7085, 0.4714};

// ===== HIDDEN LAYER WEIGHTS (4x4) =====
float W1[4][4] = {
  {-0.6463,  1.3501,  0.3805, -0.0977},
  {-1.0213,  0.0155, -0.6846,  0.3475},
  {-0.2623,  0.9451, -0.6915,  0.5350},
  { 1.0205, -0.2854, -1.1426, -0.2247}
};

float b1[4] = {0.0862, -0.1699, 0.4486, -0.6852};

// ===== OUTPUT LAYER WEIGHTS (4x3) =====
float W2[4][3] = {
  { 0.6768, -1.1627, -0.8385},
  {-0.8611, -0.7134,  1.1589},
  {-0.7475,  1.0579, -0.5474},
  {-1.2284, -0.0876, -0.3196}
};

float b2[3] = {-0.5782, 1.3401, 0.2703};

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.print("TinyML Health");
  delay(2000);
  lcd.clear();
}

void loop() {

  // ===== READ SENSORS =====
  int tempRaw = analogRead(TEMP_PIN);
  int flexRaw = analogRead(FLEX_PIN);
  int heartRaw = analogRead(HEART_PIN);
  int motion = digitalRead(PIR_PIN);

  float voltage = tempRaw * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100.0;

  float input[4] = {temperature, (float)flexRaw, (float)heartRaw, (float)motion};

  // ===== SCALE INPUT =====
  for(int i=0; i<4; i++){
    input[i] = (input[i] - mean[i]) / stdv[i];
  }

  // ===== HIDDEN LAYER =====
  float hidden[4];
  for(int i=0; i<4; i++){
    hidden[i] = b1[i];
    for(int j=0; j<4; j++){
      hidden[i] += input[j] * W1[j][i];
    }
    if(hidden[i] < 0) hidden[i] = 0;  // ReLU
  }

  // ===== OUTPUT LAYER =====
  float output[3];
  for(int i=0; i<3; i++){
    output[i] = b2[i];
    for(int j=0; j<4; j++){
      output[i] += hidden[j] * W2[j][i];
    }
  }

  // ===== ARGMAX =====
  int prediction = 0;
  float maxVal = output[0];
  for(int i=1; i<3; i++){
    if(output[i] > maxVal){
      maxVal = output[i];
      prediction = i;
    }
  }

  // ===== DISPLAY =====
  lcd.clear();

  if(prediction == 0){
    lcd.print("Status:NORMAL");
    digitalWrite(BUZZER, LOW);
  }
  else if(prediction == 1){
    lcd.print("Status:STRESS");
    tone(BUZZER, 1000);
    delay(200);
    noTone(BUZZER);
  }
  else{
    lcd.print("Status:CRITICAL");
    tone(BUZZER, 2000);
  }

  lcd.setCursor(0,1);
  lcd.print("AI Active");

  delay(2000);
}


