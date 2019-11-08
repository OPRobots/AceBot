char parametro = ' ';
long ultimaLectura = 0;
String valor = "";


/*void comprobar_serial() {
  if (Serial.available()) {
    byte actual = Serial.read();
    ultimaLectura = millis();
    if (isDigit(actual) || (char)actual == '-') {
      valor += (char)actual;
    } else {
      if (parametro != ' ') {
        actualizar_parametro(parametro, valor != "" ? valor.toInt() : 0);
      }
      valor = "";
      parametro = (char)actual;
    }

  } else if (millis() - ultimaLectura > 5) {
    actualizar_parametro(parametro, valor != "" ? valor.toInt() : 0);
    parametro = ' ';
    valor = "";
  }
  }*/

void comprobar_serial() {
  if (Serial.available()) {
    while (Serial.available()) {
      byte actual = Serial.read();
      ultimaLectura = millis();
      if (isDigit(actual) || (char)actual == '-') {
        valor += (char)actual;
      } else {
        if (parametro != ' ') {
          actualizar_parametro(parametro, valor != "" ? valor.toInt() : 0);
        }
        valor = "";
        parametro = (char)actual;
      }
    }
  } else if (millis() - ultimaLectura > 5) {
    actualizar_parametro(parametro, valor != "" ? valor.toInt() : 0);
    parametro = ' ';
    valor = "";
  }
}

void actualizar_parametro(char parametro) {
  switch (parametro) {
    case 'S':
      velocidadBrushless = 500;
      break;
    case 's':
      velocidadBrushless = 0;
      break;
    case 'R':
      if (velocidadBrushless == 0) {
        girarBrushlessClockwise = !girarBrushlessClockwise;
      }
      break;
    case 'V':
      velocidadMotores = 200;
      break;
    case 'B':
      velocidadMotores = -200;
      break;
    case 'v':
    case 'b':
      velocidadMotores = 0;
    case 'G':
      anguloIdeal += 5;
      break;
    case 'g':
      anguloIdeal -= 5;
      break;
    case 'C':
      anguloActual = 0;
      anguloIdeal = 0;
      break;
  }
}


void actualizar_parametro(char parametro, int valor) {
  switch (parametro) {
    case PARAMETRO_BRUSHLESS:
      velocidadBrushless = valor;
      break;
    case PARAMETRO_BRUSHLESS_SWITCH:
      if (velocidadBrushless == 0) {
        girarBrushlessClockwise = !girarBrushlessClockwise;
      }
      break;
    case PARAMETRO_VELOCIDAD:
      velocidadMotores = valor;
      break;
    case PARAMETRO_GIRO:
      anguloIdeal = valor;
      break;
    case PARAMETRO_GIRO_CENTRAR:
      anguloActual = 0;
      anguloIdeal = 0;
      break;
  }
}
