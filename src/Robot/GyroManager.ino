int calibracion_z = 0;
long microsAnteriorGyro = 0;
float ultimoError = 0;
long ultimoMicrosPID = 0;
bool run = false;

float kp = 2;
float ki = 0;
float kd = 10;

void actualizar_gyro() {
  if (microsAnteriorGyro == 0) {
    microsAnteriorGyro = micros();
  }
  float z = lectura_z();
  anguloActual += 1.05 * z * (micros() - microsAnteriorGyro) / 1000000.0;
  while (abs(anguloActual) > 360) {
    if (anguloActual > 360) {
      anguloActual -= 360;
    } else {
      anguloActual += 360;
    }
  }
  if (abs(anguloActual) > 180) {
    if (anguloActual > 180) {
      anguloActual = (anguloActual - 180) - 180;
    } else {
      anguloActual = (anguloActual + 180) + 180;
    }
  }
  microsAnteriorGyro = micros();

  dar_velocidad(0/*pid_gyro()*/);
}

float pid_gyro() {
  float p = 0;
  float d = 0;
  float error = anguloIdeal - anguloActual;

  p = kp * error;
  d = kd * ((error - ultimoError) / (micros() - ultimoMicrosPID));
  ultimoMicrosPID = micros();
  ultimoError = error;
  return p + d;
}

void dar_velocidad(float correccion) {
  int velI = velocidadMotores - correccion ;//+ MOTOR_IZQUIERDO_OFFSET;
  int velD = velocidadMotores + correccion ;//+ MOTOR_DERECHO_OFFSET;

  int pinD = MOTOR_DERECHO_ADELANTE;
  int pinI = MOTOR_IZQUIERDO_ADELANTE;
  // Limitar velocidad del motor derecho y selecciona la dirección.
  if (velD > 255) {
    velD = 255;
    pinD = MOTOR_DERECHO_ADELANTE;
  } else if (velD < 0) {
    velD = abs(velD);
    if (velD > 255) {
      velD = 255;
    }
    pinD = MOTOR_DERECHO_ATRAS;
  }

  // Limitar velocidad del motor izquierdo y selecciona la dirección.
  if (velI > 255) {
    velI = 255;
    pinI = MOTOR_IZQUIERDO_ADELANTE;
  } else if (velI < 0) {
    velI = abs(velI);
    if (velI > 255) {
      velI = 255;
    }
    pinI = MOTOR_IZQUIERDO_ATRAS;
  }

  if (!run) {
    run = true;
    digitalWrite(MOTOR_RUN, HIGH);
  }
  digitalWrite(MOTOR_DERECHO_ADELANTE, LOW);
  digitalWrite(MOTOR_DERECHO_ATRAS, LOW);
  digitalWrite(MOTOR_IZQUIERDO_ADELANTE, LOW);
  digitalWrite(MOTOR_IZQUIERDO_ATRAS, LOW);

  digitalWrite(pinD, HIGH);
  digitalWrite(pinI, HIGH);

  analogWrite(MOTOR_DERECHO_PWM   , constrain(velD, 0, 200));
  analogWrite(MOTOR_IZQUIERDO_PWM , constrain(velI, 0, 200));
}


int lectura_z() {
  // ---  Lectura giroscopio ---
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
  int z = ((Buf[12] << 8 | Buf[13]) - calibracion_z) / 16.4;
  return (abs(z) > 1) ? z : 0;
}



//Funcion auxiliar lectura
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  Wire.beginTransmission(Address);
    Wire.write(Register);
    Wire.endTransmission();

    Wire.requestFrom(Address, Nbytes);
    uint8_t index = 0;
    while (Wire.available())
    Data[index++] = Wire.read();
}
// Funcion auxiliar de escritura
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  Wire.beginTransmission(Address);
    Wire.write(Register);
    Wire.write(Data);
    Wire.endTransmission();
}
