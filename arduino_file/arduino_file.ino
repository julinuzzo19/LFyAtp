#include <Keypad.h>
#include <DHT.h>  // importar la Librerias Sensor temperatura y humedad
#include <DHT_U.h>

//------------------Configuracion del keypad-----------------------------------//
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '#', '0', '*' }
};
byte rowPins[ROWS] = { 12, 11, 8, 7 };  //Filas(pines del 7,8 y 12, 13)
byte colPins[COLS] = { 6, 5, 4 };       //Columnas (pines del 4 al 6)
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


//--------------------Configuración de la clave codificada------------------------//
unsigned int claveCodificada[4] = { 5, 0, 2, 3 };  // password en memoria  para la key 7896
unsigned int claveIngresada[4];
unsigned int cantDigitosIngresados = 0;
unsigned int alarmaActivada = 0;
//------------------Variables para la función modulo-----------------------------//
unsigned int a = 2;
unsigned int b = 5;
unsigned int n = 7;
char key;


//-------------Configuración y variables del sensor de temperatura y humedad----------------//
int SENSOR = A4;  // pin DATA del DHT a pin digital 2
int TEMPERATURA;
int HUMEDAD;
DHT dht(SENSOR, DHT11);
int ventilacionActivada = 0;
int riegoActivado = 0;
int calefaccionActivada = 0;


//-------------------Variables y constantes del sensor de luz LDR----------------------------//
const long Roscuridad = 1000;  //Resistencia en oscuridad en KΩ
const int Rluz = 15;           //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;             //Resistencia calibracion en KΩ
const int LDRPin = A3;         //Pin del LDR
int VOLTAJE;
int ILUMINACION;
int pinLedR = 13;

//----------------buzzer------------------------------//
int pinaltavoz = 3;
int frecuencia = 220;  // frecuencia correspondiente a la nota La




//--------------------Configuración sensor de movimiento------------------------//
int pinMovimiento = A5;
int movimientoDetectado = 0;


//-------------------Configuracion temporizador-----------------------------------//
unsigned long tiempoInicio; 
const unsigned long tiempoEspera = 1000;

void setup() {
  // init comunicación serie a 9600 baudios (monitor serial)
  Serial.begin(9600);
  // Inicialización del sensor de temperatura y humedad
  dht.begin();
  //pinLedR como output
  pinMode(pinLedR, OUTPUT);
  //pinled sensor movimiento
  pinMode(pinMovimiento, INPUT);
}




void loop() {

  // -----------------------------alarma--------------------------------------//
  key = keypad.getKey();
  activarAlarma(key);

  detectarMovimiento();


  unsigned long tiempoActual = millis();  // Obtener el tiempo actual en milisegundos

  // Si el temporizador ha expirado, realizar una acción
  if (tiempoActual - tiempoInicio >= tiempoEspera) {
    // 1 - Si la alarma esta activada y se detecta movimiento se enciende la sirena
    // 2 - Caso contrato se leen los sensores
    if (alarmaActivada && movimientoDetectado == 1) {
      activarSirena();
    }

    // ----------------------------Fin alarma----------------------------
    else {



      //--------------------------------Activacion de calefaccion, ventilacion y sirena-----------------------------//
      TEMPERATURA = dht.readTemperature();  // Obtener valor de temperatura
      HUMEDAD = dht.readHumidity();         // Obtener valor de humedad
      activarCalefaccionVentilacion(TEMPERATURA, HUMEDAD);
      //---------------------------------------Activacion de calefaccion, ventilacion y sirena-----------------------------------------------//


      // -----------------------activacion de led----------------//
      VOLTAJE = analogRead(LDRPin);                                                            // Lee entrada analógica
      ILUMINACION = ((long)VOLTAJE * Roscuridad * 10) / ((long)Rluz * Rc * (1024 - VOLTAJE));  // Obtener valor de iluminación

      activarLed(ILUMINACION);

      // ---------------------------------activacion led----------------------------//





    }

    tiempoInicio = tiempoActual;  // Reiniciar el temporizador
    Serial.print("\r\n");
  }

}

// Definicion de funciones
int codificarClave(int digitoPlano) {
  return (a * digitoPlano + b) % n;
}

void activarAlarma(char key) {

  if (key) {
    int digito = atoi(&key);
    claveIngresada[cantDigitosIngresados] = codificarClave(digito);
    cantDigitosIngresados++;

    // Al ingresar 4 caracteres se validan con la clave codificada
    if (cantDigitosIngresados == 4) {

      if (claveIngresada[0] == claveCodificada[0] && claveIngresada[1] == claveCodificada[1] && claveIngresada[2] == claveCodificada[2] && claveIngresada[3] == claveCodificada[3]) {
        if (alarmaActivada == 1) {
          Serial.print("Alarma desactivada");
          Serial.print("\r\n");
          desactivarSirena();



          alarmaActivada = 0;
        } else {
          Serial.print("Alarma activada");
          Serial.print("\r\n");


          alarmaActivada = 1;
        }
      } else {
        Serial.print("Ingrese la contrasena nuevamente");
        Serial.print("\r\n");
      }
      cantDigitosIngresados = 0;
    }
  }
}

void activarSirena() {
  tone(pinaltavoz, frecuencia);
}
void desactivarSirena() {
  noTone(pinaltavoz);
}


void detectarMovimiento() {

  movimientoDetectado = digitalRead(pinMovimiento);  // lee valor sensor movimiento
}



void activarCalefaccionVentilacion(int TEMPERATURA, int HUMEDAD) {
  Serial.print("Temperatura: ");
  Serial.print(TEMPERATURA);
  Serial.print(" - Humedad: ");
  Serial.println(HUMEDAD);

  //tone(pinaltavoz,frecuencia) --> activacion altavoz
  //noTone(pinaltavoz) --> desactivacion altavoz
  if (TEMPERATURA > 50) {
    activarSirena();
    ventilacionActivada = 0;
    calefaccionActivada = 0;
  } else if (TEMPERATURA > 25 && HUMEDAD < 80 && movimientoDetectado) {
    desactivarSirena();
    ventilacionActivada = 1;
    calefaccionActivada = 0;
  } else if (TEMPERATURA >= 23 && HUMEDAD >= 80 && movimientoDetectado) {
    desactivarSirena();
    ventilacionActivada = 1;
    calefaccionActivada = 0;
  } else if (TEMPERATURA >= 18) {
    desactivarSirena();
    ventilacionActivada = 0;
    calefaccionActivada = 0;
  } else if (TEMPERATURA < 18 && movimientoDetectado) {
    desactivarSirena();
    ventilacionActivada = 0;
    calefaccionActivada = 1;
  }

  if (ventilacionActivada) {
    Serial.print("Ventilador encendido.");
  } else {
    Serial.print("Ventilador apagado.");
  }
  Serial.print("\r\n");

  if (calefaccionActivada) {
    Serial.print("Calefaccion encendida.");
  } else {
    Serial.print("Calefaccion apagada.");
  }
  Serial.print("\r\n");
}

void activarLed(int ILUMINACION) {
  Serial.print("Iluminacion: ");
  Serial.print(ILUMINACION);
  Serial.print("\r\n");

  if (ILUMINACION < 100) {
    analogWrite(pinLedR, 255);  //encendido
    Serial.print("Led encendido.");
  } else {
    Serial.print("Led apagado.");
    analogWrite(pinLedR, 0);
  }
  Serial.print("\r\n");
}
