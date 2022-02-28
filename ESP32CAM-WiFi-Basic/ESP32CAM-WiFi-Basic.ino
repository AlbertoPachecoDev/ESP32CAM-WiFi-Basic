/*
 * Conexión simple a WiFi modificando, usando C++

 * Por: Hugo Escalpelo
 * Modificado por: Alberto Pacheco (apacheco@uach.mx)
 * Fecha: 28 de febrero de 2022
 *
 * Este programa muestra una conexión básica a WiFi para
 * ESP32CAM. Para ello es necesario que cambies el SSID y
 * contraseña por los datos correspondientes a la red que
 * deseas conectarte. Puedes observar el estado de la conexión
 * con el monitor serial. No se requieren componentes adicionales.
 *
 * En este programa se usan los leds soldados sobre la placa
 * ESP32CAM, el led flash y el led de status.
 *
 * Componente     PinESP32CAM     Estados lógicos
 * ledStatus------GPIO 33---------On=>LOW, Off=>HIGH
 * ledFlash-------GPIO 4----------On=>HIGH, Off=>LOW
 */

// Bibliotecas
#include <WiFi.h>  // Biblioteca para manejar el WiFi del ESP32CAM
#include <iostream>

using namespace std;

// Datos de Red
const char*   ssid = "******";  // Pon aquí el nombre de la red a la que deseas conectarte
const char* passwd = "******";  // Escribe la contraseña de dicha red

// Objetos
WiFiClient espClient; // Este objeto maneja las variables necesarias para una conexion WiFi

// Constantes
const int statusLedPin = 33;  // Led-status
const int flashLedPin = 4; // Led-flash
const double wait = 500;  // Espera 500ms
const int FLASH_TIMES = 10;

// Variables
double timeLast, timeNow; // Variables para el control de tiempo no bloqueante
bool statusLed = false;// Bandera que me dice si el led esta encendido o apagado
int counter = 0; // Flash counter

void setup()
{
  Serial.begin(115200);

  // Config pins
  pinMode(statusLedPin, OUTPUT);// Se configura el pin como salida
  pinMode(flashLedPin, OUTPUT);// Se configura el pin como salida
  digitalWrite(statusLedPin, HIGH);// Se comienza con el led apagado
  digitalWrite(flashLedPin, LOW);// Se comienza con el led apagado

  cout << endl << "Conectando a " << ssid << endl;

  // Init WiFi
  WiFi.begin(ssid, passwd); // Esta es la función que realiz la conexión a WiFi

  while (WiFi.status() != WL_CONNECTED) { // Este bucle espera a que se realice la conexión
    digitalWrite(statusLedPin, HIGH);
    delay(wait); //dado que es de suma importancia esperar a la conexión, debe usarse espera bloqueante
    digitalWrite(statusLedPin, LOW);
    Serial.print(".");  // Indicador de progreso
    delay(50);
  }

  // Cuando se haya logrado la conexión, el programa avanzará
  cout << endl << "WiFi conectado" << endl;
  cout << "Conectar navegador Web a IP: ";
  cout << WiFi.localIP().toString().c_str() << endl;

  // Si se logro la conexión, encender led
  if (WiFi.status()) {
    digitalWrite(statusLedPin, LOW);
  }
  timeLast = millis(); // Inicia el control de tiempo
}

void loop()
{
  timeNow = millis();  // timing
  if (timeNow - timeLast > wait) { // Check flash-time
    if (counter < FLASH_TIMES) { // @todo: Encender FLASH led para iluminar rostro enfrente cámara
      digitalWrite(flashLedPin, statusLed? LOW : HIGH);// flash led on/off
      statusLed = !statusLed; // toggle led state
      timeLast = millis(); // timing
      counter++; // update flash counter
    }
  }
}
