// Arduino with load cell

// Put two known loads on the sensor and take readings. Put those values
// here.
float analogvalA = 112.04;
float loadA = 100; // 100 g.
float analogvalB = 588.50;
float loadB = 2000; // 2000 g.
float offset=3.8408279418;
float offset2=-0.1469116210;

long time = 0;
int interval = 2000; // Take a reading every 500 ms

float analogValueAverage = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  int analogValue = analogRead(A0);
  
  // running average - We smooth the readings a little bit
  analogValueAverage = 0.99*analogValueAverage + 0.01*analogValue;

  // Is it time to print? 
  if(millis() > time + interval){
    float load = ((loadB - loadA)/(analogvalB - analogvalA)) * (analogValue - analogvalA) + loadA + offset2;
  
    Serial.print("analogValue: ");Serial.println(analogValueAverage);
    Serial.print("             load: ");Serial.println(load ,10);
    time = millis();

  }

  }
