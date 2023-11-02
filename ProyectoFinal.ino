#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"
#include <OneWire.h>
#include <DallasTemperature.h>
 
// Define el pin al que va conectado el sensor DQ
#define ONE_WIRE_BUS 4
 
// Declara una conexión 1-wire entre los dispositivos
OneWire oneWire(ONE_WIRE_BUS);	
 
// Mide los datos entre las librerías
DallasTemperature sensors(&oneWire);

//Define el sensor MQ-2
#define MQ2pin D3

// Credenciales wifi
/* 1. Define the WiFi credentials */
#define ssid "Tec-IoT"
#define password "spotless.magnetic.bridge"

/* 2. Define the API Key */
#define API_KEY "AIzaSyBOlmle1-ycvaWWceKsEADkF5HFWVbQvEM"

/* 3. Credenciales Proyecto Firebase */
const char *FIREBASE_HOST="https://base-de-datos-proyecto-7b0d7-default-rtdb.firebaseio.com/"; 
const char *FIREBASE_AUTH="mfU17eEXvX8fVbOekxAlMhdwCLJ5Y5QzODkKqPig";

// Firebase Data object in the global scope
FirebaseData firebaseData;

bool iterar = true;
int ledPin = 5; // a dónde va conectado el LED 
int sensorFotoresistencia = A0; 
int sensorValue = 0;
float sensorData;

void setup()
{
  sensors.begin();	// Inicio de las librerías (OneWire y DallasTemperature)
  Serial.begin(115200);
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(ledPin, OUTPUT);
  pinMode(sensorFotoresistencia, INPUT);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(250);
  }
  Serial.print("\nConectado al Wi-Fi");
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  Firebase.reconnectWiFi(true);
}

void loop()
{
   // Comando para recibir las temperaturas
  sensors.requestTemperatures(); 
  
  // Lee los valores de la fotoresistencia y led
  sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  
  sensorData = digitalRead(MQ2pin); // sensor GAS
  Serial.print("Sensor Gas: ");
	Serial.println(sensorData);

  Firebase.setFloat(firebaseData, "/Gas", sensorData); // envía el valor a Firebase

	delay(2000); // Espera 2 segundos para enviar el siguiente valor del gas/humo

  // Enviar datos a Firebase
  float temperatura = sensors.getTempCByIndex(0);
  Firebase.setFloat(firebaseData, "/temperatura", temperatura);
  Serial.print("temperatura enviada a Firebase: ");
  Serial.println(temperatura);

 // while (iterar)
 // {
  Firebase.getString(firebaseData, "/Led");
  if (firebaseData.stringData()=="1") {
    digitalWrite(ledPin,HIGH);
  }
  else if (firebaseData.stringData()=="0") {
    digitalWrite(ledPin,LOW);
  }
  else{
  }
  Serial.println(sensorValue);
  Firebase.setInt(firebaseData, "Sensor", sensorValue);{
  }
} // End Loop