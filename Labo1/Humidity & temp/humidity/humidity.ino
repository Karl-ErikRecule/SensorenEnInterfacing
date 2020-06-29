//UITLEG HOE LIB TOEVOEGEN: http://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/

#include <dht.h>

dht DHT;

#define DHT11_PIN 7


void setup(){
  Serial.begin(9600);
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  Serial.println("Verwarming aan:" + String(verwarming(24,1)));
  Serial.println("Temperatuur: " + String(temperatuur()));
  Serial.println("Humidity: " + String(humidity()));
  Serial.println("Airco aan: " + String(airco(60, 5)));
}


float temperatuur()
{
    int chk = DHT.read11(DHT11_PIN);
    float temp = DHT.temperature;
    delay(2000);
    return temp;
}

float humidity()
{
    int chk = DHT.read11(DHT11_PIN);
    float humidity = DHT.humidity;
    delay(2000);
    return humidity;
}

bool verwarming(int thresholdTemp, int hysterese)
{
  int chk = DHT.read11(DHT11_PIN);
  delay(2500);
  float temp = temperatuur();
  if(temp >= (thresholdTemp + hysterese))
  {
    return false;
  }
  else if(temp <= (thresholdTemp - hysterese))
  {
    return true;
  }
}

bool airco(int thresholdHumidity, int hysterese)
{
  int chk = DHT.read11(DHT11_PIN);
  float hum = humidity();
  delay(2000);
  if(hum >= (thresholdHumidity + hysterese))
  {
    return true;
  }
  else if(hum <= (thresholdHumidity - hysterese))
  {
    return false;
  }
}
