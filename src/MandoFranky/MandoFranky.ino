#include <N64Controller.h>

#define PIN_GAMEPAD 12

#define LED_ONOFF 10
#define LED_STATE 9
#define PIN_STATE 2

long refreshTimer = 0;
long ultimoCambioDireccion = 0;


N64Controller AceController(PIN_GAMEPAD);

int velocidadBrushless = 0;
int velocidadMotores = 0;
int gradosGiro = 0;
bool comandoEnviado = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED_ONOFF, OUTPUT);
  analogWrite(LED_ONOFF, 80);
  pinMode(PIN_STATE, INPUT);
  pinMode(LED_STATE, OUTPUT);
  digitalWrite(LED_ONOFF, HIGH);
  AceController.begin();
  delay(500);
}

void loop() {
  if (millis() - refreshTimer > 100) {
    digitalWrite(LED_STATE, digitalRead(PIN_STATE));
    AceController.update();
    comandoEnviado = false;

    if (AceController.D_left()) {
      gradosGiro -= 5;
      Serial.print("G");
      Serial.print(gradosGiro);
      comandoEnviado = true;
    }
    if (AceController.D_right()) {
      gradosGiro += 5;
      Serial.print("G");
      Serial.print(gradosGiro);
      comandoEnviado = true;
    }
    if (AceController.Z() && !AceController.D_down()) {
      if (velocidadMotores != 200) {
        velocidadMotores = 200;
        Serial.print("V");
        Serial.print(velocidadMotores);
        comandoEnviado = true;
      }
    } else {
      if (velocidadMotores == 200) {
        velocidadMotores = 0;
        Serial.print("V");
        Serial.print(velocidadMotores);
        comandoEnviado = true;
      }
    }

    if (AceController.D_down() && !AceController.Z()) {
      if (velocidadMotores != -200) {
        velocidadMotores = -200;
        Serial.print("V");
        Serial.print(velocidadMotores);
        comandoEnviado = true;
      }
    } else {
      if (velocidadMotores == -200) {
        velocidadMotores = 0;
        Serial.print("V");
        Serial.print(velocidadMotores);
        comandoEnviado = true;
      }
    }

    if (AceController.Start()) {
      if (velocidadBrushless < 500) {
        velocidadBrushless += 50;
        if (velocidadBrushless > 500) {
          velocidadBrushless = 500;
        }
        Serial.print("S");
        Serial.print(velocidadBrushless);
        comandoEnviado = true;
      }
    } else if (velocidadBrushless > 0) {
      if (velocidadBrushless != 0) {
        velocidadBrushless = 0;
        Serial.print("S");
        Serial.print(velocidadBrushless);
        comandoEnviado = true;
      }
    }

    if (AceController.L()) {
      Serial.print("C");
      comandoEnviado = true;
      gradosGiro = 0;
    }

    if (AceController.R()) {
      if (millis() - ultimoCambioDireccion > 1000) {
        Serial.print("R");
        ultimoCambioDireccion = millis();
        comandoEnviado = true;
      }
    }

    if (
      comandoEnviado) {
      Serial.print("\n");
    }

    refreshTimer = millis();
  }
}
