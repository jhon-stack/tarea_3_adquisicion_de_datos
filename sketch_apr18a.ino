#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht11(DHTPIN, DHTTYPE);

float X_k = 0;         // Estimated temperature
float e_EST_k = 10;    // Estimation error (initialized with a reasonable value)

void setup() {
  // Iniciar la comunicación serial
  Serial.begin(9600);
  // Iniciar el sensor DHT11
  dht11.begin();
  
  // Esperar un momento para que se estabilice el sensor
  delay(1000);
  
  //Leer el primer valor de la temperatura para que se inicialice X_k
  X_k = dht11.readTemperature();
}

void loop() {
  //retardo de medio segundo para agarrar dos lecturas por segundo
  delay(500);

  // Leer la temperatura del sensor DHT11
  float Z_k = dht11.readTemperature();

  //Calcular la ganancia de Kalman
  float e_EST_K_1 = e_EST_k;
  float e_MEA_K_1 = 1; //Medir el error
  float K_k = e_EST_K_1 / (e_EST_K_1 + e_MEA_K_1);

  //calcular el valor estimado de X_k en un  tiempo k
  X_k = X_k + K_k * (Z_k - X_k);

  // actualizar el error de estimacion e_EST_k
  e_EST_k = (1 - K_k) * e_EST_K_1;

  // Print the raw and Kalman filtered temperature values
  Serial.print("Raw Temperature: ");
  Serial.println(Z_k);
  Serial.print("Filtered Temperature: ");
  Serial.println(X_k);
}
