#include <DHT.h>
//pin 2 del arduino
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht11(DHTPIN, DHTTYPE);

//filtro de Kalman
//estimación inicial de la temperatura:
double temp_estimate = 20.0;     
//covarianza inicial de la temperatura
double temp_covariance = 1.0;    

// Parámetros del filtro de Kalman
const double dt = 1.0; //intervalo de tiempo
//cvvarianza del ruido del proceso
const double Q = 0.1;   
const double R = 1.0; //->covarianza del ruido de la medición

void setup() {
  Serial.begin(9600);
  dht11.begin();
}

void loop() {
  //lectura cruda
  double z = dht11.readTemperature();
  //plicación del filtro de Kalman
  kalman_filter(z);
  //Mostrar los resultados
  //Serial.print("Temperatura medida: ");
  Serial.print(z);
  Serial.print(",");
  //Serial.print("Temperatura estimada: ");
  Serial.println(temp_estimate);
  //Serial.println();

  delay(300);
}

void kalman_filter(double z) {
  //paso 1: Predicción
  //predicción del estado
  double x = temp_estimate;
  //Predicción de la covarianza del estado
  double P = temp_covariance + Q;
  //paso 2: Actualización
  //calculo de la ganancia de Kalman
  double K = P / (P + R);
  //actualización del estado
  temp_estimate = x + K * (z - x);
  //actualización de la covarianza del estado
  temp_covariance = (1 - K) * P;
}
