int buttonState[17];
int joystick[4];
int AnalogButton[2];

int inputCommand = 0;

#define virbrationMotorPin 2

String Buttons[17] = {
  "J2", "J1", "", "S2", "S1", "UP", "LEFT", "DOWN", "RIGHT", "1", "4", "2", "3", "RZ1", "RZ2", "LZ1", "LZ2"
};


unsigned long refreshTimer = 0;

int spinButton = 16; //L2
int switchSpinButton = 15; //L1
int resetGyroButton = 14; //R2
int speedJoystickAxis = 0; // Left Analog, Y-Axis
int gyroJoystickAxis = 3; // Right Analog, X-Axis


void setup() {
  Serial.begin(9600); // Serial de consola
  Serial1.begin(9600); // Serial del BT
  initIO();

}

#define VAL_ANT_BRUSHLESS 0
#define VAL_ANT_GIRO 1
#define VAL_ANT_MOTORES 2
#define VAL_ANT_SWITCH 3
#define VAL_ANT_CENTRAR 4
int valoresAnteriores[] = {0, 0, 0, 0, 0};

int velocidadBrushless = 0;
int velocidadMotores = 0;
int anguloGiro = 0;

void loop() {
  if (millis() - refreshTimer > 50) {
    DataUpdate();

    boolean printSomething = false;

    if (buttonState[spinButton] == 0) {
      velocidadBrushless += 10;
    } else {
      velocidadBrushless -= 10;
    }
    velocidadBrushless = constrain(velocidadBrushless, 0, 500);
    if (valoresAnteriores[VAL_ANT_BRUSHLESS] != velocidadBrushless) {
      Serial1.print("S");
      Serial1.print(velocidadBrushless);
      Serial.print("S");
      Serial.print(velocidadBrushless);
      printSomething = true;
      valoresAnteriores[VAL_ANT_BRUSHLESS] = velocidadBrushless;
    }

    if (abs(AnalogButton[speedJoystickAxis] - 505) < 10) {
      velocidadMotores = 0;
    } else {
      velocidadMotores = map(AnalogButton[speedJoystickAxis], 1022, 0, 255, -255);
    }
    if (valoresAnteriores[VAL_ANT_MOTORES] != velocidadMotores) {
      Serial1.print("V");
      Serial1.print(velocidadMotores);
      Serial.print("V");
      Serial.print(velocidadMotores);
      printSomething = true;
      valoresAnteriores[VAL_ANT_MOTORES] = velocidadMotores;
    }

    if (abs(joystick[gyroJoystickAxis] - 498) < 10) {
      anguloGiro = 0;
    } else {
      anguloGiro = map(joystick[gyroJoystickAxis], 1022, 0, -180, 180);
    }
    if (valoresAnteriores[VAL_ANT_GIRO] != anguloGiro) {
      Serial1.print("G");
      Serial1.print(anguloGiro);
      Serial.print("G");
      Serial.print(anguloGiro);
      printSomething = true;
      valoresAnteriores[VAL_ANT_GIRO] = anguloGiro;
    }


    if (buttonState[switchSpinButton] == 0) {
      if (valoresAnteriores[VAL_ANT_SWITCH] == 0) {
        Serial1.print("R");
        Serial.print("R");
        printSomething = true;
      }
    } else {
      valoresAnteriores[VAL_ANT_SWITCH] = 0;
    }

    if (buttonState[resetGyroButton] == 0) {
      if (valoresAnteriores[VAL_ANT_CENTRAR] == 0) {
        Serial1.print("C");
        Serial.print("C");
        printSomething = true;
        valoresAnteriores[VAL_ANT_CENTRAR] = 1;
      }
    } else {
      valoresAnteriores[VAL_ANT_CENTRAR] = 0;
    }

    if (printSomething) {
      Serial1.print("\n");
      Serial.print("\n");
    }


    refreshTimer = millis();
  }
}

void initIO() {
  for (int i = 0; i < 17; i++) pinMode(i, INPUT);
  pinMode(virbrationMotorPin, OUTPUT);
  digitalWrite(virbrationMotorPin, LOW); // Stop shacking of the gamepad
}

void DataUpdate() {

  for (int i = 3; i < 17; i++)  buttonState[i] = digitalRead(i);
  buttonState[0] = analogRead(0);
  buttonState[1] = analogRead(1);
  for (int i = 0; i < 4; i++)  joystick[i] = analogRead(i);
  for (int i = 4; i < 6; i++)  AnalogButton[i - 4] = analogRead(i);

}
