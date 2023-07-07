#include <Keypad.h>

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

// Variables para la configuración de la clave
char claveGuardada[4] = "5023";  // Clave codificada en memoria  para la clave 7896
int codigo[4];
int cont = 0;
int alarmaActivada = 0;
//Variables para decodificar alarma
int a = 2;
int b = 5;
int n = 7;



void setup() {
  Serial.begin(9600);
}


void loop() {
  char key = keypad.getKey();

  if (key) {

    int numero = atoi(&key);


    codigo[cont] = funcionF(numero, a, b, n);
    Serial.print("codigo caracter:");
    Serial.print(codigo[cont]);
    Serial.print("\r\n");

    Serial.print("clave guardada:");
    Serial.print(claveGuardada[cont]);
    Serial.print("\r\n");




    Serial.print("codigo caracter total:");
    Serial.print(codigo[0]);
    Serial.print(codigo[1]);
    Serial.print(codigo[2]);
    Serial.print(codigo[3]);

    Serial.print("\r\n");

    Serial.print("clave guardada total:");
    Serial.print(claveGuardada[0]);
    Serial.print(claveGuardada[1]);
    Serial.print(claveGuardada[2]);
    Serial.print(claveGuardada[3]);
    Serial.print("\r\n");


    cont = cont + 1;

    // si se ingresan 5 caracteres se chequea la clave
    if (cont == 4) {

      if (codigo[0] == claveGuardada[0] & codigo[1] == claveGuardada[1] & codigo[2] == claveGuardada[2] & codigo[3] == claveGuardada[3]) {
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
      cont = 0;
    }
  }
}


// Definición de la función para decodificar alarma
int funcionF(int x, int a, int b, int n) {
  Serial.print("x es:");
  Serial.print(x);
  int result = (a * x + b) % n;
  Serial.print("resultado es:");
  Serial.print(result);
  Serial.print("\r\n");
  return result;
}









/*
 if (key) {
    if (key != '#') {
      // Construir la clave ingresada dígito a dígito
      claveIngresada = claveIngresada * 10 + (key - '0');
    } else {
      // Verificar la clave ingresada utilizando la función f(x)
      if (funcionF(claveIngresada, a, b, n) == claveGuardada) {
        // Clave correcta, activar el sistema de alarma
        Serial.println("Clave correcta. Alarma activada.");
      } else {
        // Clave incorrecta, mostrar mensaje de error
        Serial.println("Clave incorrecta. Alarma desactivada.");
      }
      
      // Reiniciar la variable claveIngresada
      claveIngresada = 0;
    }
  }
*/





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