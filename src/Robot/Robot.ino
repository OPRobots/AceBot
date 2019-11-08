#include <Wire.h>
#include <Servo.h>

#define PARAMETRO_BRUSHLESS 'S'
#define PARAMETRO_BRUSHLESS_SWITCH 'R'
#define PARAMETRO_VELOCIDAD 'V'
#define PARAMETRO_GIRO 'G'
#define PARAMETRO_GIRO_CENTRAR 'C'

/////////////////
// CONFIG GYRO //
/////////////////
#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

///////////////////
// PINES MOTORES //
///////////////////
#define MOTOR_DERECHO_ADELANTE 8
#define MOTOR_DERECHO_ATRAS 7
#define MOTOR_DERECHO_PWM 11
#define MOTOR_IZQUIERDO_ADELANTE 4
#define MOTOR_IZQUIERDO_ATRAS 5
#define MOTOR_IZQUIERDO_PWM 3
#define MOTOR_RUN 6
#define PIN_BRUSHLESS A1

boolean girarBrushlessClockwise = true;
int velocidadBrushless = 0;
int velocidadMotores = 0;
int anguloIdeal = 0;
float anguloActual = 0;
long ultimoPulsoServo = 0;
long ultimaLecturaSerial = 0;

Servo brushless;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(MOTOR_DERECHO_ADELANTE, OUTPUT);
  pinMode(MOTOR_DERECHO_ATRAS, OUTPUT);
  pinMode(MOTOR_IZQUIERDO_ADELANTE, OUTPUT);
  pinMode(MOTOR_IZQUIERDO_ATRAS, OUTPUT);

  pinMode(MOTOR_DERECHO_PWM, OUTPUT);
  pinMode(MOTOR_IZQUIERDO_PWM, OUTPUT);
  pinMode(MOTOR_RUN, OUTPUT);
  digitalWrite(MOTOR_DERECHO_ADELANTE, LOW);
  digitalWrite(MOTOR_DERECHO_ATRAS, LOW);
  digitalWrite(MOTOR_IZQUIERDO_ADELANTE, LOW);
  digitalWrite(MOTOR_IZQUIERDO_ATRAS, LOW);
  digitalWrite(MOTOR_RUN, HIGH);

  digitalWrite(MOTOR_DERECHO_PWM, LOW);
  digitalWrite(MOTOR_IZQUIERDO_PWM, LOW);

  I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);

  brushless.attach(PIN_BRUSHLESS);
  brushless.writeMicroseconds(1000);
  delay(5000);

}

void loop() {
  if (millis() - ultimaLecturaSerial > 100) {

    comprobar_serial();

    //imprimir_parametros();

    ultimaLecturaSerial = millis();
  }
  actualizar_gyro();
  if(girarBrushlessClockwise){
    brushless.writeMicroseconds(map(velocidadBrushless, 0, 500, 1500, 2000));
  }else{
    brushless.writeMicroseconds(map(velocidadBrushless, 0, 500, 1500, 0));
  }
}


void imprimir_parametros() {
  Serial.print("B: ");
  Serial.print(velocidadBrushless);
  Serial.print("\tM: ");
  Serial.print(velocidadMotores);
  Serial.print("\tR: ");
  Serial.print((int)girarBrushlessClockwise);
  Serial.print("\tA: ");
  Serial.println(anguloIdeal);
}
