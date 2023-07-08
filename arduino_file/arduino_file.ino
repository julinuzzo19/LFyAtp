#include <Keypad.h>
#include <DHT.h>  // importar la Librerias Sensor temperatura y humedad
#include <DHT_U.h>

//Configuracion del keypad
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
//

// Configuración de la clave codificada
char claveCodificada[5] = "5023";  // password en memoria  para la key 7896
unsigned int claveIngresada[4];
unsigned int cantDigitosIngresados = 0;
unsigned int alarmaActivada = 0;
//Variables para la función modulo
unsigned int a = 2;
unsigned int b = 5;
unsigned int n = 7;
// Definición de la función para decodificar alarma
int codificarClave(int digitoPlano) {
  return (a * digitoPlano + b) % n;
}
//

//Configuración y variables del sensor de temperatura y humedad
int SENSOR = A4;  // pin DATA del DHT a pin digital 2
int TEMPERATURA;
int HUMEDAD;
DHT dht(SENSOR, DHT11);
int ventilacionActivada = 0;
int riegoActivado = 0;


void setup() {
  // init comunicación serie a 9600 baudios (monitor serial)
  Serial.begin(9600);

  // Inicialización del sensor de temperatura y humedad
  dht.begin();
}


void loop() {
  //Obtener tecla ingresada del keypad
  char key = keypad.getKey();


  // Logica de activar/desactivar alarma
  if (key) {
    int digito = atoi(&key);
    claveIngresada[cantDigitosIngresados] = codificarClave(digito);
    cantDigitosIngresados++;

    // Al ingresar 4 caracteres se validan con la clave codificada
    if (cantDigitosIngresados == 4) {

      if (claveIngresada[0] == (claveCodificada[0] - '0') && claveIngresada[1] == (claveCodificada[1] - '0') && claveIngresada[2] == (claveCodificada[2] - '0') && claveIngresada[3] == (claveCodificada[3] - '0')) {
        if (alarmaActivada == 1) {
          Serial.print("Alarma desactivada");
          Serial.print("\r\n");


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


  Serial.print("\r\n");
  //Logica del sensor de temperatura y humedad
  TEMPERATURA = dht.readTemperature();  // Obtener valor de temperatura
  HUMEDAD = dht.readHumidity();         // Obtener valor de humedad
  Serial.print("Temperatura: ");
  Serial.print(TEMPERATURA);
  Serial.print(" - Humedad: ");
  Serial.println(HUMEDAD);

  if (HUMEDAD <= 50) riegoActivado = 1;
  else if (HUMEDAD >= 75) riegoActivado = 0;

  if (TEMPERATURA >= 25) ventilacionActivada = 1;
  else ventilacionActivada = 0;

  if (riegoActivado) {
    Serial.print("Sistema de riego activado.");
  } else Serial.print("Sistema de riego desactivado.");
  Serial.print("\r\n");


  if (ventilacionActivada) {
    Serial.print("Sistema de ventilacion activado.");
  } else Serial.print("Sistema de ventilacion desactivado.");
  Serial.print("\r\n");












  delay(1500);
}














/*

#include <DHT.h>		// importa la Librerias DHT
#include <DHT_U.h>

//led
int pinLedR = 13; // pin Rojo del LED RGB
int pausa = 1000;

//buzzer
int pinaltavoz = 3;
int frecuencia=220;    // frecuencia correspondiente a la nota La

//constantes del sensor de luz
const long A = 1000;     //Resistencia en oscuridad en KΩ
const int B = 15;        //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;       //Resistencia calibracion en KΩ
const int LDRPin = A3;   //Pin del LDR

//variables del sensor de luz
int V;
int ilum;

//variables del sensor de temperatura
int SENSOR = A4;			// pin DATA de DHT22 a pin digital 2
int TEMPERATURA;
int HUMEDAD;

DHT dht(SENSOR, DHT11);		// creacion del objeto, cambiar segundo parametro
				// por DHT11 si se utiliza en lugar del DHT22
void setup(){
  Serial.begin(9600);		// inicializacion de monitor serial
  dht.begin();			// inicializacion de sensor de temperatura
  pinMode(pinLedR, OUTPUT); // pone el pinLedR como output
}

void loop(){
  //sensor de luz
  V = analogRead(LDRPin);         
  ilum = ((long)V*A*10)/((long)B*Rc*(1024-V));    //usar si LDR entre A0 y Vcc (como en el esquema anterior)
  Serial.print(" Luz: ");
  Serial.print(ilum);   

  //sensor de temperatura
  TEMPERATURA = dht.readTemperature();	// obtencion de valor de temperatura
  HUMEDAD = dht.readHumidity();		// obtencion de valor de humedad
  Serial.print(" Temperatura: ");	// escritura en monitor serial de los valores
  Serial.print(TEMPERATURA);
  Serial.print(" Humedad: ");
  Serial.println(HUMEDAD);

  //verificacion de nivel de luz
  if (ilum<100){
      Serial.println(" Led encendido");
      analogWrite(pinLedR, 255); //encendido
  }
  else
    analogWrite(pinLedR, 0); //apagado
        
  //verificacion de niveles de temperatura
  if (TEMPERATURA >25){
      Serial.println(" Ventilacion encendida");
      if (TEMPERATURA == 50) {
        Serial.println(" Alarma encendida!");
        //enciendo altavoz
        tone(pinaltavoz,frecuencia);}
       else
        noTone(pinaltavoz);
  }
  else{
    if (TEMPERATURA <5)
      Serial.println(" Calefaccion encendida");
  }
 //verificacion de niveles de humedad
 if (HUMEDAD >80)
     Serial.println(" Riego detenido");
 else
   if (HUMEDAD <50)
     Serial.println(" Riego activado");
 
 Serial.println("");

 delay(1000);
}

*/
