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
unsigned int claveCodificada[4] = {5, 0, 2, 3}; // password en memoria  para la key 7896
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
const long Roscuridad = 1000;    //Resistencia en oscuridad en KΩ
const int Rluz = 15;       //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;      //Resistencia calibracion en KΩ
const int LDRPin = A3;  //Pin del LDR
int VOLTAJE;
int ILUMINACION;
int pinLedR = 13;

//----------------buzzer------------------------------//
int pinaltavoz = 3;
int frecuencia=220;    // frecuencia correspondiente a la nota La



void setup() {
  // init comunicación serie a 9600 baudios (monitor serial)
  Serial.begin(9600);
  // Inicialización del sensor de temperatura y humedad
  dht.begin();
  //pinLedR como output
  pinMode(pinLedR, OUTPUT); 
}


void loop() {
  // -----------------------------alarma--------------------------------------//
  key = keypad.getKey();
  activarAlarma(key);
  //detectorMovimiento();
  //activarSirena();  --> falta logica de movimiento
  // ----------------------------Fin alarma----------------------------

  Serial.print("\r\n");


  //--------------------------------Activacion de calefaccion, ventilacion y sirena-----------------------------//
  TEMPERATURA = dht.readTemperature();  // Obtener valor de temperatura
  HUMEDAD = dht.readHumidity();         // Obtener valor de humedad
  //---------------------------falta agregar el movimiento deberia pasarse a la funcion y condicionarlo segun el TP----------------------------------//
  activarCalefaccionVentilacion(TEMPERATURA,HUMEDAD);
  //---------------------------------------Activacion de calefaccion, ventilacion y sirena-----------------------------------------------//


  // -----------------------activacion de led----------------//
  VOLTAJE = analogRead(LDRPin); // Lee entrada analógica
  ILUMINACION = ((long)VOLTAJE * Roscuridad * 10) / ((long)Rluz * Rc * (1024 - VOLTAJE));  // Obtener valor de iluminación
  
  activarLed(ILUMINACION);

  // ---------------------------------activacion led----------------------------//








  delay(1500);
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

void activarSirena(){
  //tone(pinaltavoz,frecuencia) --> activacion altavoz
  //noTone(pinaltavoz) --> desactivacion altavoz
  /* crear detectoMoviento
  if(alarmaActivada && detectoMoviento){
    tone(pinaltavoz,frecuencia);
  }
  else{
    noTone(pinaltavoz);
  }
  */
}
void activarCalefaccionVentilacion(int TEMPERATURA,int HUMEDAD){
  Serial.print("Temperatura: ");
  Serial.print(TEMPERATURA);
  Serial.print(" - Humedad: ");
  Serial.println(HUMEDAD);


  //tone(pinaltavoz,frecuencia) --> activacion altavoz
  //noTone(pinaltavoz) --> desactivacion altavoz
  if(TEMPERATURA > 50){
      tone(pinaltavoz,frecuencia);
      ventilacionActivada = 0;
      calefaccionActivada = 0;
  }
  else if(TEMPERATURA > 25 && HUMEDAD < 80){
    noTone(pinaltavoz);
    ventilacionActivada = 1;
    calefaccionActivada = 0;
  }
  else if(TEMPERATURA >= 23 && HUMEDAD >= 80){
    noTone(pinaltavoz);
    ventilacionActivada = 1;
    calefaccionActivada = 0;
  }
  else if(TEMPERATURA >= 18){
    noTone(pinaltavoz);
    ventilacionActivada = 0;
    calefaccionActivada = 0;
  }
  else if(TEMPERATURA < 18){
    noTone(pinaltavoz);
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

void activarLed(int ILUMINACION){
  Serial.print("Iluminacion: ");
  Serial.print(ILUMINACION);
  Serial.print("\r\n");
  
  if (ILUMINACION < 100) {
    analogWrite(pinLedR, 255); //encendido
    Serial.print("Led encendido.");
  }
  else{
    Serial.print("Led apagado.");
    analogWrite(pinLedR, 0);
  }
  Serial.print("\r\n");  
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
